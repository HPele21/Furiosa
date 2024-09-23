// MAC Address: B8:D6:1A:74:E5:98 // VESPA
// 98:83:89:fe:72:2a // PS4

#include "RC_PS4.h"
#include <PS4Controller.h>

//========================================
// CONTROLE DOS ESTADOS
//========================================
#define btn_pin 35 // VESPA botao geral pinout

enum {
  mod_stp, // modo parado
  mod_auto, // autonomo
  mod_rc, // radio controlado (ESPNOW)
  mod_ps4 // controle PS4
};

int btn_actst = LOW; // estado atual do botao
int btn_lastst = LOW; // ultimo
int mod_op = mod_stp; // modo de operaçao


//========================================
// ESPNOW + RC
//========================================
#include <esp_now.h>
#include <WiFi.h>
#include "DRV8833.h"

DRV8833 motor (4,27, 13,14);

#define AM1 4
#define AM2 27
#define BM1 13
#define BM2 14

//Servo esc; 
int val;

int XRSTK = 0;
int YRSTK = 0;
int XLSTK = 0;
int YLSTK = 0;
int POT = 0;
int SW = 0;

int motorSpeed;
int motorRotation;

int leftMotorValue;
int rightMotorValue;

const int PWM_freq = 5000;
const int PWM_resolution = 10;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_resolution) - 1);

bool isConnected = false;
esp_now_peer_info_t peerInfo;
uint8_t broadcastAddress[] = {0x48, 0x31, 0xb7, 0x3e, 0x64, 0xa0};
typedef struct pacote {
    int code;
    int len;
    int ID;
    int ch[20];
} pacote;
pacote pack_rx;

void OnDataReceived(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  if (data_len == sizeof(pacote)) {
    memcpy(&pack_rx, data, sizeof(pacote));
    //Serial.print("Received Data: ID = ");
    //Serial.print(pack_rx.ID);
    //Serial.print(", Channels: ");
    for (int i = 0; i < pack_rx.len; i++) {
      //Serial.print(" ");
      //Serial.print(pack_rx.ch[i]);
    }
    //Serial.println();

    isConnected = true;
  } else {
    Serial.println("Received data with incorrect size");
  }
}

void convert() {
  XRSTK = map(pack_rx.ch[0], 1000, 2000, 1024, -1024);
  YRSTK = map(pack_rx.ch[1], 1000, 2000, -1024, 1024);
  XLSTK = map(pack_rx.ch[2], 1000, 2000, -1024, 1024);
  YLSTK = map(pack_rx.ch[3], 1000, 2000, -1024, 1024);
  POT   = map(pack_rx.ch[4], 1000, 2000, 0, 180);
  SW    = map(pack_rx.ch[5], 1000, 2000, 0, 1);
}


//========================================
// AUTO
//========================================
#include "Auto.h"

#include <IRremote.hpp>
#include <SumoIR.h>
SumoIR IR;
int strategy = 0;


//========================================
// PIXEL
//========================================
#include "Pixel.h"


//========================================
// BLUETOOTH
//========================================
/*#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig'to and enable it
#endif

BluetoothSerial SerialBT; // variavel para guardar valores do tipo BluetoothSerial

String strategy = "default"; // valor recebido do dispositivo conectado ao bluetooth*/


