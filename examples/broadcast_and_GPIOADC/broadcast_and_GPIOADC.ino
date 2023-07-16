#include <FlexCAN_T4.h>
#include <MegaCAN.h>

#define ANALOGPIN A0 // For analogRead, set to desired pin if you'd like, or it will just read the floating pin state

const uint32_t baseID = 1512; // Must set to match Megasquirt Settings!
const uint32_t finalID = baseID + 17; // Must set to match Megasquirt Settings configured in TunerStudio!

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can; // For CAN communications between devices, this example uses the "CAN2" port/pins on a Teensy 4.0
MegaCAN MegaCAN(baseID); // For processed Megasquirt CAN protocol messages

MegaCAN_message_t recMsgMSC; // Stores received message from Megasquirt, Megasquirt CAN protocol
MegaCAN_message_t respMsgMSC; // Stores response message back to Megasquirt, Megasquirt CAN protocol
MegaCAN_broadcast_message_t bCastMsg; // Stores unpacked Megasquirt broadcast data, e.g. bCastMsg.rpm

CAN_message_t respMsg; // Actual response message back to Megasquirt, MSCAN protocol

uint16_t GPIOADC[8] = { 0 }; // Stores values to send to Megasquirt, 4 ADCS for each message

uint16_t adc0 = 10; // Sample fixed values for adc0-6
uint16_t adc1 = 20;
uint16_t adc2 = 30;
uint16_t adc3 = 40;
uint16_t adc4 = 50;
uint16_t adc5 = 60;
uint16_t adc6 = 70;
uint16_t adc7; // Will use this for updated analogRead value each cycle

void initializeCAN() {
  Can.begin();
  Can.setBaudRate(500000); // Set to 500000 for normal Megasquirt usage - need to change Megasquirt firmware to change MS CAN baud rate
  Can.setMaxMB(16); // Sets maximum number of mailboxes for FlexCAN_T4 usage
  Can.enableFIFO();
  Can.enableFIFOInterrupt();
  Can.onReceive(canMShandler); // When a CAN message is received, runs the canMShandler function
  Can.mailboxStatus();
  initializeMSCAN();
}

void initializeMSCAN() {
  respMsg.flags.extended = 1;
  respMsg.flags.remote = 0;
}

void canMShandler(const CAN_message_t &msg) {
  // For Megasquirt CAN protocol, in this example MS is requesting data:
  if (msg.flags.extended) { // Data request from MS uses extended flag, there may be a better way to implement this with more advanced applications, works fine for sending data to MS GPIOADC
    sendDataToMS(msg); // Due to the extended flag, we assume this is a MS data request and run the function to send data to MS, passing the message received from MS to the sendDataToMS function
  }

  // For Megasquirt CAN broadcast data:
  else { // Broadcast data from MS does not use extended flag, therefore a standard message from MS will contain broadcast data
    // Unpack megasquirt broadcast data into bCastMsg:
    MegaCAN.getBCastData(msg.id, msg.buf, bCastMsg); // baseID fixed in library based on const parameter entered for baseID above - converts the raw CAN id and buf to bCastMsg format

    if (msg.id == finalID) {
      /*~~~Final message for this batch of data, do stuff with the data - this is a simple example~~~*/
      Serial.print(bCastMsg.map); Serial.print(" | "); // Should be kPa
      Serial.print(bCastMsg.rpm); Serial.print(" | "); // Should be rpm
      Serial.println(bCastMsg.tps);                    // Should be %
    }
  }
}

void sendDataToMS(CAN_message_t msg) {
  MegaCAN.processMSreq(msg.id, msg.buf, recMsgMSC); // Unpack request message ("msg") from MS into recMsgMS
  adc7 = analogRead(ANALOGPIN); // You can add a sensor or potentiometer here, or just read the floating voltage for this example

  if (recMsgMSC.core.toOffset == 0) { // For GPIOADC0-3
    GPIOADC[0] = adc0; // Get oil presure, set to MSCAN ouput, send to Nextion too
    GPIOADC[1] = adc1; // Get fuel pressure, set to MSCAN output, send to Nextion too
    GPIOADC[2] = adc2; // Turbo speed/10 to send to MSCAN
    GPIOADC[3] = adc3; // Exhaust manifold absolute pressure in kPa
    MegaCAN.setMSresp(recMsgMSC, respMsgMSC, GPIOADC[0], GPIOADC[1], GPIOADC[2], GPIOADC[3]); // Packs the GPIOADC0-3 values into "respMsgMSC"
  }
  else if (recMsgMSC.core.toOffset == 1) { // For GPIOADC4-7
    GPIOADC[4] = adc4; // Keep it positive?
    GPIOADC[5] = adc5; // Nozzle area * 10, actual as reported by turbo
    GPIOADC[6] = adc6; // Dome target pressure kPa absolute
    GPIOADC[7] = adc7; // Dome actual pressure kPa absolute
    MegaCAN.setMSresp(recMsgMSC, respMsgMSC, GPIOADC[4], GPIOADC[5], GPIOADC[6], GPIOADC[7]); // Packs the GPIOADC4-7 values into "respMsgMSC"
  }

  // Send response to Megasquirt using MSCAN protocol:
  respMsg.id = respMsgMSC.responseCore;
  respMsg.len = sizeof(respMsgMSC.data.response);
  for (int i = 0; i < respMsg.len; i++) {
    respMsg.buf[i] = respMsgMSC.data.response[i];
  }
  Can.write(respMsg); // Sends the GPIOADC values stored in respMsg over CAN to Mesasquirt
  Serial.println("Data sent to Megasquirt");
}

void setup() {
  while (!Serial);
  Serial.begin(115200);
  pinMode(ANALOGPIN, INPUT);
  Serial.println("MAP | RPM | TPS");
}

void loop() {
  Can.events(); // Allows this example sketch to run the canMShandler function on an event-based timing, i.e. once a CAN message is received (set up with Can.onReceive(canMShandler) above)
}
