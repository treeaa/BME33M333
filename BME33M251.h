/*****************************************************************
File:             BME33M251.h
Author:           Weng
Description:      定义类与需要的变量
History：         
V0.0.1-- initial version；2021-06-15；Arduino IDE : ≥v1.8.15
******************************************************************/
#ifndef BME33M251_h
#define BME33M251_h
#include <Wire.h>
#include <Arduino.h>
class BME33M251
{
  public:
   void begin(); 
   int readTemperature(float *temperature, bool isFahrenheit);
   int readHumidity(float *humidity);
   int getDviceInfo(uint16_t *PID, uint8_t *Ver, uint32_t *SN);    //获取传感器信息，ID/Ver/SN

};
unsigned int CRC_cheack(volatile unsigned char *buff,unsigned char len);

#define READ_OK         0
#define READ_ERROR      1
#define NO_ACK          2
#define BME33M251_ADDR  0x5C

#endif
