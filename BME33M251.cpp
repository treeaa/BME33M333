/*****************************************************************
File:             BME33M251.cpp
Author:           Weng
Description:      与传感器进行IIC通信并且获取相应的值
History：         
V0.0.1   -- initial version；2021-06-15；Arduino IDE : ≥v1.8.15
******************************************************************/
#include "BME33M251.h"

uint8_t DataBuff[12]={0,0,0,0,0,0,0,0,0,0,0,0};
/**********************************************************
Description: CRC校验
Input:       *buff：通过IIC获取到的数据
              len ： 获取数据的总长度    
Output:         
Return:      READ_OK / READ_ERROR / NO_ACK       
Others:      
**********************************************************/
 
uint16_t CRC_cheack(volatile unsigned char*buff,unsigned char len)
{
  uint16_t temp_crc=0xffff;
  uint8_t i;
  while(len--)
  {
    temp_crc ^= *buff++;
    for(i=0;i<8;i++)
    {
      if(temp_crc & 0x01)
      {
        temp_crc >>= 1;
        temp_crc ^= 0xa001;
      }
      else
      {
        temp_crc >>=1;
      }
    }
  }
  return temp_crc;
}
/*======================清除数据缓存=========================*/
void clearBuf()
{
  for(int a = 0; a < 12; a++)
  {
    DataBuff[a] = 0;
  } 
}
/**********************************************************
Description: IIC writeData
Input:       none
Output:      none
Return:      none      
Others:      
**********************************************************/
void writeData(uint8_t p[5], uint8_t len)
{
    Wire.beginTransmission(BME33M251_ADDR);      //IIC start with 7bit addr
    Wire.write(p, len);
    Wire.endTransmission();                  //IIC Stop
    delay(2);
}
/**********************************************************
Description: IIC readData
Input:       RequestLen :Length of data to be obtained
Output:      none
Return:      none      
Others:      
**********************************************************/
void readData(int RequestLen)
{
    int i = 0;
    Wire.requestFrom(BME33M251_ADDR, RequestLen);
    while(Wire.available()>0)
    {
         DataBuff[i] = Wire.read();
         i++;
    }
    i = 0;
}
void BME33M251::begin()
{
    Wire.begin(); 
    Wire.setClock(40000);           //IIC rate = 40kHz
    Serial.begin(9600);
}
/**********************************************************
Description: readHumidity
Input:       *humidity
Output:      none
Return:      READ_OK / READ_ERROR / NO_ACK       
Others:      
**********************************************************/
int BME33M251::readHumidity(float *humidity)
{
    clearBuf();
    uint8_t sendbuf[3] = {3,0,2};
    writeData(sendbuf, 3);
    readData(6);
    if(DataBuff[0] != 0x03) return NO_ACK;
    
    uint8_t CRC_chk = CRC_cheack(&DataBuff[0],8);
    if(CRC_chk == 0)                    //CRC校正成功
    {
      *humidity = (DataBuff[2]<<8)|DataBuff[3];
      *humidity = *humidity / 10;
      
     return READ_OK; 
    }
    else return READ_ERROR;
}
/**********************************************************
Description: readTemperature
Input:       *temperature
             isFahrenheit = true:Fahrenheit degree
             isFahrenheit = false:centigrade                         
Output:      none
Return:      READ_OK / READ_ERROR / NO_ACK       
Others:      
**********************************************************/
int BME33M251::readTemperature(float *temperature, bool isFahrenheit)
{
    clearBuf();
    uint8_t sendbuf[3] = {3,2,2};
    writeData(sendbuf, 3);
    readData(6);
    if(DataBuff[0] != 0x03) return NO_ACK;
    
    uint8_t CRC_chk = CRC_cheack(&DataBuff[0],8);
    if(CRC_chk == 0)                    //CRC校正成功
    {
      *temperature = (DataBuff[2]<<8)|DataBuff[3];
      if(isFahrenheit == true)
      {
           *temperature = *temperature*0.18 + 32;  
      }
      else *temperature = *temperature / 10;
      
     return READ_OK; 
    }
    else return READ_ERROR;
}

/**********************************************************
Description: 读取设备ID、版本号、序列号
Input:       *PID, *Ver, *SN
Output:      无       
Return:      READ_OK / READ_ERROR / NO_ACK       
Others:      
**********************************************************/
int BME33M251::getDviceInfo(uint16_t *PID, uint8_t *Ver, uint32_t *SN)
{
    uint8_t sendbuf[3] = {3,8,7};
    writeData(sendbuf, 3);
    readData(11);
    if(DataBuff[0] != 0x03) return NO_ACK;

    uint8_t CRC_chk = CRC_cheack(&DataBuff[0],11);
    if(CRC_chk == 0)                        //CRC校正成功
    {
       *PID = (DataBuff[2]<<8)|DataBuff[3];
       *Ver = DataBuff[4];
       *SN  = ((long)DataBuff[5]<<24)|((long)DataBuff[6]<<16)|((long)DataBuff[7]<<8)|DataBuff[8];
       return READ_OK; 
    }
    else return READ_ERROR; 
}
