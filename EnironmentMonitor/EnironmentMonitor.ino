#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include "DS18B20_S.h"
#include <DS3231.h>
#include <SoftwareSerial.h>  //引用库文件
#include <dht11.h>
#include "C:\Users\I327500\MyWork_PXQ\other project\OurHome\EnvironmentMonitor\EnironmentMonitor\hz\hz.c"
#include "LCD.h"
#include "Light.h"
#include "AQM300.h"
#include "BME280.h"
#include "GP2Y10.h"


#define OXY_SERIAL Serial1

#define DEBUG_SERIAL 1//调试信息，发送到串口
#define PRINTF_TO_SERIAL 1//正常工作时将调试信息，发送到串口

//0,2018-05-12 12:11:50,0.0817,24,25.51,48,     5,1008.78
char Title[200] = "Number,Time,Oxy,Temperature1,Temperature2,Huminity,Lux,Atmospheric Pressure,";

/*DS18B20(byte pin)  连接管脚*/
//传感器设定为10位模式，每次转换时间<187.5ms,如果需要12位模式，请修改库文件of ds.set(0x7F);
#define DS_PIN 4
DS18B20_S Ds0(DS_PIN);  //pin DS_PIN

/*湿度传感器*/
dht11 DHT11;
#define DHT_PIN 11//湿度传感器

/*时钟模块*/
DS3231 Clock;

char* DateFlieName="05031230.txt";  //使用时间命名txt文件名 !!!!! <<<<<Length liminit>>!!!!
//char* DateFlieName="test20180512112030.txt";  //使用时间命名txt文件名
const int chipSelect = 10;  //SD卡的使能引脚
File myFile;
boolean bCardPresented = true;

void setup()
{
	// Open serial communications and wait for port to open:
	Serial.begin(115200);
	LCD_SERIAL.begin(9600);  //  For LCD(Txd) and ???(Rxd)
	//OXY_SERIAL.begin(9600);//  For LCD(Txd) and ???(Rxd)

	//Lcd init
	setLcdDirection();
	clearDisp();

	//Clock init
	Wire.begin();
	//setClock();

	Serial.print("Initializing SD card...");
	// make sure that the default chip select pin is set to
	// output, even if you don't use it:
	//pinMode(chipSelect, OUTPUT);

	//Voc Pin init
	initVocSensor();

	//Pressure init
	initBME280();

	//Dust sensor
	initGP2Y10();

	// see if the card is present and can be initialized:
	if (!SD.begin(chipSelect))
	{
		Serial.println("Card failed, or not present");
		// don't do anything more:
		bCardPresented = false;
	}

	printDirectory(SD.open("/"), 0);

	Serial.println("printDirectory done!");

	//开机获取时间，用于命名文件，存放在DateFlieName中
        if(bCardPresented)
        {
          getDateFlieName();
       	  Serial.println(DateFlieName);
          writeToSD(getMetadata(), DateFlieName);  //打印表头
  	  writeToSD(Title, DateFlieName);  //打印表头
  	  Serial.println(Title);
        }
}
unsigned long int index = 0;
unsigned long int recordIndex = 0;
String strWriteToSd="";
void loop()
{
	if (index % 3 == 0)
	{  
                strWriteToSd = displayAndGetStoreageString(recordIndex);
		Serial.println(strWriteToSd);
	}

        if(index%10 == 0)
        {
          recordIndex++;
          writeToSD(strWriteToSd, DateFlieName);//Write all data to sd 
        }
        delay(100);	  
        index++;
}

