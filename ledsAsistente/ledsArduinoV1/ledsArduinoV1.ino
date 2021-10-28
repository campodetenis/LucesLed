// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library
void cambiarLeds();
void apagarLeds();
void encenderLeds();
void cambiarColor(uint32_t nuevoColor);

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include <SoftwareSerial.h>
SoftwareSerial BT(1,0);
// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        4 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 30 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels
#define pinEsp 2
#define pinLed 8
#define sensorPin 3
int valorPin = 0;
char junk;
int valorSensor;
bool sensorActivo;
String inputString = "";
const uint32_t rojo = pixels.Color(255,0, 0);
const uint32_t verde = pixels.Color(0,155, 0);
const uint32_t rosa = pixels.Color(230,0, 126);
char currentState = ""; 
/*cuando sehace serial read se almacena el resultado aqui. 
El loop es un switch que ejecuta el case segun el valor de currentState.
De ese modo los ciclos se cumplen de manera correcta*/

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  Serial.begin(115200);
  BT.begin(9600);
  attachInterrupt(digitalPinToInterrupt(pinEsp),cambiarLeds,CHANGE);
  pinMode(pinEsp, INPUT);
  pinMode(pinLed, OUTPUT);
  pinMode(sensorPin,INPUT);
  sensorActivo = false;
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.show();
}

void loop() {
if(Serial.available()){
  while(Serial.available())
    {
      char inChar = (char)Serial.read(); //read the input
      inputString += inChar;        //make a string of the characters coming on serial
      Serial.println(inputString);
    }
    Serial.println(inputString);
    while (Serial.available() > 0)  
    { junk = Serial.read() ; }      // clear the serial buffer
    if(inputString == "1"){         //in case of 'a' turn the LED on
      //digitalWrite(pinLed, HIGH);
      encenderLeds();
        Serial.println("encender");
        digitalWrite(8,HIGH);  
    }else if(inputString == "0"){   //incase of 'b' turn the LED off
      //digitalWrite(pinLed, LOW);
      apagarLeds();
              Serial.println("apagar");
        digitalWrite(8,LOW); 
    }else if(inputString == "r"){
      cambiarColor(rojo);
    }else if(inputString == "g"){
      cambiarColor(verde);
    }else if(inputString == "w"){
      pixels.clear();
      pixels.show();
      colorWipe(rosa,20);
    }else if(inputString == "h"){
      rainbow(1);
    }
    inputString = "";
  }
}

/*usar RISING y falling en la interrupcion no funciona (igual solo puedes usar una) asi que uso change 
esto significa que cuando cambia el valor recibido en el pin se ejecuta la funcionm cambio.
La funcion cambio ella sola es capaz de detectar si hay que apagar o encender la luz segun el valor que lee en el pin*/
void cambiarLeds(){
  valorPin = digitalRead(pinEsp);
  switch(valorPin){
    case LOW:
      apagarLeds();
      break;

    case HIGH:
      encenderLeds();
      break;
  }
}

void verificarSensor(){
  if(sensorActivo){
    valorSensor = digitalRead(sensorPin);
    if(valorSensor == HIGH){
      //aun hay que darle un par de vueltas
    }
  }
}

void encenderLeds(){
 for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    //delay(DELAYVAL); // Pause before next pass through loop
  }
}

void apagarLeds(){
  pixels.clear();
  pixels.show();
}

void cambiarColor(uint32_t nuevoColor){
  pixels.clear();
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i,nuevoColor);
  }
  pixels.show();
}


/*********************************************************************
 * ANIMACIONES ADAFRUIT NEOPIXEL
 * ******************************************************************/
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
    pixels.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    pixels.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}



 // Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      pixels.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<pixels.numPixels(); c += 3) {
        pixels.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      pixels.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}


// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
    }
    pixels.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}


// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      pixels.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<pixels.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / pixels.numPixels();
        uint32_t color = pixels.gamma32(pixels.ColorHSV(hue)); // hue -> RGB
        pixels.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      pixels.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
