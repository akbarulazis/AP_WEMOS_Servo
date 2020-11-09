#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

Servo myservo1;
Servo myservo2;

// create servo object to control a servo 
const char *ssid = "SERVO A";
const char *password = "";
int pos1;
int pos2;
int servo1=13;
int initservo1 = 0; 
int maxservo1  = 180;
int speedservo1 = 20;
#define interPin 14
int servo2=12;
int initservo2 = 0; 
int maxservo2  = 90;
int speedservo2 = 10;

int val = 0;

String type1= "OFF";
String type2 = "OFF";

  int increment1 = 1;        // increment to move for each interval      // interval between updates
  unsigned long lastUpdate1;
  int increment2 = 1;        // increment to move for each interval      // interval between updates
  unsigned long lastUpdate2;

ESP8266WebServer server ( 80 );

String getPage1(){
  String page = "<html lang=fr-FR><head><meta http-equiv='refresh' content='10'/>";
  page += "<title>Remote Servo Control 1</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  page += "</head><body><h1>Remote Control Servo 1</h1>";
  page += "<p><h3>Servo status : </h3>";
  page += "Initial Rotation      :";
  page += initservo1;
  page += "<br>";
  page += "Maksmimum Rotation    :";
  page += maxservo1;
   page += "<br>";
  page += "Speed    :";
  page += speedservo1;
  page +="</p>";
  page +="<form action='/servo1' method='POST'>";
  //page += "<ul><li>";
  //page += "<INPUT type='radio' name='DX80' value='1'>ON";
  //page += "<INPUT type='radio' name='DX80' value='0'>OFF</li></ul>";
  //page += "<INPUT type='submit' value='Trigger DX80'>";
  page +="<p>Choose a Initial Rotation: </p>";
  page +="<input name='FromRange1' type='text' placeholder=";
  page +="0-180";
  page +=">";
  page +="                                           ";
  page +="<input name='submit' type='submit' value='Submit'>";
  page +="</form>";
  page +="<br><br>";
   page +="<form action='/servo1' method='POST'>";
  page +="<p>Choose a Maksimum Rotation: </p>";
  page +="<input name='ToRange1' type='text' placeholder=";
  page +="0-180";
  page +=">";
  page +="                                           ";
  page +="<input name='submit' type='submit' value='Submit'>";
  page +="</form>";
    page +="<br><br>";
   page +="<form action='/servo1' method='POST'>";
  page +="<p>Set a Speed Delay: </p>";
  page +="<input name='speedset1' type='text' value=";
  page +=speedservo1;
  page +=">";
  page +="                                           ";
  page +="<input name='submit' type='submit' value='Submit'>";
  page +="</form>";
   page +="<form action='/servo1' method='POST'>";
  page +="<p>Servo1 On/OFF: </p>";
  page +="<button name='servo1Run' type='submit' value='1'>TURN ON</button>";
  page +="                                           ";
  page +="<button name='servo1Run' type='submit' value='0'>TURN OFF</button>";
  page +="</form>";
  page += "</body></html>";
  return page;
}

