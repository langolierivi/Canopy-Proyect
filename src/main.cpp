#include <Arduino.h>
#include "BluetoothSerial.h"
#include <ESP32Servo.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define RIGHT 5
#define LEFT  18
const char *pin = "654321";
uint8_t baseMac[6];
Servo myservo;

BluetoothSerial SerialBT;
String dataString="";

void direction(char dir);
void servoMotor(int g);
Servo servos[1];

void setup() {
  Serial.begin(115200);
  pinMode(RIGHT,OUTPUT);
  pinMode(LEFT,OUTPUT);
  SerialBT.enableSSP();
  SerialBT.setPin(pin);
  
  SerialBT.begin("Toldo");
  esp_read_mac(baseMac, ESP_MAC_BT);
  myservo.attach(17); 


  
  Serial.println("El ESP32 está listo, puedes emparejarlo mediante Bluetooth");
}

void loop() {

  if (Serial.available()) {
    dataString=Serial.readString();
  
    if (dataString.startsWith("D-")){
      Serial.println(dataString); //data.c_str()
      direction(dataString.charAt(2));
      dataString="";
    }
    if (dataString.startsWith("M-")){
      Serial.println(dataString.substring(2).toInt()); //data.c_str()
      int grados=dataString.substring(2).toInt();
      servoMotor(grados);
      dataString="";
    }
    else{
      dataString="";
    }
      
  }


  if (SerialBT.available()) {
    dataString=SerialBT.readString();
    if (dataString.startsWith("D-")){
      Serial.println(dataString); //data.c_str()
      direction(dataString.charAt(2));
      dataString="";
    }
    if (dataString.startsWith("M-")){
      Serial.println(dataString.substring(2).toInt()); //data.c_str()
      int grados=dataString.substring(2).toInt();
      servoMotor(grados);
      dataString="";
    }
    else{
      dataString="";
    }
    
  }
  delay(20);

  // data=SerialBT.read();
  //   Serial.printf(data.c_str());
  //   char dataArray[data.length()];
  //   data.toCharArray(dataArray, data.length());
  //   Serial.println(dataArray); 
  //   Serial.write(atoi(dataArray));
  }

void servoMotor(int grados){
  myservo.write(grados);
}

void direction(char dir){
  Serial.println("Recibido:"+dir);
  switch (dir)
  {
  case 'I':
    Serial.println("Derecha");
    digitalWrite(LEFT,LOW);
    digitalWrite(RIGHT,HIGH);
    break;

  case 'D':
    Serial.println("Izquierda");
    digitalWrite(RIGHT,LOW);
    digitalWrite(LEFT,HIGH);
    break;

  case 'P':
    Serial.println("Para");
    digitalWrite(RIGHT,LOW);
    digitalWrite(LEFT,LOW);
    break;
  
  default:
    Serial.println("Default:"+dir);
    break;
  }

}



