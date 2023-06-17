void loadsms(void)
{
   Serial1.begin(9600);
  //Serial.println("Initializing...");
  delay(100);
  Serial1.println("AT"); //Once the handshake test is successful, it will back to OK
  //updateSerial();
  delay(100);
  Serial1.println("AT+CSMP=17,167,0,0");
  delay(100);
  Serial1.println("AT+CMGF=1"); // Configuring TEXT mode
  // updateSerial();
  delay(100);
  
    //Serial1.println("AT+CMGS=\"+254732532374\"");
    // Serial1.println("AT+CMGS=\"+254713583803\"");
      Serial1.println("AT+CMGS=\"+254704325949\"");
     // Serial1.println("AT+CMGS=\"+256775646496\""); 
  delay(100);
  Serial1.println("load connected");
  
  
Serial1.println((char)26); // End AT command with a ^Z, ASCII code 26
delay(200);
Serial.println("load connected");

}

void load_disconnected_sms(void)
{
   Serial1.begin(9600);
  //Serial.println("Initializing...");
  delay(100);
  Serial1.println("AT"); //Once the handshake test is successful, it will back to OK
  //updateSerial();
  delay(100);
  Serial1.println("AT+CSMP=17,167,0,0");
  delay(100);
  Serial1.println("AT+CMGF=1"); // Configuring TEXT mode
  // updateSerial();
  delay(100);
  
    //Serial1.println("AT+CMGS=\"+254732532374\"");
    // Serial1.println("AT+CMGS=\"+254713583803\"");
      Serial1.println("AT+CMGS=\"+254704325949\"");
     // Serial1.println("AT+CMGS=\"+256775646496\""); 
  delay(1000);
  Serial1.println("load diconnected");
  
  
Serial1.println((char)26); // End AT command with a ^Z, ASCII code 26
delay(200);
Serial.println("load diconnected");

}
