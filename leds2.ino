#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN    7
#define InterruptorA 2
#define Led 5
#define interruptorB 3

#define LED_COUNT 30

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int sensorPin = 8;
int pirState = LOW;
int valorSensor = 0;
float minutos = 1;
uint32_t period = minutos * 60000L; //5 minutos tiempo que se ejecutara el bucle
bool cambioT; //cambia entre los modos de tiempo
int switchState;
int switchState2;
int cont=0;
int interrupcion = 0;
uint32_t blanco = strip.Color(0,0,0);
uint32_t rojo = strip.Color(255,0,0);
uint32_t color = strip.Color(255,0,0);

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
  pinMode(sensorPin,INPUT);
  pinMode(InterruptorA,INPUT);
  pinMode(interruptorB,INPUT);
  pinMode(Led, OUTPUT);
  Serial.begin(9600);
  cont=0;

  //codigo de las interrupciones
  attachInterrupt(interrupcion,cambiarColor,RISING);
  attachInterrupt(1,cambiarMinutos,RISING);
}

void loop() {
    period = minutos * 60000L;
    valorSensor = digitalRead(sensorPin);
    if(valorSensor == HIGH){
     colorWipe(color,30);
    for( uint32_t tStart = millis();  (millis()-tStart) < period;  ){
      /*switch(cont){
        case 0:
        //ponerColor(strip.Color(239,0,255));
        colorWipe(strip.Color(239,   0,   255), 10);
        break;

        case 1:
        //ponerColor(strip.Color(255,255,255));
        colorWipe(strip.Color(255,   255,   255), 10);
        break;
        
        case 2:
        //ponerColor(strip.Color(255,0,0));
        colorWipe(strip.Color(255,   0,   0), 10);
        break;

        default:
        colorWipe(strip.Color(0,0,0),50);
      }
      */
      ponerColor(color);
      
    }
    colorWipe(strip.Color(0,   0,   0), 50);
    }else{
      ponerColor(strip.Color(0,0,0));
    }

}



/************************************************************************************
  MIS FUNCIONES
 ************************************************************************************/
void cambiarColor(){
      cont++;
      if(cont >2){
        cont=0;
      }
      switch(cont){
        case 0:
          color=strip.Color(239,   0,   255);
          break;

        case 1:
          color=strip.Color(255,0,0);
          break;

        case 2:
          color=strip.Color(255,255,255);
          break;
      }
}

void ponerColor(uint32_t color){
    for(int i=0; i<strip.numPixels(); i++) { 
    strip.setPixelColor(i, color);         
    strip.show();                          
  }
}

void cambiarMinutos(){
  cambioT=!cambioT;
  if(cambioT){
    minutos = .5;
    digitalWrite(Led,LOW);
  }else{
    minutos = 5;
    digitalWrite(Led,HIGH);
  }
  delay(2000);
}

/************************************************************************************
 *FUNCIONES ADAFRUIT
 ************************************************************************************/

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

void rainbow(int wait) {
  //leerInterruptor();
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) {
      //leerInterruptor();
      // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(/*wait*/1);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
