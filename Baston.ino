#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>
SoftwareSerial BT(10,11);    // Definimos los pines RX y TX del Arduino conectados al Bluetooth

#define echoPin 8 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 9 

long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

#define GSM_RX  13
#define GSM_TX  12

SoftwareSerial sim(GSM_RX, GSM_TX);  //
int cont = 0;
int puls = 0;
String number = "+59176455619"; 

int sat;
int year;
TinyGPS gps;
SoftwareSerial ss(6, 7);//pin 5(rx) y 4(tx)
int vel = 0;
void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
  BT.begin(38400);       // Inicializamos el puerto serie BT (Para Modo AT 2)
  pinMode(2,INPUT);
  Serial.begin(9600);
  ss.begin(9600);
  Serial.println("System Started...");
  delay(1000);
  
  Serial.println("Communicating GSM/GPS.....");
}
void loop()
{   
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if(distance <= 10)
   {
    Serial.println("enviandose");
    BT.write("1");
   }
   delay(50);
  ss.begin(9600);
  bool newData = false;
  unsigned long chars;
// Durante un segundo analizamos los datos de GPS e informamos algunos valores clave
  for (unsigned long start = millis(); millis() - start < 800;)
  {
    while (ss.available())
    {
      char c = ss.read();
      if (gps.encode(c)) // ??Ha entrado una nueva sentencia v??lida?
      newData = true;
    }
  }
  if (newData)
  {
//latitud y longitud, numero de satelites disponibles    
    gps.f_get_position(&latitud, &longitud);
    Serial.print("LAT=");
    Serial.print(latitud == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : latitud, 6);
    Serial.print(" LON=");
    Serial.print(longitud == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : longitud, 6);
    Serial.print(" SAT=");
    Serial.println(sat = gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());

  char buffer[10]= " ";
  String lati = dtostrf(latitud,7,5,buffer);
  String longi = dtostrf(longitud,7,5,buffer);
  String SMS = "Ya funca y estoy:" + lati + ", " + longi;
  Serial.print(SMS);
//Fecha y hora 
  }
  sim.begin(115200);

  while(digitalRead(2) == HIGH)
  {
    cont = cont + 1;
    Serial.println("contador");
    Serial.println(cont);
    if(cont > 10)
    {
      Serial.println("entro al pulsador");
      //call();
      sms();
      delay(1000);
      cont = 0;
    }
  }
  cont = 0;
  ss.begin(9600);
  delay(100); 
}

void sms()
{
  Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    
  delay(1000);
  Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); 
  delay(1000);
  char buffer[10]= " ";
  String lati = dtostrf(latitud,7,5,buffer);
  String longi = dtostrf(longitud,7,5,buffer);
  String SMS = "Ya funca y estoy:" + lati + ", " + longi;
  
   sim.println(SMS);
   
   delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
 
}

void call() {
  sim.print (F("ATD"));
  sim.print (number);
  sim.print (F(";\r\n"));
}
