/*
  ReadAnalogVoltage
  The ESP8266EX(Which IC D1 board used)  integrates a generic purpose 10-bit analog ADC. The ADC range is from 0V to 1.0V
  And on the  board, we use following circuit:
  -----A0
  |
 220K
  |--- ADC
 100K
  |
 GND
so,the input voltage(A0) can be 3.2V, and the A0=3.2*ADC
 */

// Which pin on the Arduino is connected to the LED?
#define ANALOG_PIN   A0  // Piezo attached to Analog A0 on Wemos or Gemma D2 (A1)

#define N_PIXELS  60  // Number of pixels you are using
#define LED_PIN    0  // NeoPixel LED strand is connected to GPIO #0 / D0
#define DELAYVAL  25  // Time (in milliseconds) to pause between pixels

const int threshold = 20;  // threshold value to decide when the detected sound is a knock or not
const int threshold1 = 20;
const int threshold2 = 100;
const int threshold3 = 200;
const int threshold4 = 300;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 115200 bits per second:
  // Serial.begin(115200);
  Serial.begin(9600);  // use the serial port
}

// the loop routine runs over and over again forever:
void loop() {
  int sensorReading;

  sensorReading = analogRead(ANALOG_PIN);          // Raw reading from sensor
  Serial.println("Raw sensor reading: ");
  Serial.println(sensorReading);

  delay(DELAYVAL);  // delay to avoid overloading the serial port buffer

/*
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 3.2V):
  float voltage = sensorValue * (3.2 / 1023.0);
  // print out the value you read:
  Serial.println(voltage);
*/
}
