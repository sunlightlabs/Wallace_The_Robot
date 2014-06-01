/*|
 || WALLACE_CODE_TCAMP14.ino
 ||
 || this controls an arduino running robotic greeter and sign holder 
 ||   wallace the robot for #TCamp14 
 || http://transparencycamp.org/
 ||
 || PIN assignments are the following : 
 ||
 || __digital PINS__ :
 || heartLED = 13
 || meterLED = 12
 || leftAntLED = 11
 || rightAntLED = 10
 || armServoPIN = 9
 || buttonPIN = 8 
 ||
 || __analog PINS__ :
 || armPotPIN = 1 
 || heartPotPIN = 0
 ||
 || LEDs must be connected to resistor on way to GRND . 
 || Servo is a 180 hitek servo .
 || button is a PULLUP . 
 ||
 || as compiled on my machine w/ arduino 1.0.5 for an UNO :
 || Binary sketch size: 6,078 bytes (of a 32,256 byte maximum)
 ||
 || license GPLv2 : http://www.gnu.org/licenses/gpl-2.0.html
 || 
 || --timball@sunlightfoundation.com
 || 2014/05/31
 ||
 |*/

// 
#include <TimedAction.h>
#include <LED.h>
#include <Servo.h>
#include <Button.h>

Servo armServo;

// heartLED is on 13
LED heartLED = LED(13);

// meterLED is on 12
LED meterLED = LED(12);

// the antenna are on 11 and 10
LED leftAntLED = LED(11);
LED rightAntLED = LED(10);
int antDelay = 300;

// armPIN is on 9
uint8_t armServoPIN = 9;

// button is on digital pin 8
uint8_t buttonPIN = 8;
Button button = Button(8, PULLUP);
byte buttonPresses = 0;
// number of button states to care about
const byte NUMBER_OF_STATES = 2;
int runState = 0;

// armPOT is on analog pin 1
uint8_t armPotPIN = 1;
int armPOT = 50;
uint8_t armDirex = 0;

// heartPOT is on analog pin 0
uint8_t heartPotPIN = 0;
int heartPOT = 300;

// attach hearbeat led to heartbeat
TimedAction beatHeart = TimedAction(300, heartBeat);

// attach arm waving to action  
TimedAction waveArm   = TimedAction(750, armWave);
int waveCycleCount = 0;

TimedAction blinkRightAnt = TimedAction(300, antRightBlink);
TimedAction blinkLeftAnt  = TimedAction(300, antLeftBlink);


// __init__ 
void setup()
{
  Serial.begin(9600);
  armServo.attach(armServoPIN);
  Serial.println("ready!");
  meterLED.on();
}


void armWave () {
/* CALLBACK TimedAction waveArm 
 *
 * read value from armPotPIN swing arm that amount scaled so that 
 * trimpot goes from 0 -> 180 degrees . flip the armDirex to swing 
 * arm back and forth .
 */  
   armPOT   = analogRead(armPotPIN);
   int pauseTime = 300;
   
   // flip the direction of the arm swing every other swing
   if (armDirex != 0) {
     armPOT = map(armPOT, 0, 1023, 0, 179);
     armDirex = 0;
   } else {
     armPOT = 0;
     armDirex = 1;
   }
   Serial.print("=== ARM POT: ");
   Serial.println(armPOT);
   
   armServo.write(armPOT);  
}

void heartBeat () {
/* CALLBACK TimedAction beatHeart 
 *
 * read value from hearPOT . delay led fadeOUT by that amount . 
 * hardcoded values for fadeIn/fadeOut
 */  
  
  heartPOT = analogRead(heartPotPIN);
  
  // turns out our little yellow led can't fade well 
  heartLED.fadeIn(750);
  delay(heartPOT);
  heartLED.fadeOut(300);
}


void antRightBlink () {
/* CALLBACK TimedAction blinkRightAnt 
 *
 * everything is hardcoded... even the antDelay which is set up top .
 */  
  
  leftAntLED.fadeIn(750);
  delay(antDelay);
  leftAntLED.fadeOut(300);
}

void antLeftBlink () {
/* CALLBACK TimedAction blinkRightAnt 
 *
 * everything is hardcoded... even the antDelay which is set up top .
 */  
  rightAntLED.fadeIn(750);
  delay(antDelay);
  rightAntLED.fadeOut(300);
}

void loop() {
/* main . try to figure out button states . 
 */
  if (button.uniquePress()) {
    buttonPresses = ++buttonPresses % NUMBER_OF_STATES;
    switch (buttonPresses) {
       case 0:
         runState = 1;
         break;
       case 1:
         runState = 0;
         break;
       default:
         break;
    }
  }

  if (runState == 1) {           
        Serial.print("heartPOT: ");
        Serial.println(heartPOT);
        Serial.print("armPOT: ");
        Serial.println(armPOT);
        delay(30);
        beatHeart.check();
        waveArm.check();
        blinkRightAnt.check();
        blinkLeftAnt.check();
  } else {
          Serial.println("no action");
          /* uh ... so this part doesn't actually work ... 
           * i thought by not calling .check() things would be cool ...
           * not so . */
  
  }
}
