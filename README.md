# MegaCAN
Library for interacting with Megasquirt CAN Protocol on Arduino-compatible hardware (Arduino, Teensy, Feather, etc.). Hopefully this makes it relatively easy to:
* Receive and store 11-bit broadcast data from Megasquirt.
* Receive and interpret 29-bit (extended) requests for data from a Megasquirt.
* Prepare a response to the Megasquirt's request for data.

## Installation and Adding to a Sketch
Simply download a zip of this respository, and extract it your Arduino/Libraries directory. To use, add the following to your Arduino sketch:
<#include <MegaCAN.h>>

