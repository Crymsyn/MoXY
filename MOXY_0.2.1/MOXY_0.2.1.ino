#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define LED_PIN 0  

boolean Connected = false;

char* ssid = "UMassLoell";
char* password = "";

const char WiFiAPPSK[] = "";       // Note... need to change
boolean    Lflag       = true;

ESP8266WebServer server(80);

const int led = 0;
const int led2= 2;

const int MD1Dir = 0;
const int MD1Step = 1;

Adafruit_MCP23017 mcp;

void handleRoot() {
  if (!Connected){
    String header = "HTTP/1.1 301 OK\r\nLocation: /Setup\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  String content = "<!DOCTYPE html>";
  content += "<html>";
  content += "<head>";
  content += "<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />";
  content += "<title>Hello World</title>";
  content += "";
  content += "</head>";
  content += "";
  content += "<body>";
  content += "<h1>MOXY Web Application</h1>";
  content += "<br />";
  content += "";
  content += "";
  content += "<div id='sectionTesting' style ='border:1px solid black'>";
  content += "    <h5>Blick An LED button</h5>";
  content += "    <input id='blinkLED' type='button' value='Blink LED Yo' onclick ='testing()'>";
  content += "</div>";
  content += "";
  content += "<br />";
  content += "";
  content += "<div id='sectionLeftMic'>";
  content += "    <h5>Left microphone</h5>";
  content += "    <input id='rotateLeft_L' type='button' value='Rotate Left' /><input id='rotateRight_R' type='button' value='Rotate Right' />";
  content += "    <br />";
  content += "    <input id='moveLeft_L' type='button' value=' Move Left' onclick ='moveLeftLeft()'><input id='moveRight_R' type='button' value='Move Right' onclick ='moveLeftRight()'/>";
  content += "</div>";
  content += "";
  content += "<br />";
  content += "";
  content += "<div id='sectionRightMic'>";
  content += "    <h5>Right microphone</h5>";
  content += "    <input id='rotateLeft_R' type='button' value='Rotate Left' /><input id='rotateRight_R' type='button' value='Rotate Right' />";
  content += "    <br />";
  content += "    <input id='moveLeft_R' type='button' value=' Move Left' /><input id='moveRight_R' type='button' value='Move Right' />";
  content += "</div>";
  content += "";  
  content += "<br />";
  content += "";
  content += "<div id='sectionPresets'>";
  content += "    <h5>Preset Section</h5>";
  content += "    <table style='width: 100%;'>";
  content += "        <tr>";
  content += "            <td><input id='preset1' type='button' value='Preset 1' /></td>";
  content += "            <td><input id='preset2' type='button' value='Preset 2' /></td>";
  content += "            <td><input id='preset3' type='button' value='Preset 3' /></td>";
  content += "            <td><input id='preset4' type='button' value='Preset 4' /></td>";
  content += "            <td><input id='preset5' type='button' value='Preset 5' /></td>";
  content += "        </tr>";
  content += "    </table>";
  content += "</div>";
  content += "<script src='https://ajax.googleapis.com/ajax/libs/jquery/1.12.0/jquery.min.js'></script>";
  content += "<script>";
  content += "function testing() ";
  content += "{";
  content += "  $.get('http://10.253.97.16/button');";
  content += "}";
  content += "function moveLeftLeft() ";
  content += "{";
  content += "  $.get('http://10.253.97.16/moveLeftLeft');";
  content += "}";
  content += "function moveLeftRight() ";
  content += "{";
  content += "  $.get('http://10.253.97.16/moveLeftRight');";
  content += "}";
  content += "</script>";
  content += "";
  content += "</body>";
  content += "</html>";
  content += "";
  server.send(2000, "text/html",content);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println("Failure");
}

void testFunction()
{
  digitalWrite(led, 0);
  digitalWrite(led2, 1);
  delay(1000);
  Serial.println("hello");
  digitalWrite(led, 1);
  digitalWrite(led2, 0);
  String header = "HTTP/1.1 301 OK\r\nLocation: /";
  server.sendContent(header);
  return;
}
//direction is high, closewise, direction is low counterclockwise


//framework for fuctions for MOXY control
void moveMotor(int steps, int dir, int motor)
{
  
}

void calibrate()
{
  
}

void recall(int preset)
{
  
}

void moveLeftMotorLeft()
{
  mcp.digitalWrite(MD1Dir, HIGH);
  digitalWrite(led, 0);
  for(int i = 0; i < 300; i++)
  {
    delay(1);
    mcp.digitalWrite(MD1Step, HIGH);
    delay(1);
    mcp.digitalWrite(MD1Step, LOW);
  }
  digitalWrite(led, 1);
  return;
}

void moveLeftMotorRight()
{
  digitalWrite(led, 0);
  mcp.digitalWrite(MD1Dir, LOW);

  for(int i = 0; i < 300; i++)
  {
    delay(1);
    mcp.digitalWrite(MD1Step, HIGH);
    delay(1);
    mcp.digitalWrite(MD1Step, LOW);
  }
  digitalWrite(led, 1);
  return;
}

void testButton()
{
  digitalWrite(led, 0);
  digitalWrite(led2, 1);
  delay(1000);
  //Serial.println("Goodbye");
  digitalWrite(led, 1);
  digitalWrite(led2, 0);
  return;
}

void handleSetup()
{
    Serial.print("Current SSID:");
    Serial.println(ssid);
      Serial.print("Argument:");
      Serial.println(server.arg("SSID"));
      char WiFiSSID[server.arg("SSID").length() + 1];
      memset(WiFiSSID, server.arg("SSID").length() + 1, 0);
      for (int i=0; i<server.arg("SSID").length(); i++)
        WiFiSSID[i] = server.arg("SSID").charAt(i);
      WiFiSSID[server.arg("SSID").length()] = 0;
      Serial.println(WiFiSSID);
      ssid = WiFiSSID;
    Serial.print("Result:");
    Serial.println(ssid);
    if(server.hasArg("SSID") && server.hasArg("Password"))
    {
      setup();
    }
    String content = "<!DOCTYPE html>";
    content += "<html>";
    content += "<body>";
    content += "";
    content += "<form action='Setup' method='POST'>";
    content += "  SSID:<br>";
    content += "  <input type='text' name= 'SSID' value=''>";
    content += "  <br>";
    content += "  Password:<br>";
    content += "  <input type='password' name='Password' value=''>";
    content += "  <br><br>";
    content += "  <input type='submit' value='Submit'>";
    content += "</form> ";
    server.send(2000, "text/html",content);
}

void setupAccessPoint()
{
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP("MOXY", "123456789");

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/Setup", handleSetup);
  server.begin();
  Serial.println("HTTP server started");
}

void setup() {

  mcp.begin();      // use default address 0

  mcp.pinMode(0, OUTPUT);
  mcp.pinMode(1, OUTPUT);
  
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  int timeout = 0;
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED && timeout < 25) {
    delay(500);
    Serial.print(".");
    timeout ++;
  }
  if(timeout < 25)
  {
    Connected = true;
    WiFi.mode(WIFI_STA);
  }
  if(Connected)
  {
    
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  
    if (MDNS.begin("esp8266")) {
      Serial.println("MDNS responder started");
    }
  
    server.on("/", handleRoot);
    server.on("/test", testFunction);
    server.on("/button", testButton);
    server.on("/moveLeftLeft", moveLeftMotorLeft);
    server.on("/moveLeftRight", moveLeftMotorRight);
  
    server.on("/inline", [](){
      server.send(200, "text/plain", "this works as well");
    });
  
    server.onNotFound(handleNotFound);
  
    server.begin();
    Serial.println("HTTP server started");
  }
  else
  {
    setupAccessPoint();
    server.begin();
  }
}

void loop() {
  server.handleClient();
}
