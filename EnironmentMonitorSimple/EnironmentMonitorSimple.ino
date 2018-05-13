#include <SD.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Light.h"
#include <dht11.h>
#include <DS3231.h>

char Title[200] = "Number,Date,Time,Press1,Press2,Press3,Voltage,Current,Power,Power_consumption,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,H1,H2";
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display 

const int chipSelect = 10;//SD卡的使能引脚
/*湿度传感器*/
dht11 DHT11_Master;
dht11 DHT11_Slaver;
#define DHT_M_PIN 8//湿度传感器
#define DHT_S_PIN 2//湿度传感器

/*时钟模块*/
DS3231 Clock;
bool h12;
bool PM;
bool Century = false;

char* DateFlieName="05031230.txt";  //使用时间命名txt文件名 !!!!! <<<<<Length liminit>>!!!!
File myFile;
boolean bCardPresented = true;

void setup(void) {
  Serial.begin(115200); //  For debug
    lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  //lcd.print("Hello, world!");  

  Wire.begin();

  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(chipSelect, OUTPUT);
  //setClock();
  
 	// see if the card is present and can be initialized:
	if (!SD.begin(chipSelect))
	{
		Serial.println("Card failed, or not present");
		// don't do anything more:
                lcd.setCursor(0, 3);
                lcd.print("Card not present!");  
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
          lcd.setCursor(0, 3);
          lcd.print(DateFlieName);  
  	  Serial.println(Title);
        }
  
}

unsigned long int index = 0;
unsigned long int recordIndex = 0;
String strWriteToSd="";
void loop(void)
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
  String dataWriteToFileStr = "";
    dataWriteToFileStr += storeageCounter;
    dataWriteToFileStr += ",";
    
    dataWriteToFileStr += getTime();
    dataWriteToFileStr += ",";    
    
    int chk = DHT11_Master.read(DHT_M_PIN);                 //将读取到的值赋给chk
    int temp = DHT11_Master.temperature;
    int humi = DHT11_Master.humidity;
    lcd.setCursor(0, 0);
    lcd.print("T1:");
    lcd.print(temp);
    lcd.print(" ");
    lcd.print("H1:");
    lcd.print(humi);
    lcd.print(" ");
    dataWriteToFileStr += temp;
    dataWriteToFileStr += ",";
    dataWriteToFileStr += humi;
    dataWriteToFileStr += ",";
    
    //Data 18-05-12
    lcd.print(Clock.getYear());
    lcd.print("-");
    lcd.print(Clock.getMonth(Century));
    lcd.print("-");
    lcd.print(Clock.getDate());

    chk = DHT11_Slaver.read(DHT_S_PIN);                 //将读取到的值赋给chk
    temp = DHT11_Slaver.temperature;
    humi = DHT11_Slaver.humidity;
    lcd.setCursor(0, 1);
    lcd.print("T2:");
    lcd.print(temp);
    lcd.print(" ");
    lcd.print("H2:");
    lcd.print(humi);
    dataWriteToFileStr += temp;
    dataWriteToFileStr += ",";
    dataWriteToFileStr += humi;
    dataWriteToFileStr += ",";
    
    //Time 12:30:30
    lcd.print(" ");
    lcd.print(Clock.getHour(h12, PM));
    lcd.print(":");
    lcd.print(Clock.getMinute());
    lcd.print(":");
    lcd.print(Clock.getSecond());
    
    //Light
    int lux = readLight();
    lcd.setCursor(0, 2);
    lcd.print("Light:");
    lcd.print(lux);
    lcd.print(" ");
    lcd.print("Lux");
    lcd.print("     ");
    dataWriteToFileStr += lux;
    
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
	}
	// if the file isn't open, pop up an error:
	else
	{
		Serial.print("error opening: ");
		Serial.println(filename);
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

void setClock()
{
	Clock.setSecond(0);			//Set the second
	Clock.setMinute(19);			//Set the minute
	Clock.setHour(20);  //Set the hour
	Clock.setDoW(6);    //Set the day of the week
	Clock.setDate(12);  //Set the date of the month
	Clock.setMonth(5);  //Set the month of the year
	Clock.setYear(18);  //Set the year (Last two digits of the year)
}

