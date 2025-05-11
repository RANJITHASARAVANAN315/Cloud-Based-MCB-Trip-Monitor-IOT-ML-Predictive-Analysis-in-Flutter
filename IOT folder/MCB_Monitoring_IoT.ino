#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

/////////////////////

//#include <Wire.h>
#include "ds3231.h"

#define BUFF_MAX 128

uint8_t time[8];
char recv[BUFF_MAX];
//unsigned int recv_size = 0;

//char in;
char buff[BUFF_MAX];
struct ts t;

//void parse_cmd(char *cmd, int cmdsize);

/////////////////////

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

/////////////////////

const int RAIN_SEN = A0;

const int TRIP_IN = A1;


char rain_flag, trip_flag;

char ms1000_flag = 0;

char flag2 = 0;

/////////////////////

char rain0 = 0, rain1 = 0, trip0 = 0, trip1 = 0;

int t_r, h_r, prev_t_r, prev_h_r;

char temp_flag = 0, humy_flag = 0;


ISR(TIMER1_COMPA_vect)
{
  ms1000_flag = 1;
}

void setup()
{
  Serial.begin(9600);
  //Serial.println(F("DHTxx test!"));

  dht.begin();

  /////////////////////

  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();

  delay(100);

  lcd.setCursor(0, 0); lcd.print("----------------");  //title
  lcd.setCursor(0, 1); lcd.print("----------------");

  /////////////////////

  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  memset(recv, 0, BUFF_MAX);
  //Serial.println("GET time");

  /////////////////////

  pinMode(RAIN_SEN, INPUT);
  pinMode(TRIP_IN, INPUT_PULLUP);

  delay(2000);

  lcd.clear();


    cli();//stop interrupts
  
    //set timer1 interrupt at 1Hz
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0
    // set compare match register for 1hz increments
    OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12 and CS10 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
  
    sei();//allow interrupts
}

