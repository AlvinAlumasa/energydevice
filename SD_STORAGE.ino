void SDstorage(void){
   File myFile = SD.open("PrimeBIO.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to PrimeBIO.txt...");
   
    myFile.print("Current=");       
  myFile.print(rmsVals.CurrentRMSValue/1000); 
  myFile.print("A"); 
  myFile.print(","); 
  myFile.print("Voltage=");
  delay(200);  
        
  myFile.print(rmsVals.VoltageRMSValue/1000); 
  
  myFile.print("V");
  myFile.print(","); 
 
  delay(200);  
  
  myFile.print("Active Power="); 
 myFile.print(powerVals.ActivePowerValue/1000);
 myFile.print("W");
  myFile.print(","); 
 delay(200);  
  
   
  myFile.print("Reactive Power=");
 myFile.print(powerVals.FundReactivePowerValue/1000);
 myFile.print("VAR");
 myFile.print(","); 
 delay(200);  
  
  
   
  myFile.print("Apparent Power="); 
 myFile.print(powerVals.ApparentPowerValue/1000);
 myFile.print("VA");
 myFile.print(","); 
 delay(200);  

  
   
  myFile.print("Power Factor="); 
 myFile.print(pqVals.PowerFactorValue);
 myFile.print(","); 
 delay(200);  
   
  
   
  myFile.print("Frequency="); 
  myFile.print(pqVals.FrequencyValue);
   myFile.print("Hz");
   myFile.print(","); 
 
   delay(200);  
  
  myFile.print("Temperature=");  
 myFile.print(tempVal.TemperatureVal);
 myFile.println("degC");
delay(200);  
 myFile .print("http://maps.google.com/maps?q=loc:");
myFile.print(latGPS, 6);
myFile.print(",");
myFile.println (longGPS, 6);
delay(200);
  
 myFile.println("");
  myFile.println(""); 

    delay(200);  
    // close the file:
    myFile.close();
   delay(2000);  
 
     sd_sms();
     delay(1000);
  } 
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening PrimeBIO.txt");
     sd_sms2();
     delay(1000);
  }

  
}