void setup() {

  Serial.begin(115200); // iniciando o serial

  PS4.begin("98:83:89:fe:72:2a");


  pinMode(btn_pin, INPUT);
  pinMode(l_sen, INPUT);
  pinMode(r_sen, INPUT);
  pinMode(AM1, OUTPUT);
  pinMode(BM1, OUTPUT);
  pinMode(AM2, OUTPUT);
  pinMode(BM2, OUTPUT);

  ledcSetup(0, PWM_freq, PWM_resolution);
  ledcSetup(1, PWM_freq, PWM_resolution);
  ledcSetup(2, PWM_freq, PWM_resolution);
  ledcSetup(3, PWM_freq, PWM_resolution);
  ledcAttachPin(AM1, 0);
  ledcAttachPin(BM1, 1);
  ledcAttachPin(AM2, 2);
  ledcAttachPin(BM2, 3);


  // PIXEL //
  px_begin();
  px_fill(100, 0, 0);


  // ESPNOW //
  WiFi.mode(WIFI_STA);
  
  // VERIFICANDO O ESPNOW //
  if(esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // REGISTRANDO O PEER //
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  //VERIFICANDO O PEER //
  if(esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // CALLBACK ESPNOW //
  esp_now_register_recv_cb(OnDataReceived);


  // RECEPTOR INFRAVERMELHO 
  IR.begin(25); // S2 VESPA pinout


  // MOTOR //
  motor.begin();
  motor.bip(5, 200, 2000);


  // BLUETOOTH //
  //SerialBT.begin("FuriosaMini"); // nome do bluetooth
}

void loop() {

  if(!digitalRead(btn_pin)) {
    mod_op++;
    if(mod_op > mod_ps4) mod_op = mod_stp;

    switch(mod_op) {
      case mod_stp:
      px_fill(100, 0, 0);
      break;

      case mod_auto:
      px_fill(0, 100, 0);
      break;

      case mod_rc:
      px_fill(0, 0, 100);
      break;

      case mod_ps4:
      px_fill(100, 100, 0);
      break;
    }
    delay(300);
  }


  /*if(SerialBT.available()) {
    String received = SerialBT.readStringUntil('\n');
    received.trim();

    if(received == "madmax") {
      strategy = "madmax";
      SerialBT.println("Strategy set to madmax");
    }else if(received == "japan") {
      strategy = "japan";
      SerialBT.println("Strategy set to japan");
    }else if(received == "china") {
      strategy = "greatwallofchina";
      SerialBT.println("Strategy set to gwc");
    }else if(received == "ghoul") {
       strategy = "ghoul";
      SerialBT.println("Strategy set to ghoul");
    }else if(received == "pat") {
       strategy = "pat";
      SerialBT.println("Strategy set to pat");
    }else {
      SerialBT.println("Unknown strategy");
    }
  }*/


  switch(mod_op) {

    case mod_stp:
    motor.stop();
    break;

    case mod_auto: {
      // atualizaçao do estado // 
      IR.update();

      if(IR.available()){
        int cmd = IR.read();
        if( cmd >= 4 && cmd <= 9 ){ // faixa de valores validos ( lembrando que 1, 2 e 3 são eservados pra start, stop e prepare)
          strategy = cmd;
        }
      }
      // robo preparado //
      if (IR.prepare()) {

        if (IR.available()) {
          px_fill(0, 100, 0);
          delay(80);
          px_fill(0, 0, 0);
          delay(80);
        }
      // chamada pro robo começar //
      } else if (IR.start()) {
        //
      } else if (IR.on()) {
        
        px_fill(0, 0, 0);

        //furi_japan();

        switch(strategy) {

          Serial.println("running strategy" + String(strategy));
          
          case 4:
          furi_pat();
          break;

          case 5:
          furi_japan();
          break;

          case 6:
          furi_ghoul();
          break;

          case 7:
          furi_greatwallofchina();
          break;

          case 8:
          furi_madmax();
          break;

          case 9:
          break;

        }

        //furi_japan();

        /*if(strategy == "madmax") {
          furi_madmax();

        }else if(strategy == "japan") {
          furi_japan();

        }else if(strategy == "greatwallofchina") {
          furi_greatwallofchina();

        }else if(strategy == "ghoul") {
          furi_ghoul();

        }else {
          furi_pat();
        }*/


      } else if(IR.stop()){
        motor.stop();
        rainbow(10);

      } else {
        motor.stop();
      }
    }
    break;

    case mod_rc: {
    if (isConnected) {
    //esc.write(POT);

    convert();
    motorSpeed = XRSTK;
    motorRotation = YRSTK;

    leftMotorValue = motorSpeed + motorRotation;
    rightMotorValue = motorSpeed - motorRotation;

    leftMotorValue = constrain(leftMotorValue, -MAX_DUTY_CYCLE, MAX_DUTY_CYCLE);
    rightMotorValue = constrain(rightMotorValue, -MAX_DUTY_CYCLE, MAX_DUTY_CYCLE);

    if (leftMotorValue >= 0) {
      ledcWrite(0, leftMotorValue);
      ledcWrite(1, 0);
    } 
    else if (leftMotorValue < 0){
      ledcWrite(0, 0);
      ledcWrite(1, -leftMotorValue);
    }
    else {
      ledcWrite(0, MAX_DUTY_CYCLE);
      ledcWrite(1, MAX_DUTY_CYCLE);
    }

    if (rightMotorValue >= 0) {
      ledcWrite(2, rightMotorValue);
      ledcWrite(3, 0);
    } else if (rightMotorValue < 0) {
      ledcWrite(2, 0);
      ledcWrite(3, -rightMotorValue);
    } else {
      ledcWrite(2, MAX_DUTY_CYCLE);
      ledcWrite(3, MAX_DUTY_CYCLE);
    }
  }
  delay(50);
}
    break;

    case mod_ps4:
    rc_ps4();
    break;
  } // fecha switch
} // fecha loop