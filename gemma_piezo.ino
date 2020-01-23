#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        1 // On Trinket or Gemma, suggest changing this to 1
#define MIC_PIN   A1  // Microphone is attached to Trinket GPIO #2/Gemma D2 (A1)

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 60  //16 // Popular NeoPixel ring size
#define N_PIXELS  60  // Number of pixels you are using

#define LED_PIN    0  // NeoPixel LED strand is connected to GPIO #0 / D0

// Used for Mic Sensor
// Delete these next 4 lines?
#define DC_OFFSET  0  // DC offset in mic signal - if unusure, leave 0
#define NOISE     100  // Noise/hum/interference in mic signal
#define SAMPLES   60  // Length of buffer for dynamic level adjustment
#define TOP       (N_PIXELS +1) // Allow dot to go slightly off scale

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

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
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

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

  sensorReading = analogRead(MIC_PIN);          // Raw reading from sensor

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

/*
 *   delay(DELAYVAL);  // delay to avoid overloading the serial port buffer
 *   
    // For each pixel...
    for(int i=0; i<NUMPIXELS; i++) {
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));

      pixels.show();   // Send the updated pixel colors to the hardware.

      delay(DELAYVAL); // Pause before next pass through loop
    }
  }
  else {
    leds[0] = CRGB::Black;
    FastLED.show();
  }
*/


  
/*
  n   = abs(sensorReading - 512 - DC_OFFSET);            // Center on zero
  n   = (n <= NOISE) ? 0 : (n - NOISE);      // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3;    // "Dampened" reading (else looks twitchy)
  
  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);

  if(height < 0L)       height = 0;      // Clip output
  else if(height > TOP) height = TOP;
  if(height > peak)     peak   = height; // Keep 'peak' dot at top
*/

/*
  strip.setBrightness(bright);    // Set LED brightness (if POT_PIN at top define commented out, will be full)
  
  // Color pixels based on rainbow gradient
    for(i=0; i<N_PIXELS; i++) {
      // if(i >= height)
      if(n >= threshold)
         strip.setPixelColor(i,   0,   0, 0);
      else
         strip.setPixelColor(i,Wheel(map(i,0,strip.numPixels()-1,30,150)));
    }

   strip.show(); // Update strip

  vol[volCount] = n;                      // Save sample for dynamic leveling
  if(++volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter

  // Get volume range of prior frames
  minLvl = maxLvl = vol[0];
  for(i=1; i<SAMPLES; i++) {
    if(vol[i] < minLvl)      minLvl = vol[i];
    else if(vol[i] > maxLvl) maxLvl = vol[i];
  }
  // minLvl and maxLvl indicate the volume range over prior frames, used
  // for vertically scaling the output graph (so it looks interesting
  // regardless of volume level).  If they're too close together though
  // (e.g. at very low volume levels) the graph becomes super coarse
  // and 'jumpy'...so keep some minimum distance between them (this
  // also lets the graph go to zero when no sound is playing):
  if((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
  minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6; // Dampen min/max levels
  maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6; // (fake rolling average)

  // delay(1000);  // delay to avoid overloading the serial port buffer
  delay(500);  // delay to avoid overloading the serial port buffer
*/
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
