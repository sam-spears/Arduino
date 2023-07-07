//Reading Output Voltage (Vo) and Force (N) from FSR

// DEFINE Measuring Resisitance (Rm) & Voltage Input (Vin)
long Rm = 330 * 1000000;  // Input resistor value and convert into micro ohms (1 Ω = 1,000,000 µΩ)
int Vin = 5000;           // Input Voltage in mV

// Pins and variables
int FSR_Pin = A0; //analog pin 0
float voltage = 0;
long time;
long resistance = 0;
float conductance = 0;
float force = 0;




void setup() {
Serial.begin(9600);
pinMode(FSR_Pin, INPUT);
}



void loop() {
int FSRreading = analogRead(FSR_Pin);
 
// Map analog value to obtain Vo in mV
  voltage = map(FSRreading, 0, 1023, 0, 5000);
  
//Calculate resistance using voltage divider equation

  //R_fsr = [(Vin*Rm)/Vo] - Rm
  resistance = (Vin * Rm) / voltage;
  resistance = resistance - Rm;

  //Calculate conductance (micro ohms) (1/R)
  conductance = 1 / resistance;
 
  //Use the FSR resistance vs. force graphs to estimate the force
   
  if (voltage == 0) {
    Serial.println("No force applied");
    delay(1000);
    return;
  }
  if (resistance >= 1/6E-10) {
    force = (voltage/197.96);
  }
  else {
    force = (voltage/43.39) - 20.3;
  } 

  Serial.print("Force =  ");
  Serial.print(force);
  Serial.println(" N ");
  delay(1000);

}
