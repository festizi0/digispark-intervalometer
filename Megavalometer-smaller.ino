
#include <Tiny4kOLED.h>

 const int pinIRLED = 1; // assign the Infrared emitter/ diode to pin 13
 
 const int buttonPin = 0;     // the number of the pushbutton pin // Pin 5 is Analog pin 0

 const int BUTTONU = 1;
 const int BUTTONC = 2;
 const int BUTTONL = 3;
 const int BUTTOND = 4;
 const int BUTTONR = 5;
 
 const int BUTTONULOW = 70;
 const int BUTTONUHIGH = 100;
 const int BUTTONCLOW = 570;
 const int BUTTONCHIGH = 605;
 const int BUTTONLLOW = 850;
 const int BUTTONLHIGH = 890;
 const int BUTTONDLOW = 920;
 const int BUTTONDHIGH = 959;
 const int BUTTONRLOW = 960;
 const int BUTTONRHIGH = 1000;

 // Variables will change:
 int buttonState;             // the current reading from the input pin
 int lastButtonState = LOW;   // the previous reading from the input pin

 int shotNum = 0;
 
 long delayinterval = 10000; //default delay is 10 seconds
 unsigned long previousMillis = 0;

 // the following variables are long's because the time, measured in miliseconds,
 // will quickly become a bigger number than can be stored in an int.
 long lastDebounceTime = 0;  // the last time the output pin was toggled
 long debounceDelay = 50;    // the debounce time; increase if the output flickers

// ============================================================================

void setup() {
  // Send the initialization sequence to the oled. This leaves the display turned off.
  oled.begin();
  // This example only uses a single font, so it can be set once here.
  // The characters in the 8x16 font are 8 pixels wide and 16 pixels tall.
  // 2 lines of 16 characters exactly fills 128x32.
  oled.setFont(FONT6X8);
  // Setup the first half of memory.
  initDisplay();
  // Switch the half of RAM that we are writing to, to be the half that is non currently displayed.
  oled.switchRenderFrame();
  // Setup the second half of memory.
  initDisplay();
  // Call your own display updating code.
  updateDisplay();
  // Turn on the display.
  oled.on();
  
  pinMode(pinIRLED, OUTPUT);                            
  pinMode(5, INPUT);  // Pin 5 is Analog pin 0
}

void loop() {

     // read the state of the switch into a local variable:
   int reading = analogRead(0);
   int tmpButtonState = LOW;             // the current reading from the input pin

   if(reading>BUTTONRLOW && reading<BUTTONRHIGH){
     //Read switch R
     tmpButtonState = BUTTONR;
   }else if(reading>BUTTONDLOW && reading<BUTTONDHIGH){
     //Read switch D
     tmpButtonState = BUTTOND;
   }else if(reading>BUTTONLLOW && reading<BUTTONLHIGH){
     //Read switch L
     tmpButtonState = BUTTONL;
   }else if(reading>BUTTONCLOW && reading<BUTTONCHIGH){
     //Read switch C
     tmpButtonState = BUTTONC;
   }else if(reading>BUTTONULOW && reading<BUTTONUHIGH){
     //Read switch U
     tmpButtonState = BUTTONU;
   }else{
     //No button is pressed;
     tmpButtonState = LOW;
   }

   // check to see if you just pressed the button 
   // (i.e. the input went from LOW to a buttonState),  and you've waited 
   // long enough since the last press to ignore any noise:  

   // If the switch changed, due to noise or pressing:
   if (tmpButtonState != lastButtonState) {
     // reset the debouncing timer
     lastDebounceTime = millis();
   } 

   if ((millis() - lastDebounceTime) > debounceDelay) {
     // whatever the reading is at, it's been there for longer
     // than the debounce delay, so take it as the actual current state:
     buttonState = tmpButtonState;
   }

   // save the reading.  Next time through the loop,
   // it'll be the lastButtonState:
   lastButtonState = tmpButtonState;
   
   // set the LED using the state of the button for testing:
   switch(buttonState){
     case BUTTONU:
       delayinterval += 500;
       updateDisplay();
     break;
     case BUTTONC:
//     digitalWrite(ledPin, buttonState>0);
     break;
     case BUTTONL:
//     digitalWrite(ledPin, buttonState>0);
     break;
     case BUTTOND:
       delayinterval -= 500;
       updateDisplay();
       
     break;
     case BUTTONR:
//     digitalWrite(ledPin, buttonState>0);
     break;
   }

     unsigned long currentMillis = millis();

   if (currentMillis - previousMillis >= delayinterval) {
     // save the last time you blinked the LED
     previousMillis = currentMillis;

     // do the thing
     takePicture();
     shotNum++;
     updateDisplay();
   }


}

