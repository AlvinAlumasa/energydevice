void powersms(void)
{
   Serial1.begin(9600);
  //Serial.println("Initializing...");
  delay(1000);
  Serial1.println("AT"); //Once the handshake test is successful, it will back to OK
  //updateSerial();
  delay(1000);
  Serial1.println("AT+CSMP=17,167,0,0");
  delay(1000);
  Serial1.println("AT+CMGF=1"); // Configuring TEXT mode
  // updateSerial();
  delay(1000);
   
    //Serial1.println("AT+CMGS=\"+254732532374\"");
     //Serial1.println("AT+CMGS=\"+254713583803\"");
      Serial1.println("AT+CMGS=\"+254704325949\"");
      //Serial1.println("AT+CMGS=\"+256775646496\""); 
  delay(1000);
  
   Serial1.print("Current=");       
  Serial1.print(rmsVals.CurrentRMSValue/1000); 
  Serial1.println("A"); 
  
  Serial1.print("Voltage=");  
        
  Serial1.print(rmsVals.VoltageRMSValue/1000); 
  
  Serial1.println("V");
 
  
  
  Serial1.print("Active Power="); 
 Serial1.print(powerVals.ActivePowerValue/1000);
 Serial1.println("W");
  
  
   
  Serial1.print("Reactive Power=");
 Serial1.print(powerVals.FundReactivePowerValue/1000);
 Serial1.println("VAR");
  
  
   
  Serial1.print("Apparent Power="); 
 Serial1.print(powerVals.ApparentPowerValue/1000);
 Serial1.println("VA");

  
   
  Serial1.print("Power Factor="); 
 Serial1.println(pqVals.PowerFactorValue);
   
  
   
  Serial1.print("Frequency="); 
  Serial1.print(pqVals.FrequencyValue);
   Serial1.println("Hz");
   
  
  Serial1.println("");
  Serial1.println("");
  
  

  
Serial.println("data message");
  Serial1.println((char)26); // End AT command with a ^Z, ASCII code 26
delay(1000);
  
}
void plug_power_sms(void)
{
   Serial1.begin(9600);
  //Serial.println("Initializing...");
  delay(1000);
  Serial1.println("AT"); //Once the handshake test is successful, it will back to OK
  //updateSerial();
  delay(1000);
  Serial1.println("AT+CSMP=17,167,0,0");
  delay(1000);
  Serial1.println("AT+CMGF=1"); // Configuring TEXT mode
  // updateSerial();
  delay(1000);
  
    //Serial1.println("AT+CMGS=\"+254732532374\"");
     //Serial1.println("AT+CMGS=\"+254713583803\"");
      Serial1.println("AT+CMGS=\"+254704325949\"");
      //Serial1.println("AT+CMGS=\"+256775646496\""); 
  delay(1000);
  Serial1.println("device plugged, power available");
  
  
Serial1.println((char)26); // End AT command with a ^Z, ASCII code 26
delay(200);
Serial.println("device plugged no power");

}
