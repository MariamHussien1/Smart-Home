#include<LiquidCrystal.h>
const int rs=4 ,en=5 ,d4=6, d5=7, d6=8, d7=9;

LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
int redLed = 12;
int buzzer = 13;
int smokeA0 = A0;
int button =11;
// Your threshold value
int sensorThres = 90;

void setup() {
Serial.begin(9600);
lcd.begin(16,2);
pinMode(redLed, OUTPUT);
pinMode(buzzer, OUTPUT);
pinMode(smokeA0, INPUT);
pinMode(button, INPUT);
}

void loop() {
//lcd
int val=digitalRead(button);
if(val==1){
lcd.setCursor(3,0);
digitalWrite(buzzer,HIGH);
delay(500);
lcd.print("WELCOM.!:)");
//digitalWrite(buzzer,HIGH);
delay(5000);

}
else{
  lcd.clear();
  digitalWrite(buzzer,LOW);
}
//gas sensor
  int analogSensor=0;
  analogSensor = analogRead(smokeA0);
  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThres)
  {
    digitalWrite(redLed, HIGH);
    tone(buzzer, 1000, 200);
  delay(100);
  }
  analogSensor = analogRead(smokeA0);
  if (analogSensor <= sensorThres)
  {
    digitalWrite(redLed, LOW);
digitalWrite(buzzer,LOW);
}

}
