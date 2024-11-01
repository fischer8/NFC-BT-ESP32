#include <BluetoothSerial.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5    
#define RST_PIN 22  
#define WAITING_LED 33
#define CONNECTED_LED 32

MFRC522 rfid(SS_PIN, RST_PIN); 
BluetoothSerial SerialBT;       

void setup() {
  pinMode(WAITING_LED, OUTPUT);
  pinMode(CONNECTED_LED, OUTPUT);
  Serial.begin(115200);  
  SerialBT.deleteAllBondedDevices();
  SPI.begin();           
  rfid.PCD_Init();       

  if (!SerialBT.begin("hear_vision1")) {  
    Serial.println("Falha ao iniciar Bluetooth");
    return;
  }
  Serial.println("Bluetooth iniciado");
}

void loop() {
  digitalWrite(CONNECTED_LED, HIGH);
  if (SerialBT.hasClient()) {
    if (rfid.PICC_IsNewCardPresent()) {
      if (rfid.PICC_ReadCardSerial()) {
        String uidString = "";
        for (byte i = 0; i < rfid.uid.size; i++) {
          uidString += (rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
          uidString += String(rfid.uid.uidByte[i], HEX);
        }
        Serial.println(uidString);
        SerialBT.println(uidString);
        rfid.PICC_HaltA();
      }
    }
  } else {
    Serial.println("Aguardando conexao");
    digitalWrite(CONNECTED_LED, LOW);
    digitalWrite(WAITING_LED, HIGH);
    delay(300); 
    digitalWrite(WAITING_LED, LOW);
  }
}