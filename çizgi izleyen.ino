#include <QTRSensors.h>


#define tabanhiz 60
#define sagmotor1 7 
#define sagmotor2 8
#define sagmotorpwmpin 6
#define solmotor1 10
#define solmotor2 9
#define solmotorpwmpin 11
#define MZ80 3

#define LED 13

QTRSensorsAnalog qtra((unsigned char[]) { A7, A6, A5, A4, A3, A2, A1, A0} , 8);
unsigned int sensors[8];

float Kp = 0.04;
float Kd = 0.21;
float Ki = 0.0001;
int integral = 0;
int ekhiz = 0;
int sonhata = 0;
int hata = 0;
int dik = 0;
byte K=0;
int sagmotorpwm = 0;
int solmotorpwm = 0;
int engel = 0;
unsigned char zemin = 0;
int cizgisay = 0;
long baslamazamani = 0;
long doksanzamani = 500;
long yuzyirmizamani = 700;

void setup() {

pinMode(sagmotor1, OUTPUT);
pinMode(sagmotor2, OUTPUT);
pinMode(sagmotorpwmpin, OUTPUT);
pinMode(solmotor1, OUTPUT);
pinMode(solmotor2, OUTPUT);
pinMode(solmotorpwmpin, OUTPUT);
pinMode(MZ80, INPUT);
delay (1000);

digitalWrite(LED, HIGH);

//for(int i = 0; i < 150; i++){
//  if(0 <= i && i<5) hafifsagadon();
//  if(5 <= i && i<15) hafifsoladon();
//  if(15 <= i && i<25) hafifsagadon();
//  if(25 <= i && i<35) hafifsoladon();
//  if(35 <= i && i<40) hafifsagadon();
//  if(45 <= i && i<50) hafifsoladon();
//  
//  if (i >= 50){
//    frenle(1);
//    delay(3);
//  }
//  qtra.calibrate();
//  delay(1);
//  }

for(int i=0; i<300; i++)
{
   qtra.calibrate();
   delay(1);
} 

flashyap();
//
//while(digitalRead(MZ80) == LOW) {
//  frenle(1);
//}
Serial.begin(9600);
}
void loop() {
sensoroku();
yol_ayrimi();
sag_sol_90();
sag_sol_120();
//engeldenatla();
dikcizgioku();
if(cizgisay==7) frenle(10000);


//Serial.println(hata); delay(50);
//
//if(200<(millis()-doksanzamani) && (millis()-doksanzamani)<1200) ekhiz=100;
//if(1200<(millis()-doksanzamani) && (millis()-doksanzamani)<2400) ekhiz=-30;
//if(2400<(millis()-doksanzamani) && (millis()-doksanzamani)<5000) ekhiz=100;

integral += hata;
if(abs(hata)<500) integral = 0;
int duzeltmehizi = Kp * hata + Kd * Kd * (hata - sonhata) + Ki * integral;
sonhata = hata;

sagmotorpwm = tabanhiz + duzeltmehizi + ekhiz ;
solmotorpwm = tabanhiz - duzeltmehizi + ekhiz ;

sagmotorpwm = constrain(sagmotorpwm, -50, 254);
solmotorpwm = constrain(solmotorpwm, -50, 254);
motorkontrol(solmotorpwm, sagmotorpwm);

}

void motorkontrol(int solmotorpwm, int sagmotorpwm){
  if (sagmotorpwm <= 0){
    sagmotorpwm = abs(sagmotorpwm);
    digitalWrite(sagmotor1, LOW);
    digitalWrite(sagmotor2, HIGH);
    analogWrite(sagmotorpwmpin, sagmotorpwm);
  }
  else{
    digitalWrite(sagmotor1, HIGH);
    digitalWrite(sagmotor2, LOW);
    analogWrite(sagmotorpwmpin, sagmotorpwm);
  }
  if ( solmotorpwm <=0){
    solmotorpwm = abs(solmotorpwm);
    digitalWrite(solmotor1, LOW);
    digitalWrite(solmotor2, LOW);
    analogWrite(solmotorpwmpin, solmotorpwm);
  }
  else{
    digitalWrite(solmotor1, HIGH);
    digitalWrite(solmotor2, LOW);
    analogWrite(solmotorpwmpin, solmotorpwm);  
  }
}

