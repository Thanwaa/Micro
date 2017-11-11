#include <Password.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
Password password = Password( "3012" ); // Set the Password

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);
SoftwareSerial ArduinoSerial(3,2);
const byte ROWS = 4; // Four rows
const byte COLS = 4; //  columns
int pir = 11;          
int Buzzer = A0;         
int alarmValue = 0;
int fl = 0, state = 0, state1 = 0, state2 = 0, Alarm_fl = 0; // Flag
int dir = 8, dir1 = 0;
int A1 = 1 ;
String test = "                    ";

// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {10, 9, 8, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6,5, 4}; //co
// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup()
{
  Serial.begin(9600); 
  lcd.begin();  
  pinMode(Buzzer, OUTPUT);         // Sets the digital pin as output
  keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
  // Sets First Screen LCD
  lcd.print(" Password Lock");
  lcd.setCursor(0, 1);
  lcd.print(" password ");
  lcd.setCursor(0, 2);
  ArduinoSerial.begin(4800);
}

pinMode( A1,INPUT);

void loop()
{
  keypad.getKey(); 
  motion_on();
  Line_on();     

}

 if( A1=='3012')
 { digitalWrite(buzzer,LOW );
 delay(100);
 }


void keypadEvent(KeypadEvent eKey) {
  switch (keypad.getState()) {
    case PRESSED:
      Serial.print("Pressed: ");
      Serial.println(eKey);
      lcd.setCursor(dir1, 3);
      lcd.print(test);
      test = "*";
      lcd.setCursor(dir, 3);
      lcd.print(test);
      dir1 = dir;
      switch (eKey)
      {
        case '*':
          if (fl == 1)
          {
            state2 = 1;
            lcd.setCursor(0, 3);
            lcd.print("  ** PassWord Lock **  ");
            dir  = 8;
            dir1 = 0;
            test = "                    ";
          }
          break;
        case '#':
          state2 = 0;
          checkPassword(); // Call the function CheckPassword
          password.reset(); //Reset Key
          dir  = 8;
          dir1 = 0;
          test = "                    ";
          lcd.setCursor(0, 3);
          lcd.print(test);
          break;
        default: password.append(eKey);
          dir++;
      }
  }
}

void checkPassword() // Function check the password
{
  if (password.evaluate()) {
    //Add code to run if it works
    while (state2 == 0)
    {
      Serial.println("Success");
      fl = 1;
      
      lcd.setCursor(0, 3);
      lcd.print(" Password UnLock");
      keypad.getKey();
      Alarm_off();
      password.reset();
    }
  }
  else {
    Serial.println("Eror");
    //add code to run if it did not work
    lcd.setCursor(0, 3);
    lcd.print("   Password Eror ");
    delay(1000);
  }
}


void Alarm() // Function Alarm.
{
  Serial.print("BUZZER_ON");
  digitalWrite(Buzzer, HIGH); // Turn on Buzzer
  delay(100); // delay 100 milliseconds
  digitalWrite(Buzzer, LOW); // Turn off Buzzer
  delay(100); // delay 100 milliseconds

}

void Alarm_off() // Function Alarm off.
{
  digitalWrite(Buzzer, LOW); //Set buzzer pin is HIGH for stop alarm
}

void motion_on() // Function Open PIR Motion Sensor.
{
  alarmValue = digitalRead(pir); // Read analog sensor 0 or 1.
  //Serial.println (alarmValue); // Debug Sensor value.
  if (alarmValue == 1) // Check the value
  {
    
    Alarm();
    Line_on();
    Serial.print("on");
  }
  else
  {
    Alarm_off();
     Serial.print("OFF");
  }
}
void Line_on()
{
  if (alarmValue == 1)
  {
  ArduinoSerial.print(10);
   ArduinoSerial.print("\n");
    delay(100);
    Serial.print("LINE\n");
  }
}
