#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define ANALOG_PIN  A0  // Piezo attached to Analog A0 on Wemos or Gemma D2 (A1)
#define LED_PIN     D5  // NeoPixel LED strand is connected to GPIO #0 / D0

// How many NeoPixels are attached to the Arduino?
#define N_PIXELS  113  //33  // Number of pixels you are using

#define DELAYVAL 25 // Time (in milliseconds) to pause between pixels

const int threshold1 = 20;
const int threshold2 = 50;
const int threshold3 = 65;
const int threshold4 = 100;
const int threshold5 = 200;
const int threshold = threshold3;  // threshold value to decide when the detected sound is a knock or not

int colorCount = 1;        // counter to determine which color to display next
uint8_t  bright = 255;

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
// Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t purple = pixels.Color(128, 0, 128),
          darkviolet = pixels.Color(148,0,211),
          darkorchid = pixels.Color(153,50,204),
          darkmagenta = pixels.Color(139,0,139),
          magenta = pixels.Color(255, 0, 255),
          yellow = pixels.Color(255, 255, 0),
          red = pixels.Color(255, 0, 0),
          green = pixels.Color(0, 255, 0),
          blue = pixels.Color(0, 0, 255);
uint32_t bass1color = red,
          bass2color = purple,
          bass3color = yellow,
          bass4color = green,
          bass5color = blue;
uint32_t myColor = bass2color;

void setup() {
  uint8_t  i, interimDelay;
  Serial.begin(9600);  // use the serial port
  pixels.begin();      // INITIALIZE NeoPixel strip object (REQUIRED)
  // pixels.setBrightness(bright);   // Set to full brightness for the duration of the sketch
  pixels.clear();      // Set all pixel colors to 'off'

  // Chase
  // chase(0, 0, bright);
/*
  // All on for 2 seconds
  for (i=0; i<N_PIXELS; i++)
    pixels.setPixelColor(i, pixels.Color(0, 0, bright));
  pixels.show(); // Send the updated pixel colors to the hardware.
  delay(2000);
*/

  shortFlicker(myColor);
  shortFlicker(myColor);
  shortFlicker(myColor);
  mediumFlicker(myColor);
  mediumFlicker(myColor);

  // All on for 2 seconds
  interimDelay = 150;
  pixels.fill(myColor, 0, N_PIXELS);
  pixels.setBrightness(32);
  pixels.show();
  delay(interimDelay);
  pixels.setBrightness(64);
  pixels.show();
  delay(interimDelay);
  pixels.setBrightness(128);
  pixels.show();
  delay(interimDelay);
  pixels.setBrightness(160);
  pixels.show();
  delay(interimDelay);
  pixels.setBrightness(192);
  pixels.show();
  delay(interimDelay);
  pixels.setBrightness(215);
  pixels.show();
  delay(interimDelay);
  pixels.setBrightness(230);
  pixels.show();
  delay(interimDelay);
  pixels.setBrightness(240);
  pixels.show();
  delay(interimDelay);
  pixels.setBrightness(255);
  pixels.show();
  delay(500);
  /*
  for (i=0; i<N_PIXELS; i++)
    pixels.setPixelColor(i, pixels.Color(128, 0, 128));
  */
  pixels.show(); // Send the updated pixel colors to the hardware.
  delay(2000);
}

void loop() {
  uint8_t  i;
  int      sensorReading;

  pixels.clear(); // Set all pixel colors to 'off'

  sensorReading = analogRead(ANALOG_PIN);          // Raw reading from sensor
  if (sensorReading >= threshold1) {
    Serial.println("Raw sensor reading: ");
    Serial.println(sensorReading);
  }
    
  if (sensorReading >= threshold) {
    // The first NeoPixel in a strand is #0, second is 1, all the way up
    // to the count of pixels minus one.
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255

    // Color each pixel... myColor
    pixels.fill(myColor, 0, N_PIXELS);
    //for (i=0; i<N_PIXELS; i++)
    //  pixels.setPixelColor(i, pixels.Color(128, 0, 128));
  }
  else {
    // Turn off pixels
    pixels.clear();
    //for (i=0; i<N_PIXELS; i++)
    //  pixels.setPixelColor(i,   0,   0, 0);
  }

  pixels.show(); // Send the updated pixel colors to the hardware.

  // delay(DELAYVAL);  // delay to avoid overloading the serial port buffer
}


void chase(uint8_t redValue, uint8_t greenValue, uint8_t blueValue) {
  uint8_t  i;
  for (i=0; i<N_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(redValue, greenValue, blueValue));
    pixels.show();
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    delay(30);
  }
}

void shortFlicker(uint32_t color) {
  flicker(100, 25, color);
}

void mediumFlicker(uint32_t color) {
  flicker(300, 25, color);
}

void longFlicker(uint32_t color) {
  flicker(500, 50, color);
}

void flicker(uint8_t onMS, uint8_t offMS, uint32_t color) {
/*
  uint8_t  i;
  for (i=0; i<N_PIXELS; i++)
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
*/
/*
  Serial.println("color: ");
  Serial.println(color);
  Serial.println("brightness: ");
  Serial.println(pixels.getBrightness());
*/
  
  pixels.fill(color, 0, N_PIXELS);
  pixels.show();
  delay(onMS);
  
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.show();
  delay(offMS);
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
