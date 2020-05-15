# MegaCAN
Library for interacting with Megasquirt CAN Protocol on Arduino-compatible hardware (Arduino, Teensy, Feather, etc.). Hopefully this makes it relatively easy to:
* Receive and store 11-bit advanced broadcast data from Megasquirt.
** Simple dash broadcasting not currently supported, will be added later.
* Receive and interpret 29-bit (extended) requests for data from a Megasquirt.
* Prepare a response to the Megasquirt's request for data.

## Installation and Adding to a Sketch
Simply download a zip of this respository, add a "MegaCAN" directory to your Arduino/Libraries director and extract the zip contents there.

## Usage
### Including in your Arduino sketch
To use the library in a sketch, add the following to your Arduino sketch: `#include <MegaCAN.h>`

### Limitations of the library
Note that this library does **not** actually handle CAN communications! However, it should be compatible with most CAN communication libraries. It has been tested primarily with [FlexCAN_T4](https://github.com/tonton81/FlexCAN_T4), but also on a limited basis with [SparkFun's CAN-Bus library](https://github.com/sparkfun/SparkFun_CAN-Bus_Arduino_Library). If your library can separate out an 11-bit CAN ID and the corresponding data buffer in a CAN message, the CAN broadcasting function will work. If the library can handle extended 29-bit CAN IDs, flags, and data buffer in both read and write, then you'll be able to send data back to Megasquirt!

### Using the library's structures and functions
Although you can access all structures/objects, classes, functions, etc. from your Arduino sketch, unless you have a reason, you probably only ever need to interact with the following:
* `MegaCAN_message_t`: a structure that stores the parameters of 29-bit proprietary Megasquirt CAN protocol.
  * This is only needed if you're looking to send data to your Megasquirt's GPIO ADCs.
  * Currently, you'll want to create one of these for requests from Megasquirt and another for responses to Megasquirt.
  * You shouldn't need to interact directly with all of the objects in a MegaCAN_message_t, but you might need the following:
    * If you intend to use more than one of the sets of GPIO ADCs, you'll need to check which one MS is asking about: this is in the `core.toOffset` object within the request MegaCAN_message_t, and corresponds to the "offset" setting in the "CAN bus/Testmodes > CAN Parameters" dialog in TunerStudio.
* `MegaCAN_broadcast_message_t`: a structure that stores any and all of the parameters that Megasquirt can send with its "Advanced Real-time Data Broadcast". Currently, simple dash broadcasting is not supported, but will be added in the future.
  * To access the data, if for example you store the broadcast data in a MegaCAN_broadcast_message_t called `bCastMsg`, you get the data by simply calling `bCastMsg.<dataName>`, where `<dataName>` corresponds to the names in the [2016-02-17 version of the Megasquirt CAN Broadcasting Manual Advanced Broadcasting Field List](http://www.msextra.com/doc/pdf/Megasquirt_CAN_Broadcast.pdf#page=6&zoom=100,114,89). I.e. if you want engine speed, simply call `bCastMsg.rpm`.
* `MegaCAN`: a class with the few functions needed for all of this to work:
  * `processMSreq()`: Processes a 29-bit extended CAN message received from Megasquirt, requiring the following arguments:
    * `msgCore`: A uint32_t that contains the ID parameter of the request message, which Megasquirt encodes with:
      * The table on the remote device that Megasquirt is requesting data from (see TunerStudio settings)
      * The offset within that table where Megasquirt is requesting data from (see TunerStudio settings)
      * The message type (this library currently only supports requests and responses)
      * The "From ID", i.e. the Megasquirt's CAN address/ID
      * The "To ID", i.e. the address/ID Megasquirt is looking to receive data from
    * `msgData[3]`: The three uint8_t bytes of the request message's data buffer, which Megasquirt encodes with:
      * The table in the Megasquirt where the response data from the remote device will be stored.
      * The offset within that table where Megasquirt will store the response data
      * The number of bytes of data Megasquirt is requesting
    * `&msg`: A MegaCAN_message_t where the above request message parameters will be stored.
  * `setMSresp()`: Encodes the response to a Megasquirt CAN data request into a CAN ID and data buffer, requiring the following arguments:
    * `recMsg`: The MegaCAN_message_t where the request message parameters were stored by `processMSreq()`.
    * `&respMsg`: A MegaCAN_message_t where the response message parameters will be stored.
    * `var0`, `var1`, `var2`, `var3`: uint16_t encoded data variables; the actual data that you want to send to each of the four GPIO ADC channels in the Megasquirt.
  * `getBCastData()`: Processes an 11-bit Megasquirt Advanced Data Broadcast CAN message into the values shown in the [Megasquirt CAN broadcasting manual](http://www.msextra.com/doc/pdf/Megasquirt_CAN_Broadcast.pdf#page=6&zoom=100,114,89).
    * You'll need to call this function **every time** you get a CAN broadcast message from Megasquirt if you don't want to miss data!
    * You'll also need to define a global (i.e. before your setup() function) `const uint32_t baseID = 1512;` taking care to set it to match your Megasquirt settings.
    * Requires the following arguments:
      * `id`: The uint32_t CAN message ID parameter of the received broadcast message.
      * `data[8]`: The eight uint8_t bytes of CAN message data buffer of the received broadcast message.
      * `&msg`: The MegaCAN_broadcast_message_t where you want to store the processed data.
    
## Disclaimer, Issues, Contributing, Requests
I am using it on my own car to simultaneously process broadcast messages, and send data back to a Megasquirt 2 with the Extra 3.4.3 firmware using the 29-bit extended request/response protocol for datalogging extra channels. With roughly 1000 miles of driving, it has worked flawlessly. That said, I am by no means a professional developer, and since this is my first library, I am sure there is room for improvement.
I'll do my best to help if you encounter problems, and if you have suggestions for improvement, by all means let me know what should be changed! (referring to coding practices/technical advice)
Feature requests will definitely be considered, too! But implementation will be limited by my free time and capacity to figure out how to build them. 

## To Do
* Include examples.
* Add simple dash broadcasting support.
