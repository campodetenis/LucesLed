/*
 * 
 * Este programa se ha de subir al esp8266.
 */

#include <ESP8266WiFi.h>
#include "AdafruitIO_WiFi.h"
 
#define WIFI_SSID       "casita1"
#define WIFI_PASS       "monoxidodedihidrogeno"
 
#define IO_USERNAME    "campodetenis"
#define IO_KEY         "aio_gHtA71R6cOCSHeqJ2lTN7HYrCiu3"


 
// Connect to Wi-Fi and Adafruit IO handel 
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
 
// Create a feed object that allows us to send data to
AdafruitIO_Feed *feedInterruptorLuz = io.feed("on-off-light-google");
 
 
void setup() 
{
  // Enable the serial port so we can see updates
  Serial.begin(9600);
  pinMode(5,OUTPUT);
  // Connect to Adafruit IO
  io.connect();

  feedInterruptorLuz->onMessage(handleMessage);
 
  // wait for a connection
  while(io.status() < AIO_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());

  feedInterruptorLuz->get();
}
 
void loop() 
{
  // Always keep this at the top of your main loop
  // While not confirmed, this implies that the Adafruit IO library is not event-driven
  // This means you should refrain from using infinite loops
  io.run();
 
  // Send 10 to our Workshop Temperature Feed
 
  // This is so we can see some response on the computer
  //Serial.print("Data sent");
 
  // DONT SEND A BILLION DATA POINTS! This slows down the program so that it sends readings once every 5 seconds
  delay(2000);
}


void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");

  if(data->toPinLevel() == HIGH){
    Serial.println("HIGH");
    digitalWrite(5,HIGH);    
  }else{
    Serial.println("LOW");
    digitalWrite(5,LOW);
  }
  // write the current state to the led
  //digitalWrite(LED_PIN, data->toPinLevel());

}