void loop()
{
  if(ms1000_flag == 1)
  {
    ms1000_flag = 0;

        DS3231_get(&t);

        snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d", t.year, t.mon, t.mday, t.hour, t.min, t.sec);
    
//        // there is a compile time option in the library to include unixtime support
//        #ifdef CONFIG_UNIXTIME
//        #ifdef __AVR__
//            snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d %ld", t.year,
//        #error AVR
//        #else
//            snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d %d", t.year,
//        #endif
//                     t.mon, t.mday, t.hour, t.min, t.sec, t.unixtime);
//        #else
//            snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d", t.year,
//                     t.mon, t.mday, t.hour, t.min, t.sec);
//        #endif
    
        //Serial.println(buff);

    /////////////////////

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) /*|| isnan(f)*/) 
    {
      //Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    // Compute heat index in Fahrenheit (the default)
    //float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    //float hic = dht.computeHeatIndex(t, h, false);

//    Serial.print(F("Humidity: "));
//    Serial.print(h);
//    Serial.print(F("%  Temperature: "));
//    Serial.print(t);
//    Serial.println(F("°C "));
    
//    Serial.print(f);
//    Serial.print(F("°F  Heat index: "));
//    Serial.print(hic);
//    Serial.print(F("°C "));
//    Serial.print(hif);
//    Serial.println(F("°F"));

    if (digitalRead(RAIN_SEN) ==  LOW) {
      delay(10);
      if (digitalRead(RAIN_SEN) ==  LOW) {
        delay(10);
        if (digitalRead(RAIN_SEN) ==  LOW) {
          rain_flag = 0;
        }
      }
    }
    if (digitalRead(RAIN_SEN) == HIGH) {
      delay(10);
      if (digitalRead(RAIN_SEN) == HIGH) {
        delay(10);
        if (digitalRead(RAIN_SEN) == HIGH) {
          rain_flag = 1;
        }
      }
    }

    if (digitalRead(TRIP_IN) ==  LOW) {
      delay(10);
      if (digitalRead(TRIP_IN) ==  LOW) {
        delay(10);
        if (digitalRead(TRIP_IN) ==  LOW) {
          trip_flag = 0;
        }
      }
    }
    if (digitalRead(TRIP_IN) == HIGH) {
      delay(10);
      if (digitalRead(TRIP_IN) == HIGH) {
        delay(10);
        if (digitalRead(TRIP_IN) == HIGH) {
          trip_flag = 1;
        }
      }
    }


    //lcd.setCursor(0, 0); lcd.print("T99.9'C,Rain:YES");
    //lcd.setCursor(0, 1); lcd.print("H100%RH,TRIP:YES");

    lcd.setCursor(0, 0);
    lcd.print("T");
    if (t < 10)lcd.print(" ");
    lcd.print(t, 1);
    lcd.write(223);
    lcd.print("C,Rain:");
    if (rain_flag == 1) {
      lcd.print("YES");
      ++rain1;
      if (rain1 > 2)rain1 = 2;
      rain0 = 0;
    }
    else if (rain_flag == 0) {
      lcd.print(" NO");
      ++rain0;
      if (rain0 > 2)rain0 = 2;
      rain1 = 0;
    }

    lcd.setCursor(0, 1);
    lcd.print("H");
    if (h <  10)lcd.print("  ");
    else if (h < 100)lcd.print(" ");
    lcd.print(h, 0);
    lcd.print("%RH,TRIP:");
    if (trip_flag == 1) {
      lcd.print("YES");
      ++trip1;
      if (trip1 > 2)trip1 = 2;
      trip0 = 0;
    }
    else if (trip_flag == 0) {
      lcd.print(" NO");
      ++trip0;
      if (trip0 > 2)trip0 = 2;
      trip1 = 0;
    }


    t_r = (int)(t * 10);

    if (t_r != prev_t_r)temp_flag = 1; else temp_flag = 0;

    prev_t_r = t_r;


    h_r = (int)h;

    if (h_r != prev_h_r)humy_flag = 1; else humy_flag = 0;

    prev_h_r = h_r;

    //    Serial.write(temp_flag+48);
    //    Serial.write(humy_flag+48);
    //    Serial.write(rain1+48);
    //    Serial.write(rain0+48);
    //    Serial.write(trip1+48);
    //    Serial.write(trip0+48);
    //    Serial.println();


    if ( (temp_flag == 1) || (humy_flag == 1) || (rain1 == 1) || (rain0 == 1) || (trip1 == 1) || (trip0 == 1) )
    {
      if (flag2 == 0)
      {
        Serial.write('%');
        Serial.print(buff);
        Serial.write(' ');
        Serial.print("Temperature: ");
        if (t < 10)Serial.print(" ");
        Serial.print(t, 1);
        Serial.print(" degC, Humidity: ");
        if (h <  10)Serial.print("  ");
        else if (h < 100)Serial.print(" ");
        Serial.print(h, 0);
        Serial.print(" %RH, Rain:");
        if (rain_flag == 1)Serial.print("YES");
        else if (rain_flag == 0)Serial.print(" NO");
        Serial.print(", TRIP:");
        if (trip_flag == 1)Serial.print("YES");
        else if (trip_flag == 0)Serial.print(" NO");
        Serial.println("#");

        flag2 = 1;
      }
    }
    else flag2 = 0;
  }


  //  if (Serial.available() > 0)
  //  {
  //    in = Serial.read();
  //
  //    if ((in == 10 || in == 13) && (recv_size > 0)) {
  //      parse_cmd(recv, recv_size);
  //      recv_size = 0;
  //      recv[0] = 0;
  //    } else if (in < 48 || in > 122) {
  //      ;       // ignore ~[0-9A-Za-z]
  //    } else if (recv_size > BUFF_MAX - 2) {   // drop lines that are too long
  //      // drop
  //      recv_size = 0;
  //      recv[0] = 0;
  //    } else if (recv_size < BUFF_MAX - 2) {
  //      recv[recv_size] = in;
  //      recv[recv_size + 1] = 0;
  //      recv_size += 1;
  //    }
  //
  //  }

  delay(100);
}

