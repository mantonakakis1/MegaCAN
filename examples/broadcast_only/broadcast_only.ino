#include <FlexCAN_T4.h>
#include <MegaCAN.h>

const uint32_t baseID = 1512; // Must set to match Megasquirt Settings!
const uint32_t finalID = baseID + 17; // Must set to match Megasquirt Settings configured in TunerStudio!

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can; // For CAN communications between devices, this example uses the "CAN2" port/pins on a Teensy 4.0
MegaCAN MegaCAN(baseID); // For processed Megasquirt CAN protocol messages

MegaCAN_broadcast_message_t bCastMsg; // Stores unpacked Megasquirt broadcast data, e.g. bCastMsg.rpm

CAN_message_t respMsg; // Actual response message back to Megasquirt, MSCAN protocol

void initializeCAN() {
  Can.begin();
  Can.setBaudRate(500000); // Set to 500000 for normal Megasquirt usage - need to change Megasquirt firmware to change MS CAN baud rate
  Can.setMaxMB(16); // Sets maximum number of mailboxes for FlexCAN_T4 usage
  Can.enableFIFO();
  Can.enableFIFOInterrupt();
  Can.onReceive(canMShandler); // When a CAN message is received, runs the canMShandler function
  Can.mailboxStatus();
}

void canMShandler(const CAN_message_t &msg) {
  // For Megasquirt CAN broadcast data:
  if (!msg.flags.extended) { // Broadcast data from MS does not use extended flag, therefore this should be broadcast data from MS
    // Unpack megasquirt broadcast data into bCastMsg:
    MegaCAN.getBCastData(msg.id, msg.buf, bCastMsg); // baseID based on const parameter passed into constructor - converts the raw CAN id and buf to bCastMsg format

    if (msg.id == finalID) {
      /*~~~Final message for this batch of data, do stuff with the data - this is a simple example~~~*/
      float MAP = bCastMsg.map; // You could work directly with bCastMsg.map (or any parameter), or store as a separate variable as in this example
      float RPM = bCastMsg.rpm;
      float TPS = bCastMsg.tps;
      Serial.print(MAP); Serial.print(" | "); // Should be kPa
      Serial.print(RPM); Serial.print(" | "); // Should be rpm
      Serial.println(TPS);                    // Should be %
    }
  }
}

void setup() {
  while (!Serial);
  Serial.begin(115200);
  Serial.println("MAP | RPM | TPS");
}

void loop() {
  Can.events(); // Allows this example sketch to run the canMShandler function on an event-based timing, i.e. once a CAN message is received (set up with Can.onReceive(canMShandler) above)
}
