/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/ff817f2a-111c-4cab-a681-cd38c60f9bf9 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  CloudFlowRate flow;
  CloudTemperatureSensor temperature;
  int soil;
  int soill;
  CloudRelativeHumidity humidity;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"
#include "DHT.h"
const int sensor_pin = A0;
#define DHTPIN 12  
int Value_D8;
int Value_D7;
int Pin_D7 = 13;
int Pin_D8 = 15;
int flowPin = 14;
volatile int count;
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);
#define SENSOR  14
long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
boolean ledState = LOW;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned long flowMilliLitres;
unsigned int totalMilliLitres;
float flowLitres;
float totalLitres;
int relay = 16;
int relay1 = 5;
int ala = 4;
int limit = 300;
void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  dht.begin();
  pinMode(SENSOR, INPUT_PULLUP);
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;
 
  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
 
  pinMode(A0,INPUT);
  pinMode(Pin_D7,OUTPUT);
  pinMode(Pin_D8,OUTPUT);
  pinMode(relay,OUTPUT);
  pinMode(relay1,OUTPUT);
  pinMode(ala,OUTPUT);
 
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  // Your code here 
    // Your code here 
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();  
  if (isnan(hum) || isnan(temp)  ){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  humidity = hum;
  temperature = temp;
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("°C");
  Serial.print(" Humidity: ");
  Serial.print(hum);
  Serial.print("%");
  Serial.println();
  delay(1000);
  float moisture_percentage;

  moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );

  Serial.print("Soil Moisture(in Percentage) = ");
  Serial.print(moisture_percentage);
  Serial.println("%");
  
  delay(1000);
  
  soil = Value_D8;
  soill = Value_D7;
  flow = totalLitres;
  digitalWrite(Pin_D7, HIGH);     //Turn D7 On
  delay(100);                     //Wait for sensor
  Value_D7 = analogRead(0);       //Read Analog pin as D7
  digitalWrite(Pin_D7, LOW);      //Turn D7 Off

  //Repeat for D8
  digitalWrite(Pin_D8, HIGH);     //Turn D8 On
  delay(100);                     //Wait for sensor
  Value_D8 = analogRead(0);       //Read Analog pin as D8
  digitalWrite(Pin_D8, LOW);      //Turn D8 Off
  delay(100);                     //Wait for sensor
  //Print the results to the serial monitor
  Serial.print("D7 = ");   
  Serial.print(Value_D7);
  Serial.print(" / D8 = ");   
  Serial.println(Value_D8);
  if (Value_D7<280){
    digitalWrite(16, HIGH);


  
  }
  else {
    digitalWrite(16, LOW);
  }
  delay(1000);
  if (Value_D8<280){
    digitalWrite(5, HIGH);
    
    
  }
  else {
    digitalWrite(5, LOW);
  }
  delay(1000);

    // put your main code here, to run repeatedly:
  count = 0;      // Reset the counter so we start counting from 0 again
  interrupts();   //Enables interrupts on the Arduino
  delay (1000);   //Wait 1 second
  noInterrupts(); //Disable the interrupts on the Arduino

  //Start the math
  flowRate = (count * 10);        //Take counted pulses in the last second and multiply by 2.25mL
  flowRate = flowRate * 60;         //Convert seconds to minutes, giving you mL / Minute
  flowRate = flowRate / 1000;       //Convert mL to Liters, giving you Liters / Minute

  Serial.println(flowRate);
     currentMillis = millis();
  if (currentMillis - previousMillis > interval) 
  {
    
    pulse1Sec = pulseCount;
    pulseCount = 0;
 
    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();
 
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 300) * 1000;
    flowLitres = (flowRate / 300);
 
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
    totalLitres += (flowLitres * 12);
    
    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(float(flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t"); 
    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(totalLitres);
    Serial.println("L");
      
}
  
}

void pulseCounter()
{
  pulseCount++;
}
/*
  Since LEDOutput is READ_WRITE variable, onLEDOutputChange() is
  executed every time a new value is received from IoT Cloud.
*/




