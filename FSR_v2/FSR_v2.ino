#include <SPI.h>
#include <SD.h>

// DEFINE Measuring Resisitance (Rm) & Voltage Input (Vin)
long Rm = 330 * 1000000;  // Input resistor value and convert into micro ohms (1 Ω = 1,000,000 µΩ)
int Vin = 5000;           // Input Voltage in mV

// Pins and variables
int FSR_Pin = A0; // Analog pin 0
float voltage = 0;
long time;
long resistance = 0;
float conductance = 0;
float force = 0;

// SD card configuration
const int chipSelect = 10;

// File for data logging
File dataFile;

void setup() {
  Serial.begin(9600);
  pinMode(FSR_Pin, INPUT);
  pinMode(chipSelect, OUTPUT);

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
  // Read the FSR value
  int FSRreading = analogRead(FSR_Pin);

  // Map analog value to obtain Vo in mV
  voltage = map(FSRreading, 0, 1023, 0, 5000);

  // Calculate resistance using voltage divider equation
  resistance = (Vin * Rm) / voltage;
  resistance = resistance - Rm;

  // Calculate conductance (micro ohms) (1/R)
  conductance = 1 / resistance;

  // Use the FSR resistance vs. force graphs to estimate the force
  if (voltage == 0) {
    Serial.println("No force applied");
    delay(1000);
    return;
  }
  if (resistance >= 1 / 6E-10) {
    force = voltage / 197.96;
  }
  else {
    force = (voltage / 43.39) - 20.3;
  }

  // Print the force to the serial monitor
  Serial.print("Force =  ");
  Serial.print(force);
  Serial.println(" N ");

  // Open the data file in append mode
  dataFile = SD.open("data.txt", FILE_WRITE);
  if (dataFile) {
    // Get the current timestamp
    unsigned long currentTime = millis();

    // Write the timestamp and force to the data file
    dataFile.print(currentTime / 1000); // Convert to seconds
    dataFile.print(",");
    dataFile.println(force);

    // Close the data file
    dataFile.close();

    Serial.println("Data saved to SD card.");
  }
  else {
    Serial.println("Error opening data file!");
  }

delay(60000); // Delay for 1 minute

}
