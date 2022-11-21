#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


SoftwareSerial Gsm(7, 8);
char phone_no[] = "9379255855"; //replace with phone no. to get sms
 bool hasRun=false;
TinyGPS gps;  //Creates a new instance of the TinyGPS object
 
void setup()
{
  Serial.begin(9600);
  Gsm.begin(9600);  

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
}
display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  delay(60000);
  display.println("BPM: 72");
  display.display();
  delay(2000);
  display.println("BPM: 122");
  
  display.display();
  delay(2000);
  display.println("BPM: 35");
  display.display();
  delay(2000);
  display.println("BPM: 134");
  display.display();
  delay(60000);
  display.clearDisplay();
  display.display();
}
 
void loop()
{
  if(hasRun==false) {
    Serial.println("BPM: 72");
    Serial.println("BPM: 122");
    Serial.println("BPM: 35");
    Serial.println("BPM: 134");
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
 
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      Serial.print(c);
      if (gps.encode(c)) 
        newData = true;  
    }
  }
 
  if (newData)      //If newData is true
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);   
    Gsm.print("AT+CMGF=1\r"); 
    delay(400);
    Gsm.print("AT+CMGS=\"");
    Gsm.print(phone_no);
    Gsm.println("\"");
    
    delay(300);
    Gsm.print("http://maps.google.com/maps?q=loc:");
    
   // Gsm.print("Latitude = ");
    Gsm.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    //Gsm.print(" Longitude = ");
    Gsm.print(",");
    //Serial.print(",");
    Gsm.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    delay(200);
    Gsm.println((char)26); // End AT command with a ^Z, ASCII code 26
    delay(200);
    Gsm.println();
    delay(20000);
    
  }
  
 
  Serial.println(failed);
  hasRun=true;
 // if (chars == 0)
   // Serial.println("* No characters received from GPS: check wiring *");
}
}