String displayAndGetStoreageString(unsigned long int storeageCounter)
{
	char temp_buf[50]="";
        char oxy_buf[50]="0";
        char dust_buf[10]="0";
	String tempStr = "";
	String dataWriteToFileStr = "";

	// logo
	LcdDisplay(LOGO_X, LOGO_Y, 48, hz[LOGO_HZ_INDEX], 10);
        //Crrent File name
        if(bCardPresented)
          LcdDisplay(FILENAME_X, FILENAME_Y, 24, DateFlieName, 16);
        else
          LcdDisplay(FILENAME_X, FILENAME_Y, 24, hz[FILENAME_HZ_INDEX], 1);
	//(室内安全范围密闭1小时<0.08mg/m³)
	LcdDisplay(NOTE_X, NOTE_Y, 24, hz[NOTE_HZ_INDEX], 16);

	// storeageCountere: display and storeage.
	LcdDisplay(COUNT_X, COUNT_Y, 24, hz[COUNT_HZ_INDEX], 12);	// 序
	dtostrf(storeageCounter, 10, 0, temp_buf);
	LcdDisplay(COUNT_X + 60, COUNT_Y, 24, temp_buf, 12);  // storeageCounter
	dataWriteToFileStr += storeageCounter;  //String data write to file.
	dataWriteToFileStr += ",";

	//Time: display and storeage.
	tempStr = getTime();
	LcdDisplay(TIME_X, TIME_Y, 48, hz[1], 4);  // 时间
	LcdDisplay(TIME_X + 120, TIME_Y, 48, tempStr, 2);  // 时间
	dataWriteToFileStr += tempStr;  //String data write to file.
	dataWriteToFileStr += ",";

	//甲醛浓度: display and storeage.
	LcdDisplay(OXY_X, OXY_Y, 48, hz[OXY_HZ_INDEX], 1);
	float oxy = getOxymethylene();
//	Serial.print("oxy:");
//	Serial.println(oxy); //发送数据
	if (oxy > 0.0)
	{
		dtostrf(oxy, 1, 4, oxy_buf); //将float转换为数组
	}
        LcdDisplay(OXY_X + 150, OXY_Y, 48, oxy_buf, 12); // oxy
	LcdDisplay(OXY_X + 150 + 170, OXY_Y, 48, hz[OXY_HZ_INDEX + 1], 1); // mg/m³
	dataWriteToFileStr += oxy_buf;  //String data write to file.
	dataWriteToFileStr += ",";

	//Temprature: display and storeage.
	int chk = DHT11.read(DHT_PIN);                 //将读取到的值赋给chk
	LcdDisplay(TEMP_X, TEMP_Y, 48, hz[TEMP_HZ_INDEX], 1);              //温度：
	dtostrf(DHT11.temperature, 2, 0, temp_buf);
	LcdDisplay(TEMP_X + 150, TEMP_Y, 48, temp_buf, 12); // DHT11.temperature
	LcdDisplay(TEMP_X + 150 + 80, TEMP_Y, 48, hz[TEMP_HZ_INDEX + 1], 1); //C
	dataWriteToFileStr += temp_buf;  //String data write to file.
	dataWriteToFileStr += ",";
	//BME280 Temperature
	dtostrf(getTemperatureBME280(), 2, 2, temp_buf);
	LcdDisplay(TEMP_X + 150 + 80 + 80, TEMP_Y, 24, temp_buf, 16); //BME280 Temperature
	dataWriteToFileStr += temp_buf;  //String data write to file.
	dataWriteToFileStr += ",";

        //Huminity
	LcdDisplay(HUMI_X, HUMI_Y, 48, hz[HUMI_HZ_INDEX], 1);              //湿度：
	dtostrf(DHT11.humidity, 2, 0, temp_buf);
	LcdDisplay(HUMI_X + 150, HUMI_Y, 48, temp_buf, 12);    // DHT11.humidity
	LcdDisplay(HUMI_X + 150 + 80, HUMI_Y, 48, hz[HUMI_HZ_INDEX + 1], 1); //%
	dataWriteToFileStr += temp_buf;  //String data write to file.
	dataWriteToFileStr += ",";

	LcdDisplay(LIGHT_X, LIGHT_Y, 48, hz[LIGHT_HZ_INDEX], 1); //光照:
        uint16_t lux = readLight();
	dtostrf(lux, 6, 0, temp_buf);
	LcdDisplay(LIGHT_X + 150, LIGHT_Y, 48, temp_buf, 12);  //light
	LcdDisplay(LIGHT_X + 150 + 170, LIGHT_Y, 48, hz[LIGHT_HZ_INDEX + 1], 1); //%
    	dataWriteToFileStr += lux;  //String data write to file.
	dataWriteToFileStr += ",";

	LcdDisplay(VOC_X, VOC_Y, 48, hz[VOC_HZ_INDEX], 1); //Voc
	LcdDisplay(VOC_X + 150, VOC_Y, 48, hz[VOC_HZ_INDEX + getVoc()], 12); //Voc

	//BME280 Pressure
	LcdDisplay(BME_X, BME_Y, 48, hz[BME_HZ_INDEX], 1);
	dtostrf(getPressure(), 4, 2, temp_buf);
	LcdDisplay(BME_X + 150, BME_Y, 48, temp_buf, 12);
	LcdDisplay(BME_X + 150 + 170, BME_Y, 48, hz[BME_HZ_INDEX + 1], 1);
	dataWriteToFileStr += temp_buf;  //String data write to file.
	dataWriteToFileStr += ",";

	//GP2Y10 Dust
        LcdDisplay(GP2Y10_X, GP2Y10_Y, 48, "PM2.5: ", 1);              //湿度：
        float dust = getDustDensity();
        if(dust>0)
          dtostrf(dust, 6, 2, dust_buf);
	LcdDisplay(GP2Y10_X + 150 , GP2Y10_Y, 48, dust_buf, 16); 
	dataWriteToFileStr += dust_buf;  //String data write to file.
	return dataWriteToFileStr;
}

String getTime()
{
	String str = "";
	char temp_buf[50];
	bool h12;
	bool PM;
	bool Century = false;
	snprintf(temp_buf, sizeof(temp_buf), "20%02d-%02d-%02d %02d:%02d:%02d",
			Clock.getYear(), Clock.getMonth(Century), Clock.getDate(),
			Clock.getHour(h12, PM), Clock.getMinute(), Clock.getSecond());
	//strcpy(datebuf, temp_buf);
	str += temp_buf;
	return str;
	//    Serial.println(datebuf);
}

