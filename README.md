VCD Prototyping Arduino+Ethernet Controller
===============

This is the software running on an Arduino Mega with an Arduino Ethernet Shield.

It is currently set to listen for UDP Packets on IP 192.168.0.177 and port 8888.

The datastructure of each UDP packet is very simple: 32 bytes where each byte defines the intensity of one LCD.

2015 Vincent Rebers, Jason Bruges Studio


Licence
-------


Dependencies
-------

http://arduino.cc/en/Main/ArduinoBoardMega2560
http://arduino.cc/en/Main/ArduinoEthernetShield


Setup
-------
You will need to add the unzipped SoftPWM_Diff to your library folder.
This is a slightly modified version to work with the mega.

Troubleshooting
-------


Notes
-------