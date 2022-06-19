#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,3);
const int buzzer=11;
String BluetoothData;
bool B_state=1;

void setup() {
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  Serial.begin(38400);
  mySerial.begin(38400);
  delay(400);
}

void loop() {
  BluetoothData = mySerial.read();
  if (BluetoothData=="120")
  {
    digitalWrite(buzzer,HIGH);
    BluetoothData =1;
  }
  else
  {
    digitalWrite(buzzer,LOW);
  }
  Serial.println(BluetoothData);
  delay(50);
}
