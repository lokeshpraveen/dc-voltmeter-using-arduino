#include <DS3231.h>
#include <SoftwareSerial.h>
SoftwareSerial sim(10, 11);
//sim
int _timeout;
String _buffer;
//****************
//change number
//***************
String number = "+919080931539";
//sim end
//date
int Relay = 4;

DS3231  rtc(SDA, SCL);
Time t;
//****************
//change time
//***************
const int OnHour = 16; //SET TIME TO ON RELAY (24 HOUR FORMAT)
const int OnMin = 47;
const int OffHour = 16; //SET TIME TO OFF RELAY
const int OffMin = 48;
//date end
int analogInput = 0;
int analogInput1 = 1;


float vout = 0.0;
float vin = 0.0;

float R1 = 100000.0; // resistance of R1 (100K)
float R2 = 10000.0; // resistance of R2 (10K)
int value = 0;

int a[5] = {};



void setup() {
  delay(7000);
  _buffer.reserve(50);
  Serial.begin(9600);
  pinMode(analogInput, INPUT);
  sim.begin(9600);
rtc.begin();
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, LOW);
  Serial.println("Series Check");

  delay(3000);


}
void loop() {
  datee();
  // read the value at analog input
  value = analogRead(analogInput);
  vout = (value * 5) / 1024.0;
  vin = vout / (R2 / (R1 + R2));  
  Serial.println("Voltage V :");
  Serial.println(vin);
  delay(3000);
//****************
//change voltage
//***************
if(vin >= 11.67){
  Serial.println("SMS Sent");
  SendMessage();
}

}void datee()
{
  t = rtc.getTime();
  Serial.print(t.hour);
  Serial.print(" hour(s), ");
  Serial.print(t.min);
  Serial.print(" minute(s)");
  Serial.println(" ");
  delay (1000);
  
  if(t.hour == OnHour && t.min == OnMin){
    digitalWrite(Relay,HIGH);
    Serial.println("LIGHT ON");
    }
    
    else if(t.hour == OffHour && t.min == OffMin){
      digitalWrite(Relay,LOW);
      Serial.println("LIGHT OFF");
    }
}
void SendMessage()
{
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(1000);
  //****************
//change message
//***************
  String SMS = "Fault on Light 1";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  _buffer = _readSerial();
}
String _readSerial() {
  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}
