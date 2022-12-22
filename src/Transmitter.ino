#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8);
const byte address[6] = "00001";
const int VRxser = A0; 
const int VRyser = A1; 
const int VRxm = A2; 
const int VRym = A3; 
const int mode=2;
const int SW = 4; // Connect to Digital Pin 4
const int push=5; //push button for laser
int xser;
int yser;
int xm;
int ym;
int modeled=9;
int count=0;
int buttonState;
void setup() {

  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_HIGH);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
  pinMode(push, INPUT);
  pinMode(modeled, OUTPUT);
  pinMode (mode,INPUT);
  digitalWrite(SW, HIGH);
  Serial.begin(9600);
}

void loop() {

  xser = (analogRead(VRxser));
  yser = (analogRead(VRyser));
  xm = (analogRead(VRxm));
  ym = (analogRead(VRym));
  buttonState = digitalRead(push);

  if (xser < 490 && (yser < 540 || yser > 490))
  {
    
    const char text = 'W';
    radio.write(&text, sizeof(text));
    Serial.println(text);
  }
  if (xser > 540 && (yser < 540 || yser > 490))
  {
    const char text = 'S';
    radio.write(&text, sizeof(text));
  }
  if (yser> 540 && (xser < 540 || xser > 490))
  {
    const char text = 'A';
    radio.write(&text, sizeof(text));
  }
  if (yser < 490 && (xser < 540 || xser > 490))
  {
    const char text = 'D';
    radio.write(&text, sizeof(text));
  }
  if ((xser < 540 && xser > 490) && (yser < 540 && yser > 490))
  {
   const char text = 'O';
    radio.write(&text, sizeof(text));
  }
  

///////////////////////////////////////////////////

  if (xm < 490 && (ym < 540 || ym > 490))
  {
    
    const char text = 'F';
    radio.write(&text, sizeof(text));
    Serial.println(text);
  }
  if (xm > 540 && (ym < 540 || ym > 490))
  {
    const char text = 'B';
    radio.write(&text, sizeof(text));
  }
  else if (ym> 540 && (xm < 540 || xm > 490))
  {
    const char text = 'L';
    radio.write(&text, sizeof(text));
  }
  if (ym < 490 && (xm < 540 || xm > 490))
  {
    const char text = 'R';
    radio.write(&text, sizeof(text));
  }
  if ((xm < 540 && xm > 490) && (ym < 540 && ym > 490))
  {
   const char text = 'M';
    radio.write(&text, sizeof(text));
  }

  if ( digitalRead(mode) == 1)
  {
   const char text = 'G';
   count = count+1;
   //Serial.println(count);
   if(count%2==0)
   {
   digitalWrite(modeled,HIGH);
   //Serial.println(HIGH);
   }
   else
   {
    digitalWrite(modeled,LOW);
    //Serial.println(LOW);
   }
  // Serial.println("mode");
   radio.write(&text, sizeof(text));
   delay(500);
  }
  
  if ( buttonState == 1)
  {
   const char text = 'P';
   Serial.println("laser");
    radio.write(&text, sizeof(text));
  }
 if ( buttonState == 0)
  {
   const char text = 'O';
   Serial.println("offffffff");
    radio.write(&text, sizeof(text));
  }

 //....Serial.println( buttonState);
  

  
}
