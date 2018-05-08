/*
  SD card datalogger
 
 This example shows how to log data from three analog sensors 
 to an SD card using the SD library.
  
 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 
 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
   
 */

#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include "DS18B20_S.h"
#include <LiquidCrystal_I2C.h>

#define DEBUG_SERIAL 0//调试信息，发送到串口
#define PRINTF_TO_SERIAL 1//正常工作时将调试信息，发送到串口

char Title[150] = "Number,Date,Time,Press1,Press2,Voltage,Current,Power,Power_consumption,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15";
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display 

/*DS18B20(byte pin)  连接管脚*/
//传感器设定为10位模式，每次转换时间<187.5ms,如果需要12位模式，请修改库文件of ds.set(0x7F);
#define DS_NUM 20
float ds[DS_NUM];//18B20存储
#define DS_START_PIN 22
DS18B20_S  Ds0(DS_START_PIN);//pin DS_START_PIN
DS18B20_S  Ds1(DS_START_PIN+1);//pin DS_START_PIN+1
DS18B20_S  Ds2(DS_START_PIN+2);//pin DS_START_PIN+2
DS18B20_S  Ds3(DS_START_PIN+3);//pin DS_START_PIN+3
DS18B20_S  Ds4(DS_START_PIN+4);//pin DS_START_PIN+4
DS18B20_S  Ds5(DS_START_PIN+5);//pin DS_START_PIN+5
DS18B20_S  Ds6(DS_START_PIN+6);//pin DS_START_PIN+6
DS18B20_S  Ds7(DS_START_PIN+7);//pin DS_START_PIN+7
DS18B20_S  Ds8(DS_START_PIN+8);//pin DS_START_PIN+8
DS18B20_S  Ds9(DS_START_PIN+9);//pin DS_START_PIN+9
DS18B20_S  Ds10(DS_START_PIN+10);//pin DS_START_PIN+10
DS18B20_S  Ds11(DS_START_PIN+11);//pin DS_START_PIN+11
DS18B20_S  Ds12(DS_START_PIN+12);//pin DS_START_PIN+12
DS18B20_S  Ds13(DS_START_PIN+13);//pin DS_START_PIN+13
DS18B20_S  Ds14(DS_START_PIN+14);//pin DS_START_PIN+14
DS18B20_S  Ds10(DS_START_PIN+15);//pin DS_START_PIN+10
DS18B20_S  Ds11(DS_START_PIN+16);//pin DS_START_PIN+11
DS18B20_S  Ds12(DS_START_PIN+17);//pin DS_START_PIN+12
DS18B20_S  Ds13(DS_START_PIN+18);//pin DS_START_PIN+13
DS18B20_S  Ds14(DS_START_PIN+19);//pin DS_START_PIN+14
/*时钟模块*/
RTC_DS1307 rtc;

char DateFlieName[20];//使用时间命名txt文件名

/*压力传感器*/
#define PRES_NUM 3//气压传感器个数
#define PRES_START_PIN 0//气压传感器开始的模拟量引脚

struct Power
{
  double voltage;//电压
  float current;//电流
  float power;//功率
  float power_consumption;//电量消耗
  float power_factor;//功率因素
  float carbon;//二氧化碳消耗
  float frequence;//频率
}Pow;


const int chipSelect = 10;//SD卡的使能引脚

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial1.begin(4800);
  
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
//  lcd.print("Hello, world!");  
  
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();
  //rtc.adjust(DateTime(__DATE__, __TIME__));//调整时间
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(__DATE__, __TIME__));
  }

  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(chipSelect, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
  File root = SD.open("/");
  printDirectory(root, 0);
  
  Serial.println("printDirectory done!");

  GetDateFlieName();//开机获取时间，用于命名文件，存放在DateFlieName中
  WriteToSD(Title,DateFlieName);//打印表头
  Serial.println(Title);

}


