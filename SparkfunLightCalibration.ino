#include <LiquidCrystal_I2C.h>
#include "SparkFun_AS7265X.h" //Click here to get the library: http://librarymanager/All#SparkFun_AS7265X

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
AS7265X sensor;
float sensorVal;
float irradiance;

bool DEBUG = false;

void setup() {
  delay(500);  
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on

  //Serial.begin(115200);

  //Checks if sensor is connected
  if (sensor.begin() == false)
  {
    //Print loading message on both lines of the LCD.
    lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
    lcd.print("Sensor not");
    lcd.setCursor(0,1);   //Set cursor to character 2 on line 0
    lcd.print("Detected");
    while(1);
  }
  sensor.disableIndicator(); //turn LED lights off

  //Print loading message on both lines of the LCD.
  lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
  lcd.print("Starting Up");
  lcd.setCursor(0,1);   //Set cursor to character 2 on line 0
  lcd.print("PT Calibration");

  delay(2000);
  lcd.clear();
}

void loop() {

  sensor.takeMeasurements(); //This is a hard wait while all 18 channels are measured
  //if (DEBUG == true){
  //Serial.print(sensor.getCalibratedA()); //410nm
  //Serial.print(",");
  //Serial.print(sensor.getCalibratedB()); //435nm
  //Serial.print(",");
  //Serial.print(sensor.getCalibratedC()); //460nm
  //Serial.print(",");
  //Serial.print(sensor.getCalibratedD()); //485nm
  //Serial.print(",");
  //Serial.print(sensor.getCalibratedE()); //510nm
  //Serial.print(",");
  //Serial.print(sensor.getCalibratedF()); //535nm
  //Serial.println(",");
  //}


  
  //Check peak wavelength
  float sensorData[18] = {sensor.getCalibratedA(), sensor.getCalibratedB(), sensor.getCalibratedC(), sensor.getCalibratedD(), sensor.getCalibratedE(), sensor.getCalibratedF(),
  sensor.getCalibratedG(), sensor.getCalibratedH(), sensor.getCalibratedR(), sensor.getCalibratedI(), sensor.getCalibratedS(), sensor.getCalibratedJ(),
  sensor.getCalibratedT(), sensor.getCalibratedU(), sensor.getCalibratedV(), sensor.getCalibratedW(), sensor.getCalibratedK(), sensor.getCalibratedL()};
  float maxVal = sensorData[0];

  //Serial.print("Size of sensorData is: "); 
  //Serial.println(sizeof(sensorData));

  for (int i = 0; i < (sizeof(sensorData) / sizeof(sensorData[0])); i++) { //determine the maximum value in list
    maxVal = max(sensorData[i], maxVal);
    //minVal = min(sensorData[i],minVal);
  }

  lcd.setCursor(8,0);
  lcd.print("InRange:");
  lcd.setCursor(8, 1);

  //Checks if peak is within range of effective phototherapy
  if (maxVal == sensorData[0]){
    lcd.print("410 nm");
  }
  else if (maxVal == sensorData[1]){
    lcd.print("435 nm");
  }
    else if (maxVal == sensorData[2]){
    lcd.print("460 nm");
  }
    else if (maxVal == sensorData[3]){
    lcd.print("485 nm");
  }
  else{
    lcd.clear();
    lcd.setCursor(8,0);
    lcd.print("Out of");
    lcd.setCursor(8,1);
    lcd.print("Range");
  }

 //Math to convert raw to irradiance
  sensorVal = sensorData[2];
  irradiance = 0.0038 * sensorVal;
  //irradiance = int(irradiance);


  //Print Irradiance to Screen
  lcd.setCursor(0,0);   //Set cursor to character 0 on line 0
  lcd.print("uW/cm^2");
  lcd.setCursor(0,1);
  //lcd.print(irradiance);
  lcd.print(irradiance);

  delay(2000);
  lcd.clear();

}
