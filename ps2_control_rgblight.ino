#include <Wire.h>
#include <PS2X_lib.h>  //for v1.6

/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        13      
#define PS2_CMD        12  
#define PS2_SEL        11  
#define PS2_CLK        10  

/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

int stickLX = 0;
int stickLY = 0;
int stickRX = 0;
int stickRY = 0;
int rgbRedPin = 9;
int rgbGreenPin = 6;
int rgbBluePin = 3;

float RXtoRGB;
float RYtoRGB;

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;

void setup(){
 
  Serial.begin(57600);
  pinMode(rgbRedPin, OUTPUT);
  pinMode(rgbGreenPin, OUTPUT);
  pinMode(rgbBluePin, OUTPUT);
  
  
  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
   
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successfully");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug.");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug.");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
	case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }
}

void loop() {
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */  
   
// write 0 to keep lights off when right analog stick is in neutral position
analogWrite(rgbRedPin, 0);
analogWrite(rgbGreenPin, 0);
analogWrite(rgbBluePin, 0);
   
  //DualShock Controller
  
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
    
    stickLX = ps2x.Analog(PSS_LX);
    stickLY = ps2x.Analog(PSS_LY);
    stickRX = ps2x.Analog(PSS_RX);
    stickRY = ps2x.Analog(PSS_RY);

    if (stickLY < 127) {
      Serial.print("Speeding up values for left stick: ");
      Serial.println(stickLY);
    }

    if (stickLX < 127) {
      Serial.print("Turning left values for left stick: ");
      Serial.println(stickLX);
    }

    if (stickLY > 127) {
      Serial.print("Slowing down values for left stick: ");
      Serial.println(stickLY);
    }

    if (stickLX > 128) {
      Serial.print("Turning right values for left stick: ");
      Serial.println(stickLX);
    }

    if (stickRX < 127) {
      Serial.print("Right stick pushed left: ");
      Serial.println(stickRX);
      RXtoRGB = map(stickRX, 0, 126, 255, 0);
      Serial.println(RXtoRGB);
      analogWrite(rgbRedPin, RXtoRGB);
    }

    if (stickRY < 127) {
      Serial.print("Right stick pushed up: ");
      Serial.println(stickRY);
      RYtoRGB = map(stickRY, 0, 126, 255, 0);
      analogWrite(rgbBluePin, RYtoRGB);
    }

    if (stickRX > 128) {
      Serial.print("Right stick pushed right: ");
      Serial.println(stickRX);
      RXtoRGB = map(stickRX, 129, 255, 0, 255);
      analogWrite(rgbGreenPin, RXtoRGB);
    }

    if (stickRY > 127) {
      Serial.print("Right stick pushed down: ");
      Serial.println(stickRY);
      RYtoRGB = map(stickRY, 129, 255, 0, 255);
      analogWrite(rgbGreenPin, RYtoRGB);
    }
    
    
    if(ps2x.ButtonPressed(PSB_START))         
      Serial.println("Start is being pressed");
    if(ps2x.ButtonPressed(PSB_SELECT))
      Serial.println("Select is being pressed");      

    if(ps2x.ButtonPressed(PSB_PAD_UP)) {      
      Serial.println("Up on D-Pad");
    }
    if(ps2x.ButtonPressed(PSB_PAD_RIGHT)){
      Serial.println("Right on D-Pad");
    }
    if(ps2x.ButtonPressed(PSB_PAD_LEFT)){
      Serial.println("Left on D-Pad");
    }
    if(ps2x.ButtonPressed(PSB_PAD_DOWN)){
      Serial.println("Down on D-Pad");

    } 

    if(ps2x.ButtonPressed(PSB_CROSS)) {
      Serial.println("X has been pressed");

    }

    if(ps2x.ButtonPressed(PSB_SQUARE)) {
      Serial.println("Square has been pressed - LEDs are off");
      analogWrite(rgbRedPin, 0);
      analogWrite(rgbGreenPin, 0);
      analogWrite(rgbBluePin, 0);

    }

    if(ps2x.ButtonPressed(PSB_TRIANGLE)) {
      Serial.println("Triangle has been pressed");
     
    }

    if(ps2x.ButtonPressed(PSB_CIRCLE)) {
      Serial.println("Circle has been pressed");

    }

    if(ps2x.ButtonPressed(PSB_L1)) {
      Serial.println("L1 has been pressed");
    }

    if(ps2x.ButtonPressed(PSB_L2)) {
      Serial.println("L2 has been pressed");
    }

    if(ps2x.ButtonPressed(PSB_L3)) {
      Serial.println("L3 has been pressed");
    }

    if(ps2x.ButtonPressed(PSB_R1)) {
      Serial.println("R1 has been pressed");
    }

    if(ps2x.ButtonPressed(PSB_R2)) {
      Serial.println("R2 has been pressed");
    }

    if(ps2x.ButtonPressed(PSB_R3)) {
      Serial.println("R3 has been pressed");
    }


  delay(50);  
}





//    vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
//    if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
//      if(ps2x.Button(PSB_L3))
//        Serial.println("L3 pressed");
//      if(ps2x.Button(PSB_R3))
//        Serial.println("R3 pressed");
//      if(ps2x.Button(PSB_L2))
//        Serial.println("L2 pressed");
//      if(ps2x.Button(PSB_R2))
//        Serial.println("R2 pressed");
//      if(ps2x.Button(PSB_TRIANGLE))
//        Serial.println("Triangle pressed");        
//    }
//
//    if(ps2x.ButtonPressed(PSB_CIRCLE))               //will be TRUE if button was JUST pressed
//      Serial.println("Circle just pressed");
//    if(ps2x.NewButtonState(PSB_CROSS))               //will be TRUE if button was JUST pressed OR released
//      Serial.println("X just changed");
//    if(ps2x.ButtonReleased(PSB_SQUARE))              //will be TRUE if button was JUST released
//      Serial.println("Square just released");     
//
//    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
//      Serial.print("Stick Values:");
//      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
//      Serial.print(",");
//      Serial.print(ps2x.Analog(PSS_LX), DEC); 
//      Serial.print(",");
//      Serial.print(ps2x.Analog(PSS_RY), DEC); 
//      Serial.print(",");
//      Serial.println(ps2x.Analog(PSS_RX), DEC); 
//    }     
//  }