unsigned long int Count = 1;
unsigned long int index = 0;
void loop()
{
  char DataBuf[200]="\0";//用于保存一个周期读取的传感器数据 

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  /*
      send_data_str +="Pow.voltage:";
      dtostrf(Pow.voltage,3,3,tmp_databuf);//将float转换为数组
      send_data_str += tmp_databuf;//将数组转换为String
      send_data_str +=',';
*/

//  

  
  
  if(index%3 == 0)
  {
    LcdDisp();
   GetAllInfo(DataBuf); 
  }
  if(index%9 == 0)
  {
    WriteToSD(DataBuf,DateFlieName);
    Count ++;//序号++
  }
  delay(1000);
  index++;



  
  if(PRINTF_TO_SERIAL)//正常工作时将所有的数据打印到串口
    Serial.println(DataBuf);
}
void LcdDisp()
{
  //"Number,Date,Time,Press1,Press2,Voltage,Current,Power,Power_consumption,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15";
  static char page = 0;
  if(page == 0)
  { 
    lcd.clear();
    lcd.setCursor(0, 0);
    char temp_buf[50];
    DateTime now = rtc.now();
    snprintf(temp_buf,sizeof(temp_buf),"%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());    
    lcd.print(temp_buf);

    float pressure_f[20];
    GetPressure(pressure_f);
    lcd.setCursor(0, 1);
    lcd.print("P1:");
    lcd.print(pressure_f[0]);
    lcd.print("Mpa");
    lcd.print("P2:");
    lcd.print(pressure_f[1]);
    lcd.print("Mpa");

    lcd.setCursor(0, 2);
    lcd.print("P3:");
    lcd.print(pressure_f[2]);
    lcd.print("Mpa");
//    lcd.print("V:");
//    lcd.print(Pow.voltage);////电压
    lcd.print(" ");
    lcd.print("I:");
    lcd.print(Pow.current);//电流

        
    lcd.setCursor(0, 3);
    lcd.print("P:");
    lcd.print(Pow.power);//有功功率
    lcd.print(" ");
    lcd.print("Q:");
    lcd.print(Pow.power_consumption);//有功总电能

  }
  else if(page == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T1:");
    lcd.print(ds[0]);
    lcd.print(" ");
    lcd.print("T2:");
    lcd.print(ds[1]);
    lcd.print(" ");
  
    lcd.setCursor(0, 1);
    lcd.print("T3:");
    lcd.print(ds[2]);
    lcd.print(" ");
    lcd.print("T4:");
    lcd.print(ds[3]);
    lcd.print(" ");
  
    lcd.setCursor(0, 2);
    lcd.print("T5:");
    lcd.print(ds[4]);
    lcd.print(" ");
    lcd.print("T6:");
    lcd.print(ds[5]);
    lcd.print(" ");
  
    lcd.setCursor(0, 3);
    lcd.print("T7:");
    lcd.print(ds[6]);
    lcd.print(" ");
    lcd.print("T8:");
    lcd.print(ds[7]);
    lcd.print(" ");
  }
  else if(page == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T9:");
    lcd.print(ds[8]);
    lcd.print(" ");
    lcd.print("T10:");
    lcd.print(ds[9]);
 
    lcd.setCursor(0, 1);
    lcd.print("T11:");
    lcd.print(ds[10]);
    lcd.print(" ");
    lcd.print("T12:");
    lcd.print(ds[11]);
  
    lcd.setCursor(0, 2);
    lcd.print("T13:");
    lcd.print(ds[12]);
    lcd.print(" ");
    lcd.print("T14:");
    lcd.print(ds[13]);
  
    lcd.setCursor(0, 3);
    lcd.print("T15:");
    lcd.print(ds[14]);
    lcd.print(" ");
    lcd.print("T16:");
    lcd.print(ds[15]);
  }
  else if(page == 3)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T17:");
    lcd.print(ds[16]);
    lcd.print(" ");
    lcd.print("T18:");
    lcd.print(ds[17]);
 
    lcd.setCursor(0, 1);
    lcd.print("T19:");
    lcd.print(ds[18]);
    lcd.print(" ");
    lcd.print("T20:");
    lcd.print(ds[19]);
  
  }
/*   page=0;*/

  page++;
  if(page > 3)
   page = 0;

}

void GetAllInfo(char *buf)
{
  
  float pressure_f[PRES_NUM];
  char temp_buf[50];

  dtostrf(Count,10,0,temp_buf);//将float转换为数组//此条信息的序号
  strcat(buf,temp_buf);
  strcat(buf,",");//逗号，分割每条信息
  

  GetDate(temp_buf);//时间信息
  strcat(buf,temp_buf);
  strcat(buf,",");

  GetPressure(pressure_f);
  dtostrf(pressure_f[0],2,2,temp_buf);//将float转换为数组//P1 气压
  strcat(buf,temp_buf);
  strcat(buf,",");
  dtostrf(pressure_f[1],2,2,temp_buf);//将float转换为数组//P2 气压
  strcat(buf,temp_buf);
  strcat(buf,",");
  dtostrf(pressure_f[2],2,2,temp_buf);//将float转换为数组//P3 气压
  strcat(buf,temp_buf);
  strcat(buf,",");

  GetPowerInfo();//存放在Pow结构体中
  dtostrf(Pow.voltage,2,2,temp_buf);////电压
  strcat(buf,temp_buf);
  strcat(buf,",");
  dtostrf(Pow.current,2,2,temp_buf);//电流
  strcat(buf,temp_buf);
  strcat(buf,",");
  dtostrf(Pow.power,2,2,temp_buf);//有功功率
  strcat(buf,temp_buf);
  strcat(buf,",");  
  dtostrf(Pow.power_consumption,2,2,temp_buf);//有功总电能
  strcat(buf,temp_buf);
  strcat(buf,",");

  Get18B20(ds);
  for(int i =0;i<20;i++)
  {
    dtostrf(ds[i],3,2,temp_buf);//将float转换为数组//T1~T20
    strcat(buf,temp_buf);
    strcat(buf,",");
  }

}


