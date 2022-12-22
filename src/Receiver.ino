    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>
    #include <Servo.h>
    
    RF24 radio(7, 8); // CE, CSN
    const byte address[6] = "00001";
    int m11=A0,m12=A1,m21=A2,m22=A3;
    int val = 5;
    int count =0;
    int pos1;
    int pos2;
    int laser=6;
    
    Servo myservo1;
    Servo myservo2;
    
    void setup() {
    Serial.begin(9600);
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
    }
    void loop()
    {
    pos1=myservo1.read();
    pos2=myservo2.read();  
    if (radio.available())              //Looking for the data.
    {
    char text;                 //Saving the incoming data
    radio.read(&text, sizeof(text));    //Reading the data
    //radio.read(&button_state, sizeof(button_state));    //Reading the data
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
        case 'O': Serial.println('O');
                  digitalWrite(laser,LOW);
                  break;
        case 'P': digitalWrite(laser,HIGH);
                  Serial.print("shoot");
                  break;
       }
    }
    delay(5);
    }

    void forward(){
      Serial.println("forward");
      digitalWrite(m11,HIGH);
      digitalWrite(m12,LOW);
      digitalWrite(m21,LOW);
      digitalWrite(m22,HIGH);
    }

    void backward(){
      Serial.println("back");
      digitalWrite(m11,LOW);
      digitalWrite(m12,HIGH);
      digitalWrite(m21,HIGH);
      digitalWrite(m22,LOW);
    }

    void left(){
      Serial.println("left");
      digitalWrite(m11,HIGH);
      digitalWrite(m12,LOW);
      digitalWrite(m21,LOW);
      digitalWrite(m22,LOW);
    }

    void right(){
      Serial.println("right");
      digitalWrite(m11,LOW);
      digitalWrite(m12,LOW);
      digitalWrite(m21,LOW);
      digitalWrite(m22,HIGH);
    }
    
    void stopp(){
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
        Serial.println(pos1);
         myservo1.write(pos1);              // tell servo to go to position in variable 'pos'
         delay(15);
    }
    
    void cam_down()
    {
   Serial.println("cam down"); 
        Serial.println(pos1);
        pos1=pos1-1;
          myservo1.write(pos1);              // tell servo to go to position in variable 'pos' 
        delay(15);
    }

    void cam_left()
    {
    Serial.println("cam left"); 
        { 
          pos2=pos2+1;
           myservo2.write(pos2);              // tell servo to go to position in variable 'pos'
           delay(15);
        }
    }

    void cam_right()
    {
    Serial.println("cam right");
        { 
          pos2=pos2-1;
           myservo2.write(pos2);              // tell servo to go to position in variable 'pos'
           delay(15);
        }
    }    
