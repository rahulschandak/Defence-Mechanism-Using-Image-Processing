#include <Servo.h>

Servo myservox;
Servo myservoy;
int pos1=90;
int pos2=90;
int cntx=90;
int cnty=90;
int count;
int num=1;
int laser=6;

void setup() {
  
  Serial.begin(9600);
  pinMode(laser,OUTPUT);
  myservox.attach(10);
  myservoy.attach(9);
  myservox.write(pos1);
  myservoy.write(pos2);
}

void loop() 
{
  while (Serial.available()>0)
  {
        cntx = myservox.read();
        cnty = myservoy.read();
        count = Serial.read();
       // Serial.println(count);
        
        if(count==20)
        {
          num=num;
          Serial.println("Shoot");
          digitalWrite(laser,HIGH);
          delay(50);
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
              Serial.print("x1 = ");
              Serial.println(cntx+(count - 255) );
              myservox.write(cntx+(count - 255) ); 
            }
               
            else
            {
              Serial.print("x2 = ");
              Serial.println(cntx+count);  
              myservox.write(cntx+count);
            }           
            
        }
        
        else
        {
            if(count>=200 && count<=255)
            { 
              Serial.print("y1 = ");
              Serial.println(cnty+(count - 255));
              myservoy.write(cnty+(count - 255)); 
            }
               
            else if(count!=20)
            {
              Serial.print("y2 = ");
              Serial.println(cnty+count);  
              myservoy.write(cnty+count);
            }  
            
        } 
    }
}
