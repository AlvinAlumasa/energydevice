#include <DFRobot_SIM808.h>
#include <sim808.h>

 #include <SD.h>

#include <ADE9153A.h>
#include <ADE9153AAPI.h>


#define ARM_MATH_CM0PLUS

#include <SPI.h>
#include  <ADE9153A.h>
#include <ADE9153AAPI.h>


/* Basic initializations */
#define SPI_SPEED 1000000     //SPI Speed
#define CS1_PIN PIN_PB0              //8-->Arduino Zero. 15-->ESP8266 
#define ADE9153A_RESET_PIN PIN_PC0//On-board Reset Pin
 #define CS2_PIN PIN_PB4


ADE9153AClass ade9153A;
struct EnergyRegs energyVals;  //Energy register values are read and stored in EnergyRegs structure
struct PowerRegs powerVals;    //Metrology data can be accessed from these structures
struct RMSRegs rmsVals;  
struct PQRegs pqVals;
struct AcalRegs acalVals;
struct Temperature tempVal;
void loadsms(void);
void powersms(void);
void load_disconnected_sms(void);
void plug_power_sms(void);

void plug_no_powersms(void);
void device_disconnected_sms(void);
void GPSsms(void);
void SDstorage(void);
void disconectGPS(void);
void readandwrite(void);
void sd_sms(void);
void sd_sms2(void);

void resetADE9153A(void);

int loadbutton=PIN_PC5; // brown and white cables
int powerbutton= PIN_PC4; // brown and yellow cables
int loadled=PIN_PA5;//LED D4 on the pcb
int powerled=PIN_PA6;//LED D3 on the pcb
unsigned long lastReport = 0;
unsigned long lastReport_1=0;
const long reportInterval = 50000;
const long reportInterval_1 = 180000;
char frame[100];
byte GNSSrunstatus;;
byte Fixstatus;
char UTCdatetime[18];
char latitude[11];
char logitude[11];
char altitude[8];
char speedOTG[6];
char course[6];
byte fixmode;
char HDOP[4];
char PDOP[4];
char VDOP[4];
char satellitesinview[2];
char GNSSsatellitesused[2];
char GLONASSsatellitesused[2];
char cn0max[2];
char HPA[6];
char VPA[6];
float latGPS;
float longGPS;
void plug_no_power_sms(void);
int i=1;
void GetGPS(); 
  DFRobot_SIM808 sim808(&Serial1);

void setup() {
  /* Pin and serial monitor setup */
  pinMode(ADE9153A_RESET_PIN, OUTPUT);
  pinMode(loadbutton,INPUT); 
  pinMode (powerbutton,INPUT);
  pinMode (loadled,OUTPUT); //LED D4 on the pcb
  pinMode( powerled,OUTPUT); //LED D3 on the pcb
  digitalWrite(ADE9153A_RESET_PIN, HIGH);  
  Serial.begin(9600);
  Serial1.begin(9600);
  
  
 
   Serial1.println("AT+CSMP=17,167,0,0");
  delay(1000);
  Serial1.println("AT+CMGF=1");
  delay(1000);
  //Serial1.println("AT+CGNSPWR=0");
//delay(400);

  resetADE9153A();            //Reset ADE9153A for clean startup
  delay(1000);
  /*SPI initialization and test*/
  bool commscheck = ade9153A.SPI_Init(SPI_SPEED,CS1_PIN); //Initialize SPI
  if (!commscheck) {
    Serial.println("ADE9153A Shield not detected. Plug in Shield and reset the Arduino");
    while (!commscheck) {     //Hold until arduino is reset
      delay(1000);
    }
  }
   ade9153A.SetupADE9153A(); //Setup ADE9153A according to ADE9153AAPI.h
  /* Read and Print Specific Register using ADE9153A SPI Library */
  Serial.println(String(ade9153A.SPI_Read_32(REG_VERSION_PRODUCT), HEX)); // Version of IC
  ade9153A.SPI_Write_32(REG_AIGAIN, -268435456); //AIGAIN to -1 to account for IAP-IAN swap
  delay(500);
 
  if (SD.begin(CS2_PIN)) {
   Serial.println("initialization done."); 
  } 
  if( sim808.attachGPS())
      Serial.println("Open the GPS power success");
  else 
      Serial.println("Open the GPS power failure");
   
}

