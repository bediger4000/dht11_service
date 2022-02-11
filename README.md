# Raspberry Pi Temperature and Humidity Collection

Source code for collecting temperature and relative humidity
from a DHT11 sensor,
and temperature from a DS18B20 digital thermometer.
Data send via UDP to a central server.

## Design

The idea is to have a Raspberry Pi enclosed in a small,
waterproof container, sit outside a house.
The Pi reads sensors periodically,
reporting the data back to a centralized logging service.
I assume that the Pi is connected to a WiFi network to avoid
having to run cable to it.
Because the Pi will be outside,
I can't rely on the ability to retrieve data written to a file.
Communication to centralized logging is via UDP to avoid long lasting connections.

I wrote a C program to read the DHT11 and DS18B20,
and a Go program to read and record the data sent by the Raspberry Pi.

There is a major problem with this project.

## DHT11 is rubbish

I originally purchased a box of 16 assorted "Internet of Things" sensors.
It had a DHT11 and a DS18B20.
I wanted to use both so as to have some redundancy in data collection.
There is a problem.

DHT11 is unsuitable for use.

DHT11s have a nominal temperature sensing range
of 0-50C, with an accuracy of +/- 2C.
That's just too coarse to be of any use.

It has incomprehensible documentation about
what data it returns.
Theoretically, it returns 2 bytes of relative humidity,
and 2 bytes of temperature.
One byte is the "integral part"
and one byte is the "decimal part".
What in tarnation does "decimal part" mean?
An 8-bit byte can have 256 possible values.
Does a "decimal part" byte with a value of "00000001"
mean 0.00000001 degrees, or 0.1 degrees?
What about a "decimal part" of "111110000"?
Is that 0.240 degrees or 0.8571 degrees,
or something else?
All DHT11 data sheets repeat this gibberish.

The interface is a weird serial-like
protocol that is at the very least prone to misreading.
Code to communicate with a DHT11 has to handle
regular checksum errors and read timeouts.
Most code resorts to a form of user space "bit-banging".

![why DHT11 is rubbish](dht11_goofy.png)

This graph compares DHT11 temperatures (with a 1C resolution)
to DS18B20 temperatures (with a 0.001C resolution)
over a period of about 11 days.
The Raspberry Pi reading the sensors was in my garage, sitting on a workbench.
You can see daily cycles of high and low termperatures for about 5 days.
A cold snap happened on the 5th - 7th days, with daily high temperatures
well below freezing.

Notice the "mirror image" temperatures
from 2022-02-02T12:00:00 to 2022-02-04T12:00:00.
The ambient weather was a cold snap, with daily high
temperatures of approximately 15F,
well below 0C, the specified lower bound on DHT11 temperature range.

The sudden negative excursions of the DS18B20
temperature are from opening the garage door,
letting in very cold outside air.
The DHT11 temperature "mirrors" the DS18B20,
except it returns positive values.

The manufacturers of this particular DHT11 chose
to cripple a more-functional chip by cutting off
the sign bit, but did not clamp the maximum low reading to 0C.
The hardware can sense and report below-zero temperatures.
Without a sign bit,
those values are worse than useless, they're misleading.

Do not bother with a DHT11.

## Build and Run

```sh
$ make all
$ go build -o server server.go
```

This should result in 2 executables, 
`dht_service` and `server`.
You almost certainly want to do the `make all` on a Raspberry Pi,
because the C code to read DHT11 and DS18B20 sensors won't work anywhere else.
It only works with the RPi's GPIO device.

The `server` executable should be compilable anywere a Go compiler exists.

### Running the Server

The `server` listens on a UDP port and writes any incoming bytes it reads
from that port to stdout.
There's no security, no authentication, so do not expose to The Internet.
An invocation on Linux would look something like this:

```sh
$ go build -o server server.go
$ nohup ./server 10.0.0.1 7654 > temp.out 2> server.log &
```

You want to give the `server` command line the IP address of the
network shared with the Raspberry Pi,
or at least an IP address reachable by the Raspberry Pi from its WiFi network.

### Collector

The data collector, the `dht_service` executable,
defaults to collecting data every 120 seconds,
and sending an ASCII-formatted string representation of the data
to UDP port 7896 at 10.0.0.1.

```sh
$ nohup ./dht_service -i 180 -p 9115 -s 192.168.1.12 -d > service.log 2>&1 &
```

That invocation will collect data every 180 seconds,
sending a formatted string representation to UDP port 9115
at IP address 192.168.1.12.
The `-d` flag will write some run-time trace information to stderr.