String getDateFlieName(void)  //保存在全局变量DateFlieName中
{
	bool h12;
	bool PM;
	bool Century = false;
	String timeStr = "";
	//name length limit?
	//snprintf(DateFlieName, 13, "File%02d.txt", Clock.getMinute());
      //char DateFlieName[24]="2018-05-12-11:20:30.txt";  //使用时间命名txt文件名
        snprintf(DateFlieName,13,"%02d%02d%02d%02d.txt",Clock.getMonth(Century), Clock.getDate(),
			Clock.getHour(h12, PM), Clock.getMinute());    

	timeStr += DateFlieName;
	return timeStr;
}

String getMetadata(void)
{
      String strMetadata = "metadata:";
      char* tempBuf="2018-05-12 12:55:30";
    
  	bool h12;
	bool PM;
	bool Century = false;
        snprintf(tempBuf,20,"20%02d-%02d-%02d %02d:%02d:%02d", Clock.getYear(), Clock.getMonth(Century), Clock.getDate(),
			Clock.getHour(h12, PM), Clock.getMinute(), Clock.getSecond());    

	strMetadata += tempBuf;

  return strMetadata;
}


void writeToSD(String str, char* filename)
{
	myFile = SD.open(filename, FILE_WRITE);
	if (myFile)
	{
		myFile.println(str);
		myFile.close();
		if (DEBUG_SERIAL)
			Serial.println(str);
	}
	// if the file isn't open, pop up an error:
	else
	{
		if (DEBUG_SERIAL)
		{
			Serial.print("error opening: ");
			Serial.println(filename);
		}
        bCardPresented = false;
	}

}

/*获取温度信息*/
float Get18B20(void)
{
	float temp;
	Ds0.start();  //开始测量
	temp = Ds0.get();

	if (temp < 100)
		return temp;

}

/*get pressure*/
float GetPressure(void)
{
	float temp_f;
	temp_f = 0.0;
	return temp_f;

}

/*Get huminity*/
float GetHumidity(void)  //get huminity:HT11
{
	float temp_f;
	temp_f = 0.0;
	return temp_f;
}

/*Get temperature*/
float GetTemperature(void)  //get temperature: 18B20
{
	float temp_f;
	temp_f = 0.0;
	return temp_f;
}

/*
 Reponse:
 0	|1		|2		|3	        |4			|5			|6	        |7				|8
 起始位	|气体名称	|单位		|小数位数	|气体浓度高位(ppb)	|气体浓度低位(ppb)	|满量程高位	|满量程低位			|校验值
 0xFF	|0x17=CH2O	|0x04=Ppb	|0		|0x00			|0x25			|0x07		|0xD0				|0x25

 HCHO:
 1ppm * 30 / 22.4 = 1.3393 mg/ m3

 */
/*Get oxymethylene*/

float getOxymethylene(void)  //get oxymethylene:DART WZ-S sensor
{
	unsigned char HeadGegin = 0;
	float oxymethylene;
	int index = 0;
	unsigned char buf[50];
	while (OXY_SERIAL.available()) //返回 01 03 20 *******
	{
		// get the new byte:
		unsigned char inChar = (unsigned char) OXY_SERIAL.read();
		//Serial.write(inChar);
		if (!HeadGegin) //接收包头
		{
			if (inChar == 0xFF)
			{
				buf[index++] = inChar;
				inChar = (unsigned char) OXY_SERIAL.read();
				//Serial.write(inChar);
				if (inChar == 0x17)
				{
					buf[index++] = inChar;
					inChar = (unsigned char) OXY_SERIAL.read();
					//Serial.write(inChar);
					if (inChar == 0x04)
					{
						buf[index++] = inChar;
						HeadGegin = 1;
					}
				}
			}

		}
		else //开始接收有效数据
		{
			buf[index++] = inChar;
		}
	}
//	Serial.print("index:");
//	Serial.println(index); //发送数据

	if (index >= 9) //9个数据
	{
		unsigned long int temp = buf[4] * 256 + buf[5];
		oxymethylene = (float) (temp * 0.0013393); //oxymethylene_f
		return oxymethylene;
	}
	return -1;
}

void printDirectory(File dir, int numTabs)
{
	while (true)
	{
		File entry = dir.openNextFile();
		if (!entry)
		{
			// no more files
			//Serial.println("**nomorefiles**");
			break;
		}
		for (uint8_t i = 0; i < numTabs; i++)
		{
			Serial.print('\t');
		}
		Serial.print(entry.name());
		if (entry.isDirectory())
		{
			Serial.println("/");
			printDirectory(entry, numTabs + 1);
		}
		else
		{
			// files have sizes, directories do not
			Serial.print("\t\t");
			Serial.println(entry.size(), DEC);
		}
		entry.close();
	}
}

void setClock()
{
	Clock.setSecond(0);			//Set the second
	Clock.setMinute(59);			//Set the minute
	Clock.setHour(9);  //Set the hour
	Clock.setDoW(6);    //Set the day of the week
	Clock.setDate(12);  //Set the date of the month
	Clock.setMonth(5);  //Set the month of the year
	Clock.setYear(18);  //Set the year (Last two digits of the year)
}


