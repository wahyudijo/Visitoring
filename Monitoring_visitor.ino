#include <Servo.h>
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>


char ssid [] = "REALME"; 
char pass [] = "12344321"; 
char username[] = "37bc6d90-03d0-11ed-8df2-dd50487e509b"; 
char password[] = "23af12416688b110b221c6c7279eb6b8ac0e2276"; 
char clientID[] = "7eecb240-096d-11ed-bbc1-5d0b0fa0a668"; 


const int pinSensor1 = 5; 
const int pinSensor2 = 4; 
int LED = 16; 
int BUZZER = 12; 
int hitung1 = 0; 
int hitung2 = 0; 
int kondisi1 = 0; 
int kondisi2 = 0; 
int status1;  
int status2; 
int visitor;
Servo SERVO;

void setup() {
  pinMode(pinSensor1, INPUT); 
  pinMode(pinSensor2, INPUT); 
  pinMode(BUZZER, OUTPUT); 
  pinMode(LED, OUTPUT); 
  SERVO.attach(13);
  SERVO.write(90); 
  Serial.begin(115200);
  Cayenne.begin(username, password, clientID, ssid, pass);
}

void loop() {
  hitung1 = constrain(hitung1, 0, 1000); 
  hitung2 = constrain(hitung2, 0, 1000);
  status1 = digitalRead(pinSensor1);
  status2 = digitalRead(pinSensor2);

  if (status1 == LOW){
    hitung1 = hitung1;
     kondisi1 = 1;
     SERVO.write(180);
     digitalWrite(BUZZER, HIGH);
     delay (500); 
     digitalWrite(BUZZER, LOW);
  }else{
    SERVO.write(90); 
  }
  if (status2 == LOW && visitor >0){
    hitung2 = hitung2;
    kondisi2 = 1;
    SERVO.write(0); 
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
  }
  if (status2 == LOW && visitor <0){
    SERVO.write(90);
  }else if (status1 == HIGH && kondisi1 == 1){
    hitung1 += 1;
    kondisi1 = 0;
  }else if (status2 == HIGH && kondisi2 == 1){
    hitung2 += 1;
    kondisi2 = 0;
  }else if (status1 == LOW && kondisi1 == 0){
    hitung1 = hitung1;
    kondisi1 = 0;
  }else if (status2 == LOW && kondisi2 == 0){
    hitung2 = hitung2;
    kondisi2 = 0;
  }else{
    hitung1 = hitung1;
    hitung2 = hitung2;
  }
  visitor = hitung1-hitung2;
  visitor = constrain(visitor,0,1000);


  if (visitor <= 0){
    digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW);
    Serial.println("Tidak ada pengunjung! Matikan lampu dan tutup pintu");
    Serial.print("Pengunjung saat ini : ");
    Serial.println(visitor);
    Serial.print("IN: ");
    Serial.println(hitung1);
    Serial.print("OUT: ");
    Serial.println(hitung2);
    delay(500);
  }else{
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, LOW);
    Serial.print("Current Visitor: ");
    Serial.println(visitor);
    Serial.print("IN: ");
    Serial.println(hitung1);
    Serial.print("OUT: ");
    Serial.println(hitung2);
    delay(500);   
  }
  if (WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){ 
      WiFi.begin(ssid, pass);
      Serial.print(",");

      Serial.println("\nConnected,");
    }
  }
  
  Cayenne.virtualWrite(0, visitor);
  Cayenne.virtualWrite(1, hitung1); 
  Cayenne.virtualWrite(2, hitung2);
  Cayenne.loop();
   
}