//void parse_cmd(char *cmd, int cmdsize)
//{
//  uint8_t i;
//  uint8_t reg_val;
//  char buff[BUFF_MAX];
//  struct ts t;
//
//  //snprintf(buff, BUFF_MAX, "cmd was '%s' %d\n", cmd, cmdsize);
//  //Serial.print(buff);
//
//  // TssmmhhWDDMMYYYY aka set time
//  if (cmd[0] == 84 && cmdsize == 16) {
//    //T355720619112011
//    t.sec = inp2toi(cmd, 1);
//    t.min = inp2toi(cmd, 3);
//    t.hour = inp2toi(cmd, 5);
//    t.wday = cmd[7] - 48;
//    t.mday = inp2toi(cmd, 8);
//    t.mon = inp2toi(cmd, 10);
//    t.year = inp2toi(cmd, 12) * 100 + inp2toi(cmd, 14);
//    DS3231_set(t);
//    Serial.println("OK");
//  } else if (cmd[0] == 49 && cmdsize == 1) {  // "1" get alarm 1
//    DS3231_get_a1(&buff[0], 59);
//    Serial.println(buff);
//  } else if (cmd[0] == 50 && cmdsize == 1) {  // "2" get alarm 1
//    DS3231_get_a2(&buff[0], 59);
//    Serial.println(buff);
//  } else if (cmd[0] == 51 && cmdsize == 1) {  // "3" get aging register
//    Serial.print("aging reg is ");
//    Serial.println(DS3231_get_aging(), DEC);
//  } else if (cmd[0] == 65 && cmdsize == 9) {  // "A" set alarm 1
//    DS3231_set_creg(DS3231_CONTROL_INTCN | DS3231_CONTROL_A1IE);
//    //ASSMMHHDD
//    for (i = 0; i < 4; i++) {
//      time[i] = (cmd[2 * i + 1] - 48) * 10 + cmd[2 * i + 2] - 48; // ss, mm, hh, dd
//    }
//    uint8_t flags[5] = { 0, 0, 0, 0, 0 };
//    DS3231_set_a1(time[0], time[1], time[2], time[3], flags);
//    DS3231_get_a1(&buff[0], 59);
//    Serial.println(buff);
//  } else if (cmd[0] == 66 && cmdsize == 7) {  // "B" Set Alarm 2
//    DS3231_set_creg(DS3231_CONTROL_INTCN | DS3231_CONTROL_A2IE);
//    //BMMHHDD
//    for (i = 0; i < 4; i++) {
//      time[i] = (cmd[2 * i + 1] - 48) * 10 + cmd[2 * i + 2] - 48; // mm, hh, dd
//    }
//    uint8_t flags[5] = { 0, 0, 0, 0 };
//    DS3231_set_a2(time[0], time[1], time[2], flags);
//    DS3231_get_a2(&buff[0], 59);
//    Serial.println(buff);
//  } else if (cmd[0] == 67 && cmdsize == 1) {  // "C" - get temperature register
//    Serial.print("temperature reg is ");
//    Serial.println(DS3231_get_treg(), DEC);
//  } else if (cmd[0] == 68 && cmdsize == 1) {  // "D" - reset status register alarm flags
//    reg_val = DS3231_get_sreg();
//    reg_val &= B11111100;
//    DS3231_set_sreg(reg_val);
//  } else if (cmd[0] == 70 && cmdsize == 1) {  // "F" - custom fct
//    reg_val = DS3231_get_addr(0x5);
//    Serial.print("orig ");
//    Serial.print(reg_val, DEC);
//    Serial.print("month is ");
//    Serial.println(bcdtodec(reg_val & 0x1F), DEC);
//  } else if (cmd[0] == 71 && cmdsize == 1) {  // "G" - set aging status register
//    DS3231_set_aging(0);
//  } else if (cmd[0] == 83 && cmdsize == 1) {  // "S" - get status register
//    Serial.print("status reg is ");
//    Serial.println(DS3231_get_sreg(), DEC);
//  } else {
//    Serial.print("unknown command prefix ");
//    Serial.println(cmd[0]);
//    Serial.println(cmd[0], DEC);
//  }
//}
