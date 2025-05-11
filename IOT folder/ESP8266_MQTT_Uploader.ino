/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Hotspot_name"
#define WLAN_PASS       "Hotspot_password"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "AIO_USERNAME"
#define AIO_KEY         "AIO_KEY"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/msg");

//// Setup a feed called 'onoff' for subscribing to changes.
//Adafruit_MQTT_Subscribe onoffbutton1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay1");
//
//Adafruit_MQTT_Subscribe onoffbutton2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay2");
//
//Adafruit_MQTT_Subscribe onoffbutton3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay3");
//
//Adafruit_MQTT_Subscribe onoffbutton4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay4");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();


char arr[] = "2023.11.22 17:40:40 Temperature: 31.8 degC, Humidity:  84 %RH, Rain: NO, TRIP: NO";

char inByte, ii, start_flag = 0, over_flag = 0;

void setup() 
{
  Serial.begin(9600);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

//  // Setup MQTT subscription for onoff feed.
//  mqtt.subscribe(&onoffbutton1);
//  mqtt.subscribe(&onoffbutton2);
//  mqtt.subscribe(&onoffbutton3);
//  mqtt.subscribe(&onoffbutton4);  
}

uint32_t x=0;

void loop() 
{  
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

//  Adafruit_MQTT_Subscribe *subscription;
//  while((subscription = mqtt.readSubscription(5000))) 
//  {
//   if(subscription == &onoffbutton1) 
//   {
//    if(0 == strcmp((char *)onoffbutton1.lastread, "OFF"))digitalWrite(RLY_T, HIGH);
//    if(0 == strcmp((char *)onoffbutton1.lastread, "ON"))digitalWrite(RLY_T, LOW);    
//   }
//   else if(subscription == &onoffbutton2) 
//   {
//    if(0 == strcmp((char *)onoffbutton2.lastread, "OFF"))digitalWrite(RLY_L, HIGH);
//    if(0 == strcmp((char *)onoffbutton2.lastread, "ON"))digitalWrite(RLY_L, LOW);    
//   }
//   else if(subscription == &onoffbutton3) 
//   {
//    if(0 == strcmp((char *)onoffbutton3.lastread, "OFF"))digitalWrite(RLY_F, HIGH);
//    if(0 == strcmp((char *)onoffbutton3.lastread, "ON"))digitalWrite(RLY_F, LOW);    
//   }   
//   else if(subscription == &onoffbutton4) 
//   {
//    if(0 == strcmp((char *)onoffbutton4.lastread, "OFF"))digitalWrite(RLY_G, HIGH);
//    if(0 == strcmp((char *)onoffbutton4.lastread, "ON"))digitalWrite(RLY_G, LOW);    
//   }
//  }

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
   photocell.publish(arr);

   over_flag = 0;
  }

  /*
  // Now we can publish stuff!
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  if (! photocell.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  */
  
  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
