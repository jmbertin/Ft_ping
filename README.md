# Ft_Ping
This repository contains a custom implementation of the ping utility, written in C. The program sends ICMP ECHO_REQUEST packets to network hosts and displays the response to measure the round-trip time and packet loss.

----

## Features
- Sends ICMP ECHO_REQUEST packets.
- Measures round-trip time and packet loss.
- Supports various command-line options to customize behavior.
- Verbose and flood mode support.
- Handles both IPv4 addresses and domain name resolution.

----

## Compilation
To compile the program, use the provided Makefile:

````
make
````

This will create an executable named ft_ping.

----

## Usage
To use ft_ping, run the executable with a destination host as an argument:


````
./ft_ping [options] <destination>
````

You can test and compare with the original ping command, with the integrated dockerfile :
- launch the docker with ``make test``
- inside the docker launch the script test with ``./tests.sh`` or just try ``ping`` and compare with ``ft_ping``

**Options**

````
-h: Display help.
-v: Verbose output.
-c NUMBER: Stop after sending (and receiving) NUMBER packets.
-i NUMBER: Wait NUMBER seconds between sending each packet.
-w NUMBER: Stop after NUMBER seconds.
-f: Flood ping (send packets as fast as possible).
````

**Examples**

Ping a specific IP address:

````
./ft_ping 192.168.1.1
````

Ping a domain with a specific count:

````
./ft_ping -c 4 example.com
````

----

## Understanding ICMP and ECHO_REQUEST Packets

### ICMP (Internet Control Message Protocol)
ICMP is a fundamental protocol used within the Internet Protocol suite. It is primarily used by network devices, like routers, to send error messages and operational information. For example, ICMP messages are used to report unreachable hosts or network segments, to manage network traffic by redirecting routes, and for diagnostic or troubleshooting purposes.

#### Structure of an ICMP Packet
An ICMP packet generally consists of:

````
Type: Specifies the ICMP message type. For example, type 8 is an Echo Request, and type 0 is an Echo Reply.
Code: Provides additional context for the ICMP message type. Typically 0 for Echo Requests and Replies.
Checksum: Used for error-checking the header and data.
Data: Additional data for the ICMP message, varying by type. For Echo Requests and Replies, this includes an identifier and a sequence number, followed by optional data.
````


### ECHO_REQUEST and ECHO_REPLY
The **ECHO_REQUEST** and **ECHO_REPLY** messages are a pair used primarily to check the reachability of a host in an IP network.

- **ECHO_REQUEST** (Type 8): Sent by the ping initiator to the target host. It can contain arbitrary data, which the responder is expected to send back.

- **ECHO_REPLY** (Type 0): The response from the target host to the ECHO_REQUEST, containing the same data received in the request.
These messages are crucial for the ping utility's functionality, allowing it to measure round-trip times and packet loss across network paths.