String getPage2(){
  String page = "<html lang=fr-FR><head><meta http-equiv='refresh' content='10'/>";
  page += "<title>Remote Servo Control 2</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  page += "</head><body><h1>Remote Control Servo 2</h1>";
  page += "<p><h3>Servo status : </h3>";
  page += "Initial Rotation      :";
  page += initservo2;
  page += "<br>";
  page += "Maksmimum Rotation    :";
  page += maxservo2;
   page += "<br>";
  page += "Speed    :";
  page += speedservo2;
  page +="</p>";
  page +="<form action='/servo2' method='POST'>";
  page +="<p>Choose a Initial Rotation: </p>";
  page +="<input name='FromRange2' type='text' placeholder=";
  page +="0-180";
  page +=">";
  page +="                                           ";
  page +="<input name='submit' type='submit' value='Submit'>";
  page +="</form>";
  page +="<br><br>";
   page +="<form action='/servo2' method='POST'>";
  page +="<p>Choose a Maksimum Rotation: </p>";
  page +="<input name='ToRange2' type='text' placeholder=";
  page +="0-180";
  page +=">";
  page +="                                           ";
  page +="<input name='submit' type='submit' value='Submit'>";
  page +="</form>";
    page +="<br><br>";
   page +="<form action='/servo2' method='POST'>";
  page +="<p>Set a Speed Delay: </p>";
  page +="<input name='speedset2' type='text' value=";
  page +=speedservo2;
  page +=">";
  page +="                                           ";
  page +="<input name='submit' type='submit' value='Submit'>";
  page +="</form>";
     page +="<form action='/servo2' method='POST'>";
  page +="<p>Servo2 On/OFF: </p>";
  page +="<button name='servo2Run' type='submit' value='1'>TURN ON</button>";
  page +="                                           ";
  page +="<button name='servo2Run' type='submit' value='0'>TURN OFF</button>";
  page +="</form>";
  page += "</body></html>";
  return page;
}

void handleRoot1(){ 
  if ( server.hasArg("FromRange1") ) {
    handleInit1();
  }
  else if(server.hasArg("ToRange1"))
  {
    handleMax1();
 }
 else if(server.hasArg("speedset1")){
    handleSpeed1();
  }
  else if(server.hasArg("servo1Run")){
       String ChangeType1;
      ChangeType1 = server.arg("servo1Run");
      if (ChangeType1 == "1"){
        type1 = "RUN";
        }
      else if (ChangeType1 == "0"){
        type1 = "RESET";
        }
        server.send ( 200, "text/html", getPage1() );
  } 
  else {
    server.send ( 200, "text/html", getPage1() );
  }  
}

void handleInit1() {
  // Actualise le GPIO / Update GPIO 
  String FromRange1;
  FromRange1 = server.arg("FromRange1");
  if (FromRange1) {
    Serial.print("Set Initial Servo 1 to:"); Serial.println(FromRange1);                                  
    initservo1 = FromRange1.toInt();
        pos1 = initservo1; 
     myservo1.write(pos1);
    server.send ( 200, "text/html", getPage1() );
  }
  else {
    Serial.println("Error Value");
    server.send ( 200, "text/html", getPage1() );
  }
}

void handleMax1 (){
   String ToRange1;
  ToRange1 = server.arg("ToRange1");
  if (ToRange1) {
    Serial.print("Set Max Rotation Servo 1 to:"); Serial.println(ToRange1);
    maxservo1 = ToRange1.toInt();
    server.send ( 200, "text/html", getPage1() );
  }
    else {
    Serial.println("Error Value");
    server.send ( 200, "text/html", getPage1() );
  }
  }

void handleSpeed1 (){
   String speedset1;
  speedset1 = server.arg("speedset1");
  if (speedset1) {
    Serial.print("Set Speed Servo 1 to:"); Serial.println(speedservo1);
    speedservo1 = speedset1.toInt();

    server.send ( 200, "text/html", getPage1() );
  }
    else {
    Serial.println("Error Value");
    server.send ( 200, "text/html", getPage1() );
  }
  }

void handleRoot2(){ 
  if ( server.hasArg("FromRange2") ) {
    handleInit2();
  }
  else if(server.hasArg("ToRange2"))
  {
    handleMax2();
 }
 else if(server.hasArg("speedset2")){
    handleSpeed2();
  }
 else if(server.hasArg("servo2Run")){
     String ChangeType2;
    ChangeType2 = server.arg("servo2Run");
    if (ChangeType2 == "1"){
      type2 = "RUN";
      }
    else if (ChangeType2 == "0"){
      type2 = "RESET";
      }
      server.send ( 200, "text/html", getPage2() );
    } 
  else {
    server.send ( 200, "text/html", getPage2() );
  }  
}