void GetDate(char *datebuf)
{
    char temp_buf[50];
    DateTime now = rtc.now();
    snprintf(temp_buf,sizeof(temp_buf),"%04d-%02d-%02d,%02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());    
    strcpy(datebuf,temp_buf);
//    Serial.println(datebuf);
}

void GetDateFlieName(void)//保存在全局变量DateFlieName中
{
   DateTime now = rtc.now();
   snprintf(DateFlieName,sizeof(DateFlieName),"%02d%02d%02d%02d.txt", now.month(), now.day(), now.hour(), now.minute());    
}
void WriteToSD(char *databuf,char *filename)
{
  File dataFile = SD.open(filename, FILE_WRITE);


  // if the file is available, write to it:
  if (dataFile) 
  {
    dataFile.println(databuf);
    dataFile.close();
    // print to the serial port too:
    if(DEBUG_SERIAL)
     Serial.println(databuf);
  }  
  // if the file isn't open, pop up an error:
  else 
  {
    if(DEBUG_SERIAL)
    {
     Serial.print("error opening ");
     Serial.println(filename);
    }
  } 
}

/*获取温度信息*/
void Get18B20(float *ds_tmp)
{
  float temp;
  Ds0.start();//开始测量
  temp=Ds0.get();  
  if(temp<100) ds_tmp[0] = temp;

  Ds1.start();//开始测量
  temp=Ds1.get();  
  if(temp<100) ds_tmp[1] = temp;

  Ds2.start();//开始测量
  temp=Ds2.get();  
  if(temp<100) ds_tmp[2] = temp;

  Ds3.start();//开始测量
  temp=Ds3.get();  
  if(temp<100) ds_tmp[3] = temp;

  Ds4.start();//开始测量
  temp=Ds4.get();  
  if(temp<100) ds_tmp[4] = temp;

  Ds5.start();//开始测量
  temp=Ds5.get();  
  if(temp<100) ds_tmp[5] = temp;

  Ds6.start();//开始测量
  temp=Ds6.get();  
  if(temp<100) ds_tmp[6] = temp;

  Ds7.start();//开始测量
  temp=Ds7.get();  
  if(temp<100) ds_tmp[7] = temp;

  Ds8.start();//开始测量
  temp=Ds8.get();  
  if(temp<100) ds_tmp[8] = temp;

  Ds9.start();//开始测量
  temp=Ds9.get();  
  if(temp<100) ds_tmp[9] = temp;

  Ds10.start();//开始测量
  temp=Ds10.get();  
  if(temp<100) ds_tmp[10] = temp;

  Ds11.start();//开始测量
  temp=Ds11.get();  
  if(temp<100) ds_tmp[11] = temp;

  Ds12.start();//开始测量
  temp=Ds12.get();  
  if(temp<100) ds_tmp[12] = temp;

  Ds13.start();//开始测量
  temp=Ds13.get();  
  if(temp<100) ds_tmp[13] = temp;

  Ds14.start();//开始测量
  temp=Ds14.get();  
  if(temp<100) ds_tmp[14] = temp;

  Ds10.start();//开始测量
  temp=Ds15.get();  
  if(temp<100) ds_tmp[15] = temp;

  Ds11.start();//开始测量
  temp=Ds16.get();  
  if(temp<100) ds_tmp[16] = temp;

  Ds12.start();//开始测量
  temp=Ds17.get();  
  if(temp<100) ds_tmp[17] = temp;

  Ds13.start();//开始测量
  temp=Ds18.get();  
  if(temp<100) ds_tmp[18] = temp;

  Ds14.start();//开始测量
  temp=Ds19.get();  
  if(temp<100) ds_tmp[19] = temp;

}

/*获取气压信息*/
void GetPressure(float *pres_mpa)//从 PRES_START_PIN 开始，获取 PRES_NUM 个气压传感器的模拟量值；待处理
{
  float temp_f;
  for (int analogPin = PRES_START_PIN; analogPin < PRES_NUM; analogPin++) 
  {
     temp_f = analogRead(analogPin);
     pres_mpa[analogPin]= temp_f/102.4;//MPa  0~5V   0~10MPa
   }  
}



unsigned char HeadGegin = 0;

/*获取用电信息*/
void GetPowerInfo(void)
{
  char tmp_databuf[50];
  String send_data_str;//待发送的string类型数据
  unsigned long int temp_long;
  char s_buf[]={0x01,0x03,0x00,0x48,0x00,0x06,0x45,0xDE};
  unsigned char buf[1024];
   int index = 0;
  for(char i = 0;i <8;i++)
  {
    Serial1.write(s_buf[i]);//发送读取命令 
  }
  
  delay(300);

  while (Serial1.available()) //返回 01 03 20 *******
  {
        
    // get the new byte:
    unsigned char inChar = (unsigned char)Serial1.read();
    //Serial.write(inChar);
    if(!HeadGegin)//接收包头
    {
      if(inChar == 0x01)
      {
        buf[index++] = inChar;
        inChar = (unsigned char)Serial1.read();
        if(inChar == 0x03)
        {
          buf[index++] = inChar;
          inChar = (unsigned char)Serial1.read();
          if(inChar == 0x0C)
          {
            buf[index++] = inChar;
            HeadGegin = 1;
          }
        }
      }

    }
    else//开始接收有效数据
    {
      buf[index++] = inChar;  
    }
  } 
Serial.print("index:");Serial.println(index);//发送数据
  if(index == 17)//17个数据  12+5
  {
    temp_long = buf[3]*256 + buf[4];//电压
    Pow.voltage = (float)(temp_long/100.0);

    temp_long = buf[5]*256 + buf[6];//电流
    Pow.current = (float)(temp_long/1000.0);

    temp_long = buf[7]*256 + buf[8];//有功功率
    Pow.power = (float)(temp_long);


    temp_long = buf[9]*16777216 + buf[10]*65536+ buf[11]*256 + buf[12];//有功总电能
    Pow.power_consumption = (float)(temp_long/3200.0);

    temp_long = buf[13]*256 + buf[14];//功率因数
    Pow.power_factor = (float)(temp_long/1000.0);

    
//打印数据到串口
  if(DEBUG_SERIAL)
  {
      send_data_str +="Pow.voltage:";
      dtostrf(Pow.voltage,3,3,tmp_databuf);//将float转换为数组
      send_data_str += tmp_databuf;//将数组转换为String
      send_data_str +=',';
  
      send_data_str +="Pow.current:";
      dtostrf(Pow.current,3,3,tmp_databuf);//将float转换为数组
      send_data_str += tmp_databuf;//将数组转换为String
      send_data_str +=',';
  
      send_data_str +="Pow.power:";
      dtostrf(Pow.power,3,3,tmp_databuf);//将float转换为数组
      send_data_str += tmp_databuf;//将数组转换为String
      send_data_str +=',';
  
      send_data_str +="Pow.power_consumption:";
      dtostrf(Pow.power_consumption,3,3,tmp_databuf);//将float转换为数组/有功总电能
      send_data_str += tmp_databuf;//将数组转换为String
      send_data_str +=',';
  
      send_data_str +="Pow.power_factor:";
      dtostrf(Pow.power_factor,3,3,tmp_databuf);//将float转换为数组//功率因数
      send_data_str += tmp_databuf;//将数组转换为String
  
      Serial.println(send_data_str);//发送数据
  }
  }


}





void printDirectory(File dir, int numTabs) {
   while(true) 
   {
     
     File entry =  dir.openNextFile();
     if (! entry) 
     {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) 
     {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) 
     {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else 
     {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}

/*CRC check*/
typedef union _CRC
{
  unsigned short crc16 ;
  unsigned char by [ 2];
} CRC ;

//输入不带CRC码的数据时，返回值是CRC码
//输入带CRC码的数据时，则可以进行校验，返回0时CRC校验成功，否则CRC校验失败
unsigned short CRC16(unsigned char *ba,int size)
{
  CRC crc;
  crc.crc16=0xffff;
  int i,l;
  for(i=0;i<size;i++)
    {
      unsigned char ch=ba[i];
      crc.by[0]=crc.by[0]^ch;
      for(l=0;l<8;l++)
      {
        if(crc.by[0]&0x01)
        {
          crc.crc16=crc.crc16>>1;
          crc.crc16=crc.crc16^0xa001;
        }
        else
        {
          crc.crc16=crc.crc16>>1;
        }
      }
    }
  unsigned char swap=crc.by[0];
  crc.by[0]=crc.by[1];
  crc.by[1]=swap;
  return crc.crc16;
}









