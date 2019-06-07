#include <Servo.h>          

#define Esc 11
#define TurnServo 12
#define BrakeServo 13
#define IRC 350
#define CenterAngle 97

Servo myservo;
Servo mybrake;
Servo myesc;

int IR[10];
int CurrentError=0;
int PreviousError=0;
int Brushless=1000;
int i=0,j=0;
int x=1;
int FinishLine=0;
double TotalAngle=CenterAngle;
const double KP=3.5;
const double KD=1.5;

void SlopeFunc();


void setup() {
  Serial.begin(9600);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT); 
  pinMode(A5,INPUT);
  pinMode(A6,INPUT);
  pinMode(A7,INPUT);
  
  myservo.attach(TurnServo);
  mybrake.attach(BrakeServo);
  myesc.attach(Esc);
  
  myesc.writeMicroseconds(1000);
  myservo.write(CenterAngle);
  mybrake.write(0);
  
  delay(3000);

  myesc.writeMicroseconds(1270);  //  ********** Change this number to adjust beginning speed for 1.5 seconds  ********
  delay(1500);
}
             
void loop() {
  IR[1]=analogRead(A1);
  IR[2]=analogRead(A2);
  IR[3]=analogRead(A3);
  IR[4]=analogRead(A4);
  IR[5]=analogRead(A5);
  IR[6]=analogRead(A6);
  IR[7]=analogRead(A7);

  FinishLine=0;
  
  for(i=1;i<=7;i++){
    if(IR[i]>IRC) FinishLine++; 
  }
  
  /*Serial.print(" IR 1=");
  Serial.print(IR[1]);
  
  Serial.print(" IR 2=");
  Serial.print(IR[2]);

  Serial.print(" IR 3=");
  Serial.print(IR[3]);

  Serial.print(" IR 4=");
  Serial.print(IR[4]);

  Serial.print(" IR 5=");
  Serial.print(IR[5]);
  
  Serial.print(" IR 6=");
  Serial.print(IR[6]);
  
  Serial.print(" IR 7=");
  Serial.println(IR[7]); */
  
  if( IR[1]<IRC && IR[2]<IRC && IR[3]<IRC && IR[4]<IRC && IR[5]<IRC && IR[6]<IRC && IR[7]<IRC ) {
    TotalAngle=TotalAngle;
  }
  else {
    if( IR[1]>IR[2] && IR[1]>IR[3] && IR[1]>IR[4] && IR[1]>IR[5] && IR[1]>IR[6] && IR[1]>IR[7] ) CurrentError=4;  
    else if( IR[2]>IR[3] && IR[2]>IR[4] && IR[2]>IR[5] && IR[2]>IR[6] && IR[2]>IR[7] ) CurrentError=2;    
    else if( IR[3]>IR[4] && IR[3]>IR[5] && IR[3]>IR[6] && IR[3]>IR[7] ) CurrentError=1;
    else if( IR[4]>IR[5] && IR[4]>IR[6] && IR[4]>IR[7] ) CurrentError=0;     
    else if( IR[5]>IR[6] && IR[5]>IR[7] ) CurrentError=-1; 
    else if( IR[6]>IR[7] ) CurrentError=-2;
    else CurrentError=-4;
    TotalAngle = CenterAngle + KP*CurrentError + KD*(CurrentError-PreviousError);
  }
  
  if(TotalAngle > (CenterAngle+15)) TotalAngle=(CenterAngle+15);
  if(TotalAngle < (CenterAngle-15)) TotalAngle=(CenterAngle-15);
  myservo.write(TotalAngle);
  PreviousError=CurrentError;

  if(FinishLine>=4 && x==0) {
    Brushless=1000;
    myesc.writeMicroseconds(Brushless);
    delay(3600000);
  }
  
  else if(FinishLine>=4 && x==1) {
    Serial.println("Going to Slope Function");
    x=0;
    SlopeFunc();
  }
  
  else {
    if(abs(TotalAngle-CenterAngle)>+7) Brushless = 1265;   // ******* Change this number to adjust the speed when car is turning ****
    else Brushless=1255;                                   // *************** Change this number to adjust normal propeller speed ******
    myesc.writeMicroseconds(Brushless);
  }
  
  //Serial.println(Brushless);
  //Serial.println(TotalAngle);
}

void SlopeFunc() {
  for(j=0;j<=600;j++) {
    IR[1]=analogRead(A1);
    IR[2]=analogRead(A2);
    IR[3]=analogRead(A3);
    IR[4]=analogRead(A4);
    IR[5]=analogRead(A5);
    IR[6]=analogRead(A6);
    IR[7]=analogRead(A7);

    if( IR[1]<IRC && IR[2]<IRC && IR[3]<IRC && IR[4]<IRC && IR[5]<IRC && IR[6]<IRC && IR[7]<IRC ) {
      TotalAngle=TotalAngle;
    }
    else {
      if( IR[1]>IR[2] && IR[1]>IR[3] && IR[1]>IR[4] && IR[1]>IR[5] && IR[1]>IR[6] && IR[1]>IR[7] ) CurrentError=3;  
      else if( IR[2]>IR[3] && IR[2]>IR[4] && IR[2]>IR[5] && IR[2]>IR[6] && IR[2]>IR[7] ) CurrentError=2;    
      else if( IR[3]>IR[4] && IR[3]>IR[5] && IR[3]>IR[6] && IR[3]>IR[7] ) CurrentError=1;
      else if( IR[4]>IR[5] && IR[4]>IR[6] && IR[4]>IR[7] ) CurrentError=0;     
      else if( IR[5]>IR[6] && IR[5]>IR[7] ) CurrentError=-1; 
      else if( IR[6]>IR[7] ) CurrentError=-2;
      else CurrentError=-3;
      TotalAngle = CenterAngle + KP*CurrentError + KD*(CurrentError-PreviousError);
    }
    if(TotalAngle > (CenterAngle+15)) TotalAngle=(CenterAngle+15);
    if(TotalAngle < (CenterAngle-15)) TotalAngle=(CenterAngle-15);
    myservo.write(TotalAngle);
    PreviousError=CurrentError;
    
    if(j<=225) {
      Brushless=1355;                    // **********  Change this number to adjust the speed while climbing the slope  ********
    }
    else {
      Brushless=1180;
    }
    myesc.writeMicroseconds(Brushless);
    delay(10);
  }
  
  myesc.writeMicroseconds(1270);
  delay(500);
}
