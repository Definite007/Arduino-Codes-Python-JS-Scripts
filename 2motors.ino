#define sensor 5
#define led 13
// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4;
const int stepPin2 = 6;
int j = 0,val = 0;

void setup()
   {
       pinMode(sensor, INPUT);
       pinMode(led, OUTPUT);
       pinMode(stepPin,OUTPUT); 
       pinMode(dirPin,OUTPUT);
       pinMode(7,OUTPUT);
       digitalWrite(7,HIGH);
    }
void loop()
{
  j = 0;
   while(j < 2){
     if(j == 0){
     digitalWrite(dirPin,HIGH);
     motorpositive();
     delay(1000);
     }
     else {
     digitalWrite(dirPin,LOW);
     motorpositive();
     delay(1000);
     }
     j++;
   }
}
int sensorfunc(){
  int readsen = digitalRead(sensor);
  if( readsen == LOW) {
       digitalWrite(led, LOW);
       Serial.println(readsen);
       return 1;
  }
  else {
       digitalWrite(led,HIGH);
       return 0;
      }
}
void motorpositive(){
  for(int x = 0; x < 180; x++) { 
    digitalWrite(stepPin,HIGH);
    if(j == 1 && x < 50)
    digitalWrite(stepPin2,HIGH);
    delay(5);
    digitalWrite(stepPin,LOW);
    if(j == 1 && x < 50)
    digitalWrite(stepPin2,LOW);
    delay(5);
    do{
      val = sensorfunc();
    }while(val == 1);
  }
}
