#define Serial SerialUSB



void setup() {
  while(!Serial);
  Serial.begin(115200);
}

void loop() {
  // Spoofed MS request variables
  uint32_t reqTable = 0;
  uint32_t reqToID = 0;
  uint32_t reqFromID = 0;
  uint32_t reqMsgType = 0;
  uint32_t reqOffset = 0;
  uint8_t reqVarByt = 0;
  uint16_t reqVarOffset = 0;
  uint8_t reqVarBlk = 0;
  
  // Spoofed MS request header and bytes
  uint32_t reqHeader = 0;
  uint8_t reqData0 = 0;
  uint8_t reqData1 = 0;
  uint8_t reqData2 = 0;
  
  // Unpacked variables from spoofed MS request header and bytes
  uint8_t recdVarByt = 0;
  uint16_t recdVarOffset = 0;
  uint8_t recdVarBlk = 0;
  uint8_t recdTable = 0;
  uint8_t recdToID = 0;
  uint8_t recdFromID = 0;
  uint8_t recdMsgType = 0;
  uint16_t recdOffset = 0;
  
  Serial.println("Spoofing MS request, begin data entry");
  
  Serial.println("Step 1: enter a target table value, 0-31");
  while(!Serial.available());
  reqTable = (uint32_t)Serial.parseInt();
  Serial.print("Received: "); Serial.println(reqTable);
  
  Serial.println("Step 2: enter a To ID, 0-15");
  while(!Serial.available());
  reqToID = Serial.parseInt();
  Serial.print("Received: "); Serial.println(reqToID);

  Serial.println("Step 3: enter a From ID, 0-15");
  while(!Serial.available());
  reqFromID = Serial.parseInt();
  Serial.print("Received: "); Serial.println(reqFromID);

  Serial.println("Step 4: enter a Message Type, 0-7");
  while(!Serial.available());
  reqMsgType = Serial.parseInt();
  Serial.print("Received: "); Serial.println(reqMsgType);

  Serial.println("Step 5: enter a target Offset, 0-2047");
  while(!Serial.available());
  reqOffset = Serial.parseInt();
  Serial.print("Received: "); Serial.println(reqOffset);

  Serial.println("Step 6: enter a response message length (bytes), 0-8");
  while(!Serial.available());
  reqVarByt = Serial.parseInt();
  Serial.print("Received: "); Serial.println(reqVarByt);

  Serial.println("Step 7: enter a response target offset, 0-2047");
  while(!Serial.available());
  reqVarOffset = Serial.parseInt();
  Serial.print("Received: "); Serial.println(reqVarOffset);

  Serial.println("Step 8: enter a response target table, 0-31");
  while(!Serial.available());
  reqVarBlk = Serial.parseInt();
  Serial.print("Received: "); Serial.println(reqVarBlk);

  uint32_t startTime = micros();
  
  reqHeader = reqHeader | ((reqTable >> 4) << 2); //isolate table bit 4, write to header bit 2
  reqHeader = reqHeader | ((reqTable & ~(16)) << 3); //isolate table bits 3-0, write to header bits 6-3
  reqHeader = reqHeader | (reqToID << 7); //write ToID bits 3-0 to header bits 10-7
  reqHeader = reqHeader | (reqFromID << 11); //write FromID bits 3-0 to header bits 14-11
  reqHeader = reqHeader | (reqMsgType << 15); //write MsgType bits 2-0 to header bits 17-15
  reqHeader = reqHeader | (reqOffset << 18); //write Offset bits 10-0 to header bits 28-18

  reqData2 = reqVarByt; //write varbyt bits 3-0 to reqData2 bits 3-0
  reqData2 = reqData2 | (reqVarOffset << 5); //write varoffset bits 2-0 to reqData2 bits 7-5
  reqData1 = reqVarOffset >> 3; //write varoffset bits 10-3 to reqData1 bits 7-0
  reqData0 = reqVarBlk; //write varblk bits 4-0 to reqData0 bits 4-0  
  
  recdVarByt = reqData2 & ~(0b11110000); //clear reqData2 bits 7-4
  recdVarOffset = reqData1; //set bits 7-0 of recdVarOffset to bits 7-0 of reqData1
  recdVarOffset = (recdVarOffset << 3) | (reqData2 >> 5); //left shift by 3, then append reqData2 bits 7-5 to recdOffset bits 2-0
  recdVarBlk = reqData0; //set recdVarBlk to reqData0
  recdTable =   (reqHeader & ~(0b11111111111111111111111111111011)) << 2; //set bit 4 of recdTable to bit 2 of reqHeader
  recdTable |=  (reqHeader & ~(0b11111111111111111111111110000111)) >> 3; //set bits 3-0 of recdTable to bits 6-3 of reqHeader
  recdToID =    (reqHeader & ~(0b11111111111111111111100001111111)) >> 7; //set recdToID to bits 10-7 of reqHeader
  recdFromID =  (reqHeader & ~(0b11111111111111111000011111111111)) >> 11; //set recdFromID to bits 14-11 of reqHeader
  recdMsgType = (reqHeader & ~(0b11111111111111000111111111111111)) >> 15; //set recdMsgType to bits 17-15 of reqHeader
  recdOffset =  (reqHeader & ~(0b11100000000000111111111111111111)) >> 18; //set recdOffset to bits 28-18 of reqHeader
  
  uint32_t duration = micros() - startTime;

  Serial.println("Assembled MegaSquirt Request Message: ");
  Serial.print(reqHeader); Serial.print("(dec) "); Serial.print(reqHeader, BIN); Serial.println("(bin)");
  Serial.print("Data 0: "); Serial.print(reqData0); Serial.print("(dec) "); Serial.print(reqData0, BIN); Serial.println("(bin)");
  Serial.print("Data 1: "); Serial.print(reqData1); Serial.print("(dec) "); Serial.print(reqData1, BIN); Serial.println("(bin)");
  Serial.print("Data 2: "); Serial.print(reqData2); Serial.print("(dec) "); Serial.print(reqData2, BIN); Serial.println("(bin)");

  Serial.println("Unpacking Spoofed Megasquirt Request...");
  Serial.print("Requested Table: "); Serial.println(recdTable);
  Serial.print("Request ToID: "); Serial.println(recdToID);
  Serial.print("Request FromID: "); Serial.println(recdFromID);
  Serial.print("Message Type: "); Serial.println(recdMsgType);
  Serial.print("Requested Offset: "); Serial.println(recdOffset);
  Serial.print("Requested Number of Bytes: "); Serial.println(recdVarByt);
  Serial.print("Requested Target Offset: "); Serial.println(recdVarOffset);
  Serial.print("Requested Target Table: "); Serial.println(recdVarBlk);

  Serial.print("Duration of calculations in useconds: "); Serial.println(duration);
}
