#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define ANALOG_PIN  A0  // Piezo attached to Analog A0 on Wemos or Gemma D2 (A1)
#define LED_PIN     D5  // NeoPixel LED strand is connected to GPIO #0 / D0

// How many NeoPixels are attached to the Arduino?
#define N_PIXELS  113  //33  // Number of pixels you are using

#define DELAYVAL 25 // Time (in milliseconds) to pause between pixels

const int threshold1 = 20;
const int threshold2 = 65;
const int threshold3 = 100;
const int threshold4 = 200;
const int threshold5 = 300;
const int threshold = threshold1;  // threshold value to decide when the detected sound is a knock or not

int colorCount = 1;        // counter to determine which color to display next
uint8_t  bright = 255;

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
// Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  uint8_t  i;
  Serial.begin(9600);  // use the serial port
  pixels.begin();      // INITIALIZE NeoPixel strip object (REQUIRED)
  
  pixels.clear(); // Set all pixel colors to 'off'

  // Chase
  chase(0, 0, bright);
/*
  // All on for 2 seconds
  for (i=0; i<N_PIXELS; i++)
    pixels.setPixelColor(i, pixels.Color(0, 0, bright));
  pixels.show(); // Send the updated pixel colors to the hardware.
  delay(2000);
*/
  /*
  shortFlicker(0, 0, bright);
  shortFlicker(0, 0, bright);
  shortFlicker(0, 0, bright);
  mediumFlicker(0, 0, bright);
  longFlicker(0, 0, bright);
  */

  // All on for 3 seconds
  for (i=0; i<N_PIXELS; i++)
    pixels.setPixelColor(i, pixels.Color(0, 0, bright));
  pixels.show(); // Send the updated pixel colors to the hardware.
  delay(3000);
}

void loop() {
  uint8_t  i;
  int      sensorReading;

  pixels.clear(); // Set all pixel colors to 'off'

  sensorReading = analogRead(ANALOG_PIN);          // Raw reading from sensor

  if (sensorReading >= threshold) {
    Serial.println("Raw sensor reading: ");
    Serial.println(sensorReading);
    
    // The first NeoPixel in a strand is #0, second is 1, all the way up
    // to the count of pixels minus one.
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255

    // For each pixel... color each bright blue
    for (i=0; i<N_PIXELS; i++)
      pixels.setPixelColor(i, pixels.Color(0, 0, bright));
        
    // Display a different color on each trigger
    /* if (colorCount == 1) {
      Serial.println("red");
      // For each pixel... color each bright red
      for (i=0; i<N_PIXELS; i++)
        pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      colorCount++;
    }
    else if (colorCount == 2) {
      Serial.println("green");
      // For each pixel... color each bright green
      for (i=0; i<N_PIXELS; i++)
        pixels.setPixelColor(i, pixels.Color(0, 255, 0));
      colorCount++;
    }
    else if (colorCount == 3) {
      Serial.println("blue");
      // For each pixel... color each bright blue
      for (i=0; i<N_PIXELS; i++)
        pixels.setPixelColor(i, pixels.Color(0, 0, 255));
      colorCount++;
    }
    else {
      Serial.println("rainbow");
      // For each pixel... color each based on rainbow gradient
      for (i=0; i<N_PIXELS; i++)
        pixels.setPixelColor(i,Wheel(map(i,0,pixels.numPixels()-1,30,150)));
      colorCount = 1;
    }
    */
  }
  else {
    // Turn off pixels
    for (i=0; i<N_PIXELS; i++)
      pixels.setPixelColor(i,   0,   0, 0);
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

void shortFlicker(uint8_t red, uint8_t green, uint8_t blue) {
  flicker(100, 25, red, green, blue);
}

void mediumFlicker(uint8_t red, uint8_t green, uint8_t blue) {
  flicker(300, 25, red, green, blue);
}

void longFlicker(uint8_t red, uint8_t green, uint8_t blue) {
  flicker(500, 50, red, green, blue);
}

void flicker(uint8_t onMS, uint8_t offMS, uint8_t red, uint8_t green, uint8_t blue) {
  uint8_t  i;
  for (i=0; i<N_PIXELS; i++)
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
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
