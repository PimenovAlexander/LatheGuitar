//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
//#include <Wire.h> 

#if 0
//#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#else
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#endif

#define EN    4
#define STEP  5
#define DIR   6


volatile int64_t counter = 0;  //This variable will increase or decrease depending on the rotation of encoder
volatile int64_t runner = 0;


enum Pressed_Key
{
  Key_None,
  Key_Right,
  Key_Up,
  Key_Down,
  Key_Left,
  Key_Select
};


const char *menu[] = {
   "<Move>",
   "<Slider>",
   "<Path>",
   "<Thread>",
   "<Measure>"
};

int menuPosition = 0;

void renderMenu()
{
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print(menu[menuPosition]);

   if (menuPosition == 0)
   {
     
   }

   if (menuPosition == 4)
   {
     lcd.setCursor(0,1);
     /*Serial.print ((long)(counter * 360 / 1600));
     Serial.print (" "); 
     Serial.println ((long)runner * 360 / 1600);*/
  
     lcd.print((long)(counter * 360 / 800) % 360);
     lcd.print((long)(runner  * 360 / 800) % 360);
     
   }
   
  
}

void MenuKeyLeftPressed()
{
  menuPosition--;
  if (menuPosition < 0) menuPosition = 4;
  //renderMenu();
}

void MenuKeyRightPressed()
{
  menuPosition++;
  if (menuPosition > 4) menuPosition = 0;
  //renderMenu();
}
void MenuKeyUpPressed()
{}
void MenuKeyDownPressed()
{}
void MenuKeySelectPressed()
{
  
}


void Menu()
{
  byte activeKey;
  int ADC_value = analogRead(A0);
  if (ADC_value < 72)       activeKey = Key_Right;
  else if (ADC_value < 237) activeKey = Key_Up;
  else if (ADC_value < 417) activeKey = Key_Down;
  else if (ADC_value < 623) activeKey = Key_Left;
  else if (ADC_value < 882) activeKey = Key_Select;
  else activeKey = Key_None;
  
    switch (activeKey)
    {
      case Key_Left:
        MenuKeyLeftPressed();
        break;
      case Key_Right:
        MenuKeyRightPressed();
        break;
      case Key_Up:
        MenuKeyUpPressed();
        break;
      case Key_Down:
        MenuKeyDownPressed();
        break;       
      case Key_Select:
        MenuKeySelectPressed();    
        break;
    }

} 



ISR(TIMER1_OVF_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  TCNT1 = 34286;            // preload timer
  if (counter != runner)
  {
    if (counter > runner)
    { 
      digitalWrite(DIR, HIGH);
      digitalWrite(STEP, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(10);              // wait for a second
      digitalWrite(STEP, LOW);    // turn the LED off by making the voltage LOW
      delay(10);              // wait for a second
      runner++;
    } else {
      digitalWrite(DIR, LOW);
      digitalWrite(STEP, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(10);              // wait for a second
      digitalWrite(STEP, LOW);    // turn the LED off by making the voltage LOW
      delay(10);              // wait for a second
      runner--;      
    }
  }
}


void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(20) == LOW) {
    counter++;
  }else{
    counter--;
  }

  Serial.print("A");
}

void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(21) == LOW) {
    counter--;
  } else{
    counter++;
  }
  Serial.print("B");

}


void setup()
{
//  lcd.init();                      // initialize the lcd 
  lcd.begin(16, 2);
  // Print a message to the LCD.
//  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello, lathe!");
  lcd.setCursor(0,1);
  lcd.print("Electroguitar");
  delay(500);

  /*   */
  Serial.begin (34800);

  /* Prepare Step*/
  pinMode(EN, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);

  digitalWrite(EN, LOW); 


  /* Prepare timer */
  
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz
//  TCCR1B |= (1 << CS10);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts

  /*   */
  //Setting up interrupt
  pinMode(20, INPUT);
  pinMode(21, INPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);

  //digitalWrite(20, LOW); 
  //digitalWrite(21, LOW); 
  
  
  
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(digitalPinToInterrupt(21), ai0, RISING);  
  attachInterrupt(digitalPinToInterrupt(20), ai1, RISING);  
  
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  //attachInterrupt(20, ai1, RISING);
}



void loop() {
  
  // Send the value of counter
 
  
 /* lcd.print("   ");
  uint8_t v = PIND;  
  lcd.print(v & 0x01 ? 'X' : '0');
  lcd.print(v & 0x02 ? 'X' : '0');
  lcd.print(v & 0x04 ? 'X' : '0');
  lcd.print(v & 0x10 ? 'X' : '0');*/
  
  Menu();
  
}


