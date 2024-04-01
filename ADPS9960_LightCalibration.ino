//FAST LAB
//Compatible with the Arduino IDE 1.0

#include <LiquidCrystal_I2C.h>
#include <Arduino_APDS9960.h>


bool displaySerial = false; //uncomment for testing/debug
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  lcd.init();
  lcd.clear();         
  lcd.backlight();  // Make sure backlight is on

  Serial.begin(9600);
  while (!Serial);

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor.");
  }
  else{
    Serial.println("Screen Connected");
  }

  delay(2000);

}

void loop() {
  // check if a color reading is available
  while (! APDS.colorAvailable()) {
    delay(5);
  }
  int r, g, b;

  // read the color
  APDS.readColor(r, g, b);

  if (displaySerial){
    // print the values
    Serial.print("r = ");
    Serial.println(r);
    Serial.print("g = ");
    Serial.println(g);
    Serial.print("b = ");
    Serial.println(b);
    Serial.println();
  }

  // wait a bit before reading again
  delay(1000);
  //Map ADP sensor values to range of 0 to ~40mW/cm^2
  float slope = 0.0178;
  float irrad = b * slope;

  //Calculate Percentage of blue (460nm) wavelength that is of total color space
  float bluePercent = float(b)/( float(r) + float(g) + float(b) );

  //Is the blue value greater than red and green channels
  bool isBlueGreatest;
  String isBlue;
  if(b > g && b > r){
    isBlueGreatest = true;
    isBlue = "In Range";
    } 
    else {
      isBlueGreatest = false;
      isBlue = "Out of Range";
    }
 
  String irradianceOutput = "Irrad:" + String(irrad) + "uW";
  String percentage = String(bluePercent);

  lcd.clear();
  lcd.setCursor(0, 0);
  //lcd.print(irradianceOutput);
  lcd.print(irrad);
  lcd.setCursor(0, 1);
  lcd.print(isBlue);



}
