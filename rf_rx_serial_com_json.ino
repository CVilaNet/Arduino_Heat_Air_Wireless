#include <VirtualWire.h>              //Download it here: http://electronoobs.com/eng_arduino_virtualWire.php
byte message[VW_MAX_MESSAGE_LEN]; // a buffer to store the incoming messages
byte messageLength = VW_MAX_MESSAGE_LEN; // the size of the message
int received_number = 0;
int LED = 5;
int pin_rx = 3;

// https://hetpro-store.com/TUTORIALES/pyserial-python-arduino-comunicacion-serial/
long randNumber;
//String inputString;

#define dimension 8
String sensor_tipo;
byte package_num;

byte TX_buffer_head[dimension]={1, 1, 1, 1, 1, 1, 1, 1};
String val_head="\"val_0\":[0, 0, 0, 0, 0, 0, 0, 0]";
String json_data;

byte cont_test=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(0));

  pinMode(LED, OUTPUT);
  //Serial.println("Ready...");
  vw_set_rx_pin(pin_rx); // pin
  vw_setup(2000); // bps
  vw_rx_start();
}

void loop() {
  // put your main code here, to run repeatedly:
 

  if (vw_get_message(message, &messageLength)) // non-blocking
  {
    for (int i = 0; i < messageLength; i++)
    {
      received_number = message[i];
      //Serial.print(received_number);
      //Serial.print(" - ");
    }
    //Serial.println();
    analogWrite(LED, received_number);

    sensor_tipo="AMG8833";
    SerialEvent(sensor_tipo);
  }
  
  //delay(100);
  //received_test_data();
  //sensor_tipo="AMG8833";
  //SerialEvent(sensor_tipo);
}

void SerialEvent(String sensor_tipo){
  /*while(Serial.available()){
    char inputChar = Serial.read();
    inputString += inputChar;
    }*/
    
  //if(inputString.indexOf("getValue")>=0){
  //if(0==0){
  randNumber = random(100);
  json_data = "";

  HeadJsonString();
  //json_data = "{\"Sensor_id\":\"" + sensor_tipo + "\",\"Value\":" + (String)randNumber + "}";
  json_data = "{\"Sensor_id\":\"" + sensor_tipo + "\", \"Value\":" + (String)randNumber + ", \"package_num\":" + (String)package_num + ", " + val_head + "}";
  Serial.println(json_data);
  //}
    
  //delay(1000);
  //inputString = "";
  }

void HeadJsonString(){
  package_num = message[0];
  for (int i=1; i<dimension+1; i++){
    TX_buffer_head[i-1]=message[i];
    }
    JsonData();
  }

void JsonData(){
  val_head = "\"val_head\": [";
  for (byte i=0; i<dimension; i++){
    val_head += (String)TX_buffer_head[i];
    if (i<7){
        val_head += ", ";
      }else{
        val_head += "]";
      }
    }  
  }

  void received_test_data(){
    message[0]=cont_test;
    cont_test++;
    if (cont_test==9) cont_test=0;
    
    for (byte i=1; i<9; i++){
      message[i]=random(50);
      }
    }
