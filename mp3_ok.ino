#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

const int trigPin1 = 8;
const int echoPin1 = 9;
int minLeftDistance   = 50;

const int trigPin2 = 6;
const int echoPin2 = 7;
int minRightDistance   = 50;

static unsigned long timer = millis();
int waitTime = 3500;

// Use pins 2 and 3 to communicate with DFPlayer Mini
static const uint8_t PIN_MP3_TX = 2; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 3; // Connects to module's TX 
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

// Create the Player object
DFRobotDFPlayerMini player;

void leftAlert(int distance) {
  player.playLargeFolder(01, distance);
//  if (millis() - timer > waitTime) {
//    timer = millis();
//    
//  } 
analogWrite(12,170);
delay(3000);
analogWrite(12,0);
}

void rightAlert(int distance) {
  player.playLargeFolder(03, distance);
//  if (millis() - timer > waitTime) {
//    timer = millis();
//    
//  } 
analogWrite(12,170);
delay(3000);
analogWrite(12,0);
}

float calculateDistance(int trigPin, int echoPin)
{
  // Calculating Distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  int duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  return (float) duration*0.034/2;
}



void setupDFPlayer() {



  softwareSerial.begin(9600);
  Serial.println();
  Serial.println("Blind assistancs smart glass...");
  Serial.println("Initializing DFPlayer ... (May take 3~5 seconds)");
  
  if (!player.begin(softwareSerial)){
    Serial.println("Unable to begin:");
    Serial.println("1.Please recheck the connection!");
    Serial.println("2.Please insert the SD card!");
    while(true);
  }
  Serial.println("DFPlayer Mini online.");
  
  player.setTimeOut(500); //Set serial communictaion time out 500ms
  player.volume(30);
  player.EQ(DFPLAYER_EQ_NORMAL);
  player.outputDevice(DFPLAYER_DEVICE_SD);
}



void setup() {
  pinMode(12,OUTPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  digitalWrite(trigPin1,LOW);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  digitalWrite(trigPin2,LOW);
  Serial.begin(9600);
  
  
  // Init serial port for DFPlayer Mini
  softwareSerial.begin(9600);
  setupDFPlayer();
}

void loop() {
  delay (500);
  int leftDistance  = calculateDistance(trigPin1, echoPin1);
  int RightDistance  = calculateDistance(trigPin2, echoPin2);
   
   //  For debugging
   Serial.print("Left: ");
   Serial.println(leftDistance);
   Serial.print("Right: ");
   Serial.println(RightDistance);


   if(leftDistance < minLeftDistance && leftDistance > 1) {
    
    Serial.print("Obstacle ");
    Serial.print(leftDistance);
    Serial.println(" CM to the left");

    leftAlert(leftDistance);
  }

   if(RightDistance < minRightDistance && RightDistance > 1) {
    
    Serial.print("Obstacle ");
    Serial.print(RightDistance);
    Serial.println(" CM to the right");

    rightAlert(RightDistance);
  }
   
}
