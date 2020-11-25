# VIS_A1_SocketCommunication
Distributed Software Systems Assignment 1.

## Intro
This Server/Client System mimics IOT sensors reading. <br>
The Client sends the Server requests asking for the sensor vlaues.
<br><br>
The Server application can start TCP IPv4 and TCP IPv6 and UDP Servers. However, the UDP Server is just an echo server.
<br>
![Server UI](https://i.imgur.com/PDwbMPB.png)

#### Queries
Any Connected client can ask the server about the following queries:
 - getSensortypes()#
 - getAllSensors()#
 - getSensor(<SENSORNAME>)#
<br>
Any other input will make the Server act like an echo server.

![Client Response](https://i.imgur.com/K7EFVqi.png)


### Files
In the **Sources** folder there are **CPP** and **Java** Folders, each of them has its implementation of Server and Client.
<br> **However,** The Application should be tested using the Java Client and the C++ Server to have full functionality.

#### Documentation
Check the Javadocs and Doxygen files in the **Docs** folder.
