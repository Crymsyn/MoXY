#include <Wire.h>
#include <Adafruit_MCP23017.h>

//INVOLT ARDUINO BASIC SKETCH

/*
  AUTO PINMODE
  Involt by default automatically adds pinMode to received
  pin data so you don't need to add pinMode output in setup for
  basic interactions. This mode is not recommended when
  mixing digital inputs and outputs.
*/
boolean autoPinMode = false;
/*
  DIRECT MODE
  Direct mode sends data from app to digital pins without storing
  it in involtDigital array and without additional code in void
  loop. It can be used for testing interaction with Arduino
  inside App. In direct mode digitalWrite/analogWrite is
  automatic only for values received from Involt.
*/
boolean directMode = false;
/*
  ARRAY FOR DIGITAL/PWM PINS
  IMPORTANT: You need to declare the beginning number of variables.
  Without the declared length there may occur problems when sending
  undeclared value to declared pin in void loop.
  Default is Arduino UNO total pin number +1. 
*/
int involtDigital[14] = {};
/*
  ARRAY FOR RECEIVED STRINGS
  You can save the received data as string if pin parameter of HTML 
  element is S0,S1 etc.
*/
String receivedString[] = {};
/*
  FUNCTION TRIGGER
  Used if something is received with fn HTML attribute.
*/
String fname;
Adafruit_MCP23017 mcp;
void setup() {
  //INVOLT SETUP
  //Bitrate must remain same as in app.
  Serial.begin(115200);

  delay(10);
  // prepare GPIO0
  pinMode(0, OUTPUT);

  /*****************************/
  //SETUP MCP
  mcp.begin();      // use default address 0

  mcp.pinMode(0, OUTPUT);
  mcp.pinMode(1, OUTPUT);
}


void loop() {
  //receive data from your app, do not remove this line.
  involtReceive();
  
  //ADD YOUR CODE HERE
  //digitalWrite(0, involtDigital[0]);

  //clear the fname to prevent from duplicating functions
  //fname = "";

  //move motor left
  if(receivedString[0] == "left")
  {
    //delay(3000);
    mcp.digitalWrite(0,HIGH);
    for(int i = 0; i < 20; i++){
    delay(1);
    mcp.digitalWrite(1, HIGH);
    delay(1);
    mcp.digitalWrite(1, LOW);
    }
    receivedString[0] = "stop"; 
   }

   //move moto to the right 
   if(receivedString[0] == "right")
   {
    //delay(3000);
    mcp.digitalWrite(0, LOW);
    for(int i = 0; i < 20; i++){
    delay(1);
    mcp.digitalWrite(1, HIGH);
    delay(1);
    mcp.digitalWrite(1, LOW);
    }
    receivedString[0] = "stop"; 
  }
  
}

//----------------------
//INVOLT FUNCTIONS

String V = "V";
String A = "A";
String E = "E";

void involtSend(int pinNumber, int sendValue) {
  Serial.println(A + pinNumber + V + sendValue + E);
}

void involtSendString(int pinNumber, String sendString) {
  Serial.println(A + pinNumber + V + sendString + E);
}

void involtReceive() {

  String involt;
  int involtLen;
  int pin;
  int val;
  
  String pwm = "P";
  String dig = "D";
  String fn = "FN";
  
  if (Serial.available() > 0) {
    involt = Serial.readStringUntil('\n');
    involtLen = involt.length();

    if (involt.indexOf(fn) == 0) {
      fname = involt.substring(2, involtLen);
    }
    else if (involt.indexOf(dig) == 0 || involt.indexOf(pwm) == 0 ){
      pin = involt.substring(1, involt.indexOf(V)).toInt();
      val = involt.substring(involt.indexOf(V) + 1, involtLen).toInt();
      

      if (autoPinMode) {
        pinMode(pin, OUTPUT);
      };

      if (directMode) {
        if (involt.indexOf(dig) == 0) {
          digitalWrite(pin, val);
        }
        else if (involt.indexOf(pwm) == 0 ) {
          analogWrite(pin, val);
        };
      }
      else {
        involtDigital[pin] = val;
      };

    }
    else if(involt.indexOf("S") == 0){
       pin = involt.substring(1, involt.indexOf(V)).toInt();
       receivedString[pin] = involt.substring(involt.indexOf(V) + 1, involtLen);
    };
    
  };
};
