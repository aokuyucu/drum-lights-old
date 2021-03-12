#include <Adafruit_NeoPixel.h>
#include "drum_config.h"

// Wemos board, Piezo trigger, Pushbutton interrupt color change, NeoPixel LED library.
// Pushbutton cycles through 5 different LED colors.
// Drum and color definitions in header file.
// Second LED "strip" with a single pixel as an indicator light.
// LED grid instead of strip.
// Trial 05: Switch colors based on show timing


// Which pin on the Arduino is connected to the NeoPixels?
#define ANALOG_PIN        A0  // Piezo is connected to Analog A0 on Wemos or Gemma D2 (A1)
#define LED_PIN_MAIN      D5  // NeoPixel LED strand is connected to D5
#define LED_PIN_INDICATOR D7  // Single, indicator LED is connected to D7
#define BUTTON_PIN        D4  // Push button is connected to D4 on Wemos withut resistor

// Number of LEDs attached to the Arduino


#define DELAYVAL 25     // Time (in milliseconds) to pause between pixels
#define LOOP_DELAY 200  // Time (in milliseconds) to pause between loops

#define DRUM_ID snare1          // See drums.h
//#define COLOR 65523
#define MAIN_BRIGHTNESS 255     // Brightness of the main light strip or grid. Valid values are 0-255.
#define INDICATOR_BRIGHTNESS 25 // Brightness of the single pixel inidicator. Valid values are 0-255.
#define THRESHOLD 65            // threshold value to decide when the detected sound is a knock.
                                // Other useful values: 20, 50, 100, 200
#define N_PIXELS 64             // Number of LEDs attached to the Arduino
                                // 150 for LED strip; 64 for LED grid.
#define N_PIXELS_INDICATOR 1    // Single pixel indicator light.
#define TIME_01 31000           // Time (in ms) that the corresponding color should be on until
#define COLOR_01 000000         // BLACK/OFF RGB 0,0,0 HEX 000000
#define TIME_02 51000           // Time (in ms) that the corresponding color should be on until
#define COLOR_02 16711680       // RED RGB 255,0,0 HEX FF0000
#define TIME_03 93000           // Time (in ms) that the corresponding color should be on until
#define COLOR_03 16711680       // RED RGB 255,0,0 HEX FF0000
#define TIME_04 110000          // Time (in ms) that the corresponding color should be on until
#define COLOR_04 16753920       // ORANGE RGB 255,165,0 HEX FFA500
#define TIME_05 120000          // Time (in ms) that the corresponding color should be on until
#define COLOR_05 16766720       // GOLD RGB 255,215,0 HEX FFD700
#define TIME_06 138000          // Time (in ms) that the corresponding color should be on until
#define COLOR_06 16776960       // YELLOW RGB 255,255,0 HEX FFFF00
#define TIME_07 166000          // Time (in ms) that the corresponding color should be on until
#define COLOR_07 65280          // GREEN RGB 0,255,0 HEX 00FF00
#define TIME_08 173000          // Time (in ms) that the corresponding color should be on until
#define COLOR_08 000000         // BLACK/OFF RGB 0,0,0 HEX 000000
#define TIME_09 180000          // Time (in ms) that the corresponding color should be on until
#define COLOR_09 8388736        // PURPLE RGB 128,0,128 HEX 800080

uint32_t myTime;

//const int threshold = 65;

//uint8_t bright = 255;
//uint8_t indicatorBrightness = 25;

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
// Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN_MAIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels(N_PIXELS, LED_PIN_MAIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel indicator(N_PIXELS_INDICATOR, LED_PIN_INDICATOR, NEO_GRB + NEO_KHZ800);

uint32_t purple = pixels.Color(128, 0, 128),
         yellow = pixels.Color(128, 128, 0),
         red = pixels.Color(255, 0, 0),
         green = pixels.Color(0, 255, 0),
         blue = pixels.Color(0, 0, 255),
         darkyellow1 = pixels.Color(204, 204, 0),
         darkviolet = pixels.Color(148,0,211),
         darkorchid = pixels.Color(153,50,204),
         darkmagenta = pixels.Color(139,0,139),
         magenta = pixels.Color(255, 0, 255);

uint32_t bass1color = red,
          bass2color = purple,
          bass3color = yellow,
          bass4color = green,
          bass5color = blue;

volatile uint32_t myColor = COLOR_01;

//volatile modeID gTriggerMode = setupMode;
volatile unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
volatile unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
volatile uint32_t interruptMillis = millis();
volatile uint32_t interruptDebounce = 150;
int buttonState = 0;         // variable for reading the pushbutton status
volatile uint8_t colorSwitch = 1;

