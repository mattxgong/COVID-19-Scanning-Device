/*
 COVID-19 Scanning Device
 
 It makes sure that any person that tries to pass it doesn't show any
 symptoms of COVID-19. Once the button is pressed, it scans the user
 and makes sure that their temperature is within the normal range. If
 at any point, the user tries to pass without being cleared first, an
 alarm will activate. The buzzer will sound and the LED will flash.
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
// set up the pins for the tmp36, button, buzzer, and led
const int temp = A5, button = 7, pirPin = 13, buzzer = 6, led = 8;
// creating lcd with the pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// track the input from the different sensors
int sensorValue, buttonPress, pirVal;
// hold the current temperature from the tmp36
float currentTemp;
// whether anyone is allowed to pass
bool okayToPass = false;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(50, 2);
  // set up the button, PIR, and LED
  pinMode(button, INPUT_PULLUP);
  pinMode(pirPin, INPUT);
  pinMode(led, OUTPUT);
}

// sounding the alarm function
void alarm(){
  for(int i = 0; i < 15; i++){
    // makes a high-pitched noise and flashes the LED 15 times
  	tone(buzzer, 1047, 700);
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
  }
}

// checking if something trying pass when it's not allowed to
void checkForPass(){
  // if they are not allowed to pass
  if(!okayToPass){
    // see if anything moved
    pirVal = digitalRead(pirPin);
    if(pirVal == HIGH){
      // printing the message
      lcd.clear();
      lcd.print("WARNING!");
      lcd.setCursor(0, 1);
      scrollPrint("TRY TO PASS WITHOUT CONFIRMATION");
      // set off the alarm
      alarm();
      lcd.clear();
    }
  }
}

// print a message longer than 16 characters, scrolling through it
void scrollPrint(String message){
  lcd.print(message);
  for(int i = 0; i < 40; i++){
  	lcd.scrollDisplayLeft();
    delay(100);
  }
}

void loop() {
  // set the cursor to column 0, line 0
  lcd.setCursor(0, 0);
  // get button press input
  buttonPress = digitalRead(button);
  checkForPass();
  
  // check if the button has been pressed
  if(buttonPress == LOW){
    lcd.clear();
    // gets the temperature
    sensorValue = analogRead(temp);
    currentTemp = ((sensorValue * 5.0/1023) - 0.5) * 100;
    // printing out the detected temperature
    String num = String(currentTemp);
    scrollPrint("Your current temperature is " + num);
    checkForPass();
    delay(3000);
    lcd.clear();
    // checking the temperature
    if(currentTemp <= 36.0 || currentTemp < 37.8){
      // it is not within normal range
      scrollPrint("Abnormal temperature. No entry allowed");
      // not allowed to pass
      okayToPass = false;
      // constantly check if something is trying to pass
      for(int i = 0; i < 100; i++){
      	checkForPass();
        delay(50);
      }
    }
    else{
      // it is normal
      scrollPrint("Normal temperature range. You may pass");
      // allowed to pass
      okayToPass = true;
      delay(5000);
    }
    lcd.clear();
  }
  else{
    // nothing has been pressed so it continously reads a default message
  	scrollPrint("Please press for your scan");
    // constantly check if something is trying to pass
    for(int i = 0; i < 20; i++){
      checkForPass();
      delay(50);
    }
    // not allowed to pass
    okayToPass = false;
  }
}
