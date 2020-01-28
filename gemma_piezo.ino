#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define ANALOG_PIN   A1  // Microphone is attached to Trinket GPIO #2/Gemma D2 (A1)

// How many NeoPixels are attached to the Arduino?
#define N_PIXELS  33  // Number of pixels you are using

#define LED_PIN    0  // NeoPixel LED strand is connected to GPIO #0 / D0

// Used for Mic Sensor
// Delete these next 4 lines?
#define DC_OFFSET  0  // DC offset in mic signal - if unusure, leave 0
#define NOISE     100  // Noise/hum/interference in mic signal
#define SAMPLES   60  // Length of buffer for dynamic level adjustment
#define TOP       (N_PIXELS +1) // Allow dot to go slightly off scale

#define DELAYVAL 1000 // Time (in milliseconds) to pause between pixels

byte
  peak      = 0,      // Used for falling dot
  dotCount  = 0,      // Frame counter for delaying dot-falling speed
  volCount  = 0;      // Frame counter for storing past volume data

// Used for Mic Sensor
// Delete these next 5 lines?
int
  vol[SAMPLES],       // Collection of prior volume samples
  lvl       = 10,     // Current "dampened" audio level
  minLvlAvg = 0,      // For dynamic adjustment of graph low & high
  maxLvlAvg = 512;

// Adafruit_NeoPixel  strip = Adafruit_NeoPixel(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

const int threshold = 20;  // threshold value to decide when the detected sound is a knock or not
int colorCount = 1;        // counter to determine which color to display next

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
// Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // Used for Mic Sensor
  // Delete these next 2 lines?
  //memset(vol, 0, sizeof(vol));
  memset(vol,0,sizeof(int)*SAMPLES);//Thanks Neil!

  Serial.begin(9600);       // use the serial port
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  uint8_t  i;
  uint16_t minLvl, maxLvl;
  int      sensorReading, height;
  uint8_t  bright = 255;

  pixels.clear(); // Set all pixel colors to 'off'

  sensorReading = analogRead(ANALOG_PIN);          // Raw reading from sensor

  if (sensorReading >= threshold) {
    Serial.println("Raw sensor reading: ");
    Serial.println(sensorReading);
    
    // The first NeoPixel in a strand is #0, second is 1, all the way up
    // to the count of pixels minus one.
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255

    // Display a different color on each hit
    if (colorCount == 1) {
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
    // pixels.show(); // Send the updated pixel colors to the hardware.
  }
  else {
    // Turn off pixels
    for (i=0; i<N_PIXELS; i++)
      pixels.setPixelColor(i,   0,   0, 0);
    // pixels.show(); // Update strip
  }

  pixels.show(); // Send the updated pixel colors to the hardware.

  // delay(DELAYVAL);  // delay to avoid overloading the serial port buffer
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