void loop() {
    byte buttonState1=digitalRead(loadbutton);// brown and white cables
    byte buttonState2=digitalRead(powerbutton); // brown and yellow cables
  if(buttonState1==1){
     digitalWrite(loadled,HIGH); //LED D4 on the pcb
       loadsms();
       delay(1000);
       Serial.println("load connected");
    delay(100);
    }
  else{
    digitalWrite(loadled,LOW);//LED D4 on the pcb
  
     delay(1000);
    Serial.println("load disconnected");
  }
  if(buttonState2==1){

  
   
  digitalWrite(powerled,HIGH);// brown and yellow cables
    
  
    readandwrite();
    delay(1000);
  
     
     float volts=  rmsVals.VoltageRMSValue/1000;
    if(volts<180){
      plug_no_power_sms();
      delay(100);
    }
    if(volts>180){
       plug_power_sms();
       delay(100);
    }
   
    powersms();
    delay(5000);
    
    SDstorage();
   delay(5000);
   Serial.println("device connected");
     
 
  }
  //if(buttonState2==0)
  else 
  {
    
     //digitalWrite(powerled,LOW);// brown and yellow cables
    // device_disconnected_sms();
    //delay(5000);
    // GetGPS();

   // delay(10000);
    sim808.getGPS();
   
    Serial.print(sim808.GPSdata.year);
    Serial.print("/");
    Serial.print(sim808.GPSdata.month);
    Serial.print("/");
    Serial.print(sim808.GPSdata.day);
    Serial.print(" ");
    Serial.print(sim808.GPSdata.hour);
    Serial.print(":");
    Serial.print(sim808.GPSdata.minute);
    Serial.print(":");
    Serial.print(sim808.GPSdata.second);
    Serial.print(":");
    Serial.println(sim808.GPSdata.centisecond);
    
    Serial.print("latitude :");
    Serial.println(sim808.GPSdata.lat,6);
    
    sim808.latitudeConverToDMS();
    Serial.print("latitude :");
    Serial.print(sim808.latDMS.degrees);
    Serial.print("\^");
    Serial.print(sim808.latDMS.minutes);
    Serial.print("\'");
    Serial.print(sim808.latDMS.seconeds,6);
    Serial.println("\"");
    Serial.print("longitude :");
    Serial.println(sim808.GPSdata.lon,6);
    sim808.LongitudeConverToDMS();
    Serial.print("longitude :");
    Serial.print(sim808.longDMS.degrees);
    Serial.print("\^");
    Serial.print(sim808.longDMS.minutes);
    Serial.print("\'");
    Serial.print(sim808.longDMS.seconeds,6);
    Serial.println("\"");
    delay(3000);
/*
    //Serial1.println("AT+CMGS=\"+254732532374\"");
    // Serial1.println("AT+CMGS=\"+254713583803\"");
      Serial1.println("AT+CMGS=\"+254704325949\"");
     // Serial1.println("AT+CMGS=\"+256775646496\""); 
  delay(100);

 Serial1.print("http://maps.google.com/maps?q=loc:");
Serial1.print(sim808.GPSdata.lat,6);
Serial1.print(",");
Serial1.print (sim808.GPSdata.lon,6);
delay(200);
Serial1.println((char)26); // End AT command with a ^Z, ASCII code 26
delay(10000);
   
/*   else
   
     Serial.println("gps connecting...");
   
   
   */
   
   
    
   
}
}

void readandwrite()
{ 
 /* Read and Print WATT Register using ADE9153A Read Library */
 
  ade9153A.ReadPowerRegs(&powerVals);    //Template to read Power registers from ADE9000 and store data in Arduino MCU
  ade9153A.ReadRMSRegs(&rmsVals);
  ade9153A.ReadPQRegs(&pqVals);
  ade9153A.ReadTemperature(&tempVal);
  
  Serial.print("Current=");       
  Serial.print(rmsVals.CurrentRMSValue/1000); 
  Serial.println("A"); 
  
  Serial.print("Voltage=");  
        
  Serial.print(rmsVals.VoltageRMSValue/1000); 
  
  Serial.println("V");
 
  
  
  Serial.print("Active Power="); 
 Serial.print(powerVals.ActivePowerValue/1000);
 Serial.println("W");
  
  
   
  Serial.print("Reactive Power=");
 Serial.print(powerVals.FundReactivePowerValue/1000);
 Serial.println("VAR");
  
  
   
  Serial.print("Apparent Power="); 
 Serial.print(powerVals.ApparentPowerValue/1000);
 Serial.println("VA");

  
   
  Serial.print("Power Factor="); 
 Serial.println(pqVals.PowerFactorValue);
   
  
   
  Serial.print("Frequency="); 
  Serial.print(pqVals.FrequencyValue);
   Serial.println("Hz");
   
  
  Serial.print("Temperature=");  
 Serial.print(tempVal.TemperatureVal);
 Serial.println("degC");
  
  Serial.println("");
  Serial.println("");
}
void resetADE9153A(void){
  

 digitalWrite(ADE9153A_RESET_PIN, LOW);
 delay(100);
 digitalWrite(ADE9153A_RESET_PIN, HIGH);
 delay(1000);
 Serial.println("Reset Done");
}
/*void GPSsms()
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
     //Serial1.println("AT+CMGS=\"+254713583803\"");
     Serial1.println("AT+CMGS=\"+254704325949\"");
     //Serial1.println("AT+CMGS=\"+256775646496\""); 
     
  delay(100);
  Serial1.print("http://maps.google.com/maps?q=loc:");
Serial1.print(latGPS, 6);
Serial1.print(",");
Serial1.println (longGPS, 6);
delay(200);
  
  
Serial1.println((char)26); // End AT command with a ^Z, ASCII code 26
delay(1000);

}*/
void device_disconnected_sms()
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
  
   // Serial1.println("AT+CMGS=\"+254732532374\"");
     //Serial1.println("AT+CMGS=\"+254713583803\"");
     Serial1.println("AT+CMGS=\"+254704325949\"");
    // Serial1.println("AT+CMGS=\"+256775646496\""); 
  delay(1000);
  Serial1.println("device diconnected");
  
  
Serial1.println((char)26); // End AT command with a ^Z, ASCII code 26
delay(200);
Serial.println("device diconnected");
}
void plug_no_power_sms(void){

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
  
   // Serial1.println("AT+CMGS=\"+254732532374\"");
    // Serial1.println("AT+CMGS=\"+254713583803\"");
     Serial1.println("AT+CMGS=\"+254704325949\"");
    // Serial1.println("AT+CMGS=\"+256775646496\""); 
  delay(1000);
  Serial1.println("device plugged, no power");
  
  
Serial1.println((char)26); // End AT command with a ^Z, ASCII code 26
delay(200);
Serial.println("device plugged, no power");

}
