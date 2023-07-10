#include <SPI.h>
#include <SD.h>

// DEFINE Measuring Resisitance (Rm) & Voltage Input (Vin)
long Rm = 330 * 1000000;  // Input resistor value and convert into micro ohms (1 Ω = 1,000,000 µΩ)
int Vin = 5000;           // Input Voltage in mV

// Pins and variables
int FSR_Pin = A0;         // Analog pin 0
int buttonPin = 2;        // Digital pin for the start/stop button
int ledPin = 8;          // Digital pin for the LED indicator
float voltage = 0;
long time;
long resistance = 0;
float conductance = 0;
float force = 0;

// SD card configuration
const int chipSelect = 10;

// File for data logging
File dataFile;

// Flag variables
boolean programRunning = false;  // Flag to indicate if the program is running

void setup() {
  Serial.begin(9600);
  pinMode(FSR_Pin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // Use internal pull-up resistor for the button
  pinMode(ledPin, OUTPUT);

  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("Card Initialized");
  delay(2000);

  // Create a new file for data logging
  dataFile = SD.open("data.txt", FILE_WRITE);
  if (!dataFile) {
    Serial.println("Error opening data file!");
    return;
  }

  // Write header to the data file
  dataFile.println("Time,Force (N)");

  // Close the data file
  dataFile.close();
}

void loop() {
  // Check the button state
  int buttonState = digitalRead(buttonPin);

  // If the button is pressed, toggle the programRunning flag and update LED state
  if (buttonState == LOW) {
    programRunning = !programRunning;
    digitalWrite(ledPin, programRunning ? HIGH : LOW);
    delay(200);  // Add a small delay for button debounce
  }

  // If the program is not running, return and do nothing
  if (!programRunning) {
    return;
  }

  // Read the FSR value
  int FSRreading = analogRead(FSR_Pin);

  // Map analog value to obtain Vo in mV
  voltage = map(FSRreading, 0, 1023, 0, 5000);
  // Record Time
  time = millis();

  // Print the force to the serial monitor
  Serial.print("voltage =  ");
  Serial.print(voltage);
  Serial.println(" mV ");

  // Open the data file in append mode
  dataFile = SD.open("data.txt", FILE_WRITE);
  if (dataFile) {
    // Get the current timestamp
    unsigned long currentTime = millis();

    // Write the timestamp and force to the data file
    dataFile.print(currentTime / 1000); // Convert to seconds
    dataFile.print(",");
    dataFile.println(voltage);

    // Close the data file
    dataFile.close();

    Serial.println("Data saved to SD card.");
  }
  else {
    Serial.println("Error opening data file!");
  }

  delay(2000);  // Delay between each iteration
}
