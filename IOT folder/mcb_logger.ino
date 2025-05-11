#include <SPI.h>
#include <SD.h>

File myFile;

char arr[] = "2023.11.22 17:40:40 Temperature: 31.8 degC, Humidity:  84 %RH, Rain: NO, TRIP: NO";

char inByte, ii, start_flag = 0, over_flag = 0;

void setup()
{
  Serial.begin(9600);
  
  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");  
}

void loop()
{
  while(Serial.available() > 0) 
  {
   inByte = Serial.read();

   if(start_flag == 0)
   {
    if(inByte == '%')
    {
     ii = 0;

     start_flag = 1; 
    } 
   }
   else if(start_flag == 1)
   {
    if(inByte == '#')
    {
     start_flag = 0; 

     over_flag = 1; 
    }
    else 
    {
     arr[ii++] = inByte;
    } 
   }
  }

  if(over_flag == 1)
  {
      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
      myFile = SD.open("log.txt", FILE_WRITE);

      // if the file opened okay, write to it:
      if (myFile)
      {
        //Serial.print("Writing to test.txt...");
        //myFile.println("testing 1, 2, 3.");

        myFile.println(arr);

        // close the file:
        myFile.close();
        Serial.println("done.");
      }
      else
      {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
      }

      
   over_flag = 0;
  }

  delay(100);
}
