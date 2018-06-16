#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

SoftwareSerial BTserial(0, 1); // RX | TX

#define OLED_RESET 12
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

const long baudRate = 9600;

String dataString;

int ledPin = 11;
const int trigPin = 10;
const int echoPin = 9;

long duration;
int distanceCm, distanceInch;
long timer = 0;

bool ultrasonicState = false;

void setup()   {
  delay(2000);

  Serial.begin(baudRate);
  BTserial.begin(baudRate);

  pinMode(ledPin, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  initAdaDisplay();
}

void loop() {
  display.clearDisplay();

  bluetoothReadData();

  if (ultrasonicState) {
    ultrasonicSensor();
  }

  display.display();
}


void bluetoothReadData() {

  if (BTserial.available() > 0) {

    dataString = BTserial.readString();

    BTserial.println(dataString);

    if (dataString == "led on") {
      digitalWrite(ledPin, HIGH);
      displayText("LED ON", 10, 15);
    }
    else if (dataString == "led off") {
      digitalWrite(ledPin, LOW);
      displayText("LED OFF", 10, 15);
    }
  }
  displayText(dataString, 10, 15);

  if (dataString == "ultra on") {
    ultrasonicState = true;
    digitalWrite(trigPin, HIGH);
  } else if (dataString == "ultra off") {
    ultrasonicState = false;
    digitalWrite(trigPin, LOW);
  }
}

//static uint32_t previousMillis;
unsigned long previousMillis = 0;
const uint32_t interval = 3000;

void ultrasonicSensor() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.034 / 2;
  distanceInch = duration * 0.0133 / 2;

  displayText("Distance: " + String(distanceCm) + " cm", 10, 55);
  delay(10);
}

void initAdaDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  delay(2000);
  display.clearDisplay();
  displayText("...", 10, 15);
}

void displayText(String value, int xpos, int ypos) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(xpos, ypos);
  display.println(value);
}


