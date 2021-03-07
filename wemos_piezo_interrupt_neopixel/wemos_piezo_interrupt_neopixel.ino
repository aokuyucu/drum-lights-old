#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define ANALOG_PIN  A0  // Piezo is connected to Analog A0 on Wemos or Gemma D2 (A1)
#define LED_PIN     D5  // NeoPixel LED strand is connected to GPIO #0 / D0
#define LED_PIN_2   D7  // second LED
#define BUTTON_PIN  D4  // Push button is connected to D4 on Wemos withut resistor

// Number of LEDs attached to the Arduino
#define N_PIXELS    150
#define N_PIXELS_2  1

#define DELAYVAL 25 // Time (in milliseconds) to pause between pixels
#define LOOP_DELAY 200 // Time (in milliseconds) to pause between loops

unsigned long myTime;

const int threshold1 = 20;
const int threshold2 = 50;
const int threshold3 = 65;
const int threshold4 = 100;
const int threshold5 = 200;

const int threshold = threshold3;  // threshold value to decide when the detected sound is a knock or not

uint8_t  bright = 255;

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
// Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

uint8_t ShortFlash_OnMS = 100,
        ShortFlash_OffMS = 25,
        MediumFlash_OnMS = 300,
        MediumFlash_OffMS = 25,
        LongFlash_OnMS = 500,
        LongFlash_OffMS = 50;

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

volatile uint32_t myColor = bass5color;

//volatile modeID gTriggerMode = setupMode;
volatile unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
volatile unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
volatile uint32_t interruptMillis = millis();
volatile uint32_t interruptDebounce = 150;
int buttonState = 0;         // variable for reading the pushbutton status
volatile uint8_t colorSwitch = 1;

void setup() {
  Serial.begin(9600);  // use the serial port

  pixels.begin();      // INITIALIZE NeoPixel strip object (REQUIRED)

  // Flicker a few times...
  flash(ShortFlash_OnMS, ShortFlash_OffMS, myColor);
  flash(ShortFlash_OnMS, ShortFlash_OffMS, myColor);
  flash(ShortFlash_OnMS, ShortFlash_OffMS, myColor);
  flash(MediumFlash_OnMS, MediumFlash_OffMS, myColor);
  flash(MediumFlash_OnMS, MediumFlash_OffMS, myColor);

  // ... then gradually ascend to full brightness
  gradualAscent(myColor);

  // Set to full brightness for the duration of the sketch
  pixels.setBrightness(bright);

  // initialize the pushbutton pin as an input:
  //pinMode(BUTTON_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, RISING);
}

void loop() {
  uint8_t  i;
  int      sensorReading;

  pixels.clear(); // Set all pixel colors to 'off'

  sensorReading = analogRead(ANALOG_PIN); // Raw reading from sensor

  // Limit the number of readings printed
  /*if (sensorReading >= threshold1) {
    Serial.println("Raw sensor reading: ");
    Serial.println(sensorReading);
  }*/

  if (sensorReading >= threshold) {
    // The first NeoPixel in a strand is #0, second is 1, all the way up
    // to the count of pixels minus one.
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255

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
  Serial.println("handleInterrupt");
  if ((millis() - lastDebounceTime) >= interruptDebounce) {
    /*
    if (gTriggerMode != myDrumLight.triggerMode) {
        gTriggerMode = myDrumLight.triggerMode;
      }
      else {
        gTriggerMode = offMode;
      }
      */
    Serial.println("button pressed --> switch to the next color");
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
    lastDebounceTime = millis();
  }
}