void frenle(int bekle){motorkontrol(0,0); delay(bekle);}

void hafifsagadon() {motorkontrol(50, -50);}

void hafifsoladon() {motorkontrol(-50, 50);}

void sensoroku(){
  unsigned int position = qtra.readLine(sensors,1,zemin);
  hata= position-3500;

  if(sensors[0]<100 && sensors[7]<100) {zemin=0;}
  if(sensors[0]>750 && sensors[7]>750) {zemin=1;}
}

void flashyap(){
  for(int x = 0; x<10; x++){
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    delay(200);
  }
}

void dikcizgioku(){
  if(sensors[0]<100 && sensors[1]<100 && sensors[2]<100 && sensors[3]<100 && sensors[4]<100 && sensors[5]<100 && sensors[6]<100 && sensors[7]<100){
       cizgisay++; delay(50);
     }
}

void sag_sol_90(){
  if(sensors[0]<100 && sensors[1]<100 && sensors[2]<100 && sensors[3]<100 && sensors[6]>500 && sensors[7]>700 ){
      motorkontrol(-250, -100); delay(25);
      doksanzamani=millis();

      do{sensoroku(); motorkontrol(200, -50);}
      while(hata==-3500);
  }
  if(sensors[7]<100 && sensors[6]<100 && sensors[5]<100 && sensors[4]<100 && sensors[1]>500 && sensors[0]>700 ){
      motorkontrol(-100, -250); delay(25);
      doksanzamani=millis();

      do{sensoroku(); motorkontrol(-50, 200);}
      while(hata==3500);
  }
  
}

void yol_ayrimi(){
  if(sensors[0]>800 && sensors[2]>600 && sensors[3]>100 && sensors[4]>200 && sensors[5]>600 && sensors[7]>800 ){
    motorkontrol(180,80); delay(300);
    sag_sol_90();
}
}

void engeldenatla(){
  if(digitalRead(MZ80)==LOW){
    engel++;
    if(engel==1){
    motorkontrol(150,0); delay(80);
//    motorkontrol(150, 150); delay(100);
    motorkontrol(0,150); delay(80);
    do{sensoroku(); motorkontrol(150,120);}    
    while(hata==0);
    for(int i=0; i<100; i++){sensoroku(); motorkontrol(solmotorpwm, sagmotorpwm); delay(1);}
//    motorkontrol(150,150); delay(200);
    motorkontrol(0,150); delay(210);
//    motorkontrol(150,150); delay(800);
//    motorkontrol(150,0); delay(100);
    do{sensoroku(); motorkontrol(150,150);}    
    while(hata==0);
    engel++;
    }}
}

void sag_sol_120(){
if(sensors[0]<500 && sensors[3]<100 && sensors[4]<100 && sensors[5]<100 && sensors[7] > 700){
  motorkontrol(-200, -250); delay(25);
  yuzyirmizamani = millis();

      do{sensoroku(); motorkontrol(200, -50);}
      while(hata==-3500);
}
if(sensors[7]<500 &&sensors[4]<100 && sensors[3]<100 && sensors[2]<100 && sensors[0] > 700){
  motorkontrol(-250, -200); delay(25);
  yuzyirmizamani = millis();

      do{sensoroku(); motorkontrol(-50, 200);}
      while(hata==3500);
}
}
/*void okuveyaz(){
  for(unsigned char i = 0; i<8; i++){
    Serial.print(sensors[i]);
    Serial.print('\t');
  }
  Serial.println();
  delay(250);
}*/