void setup() {
  Serial.begin(9600);  // use the serial port

  pixels.begin();     // INITIALIZE NeoPixel strip object (REQUIRED)
  indicator.begin();  // Initialize the Indicator strip

  //chase(red, green, blue);
  //startup(myColor);   // Flash and gradually ascent at startup
  
  // Set to full brightness for the duration of the sketch
  pixels.setBrightness(MAIN_BRIGHTNESS);
  indicator.setBrightness(INDICATOR_BRIGHTNESS);

  setIndicator(myColor); // Indicator pixel should always be ON and should show the strip color.
  
  // initialize the pushbutton pin as an input:
  //pinMode(BUTTON_PIN, INPUT);
  //pinMode(BUTTON_PIN, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, RISING);
}

void loop() {
  int sensorReading;

  pixels.clear(); // Set all pixel colors to 'off'

  // Check if it's time to change color
  myTime = millis();
//Serial.print("myTime: ");
//Serial.println(myTime);
  if (myTime < TIME_01)
    myColor = COLOR_01;
  else if (myTime < TIME_02)
    myColor = COLOR_02;
  else if (myTime < TIME_03)
    myColor = COLOR_03;
  else if (myTime < TIME_04)
    myColor = COLOR_04;
  else if (myTime < TIME_05)
    myColor = COLOR_05;
  else if (myTime < TIME_06)
    myColor = COLOR_06;
  else if (myTime < TIME_07)
    myColor = COLOR_07;
  else if (myTime < TIME_08)
    myColor = COLOR_08;
  else if (myTime < TIME_09)
    myColor = COLOR_09;

  setIndicator(myColor); // Indicator pixel should always be ON and should show the strip color.
  
  sensorReading = analogRead(ANALOG_PIN); // Raw reading from sensor
  if (sensorReading >= THRESHOLD) {
    // The first NeoPixel in a strand is #0, second is 1, all the way up
    // to the count of pixels minus one.
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
  
    // Limit the number of readings printed
    //Serial.print("Raw sensor reading: ");
    //Serial.println(sensorReading);

    // Color all pixels myColor, starting at position 0 
    pixels.fill(myColor, 0, N_PIXELS);
  }
  else {
    // Turn off pixels
    pixels.clear();
  }

  pixels.show(); // Send the updated pixel colors to the hardware.

  //delay(LOOP_DELAY);  // delay to avoid overloading the serial port buffer
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

// Startup function for the main LED strip
// Flicker a few times at the start, then gradually ascend to full brightness.
void startup(uint32_t color) {
  // Flicker a few times...
  flash(ShortFlash_OnMS, ShortFlash_OffMS, color);
  flash(ShortFlash_OnMS, ShortFlash_OffMS, color);
  flash(ShortFlash_OnMS, ShortFlash_OffMS, color);
  flash(MediumFlash_OnMS, MediumFlash_OffMS, color);
  flash(MediumFlash_OnMS, MediumFlash_OffMS, color);

  // ... then gradually ascend to full brightness
  gradualAscent(color);
}

void flash(uint8_t onMS, uint8_t offMS, uint32_t color) {
  pixels.fill(color, 0, N_PIXELS);
  pixels.show();
  delay(onMS);
  
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.show();
  delay(offMS);
}

// Gradually ascend to full brightness for the given color
void gradualAscent(uint32_t color) {
    uint8_t  interimDelay = 150;  // delay between each increasing step of brightness in the gradual ascent
    
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
}

ICACHE_RAM_ATTR void handleInterrupt() {
  //Serial.println("handleInterrupt");
  if ((millis() - lastDebounceTime) >= interruptDebounce) {
    //Serial.println("button pressed --> switch to the next color");
    switch (colorSwitch) {
      case 1:
        myColor = purple;
        colorSwitch++;
        break;
      case 2:
        myColor = yellow;
        colorSwitch++;
        break;
      case 3:
        myColor = blue;
        colorSwitch++;
        break;
      case 4:
        myColor = green;
        colorSwitch++;
        break;
      case 5:
        myColor = red;
        colorSwitch = 1;
        break;
      }
    setIndicator(myColor);  // Change the indicator LED to match the new color
    lastDebounceTime = millis();
  }
}

// Set the first pixel on D7 to be always ON.
void setIndicator(uint32_t color) {
  indicator.clear();
  indicator.fill(color, 0, 1); // Indicator pixel should always be ON.
  indicator.show();
}
