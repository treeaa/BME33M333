/*****************************************************************
// FILE: readTemperatureAndHumidityWithIIC
// VERSION: 0.0.1
// PURPOSE: Bme33m251 Temperature & Humidity Sensor library for Arduino
// PIN:(default)SCL SDA VCC GND
// HISTORY:
******************************************************************/

#include "Bme33m251.h"
BME33M251 Bme33m251;

float     humidity, temperature; 
uint16_t  savePID;
uint8_t   saveVer;
uint32_t  saveSN;

void setup() {
  
    Bme33m251.begin();             
    uint8_t chk1 = Bme33m251.getDviceInfo(&savePID, &saveVer, &saveSN);//上电获取一次设备信息，之后不再获取
    switch(chk1)
    {
        case READ_OK   :break;
        case READ_ERROR:Serial.println("Read sensor: CRC error ");Serial.println("Waiting for reconnection...");break;
        case NO_ACK    :Serial.println("Read sensor: NO_ACK ");Serial.println("Waiting for reconnection...");break;
        default        :Serial.println("Read sensor: Unknown error ");Serial.println("Please check connection...");break;
    }
    Serial.println();
    while(chk1 != READ_OK)        
    {
      chk1 = Bme33m251.getDviceInfo(&savePID, &saveVer, &saveSN);//获取设备信息
      delay(1000);
    };
    if(chk1 == READ_OK)
    {
      Serial.println(" Read sensor: OK ");
      Serial.println("------Device_Info------- "); 
      Serial.print("PID : ");
      Serial.println(savePID,HEX);
      Serial.print("Version : ");
      Serial.println(saveVer,HEX);
      Serial.print("Serial_Num : ");
      Serial.println(saveSN,HEX); 
      Serial.println("------------------------ ");
    } 
    delay(1000);
}

void loop() {
    if(Bme33m251.readHumidity(&humidity) == READ_OK)
    {
        Serial.print("Humidity : ");
        Serial.print(humidity);
        Serial.print(" %    ");
    }
    if(Bme33m251.readTemperature(&temperature, true) == READ_OK)   //true:Fahrenheit degree; false:centigrade
    {
        Serial.print("Temperature : ");
        Serial.print(temperature);
        Serial.println(" °F ");
    }
    delay(1000);
}
