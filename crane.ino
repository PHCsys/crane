#include <IRremote.h>
#include <Servo.h>

const long int BTUP =  1775605197;
const long int BTDOWN = 1775605198;
const long int BTSTOP = 2430079433;
const long int BTLEFT = 3460670541;
const long int BTRIGHT = 2354609905;
const int M_UP = 6;
const int M_DOWN = 7;
const int SERVO = 9;
const int RECV_PIN = 11;
int pos = 0;
int pos_step = 2;
int failback = 0;

IRrecv irrecv(RECV_PIN);
decode_results results;
Servo servo;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  servo.attach(SERVO);

  pinMode(M_UP, OUTPUT);
  pinMode(M_DOWN, OUTPUT);

  digitalWrite(M_UP,HIGH);
  digitalWrite(M_DOWN,HIGH);
}

void loop() {
  if (irrecv.decode(&results)) {
       if (results.value == BTUP) {
          failback = 1;
          digitalWrite(M_UP,LOW);
          digitalWrite(M_DOWN,HIGH);
       }
       
       if (results.value == BTDOWN) {
          failback = 1;
          digitalWrite(M_UP,HIGH);
          digitalWrite(M_DOWN,LOW);
       }

       if (results.value == BTSTOP) {
          failback = 0;
          digitalWrite(M_UP,HIGH);
          digitalWrite(M_DOWN,HIGH);
       }

       if (results.value == BTLEFT) {
        if(pos <= 180 - pos_step) {
            pos += pos_step;
            servo.write(pos);
          }
       }

       if (results.value == BTRIGHT) {
        if(pos >= pos_step) {
            pos -= pos_step;
            servo.write(pos);
          }
       }
              
       irrecv.resume();
    }
    if(failback > 0) {
      failback++;
      if(failback > 5000) {
        digitalWrite(M_UP,HIGH);
        digitalWrite(M_DOWN,HIGH);
        failback = 0;
      }
      delay(1);
    }
}
