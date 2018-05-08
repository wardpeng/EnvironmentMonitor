#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include "DS18B20_S.h"
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>

#define DEBUG_SERIAL 1//调试信息，发送到串口
#define PRINTF_TO_SERIAL 1//正常工作时将调试信息，发送到串口

char Title[200] =
		"Number,Date,Time,Press1,Press2,Press3,Voltage,Current,Power,Power_consumption,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,H1,H2";
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

/*DS18B20(byte pin)  连接管脚*/
//传感器设定为10位模式，每次转换时间<187.5ms,如果需要12位模式，请修改库文件of ds.set(0x7F);
float ds;  //18B20存储
#define DS_PIN 22
DS18B20_S Ds0(22);  //pin DS_PIN

/*时钟模块*/
DS3231 Clock;

char DateFlieName[30];  //使用时间命名txt文件名

/*压力传感器*/
#define PRES_PIN 0//气压传感器开始的模拟量引脚

/*湿度传感器*/
#define HUMI_PIN 3//湿度传感器

const int chipSelect = 10;  //SD卡的使能引脚
File myFile;

void setup()
{
	// Open serial communications and wait for port to open:
	Serial.begin(115200);
	//Serial.begin(4800);

	// lcd.init();                      // initialize the lcd
	// Print a message to the LCD.
	// lcd.backlight();
	// lcd.print("Hello, world!");

	Wire.begin();
	//setClock();

	Serial.print("Initializing SD card...");
	// make sure that the default chip select pin is set to
	// output, even if you don't use it:
	pinMode(chipSelect, OUTPUT);

	// see if the card is present and can be initialized:
	if (!SD.begin(chipSelect))
	{
		Serial.println("Card failed, or not present");
		// don't do anything more:
		return;
	}
	Serial.println("card initialized.");

	File root = SD.open("/");
	printDirectory(root, 0);

	Serial.println("printDirectory done!");

	getDateFlieName();  //开机获取时间，用于命名文件，存放在DateFlieName中
	Serial.println(DateFlieName);
	/*	WriteToSD(Title, DateFlieName);  //打印表头
	 Serial.println(Title);
	 */
}

unsigned long int Count = 1;
unsigned long int index = 0;
void loop()
{
	char DataBuf[200] = "\0";  //用于保存一个周期读取的传感器数据

	if (index % 3 == 0)
	{
		getAllInfo(DataBuf);
		writeToSD(DataBuf, DateFlieName);
		Count++;  //序号++
		if (PRINTF_TO_SERIAL)  //正常工作时将所有的数据打印到串口
			Serial.println(DataBuf);
	}
	delay(1000);
	index++;

}

void getAllInfo(char *buf)
{

	//float pressure_f;//BMP280
//	float humidity_f;		//HT11
//	float temperature_f;	//HT11
//	float ds_f;				//18B20
	char temp_buf[50];

	dtostrf(Count, 10, 0, temp_buf);  //将float转换为数组//此条信息的序号
	strcat(buf, temp_buf);
	strcat(buf, ",");  //逗号，分割每条信息

	getTime(temp_buf);  //时间信息

	strcat(buf, temp_buf);
	strcat(buf, ",");

//	pressure_f = GetPressure();
//
//	ds_f = Get18B20();
//
//	humidity_f = GetHumidity();

}

void getTime(char *datebuf)
{
	char temp_buf[50];
	bool h12;
	bool PM;
	bool Century = false;
	snprintf(temp_buf, sizeof(temp_buf), "20%02d-%02d-%02d,%02d:%02d:%02d",
			Clock.getYear(), Clock.getMonth(Century), Clock.getDate(),
			Clock.getHour(h12, PM), Clock.getMinute(), Clock.getSecond());
	strcpy(datebuf, temp_buf);
	//    Serial.println(datebuf);
}

void getDateFlieName(void)  //保存在全局变量DateFlieName中
{
	bool h12;
	bool PM;
	bool Century = false;
	//name length limit?
	snprintf(DateFlieName, 13, "File%02d.txt",
			 Clock.getMinute());
//	snprintf(DateFlieName, 13, "%02d%02d%02d%02d.txt",
//			 Clock.getMonth(Century), Clock.getDate(),
//			Clock.getHour(h12, PM), Clock.getMinute());
}

void writeToSD(char *databuf, char *filename)
{
	//myFile = SD.open("Env.txt", FILE_WRITE);
	myFile = SD.open(filename, FILE_WRITE);
	if (myFile)
	{
		myFile.println(databuf);
		myFile.close();
		if (DEBUG_SERIAL)
			Serial.println(databuf);
	}
	// if the file isn't open, pop up an error:
	else
	{
		if (DEBUG_SERIAL)
		{
			Serial.print("error opening ");
			Serial.println(filename);
		}
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
 0		|1		|2						|3						|4		|5		|6					|7					|8
 起始位	|命令	|气体浓度高位(ug/m3)	|气体浓度低位(ug/m3)	|保留	|保留	|气体浓度低位(ppb)	|气体浓度低位(ppb)	|校验值
 0xFF	|0x86	|0x00					|0x2a					|0		|0		|0x00				|0x20				|0x30
 */
/*Get oxymethylene*/
unsigned char HeadGegin = 0;
float GetOxymethylene(void)  //get oxymethylene:DART WZ-S sensor
{
	delay(300);
	String send_data_str = "";
	float oxymethylene_f = 0;
	int index = 0;
	unsigned char buf[10];
	while (Serial.available()) //返回 01 03 20 *******
	{

		// get the new byte:
		unsigned char inChar = (unsigned char) Serial.read();
		//Serial.write(inChar);
		if (!HeadGegin) //接收包头
		{
			if (inChar == 0xFF)
			{
				buf[index++] = inChar;
				inChar = (unsigned char) Serial.read();
				if (inChar == 0x86)
				{
					buf[index++] = inChar;
					HeadGegin = 1;
				}
			}

		}
		else //开始接收有效数据
		{
			buf[index++] = inChar;
		}
	}
	Serial.print("index:");
	Serial.println(index); //发送数据

	if (index == 9) //9个数据
	{
		oxymethylene_f = buf[2] * 256 + buf[3]; //oxymethylene_f
		//打印数据到串口
		if (DEBUG_SERIAL)
		{
			send_data_str += "{oxymethylene_f:";
			//send_data_str += oxymethylene_f;
			send_data_str += "}";
			Serial.println(send_data_str);		//发送数据
		}
	}

	return oxymethylene_f;
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
	Clock.setSecond(00);			//Set the second
	Clock.setMinute(56);			//Set the minute
	Clock.setHour(22);  //Set the hour
	Clock.setDoW(5);    //Set the day of the week
	Clock.setDate(5);  //Set the date of the month
	Clock.setMonth(5);  //Set the month of the year
	Clock.setYear(18);  //Set the year (Last two digits of the year)
}