void initDisplay() {
  // Clear whatever random data has been left in memory.
  oled.clear();
  // Position the text cursor
  // In order to keep the library size small, text can only be positioned
  // with the top of the font aligned with one of the four 8 bit high RAM pages.
  // The Y value therefore can only have the value 0, 1, 2, or 3.
  // usage: oled.setCursor(X IN PIXELS, Y IN ROWS OF 8 PIXELS STARTING WITH 0);
  //oled.setCursor(0, 1);
  // Write text to oled RAM (which is not currently being displayed).
  //oled.print(F("Delay:"));
}


void updateDisplay() {
  // Position the text cursor
  oled.clear();
  oled.setCursor(0, 0);
  oled.print(F("Shots:"));
  oled.setCursor(38, 0);
  oled.print(shotNum);
  oled.setCursor(0, 2);
  
  oled.print(F("Delay:"));
  // Write the number of milliseconds since power on.
  // The number increases, so always overwrites any stale data.
  // This means we do not need to repeatedly clear and initialize the display.
  oled.setCursor(38, 2);
  oled.print(delayinterval);
  // Swap which half of RAM is being written to, and which half is being displayed.
  // This is equivalent to calling both switchRenderFrame and switchDisplayFrame.
  oled.switchFrame();
}


// sets the pulse of the IR signal.
void pulseON(int pulseTime) {
  unsigned long endPulse = micros() + pulseTime;        // create the microseconds to pulse for
  while( micros() < endPulse) {
    digitalWrite(pinIRLED, HIGH);                       // turn IR on
    delayMicroseconds(13);                              // half the clock cycle for 38Khz (26.32Ã—10-6s) - e.g. the 'on' part of our wave
    digitalWrite(pinIRLED, LOW);                        // turn IR off
    delayMicroseconds(13);                              // delay for the other half of the cycle to generate wave/ oscillation
  }
}
void pulseOFF(unsigned long startDelay) {
  unsigned long endDelay = micros() + startDelay;       // create the microseconds to delay for
  while(micros() < endDelay);
}
void takePicture() {
  for (int i=0; i < 2; i++) {
pulseON(2336);
pulseOFF(646);
pulseON(1168);
pulseOFF(621);
pulseON(572);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(572);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(572);
pulseOFF(646);
pulseON(572);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(572);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(572);
pulseOFF(621);
pulseON(572);
pulseOFF(646);
pulseON(572);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(1168);
pulseOFF(11008);
pulseON(2336);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(572);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(572);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(572);
pulseOFF(646);
pulseON(572);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(572);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(572);
pulseOFF(621);
pulseON(572);
pulseOFF(646);
pulseON(572);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(1168);
pulseOFF(11008);
pulseON(2336);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(572);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(572);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(572);
pulseOFF(646);
pulseON(572);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(572);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(1168);
pulseOFF(621);
pulseON(1093);
pulseOFF(696);
pulseON(572);
pulseOFF(621);
pulseON(572);
pulseOFF(621);
pulseON(572);
pulseOFF(621);
pulseON(572);
pulseOFF(1218);
pulseON(497);
pulseOFF(1292);
pulseON(422);
pulseOFF(1367);
pulseON(373);
pulseOFF(11803);
pulseON(298);
pulseOFF(2659);
pulseON(199);
pulseOFF(1590);
pulseON(174);
pulseOFF(1019);
pulseON(174);
pulseOFF(1615);
pulseON(174);
pulseOFF(1615);
pulseON(149);
pulseOFF(1044);
pulseON(149);
pulseOFF(1640);
pulseON(124);
pulseOFF(1093);
pulseON(149);
pulseOFF(1044);
pulseON(124);
pulseOFF(1665);
pulseON(124);
pulseOFF(1068);
pulseON(124);
pulseOFF(1665);
pulseON(99);
pulseOFF(1690);
pulseON(99);
pulseOFF(1690);
pulseON(99);
pulseOFF(1093);
pulseON(99);
pulseOFF(1118);
pulseON(99);
pulseOFF(1093);
pulseON(99);
pulseOFF(1690);
pulseON(99);
pulseOFF(1690);
pulseON(75);
pulseOFF(1715);
pulseON(75);
pulseOFF(12101);
pulseON(149);
pulseOFF(2833);
pulseON(75);
pulseOFF(1715);
pulseON(75);
pulseOFF(1118);
pulseON(75);
pulseOFF(1715);
pulseON(75);
pulseOFF(1715);
pulseON(75);
pulseOFF(1118);
pulseON(75);
pulseOFF(1715);
pulseON(75);
pulseOFF(1118);
pulseON(99);
pulseOFF(1093);
pulseON(99);
pulseOFF(1690);
pulseON(99);
pulseOFF(1093);
pulseON(99);
pulseOFF(1690);
pulseON(99);
pulseOFF(1690);
pulseON(99);
pulseOFF(1690);
pulseON(99);
pulseOFF(1093);
pulseON(99);
pulseOFF(1118);
pulseON(99);
pulseOFF(1093);
pulseON(99);
pulseOFF(1690);
pulseON(99);
pulseOFF(1690);
pulseON(99);
pulseOFF(1690);
pulseON(99);
pulseOFF(646);
  }                                                     // loop the signal twice.
}