void handleInit2() {
  // Actualise le GPIO / Update GPIO 
  String FromRange2;
  FromRange2 = server.arg("FromRange2");
  if (FromRange2) {
    Serial.print("Set Initial Servo 2 to:"); Serial.println(FromRange2);                                  
    initservo2 = FromRange2.toInt();
    pos2 = initservo2;
     myservo2.write(pos2);
    server.send ( 200, "text/html", getPage2() );
  }
  else {
    Serial.println("Error Value");
    server.send ( 200, "text/html", getPage2() );
  }
}

void handleMax2 (){
   String ToRange2;
  ToRange2 = server.arg("ToRange2");
  if (ToRange2) {
    Serial.print("Set Max Rotation Servo 2 to:"); Serial.println(ToRange2);
    maxservo2 = ToRange2.toInt();
    server.send ( 200, "text/html", getPage2() );
  }
    else {
    Serial.println("Error Value");
    server.send ( 200, "text/html", getPage2() );
  }
  }

void handleSpeed2 (){
   String speedset2;
  speedset2 = server.arg("speedset2");
  if (speedset2) {
    Serial.print("Set Speed Servo 2 to:"); Serial.println(speedservo2);
    speedservo2 = speedset2.toInt();
    server.send ( 200, "text/html", getPage2() );
  }
    else {
    Serial.println("Error Value");
    server.send ( 200, "text/html", getPage2() );
  }
  }
  
void setup() {

  myservo1.write(initservo1);
  myservo2.write(initservo2);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  pinMode(interPin, INPUT);
//  attachInterrupt(digitalPinToInterrupt(interPin), ServoOn, RISING);
  
  WiFi.softAP(ssid, password);
  myservo1.attach(servo1); 
  myservo2.attach(servo2); // attaches the servo on GIO2 to the servo object
  delay(1000);
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.print("Server MAC address: ");
  Serial.println(WiFi.softAPmacAddress());  // On branche la fonction qui gère la premiere page / link to the function that manage launch page 
  server.on ( "/servo1", handleRoot1 );  server.begin();
  server.on ( "/servo2", handleRoot2 );  server.begin();
  Serial.println ( "HTTP server started" );
  
}

void loop() {
  server.handleClient();
  val = digitalRead(interPin);  // read input value
//  Serial.println(type1);
  if (val == HIGH) {     
    ServoOn()  ;
    type1="RESET";
    type2="RESET";
  }
  else if (val == LOW){
    if (type1=="RESET"){
      myservo1.write(initservo1);
      type1="OFF";
      }
     else if (type1=="RUN"){
      servoA();
      }
    if (type2 =="RESET"){
      myservo2.write(initservo2);
      type2="OFF";
      }
     else if(type2 =="RUN"){
      servoB();
      }
    
    }
}

void ServoOn(){
  servoA();
  servoB();
  }

void servoA(){

      if((millis() - lastUpdate1) > speedservo1)  // time to update
    {
      lastUpdate1 = millis();
      pos1 += increment1;
      myservo1.write(pos1);
//      Serial.println(pos1);
      if (maxservo1 > initservo1){
      if ((pos1 >= maxservo1) || (pos1 <= initservo1)) // end of sweep
      {
        // reverse direction
        increment1 = -increment1;
      }
      }
      else if (initservo1 > maxservo1){
        if ((pos1 >= initservo1) || (pos1 <= maxservo1)) // end of sweep
      {
        // reverse direction
        increment1 = -increment1;
      } 
        
        }
    }

}

void servoB(){


      if((millis() - lastUpdate2) > speedservo2)  // time to update
    {
      lastUpdate2 = millis();
      pos2 += increment2;
      myservo2.write(pos2);
//      Serial.println(pos2);
           if (maxservo2 > initservo2){
      if ((pos2 >= maxservo2) || (pos2 <= initservo2)) // end of sweep
      {
        // reverse direction
        increment2 = -increment2;
      }
      }
      else if (initservo2 > maxservo2){
        if ((pos2 >= initservo2) || (pos1 <= maxservo2)) // end of sweep
      {
        // reverse direction
        increment2 = -increment2;
      } 
    }
  }
}
