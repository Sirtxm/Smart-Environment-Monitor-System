#include <Wire.h>
#include <Servo.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

// Pin Definitions
#define trig 12          // Ultrasonic sensor trigger pin
#define echo 13          // Ultrasonic sensor echo pin
#define DHTPIN 4         // DHT11 data pin
#define DHTTYPE DHT11    // DHT sensor type
#define BUTTON_PIN 3     // Mode toggle button pin (interrupt-enabled)
#define PIR_PIN 7        // PIR motion sensor pin (not used yet)

unsigned long pirPreviousMillis = 0;   // For PIR (reserved for future use)
unsigned long previousMillis = 0;      // Timing for non-blocking delays
const long interval = 1000;            // Delay between sensor reads
const long pirInterval = 500;          // Reserved for future PIR use

long duration;
float distance;

// System states
const int IDLE = 0;
const int MEASURE = 1;
const int DISTANCE = 2;
int state;
int displayMode = MEASURE;             // Initial display mode
volatile bool flag_change_mode = false; // Flag set by button interrupt

Servo myservo;
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);    // LCD address 0x27, 16 cols, 2 rows

// Interrupt Service Routine (ISR) to toggle display mode on button press
void handleButtonInterrupt() { 
    flag_change_mode = true;
}

void setup() {
    pinMode(PIR_PIN, INPUT_PULLUP);         // Optional PIR input
    pinMode(trig, OUTPUT);                  // Trigger pin for ultrasonic
    pinMode(echo, INPUT);                   // Echo pin for ultrasonic
    pinMode(BUTTON_PIN, INPUT_PULLUP);      // Button with pull-up
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonInterrupt, RISING); // Interrupt on rising edge

    Serial.begin(9600);                     // Initialize serial communication
    myservo.attach(9);                      // Attach servo to pin 9
    lcd.begin();                            // Initialize LCD
    lcd.backlight();                        // Turn on backlight
    dht.begin();                            // Start DHT sensor
    state = IDLE;                           // Start in IDLE state
}

void loop() {
    unsigned long currentMillis = millis(); // Capture current time for non-blocking delay

    if (state == IDLE) {
        if (flag_change_mode) {
            flag_change_mode = false; // Reset interrupt flag

            // Toggle between MEASURE and DISTANCE modes
            if (displayMode == MEASURE) {
                displayMode = DISTANCE;
            } else {
                displayMode = MEASURE;
            }
        }
        state = displayMode; // Update state based on selected display mode
    }

    else if (state == MEASURE) {
        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;

            // Read DHT sensor
            float h = dht.readHumidity();
            float t = dht.readTemperature();

            // Print to Serial Monitor
            Serial.print("Humidity: ");
            Serial.print(h);
            Serial.print("%, Temperature: ");
            Serial.print(t);
            Serial.println("°C");

            // Display on LCD
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Humidity: ");
            lcd.print(h);
            lcd.print("%");
            lcd.setCursor(0, 1);
            lcd.print("Temp: ");
            lcd.print(t);
            lcd.print(" C");
        }
        state = IDLE;
    }
    
    else if (state == DISTANCE) {
        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;

            // Trigger ultrasonic sensor
            digitalWrite(trig, LOW);
            delayMicroseconds(2);
            digitalWrite(trig, HIGH);
            delayMicroseconds(10);
            digitalWrite(trig, LOW);

            // Read echo and calculate distance
            duration = pulseIn(echo, HIGH);
            distance = (duration * 0.034) / 2;

            // Display distance on LCD
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Distance: ");
            lcd.print(distance);
            lcd.print(" cm");

            // Servo control based on distance
            if (distance < 10) {
                myservo.write(0); // Close or activate
            } else {
                myservo.write(90); // Open or idle position
            }
        }
        state = IDLE;
    }

    // Remove the stray "1" at the end — not needed
}
