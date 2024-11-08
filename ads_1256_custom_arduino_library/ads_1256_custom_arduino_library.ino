#include <SPI.h>
#include <Wire.h>
#include <digitalWriteFast.h> 

#define ADS_RST_PIN    9 // ADS1256 reset pin
#define ADS_RDY_PIN    2 // ADS1256 data ready
#define ADS_CS_PIN    10 // ADS1256 chip select

double resolution = 8388608.; // 2^23-1
double Gain = 64.; 
double vRef = 5.0; 
double bitToVolt = 0.;

int32_t val1;
int32_t val2;
int32_t val3;
int32_t val4; // Declare val4

void setup() {
  delay(1000);
  Serial.begin(115200);
  Wire.begin(8); // Initialize I2C as a slave with address 8
  Wire.onRequest(requestEvent); // Register the request event

  pinMode(ADS_CS_PIN, OUTPUT);
  pinMode(ADS_RDY_PIN, INPUT);
  pinMode(ADS_RST_PIN, OUTPUT);

  SPI.begin();

  initADS();
  Serial.println("done init");

  bitToVolt = resolution * Gain / vRef;
}

void loop() {
  // Read values from all four channels
  read_four_values();
  
  // Optional: Add a delay to control loop frequency
  //delay(1000);
}

void requestEvent() {
  // Create a buffer to hold the values to be sent
  byte buffer[16]; // 4 values x 4 bytes each = 16 bytes

  // Copy the values into the buffer
  memcpy(buffer, &val1, sizeof(val1));
  memcpy(buffer + 4, &val2, sizeof(val2));
  memcpy(buffer + 8, &val3, sizeof(val3));
  memcpy(buffer + 12, &val4, sizeof(val4));

  // Send the buffer over I2C
  Wire.write(buffer, sizeof(buffer));
}
