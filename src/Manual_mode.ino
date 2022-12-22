#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

    static const int RX = 4, TX = 3;
    static const uint32_t GPSBaud = 9600;

    TinyGPSPlus gps;
    SoftwareSerial ss(RX, TX);
    
    RF24 radio(7, 8); // CE, CSN
    const byte address[6] = "00001";
    int m11=2,m12=3,m21=4,m22=5;
    int val = 5;                        
    int count =0;                             //Count for manual mode
    int pos1=90;
    int pos2=90;
    int laser=6;
    int mode_count=0;

    int cntx=90;            
    int cnty=90;
    int count2;                               //Count for auto mode
    int num=0;  
    
    Servo myservo1;
    Servo myservo2;
    
    void setup() 
    {
    Serial.begin(9600);
    ss.begin(GPSBaud);
    pinMode(laser,OUTPUT);
    pinMode(m11,OUTPUT);
    pinMode(m12,OUTPUT);
    pinMode(m21,OUTPUT);
    pinMode(m22,OUTPUT);
    pinMode(val,OUTPUT);
    digitalWrite(val,LOW);
    radio.begin();
    radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
    radio.setPALevel(RF24_PA_HIGH);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
    radio.startListening();              //This sets the module as receiver
    
    myservo1.attach(9);
    myservo2.attach(10);
    myservo1.write(90);
    myservo2.write(90);
    
    }
    void loop()
    {
    pos1=myservo1.read();
    pos2=myservo2.read();  
    if (radio.available())              //Looking for the data.
    {
        char text;                 //Saving the incoming data
        radio.read(&text, sizeof(text));    //Reading the data
        Serial.println(text);
        if(text=='G')
        {
            count=count+1;
            // Serial.println(count);
        }

        if(count%2==0)
        {
           auto_mode(text);  
         }
        else
        {
           manual_mode(text);
        }
       
         delay(5);
     }
    }

    void manual_mode(char text)                        //entering manual mode
    {
        Serial.println("Manual Mode Enabled");
         switch(text)
         {
           case 'F': forward();
                  break;
           case 'B': backward();
                  break;
           case 'L': right();
                  break;
           case 'R': left();
                  break;
           case 'W': cam_up();
                  break;
           case 'D': cam_right();
                  break;
           case 'A': cam_left();
                  break;
           case 'S': cam_down();
                  break;
           case 'M': stopp();
                  break;
           case 'O':// Serial.println('O');
                  digitalWrite(laser,LOW);
                  break;
           case 'P': digitalWrite(laser,HIGH);
                  Serial.print("shoot");
                  break;
         }
    }

    void forward(){
      Serial.println("forward");
      digitalWrite(m11,HIGH);
      digitalWrite(m12,LOW);
      digitalWrite(m21,LOW);
      digitalWrite(m22,HIGH);
    }

    void backward()
    {
      Serial.println("back");
      digitalWrite(m11,LOW);
      digitalWrite(m12,HIGH);
      digitalWrite(m21,HIGH);
      digitalWrite(m22,LOW);
    }

    void left()
    {
      Serial.println("left");
      digitalWrite(m11,HIGH);
      digitalWrite(m12,LOW);
      digitalWrite(m21,LOW);
      digitalWrite(m22,LOW);
    }

    void right()
    {
      Serial.println("right");
      digitalWrite(m11,LOW);
      digitalWrite(m12,LOW);
      digitalWrite(m21,LOW);
      digitalWrite(m22,HIGH);
    }
    
    void stopp()
    {
      Serial.println("stop");
      digitalWrite(m11,LOW);
      digitalWrite(m12,LOW);
      digitalWrite(m21,LOW);
      digitalWrite(m22,LOW);
    }

    void cam_up()
    {
        Serial.println("cam up");
        pos1=pos1+1;
       // Serial.println(pos1);
        myservo1.write(pos1);             
        delay(15);
    }
    
    void cam_down()
    {
        Serial.println("cam down"); 
       // Serial.println(pos1);
        pos1=pos1-1;
        myservo1.write(pos1);               
        delay(15);
    }

    void cam_left()
    {
        Serial.println("cam left"); 
        { 
           pos2=pos2+1;
           myservo2.write(pos2);              
           delay(15);
        }
    }

    void cam_right()
    {
        Serial.println("cam right");
        { 
           pos2=pos2-1;
           myservo2.write(pos2);              
           delay(15);
        }
    }    


void auto_mode(char text)                        //entering auto mode
{
       Serial.println("Auto Mode Enabled");

       //////////////////////////////////////////////////////////////////////////////////////
       
       while (ss.available() > 0)
       if (gps.encode(ss.read()))
       Location();

       if (millis() > 5000 && gps.charsProcessed() < 10)
       {
           Serial.println(F("GPS module not detected"));
           while(true);
       }

  ////////////////////////////////////////////////////////////////////////////////////////
  
     while (Serial.available()>0)
  {
        cntx = myservo2.read();
        cnty = myservo1.read();
        count = Serial.read();
       // Serial.println(count);
        
        if(count==20)
        {
          num=num;
          Serial.println("Shoot");
          digitalWrite(laser,HIGH);
        }
        
        else
        {
          num=num+1;
          digitalWrite(laser,LOW);
        }  
        
        if(num%2==0)
        {
            if(count>=200 && count<=255)
            {
              Serial.print("xR = ");
              Serial.println(cntx+(count - 255) );
              myservo2.write(cntx+(count - 255) ); 
            }
               
            else
            {
              Serial.print("xL = ");
              Serial.println(cntx+count);  
              myservo2.write(cntx+count);
            }           
            
        }
        
        else
        {
            if(count>=200 && count<=255)
            { 
              Serial.print("yD = ");
              Serial.println(cnty+(count - 255));
              myservo1.write(cnty+(count - 255)); 
            }
               
            else if(count!=20)
            {
              Serial.print("yU = ");
              Serial.println(cnty+count);  
              myservo1.write(cnty+count);
            }  
            
        } 
    }
                     
}

void Location()
{
    Serial.print(F("Location: ")); 
    if (gps.location.isValid())
    {
       Serial.print(gps.location.lat(), 6);
       Serial.print(F(" , "));
       Serial.print(gps.location.lng(), 6);
    }
    else
    {
       Serial.print(F("Unable to get latitude and longitude"));
    }  
    Serial.println();
}
