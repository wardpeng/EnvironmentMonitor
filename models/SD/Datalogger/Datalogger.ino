#include <SD.h>

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 10;
File myFile;
void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(115200);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  

  Serial.println("card initialized.");
    Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  pinMode(10, OUTPUT);

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  if (SD.exists("example.txt")) {
    Serial.println("example.txt exists.");
  }
  else {
    Serial.println("example.txt doesn't exist.");
  }

  // open a new file and immediately close it:
  Serial.println("Creating example.txt...");
  myFile = SD.open("example.txt", FILE_WRITE);
  myFile.close();

  // Check to see if the file exists: 
  if (SD.exists("example.txt")) {
    Serial.println("example.txt exists.");
  }
  else {
    Serial.println("example.txt doesn't exist.");  
  }

//  // delete the file:
//  Serial.println("Removing example.txt...");
//  SD.remove("example.txt");
//
//  if (SD.exists("example.txt")){ 
//    Serial.println("example.txt exists.");
//  }
//  else {
//    Serial.println("example.txt doesn't exist.");  
//  }
}
int count = 1;
void loop()
{  
  
  writeToSd("example.txt");
  count++;
  delay(1000);
}

void writeToSd(char *filename)
{
	char temp_buf[50];

	dtostrf(count, 10, 0, temp_buf);  //将float转换为数组//此条信息的序号
	strcat(temp_buf, ",");  //逗号，分割每条信息
        myFile = SD.open(filename, FILE_WRITE);
  	if (myFile)
	{
          myFile.println(temp_buf);
          myFile.close();
          Serial.println(temp_buf);
	}
	// if the file isn't open, pop up an error:
	else
	{
			Serial.print("error opening ");
			Serial.println(filename);
  
	}

}









