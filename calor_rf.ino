/***************************************************************************
  This is a library for the AMG88xx GridEYE 8x8 IR camera

  This sketch tries to read the pixels from the sensor

  Designed specifically to work with the Adafruit AMG88 breakout
  ----> http://www.adafruit.com/products/3538

  These sensors use I2C to communicate. The device's I2C address is 0x69

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Dean Miller for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <Adafruit_AMG88xx.h>

Adafruit_AMG88xx amg;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
//AMG88xx_PIXEL_ARRAY_SIZE = 64 (64 pixeles)



/* 1byte 433Mhz TRANSMITTER example.
/* Tutorial link: http://electronoobs.com/eng_arduino_tut99.php
 * Code: http://electronoobs.com/eng_arduino_tut99_code1.php
 * Scheamtic: http://electronoobs.com/eng_arduino_tut99_sch1.php
 * Youtube Channel: http://www.youtube/c/electronoobs   
// Arduino          433Mhz TX
// GND              GND
// 5V               VCC
// D3               Data
*/

#include <VirtualWire.h> //Download it here: http://electronoobs.com/eng_arduino_virtualWire.php
#define size 9
int pot = A0;
byte TX_buffer[size]={0};
byte i;



//Los adc_MQ
int pin_MQ3 = A1;
int pin_MQ135 = A2;

void setup() {
    Serial.begin(9600);
    Serial.println(F("AMG88xx pixels"));

    bool status;
    // default settings
    status = amg.begin();

    if (!status) {
        Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
        while (1);
    }
    Serial.println("-- Pixels Test --");
    Serial.println();

    delay(100); // let sensor boot up


  // virtual wire
  vw_set_tx_pin(3); // pin
  vw_setup(2000); // bps
  for(int i=0;i<size;i++)
  {
     TX_buffer[i]=i;
  }
  
}


void loop() { 

  //Variables sensor MQ3
  int adc_MQ3 = analogRead(pin_MQ3);
  float voltaje_MQ3 = adc_MQ3 * (5.0 / 1023.0);
  float Rs_MQ3 = 1000 * ((5-voltaje_MQ3) / voltaje_MQ3);  //Calculamos Rs_MQ3 con un RL de 1kΩ
  double alcohol = 0.4091 * pow(Rs_MQ3 / 5463, -1.497); //Calculamos la concentración de alcohol con la ecuación obtenida
  float rango_MQ3 = adc_MQ3 * (100.0 / 1023.0);

  //Variables sensor MQ135
  int adc_MQ135 = analogRead(pin_MQ135);
  float voltaje_MQ135 = adc_MQ135 * (5.0 / 1023.0);
  float Rs_MQ135 = 2000 * ((5-voltaje_MQ135) / voltaje_MQ135);  //Calculamos Rs_MQ135 con un RL de 2kΩ
  double co2 = pow((Rs_MQ135/20767.17) / 5.2735, 1/-0.3503); //Calculamos la concentración de co2 con la ecuación obtenida
  float rango_MQ135 = adc_MQ135 * (100.0 / 1023.0);

    // Transmision
    int val = map(analogRead(pot),0,1023,0,255);

    //read all the pixels
    amg.readPixels(pixels);

    Serial.print("[");
    for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
      Serial.print(pixels[i-1]);
      Serial.print(", ");
      if( i%8 == 0 ) Serial.println();
    }
    Serial.println("]");
    //Serial.println();

    // Cargando el Buffer TX
    byte trama=0;
    byte trama_bit=0;
    TX_buffer[trama_bit]=trama;
    
    for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++)
    {
      trama_bit++;
      TX_buffer[trama_bit]=(byte)(10*(pixels[i-1] - 20) + 0.5);

      Serial.print(TX_buffer[trama_bit]);
      Serial.print(" - ");
      if( i%8 == 0 )
      {
        vw_send(TX_buffer, size); 
        vw_wait_tx(); 
        delay(10);

        trama++;
        trama_bit=0;
        TX_buffer[trama_bit]=trama;
        Serial.println("");
      }
    }



      Serial.print(" - ");

/*if( trama == 8 ) {
      trama_bit ++;
      int alcohol_part_1=alcohol / pow(10, 4);
      TX_buffer[trama_bit]=alcohol_part_1;
      double alcohol_new=alcohol - alcohol_part_1*pow(10, 4);
      Serial.print(TX_buffer[trama_bit]);
      Serial.print(" - ");

      trama_bit ++;
      int alcohol_part_2=alcohol_new / pow(10, 2);
      TX_buffer[trama_bit]=alcohol_part_2;
      alcohol_new=alcohol_new - alcohol_part_2*pow(10, 2);
      Serial.print(TX_buffer[trama_bit]);
      Serial.print(" - ");

      trama_bit ++;
      int alcohol_part_3=alcohol_new / pow(10, 0);
      TX_buffer[trama_bit]=alcohol_part_3;
      alcohol_new=alcohol_new - alcohol_part_3*pow(10, 0);
      Serial.print(TX_buffer[trama_bit]);
      Serial.print(" - ");

      trama_bit ++;
      int alcohol_part_4=alcohol_new / pow(10, -2);
      TX_buffer[trama_bit]=alcohol_part_4;
      alcohol_new=alcohol_new - alcohol_part_4*pow(10, -2);
      Serial.print(TX_buffer[trama_bit]);
      Serial.print(" - ");

      
      trama_bit ++;
      int co2_part_1=co2 / pow(10, 4);
      TX_buffer[trama_bit]=co2_part_1;
      double co2_new=co2 - co2_part_1*pow(10, 4);
      Serial.print(TX_buffer[trama_bit]);
      Serial.print(" - ");

      trama_bit ++;
      int co2_part_2=co2_new / pow(10, 2);
      TX_buffer[trama_bit]=co2_part_2;
      co2_new=co2_new - co2_part_2*pow(10, 2);
      Serial.print(TX_buffer[trama_bit]);
      Serial.print(" - ");

      trama_bit ++;
      int co2_part_3=co2_new / pow(10, 0);
      TX_buffer[trama_bit]=co2_part_3;
      co2_new=co2_new - co2_part_3*pow(10, 0);
      Serial.print(TX_buffer[trama_bit]);
      Serial.print(" - ");

      trama_bit ++;
      int co2_part_4=co2_new / pow(10, -2);
      TX_buffer[trama_bit]=co2_part_4;
      co2_new=co2_new - co2_part_4*pow(10, -2);
      Serial.print(TX_buffer[trama_bit]);
      Serial.print(" - ");     

       vw_send(TX_buffer, size); 
        vw_wait_tx(); 
        delay(10);
}*/

double alcohol_new = alcohol;
int alcohol_part;
double co2_new = co2;
int co2_part;
for(int i=1; i<=8; i++)
 {
      trama_bit ++;
      if( i<=4 )
       {
            alcohol_part = alcohol_new / pow(10, 6 - 2*i);
            TX_buffer[trama_bit]=alcohol_part;
            alcohol_new = alcohol_new - alcohol_part*pow(10, 6 - 2*i);
            Serial.print(TX_buffer[trama_bit]);
            Serial.print(" - ");
      } else {
            i = i - 4;
            co2_part = co2_new / pow(10, 6 - 2*i);
            TX_buffer[trama_bit]=co2_part;
            co2_new = co2_new - co2_part*pow(10, 6 - 2*i);
            Serial.print(TX_buffer[trama_bit]);
            Serial.print(" - ");
            i = i + 4;
      }
}
vw_send(TX_buffer, size); 
vw_wait_tx(); 
delay(10);


    
    Serial.println("\n\n");
    Serial.println("----------------------------------------------------------");

  //Monitor Serie
  Serial.print("adc_MQ3:");
  Serial.print(adc_MQ3); Serial.print("\t");
  Serial.print("voltaje_MQ3:");
  Serial.print(voltaje_MQ3); Serial.print("\t");
  Serial.print("Rs_MQ3:");
  Serial.print(Rs_MQ3); Serial.print("\t");
  Serial.print("Alcohol:");
  Serial.print(alcohol);
  Serial.print("mg/L"); Serial.print("\t");
  Serial.print("rango_MQ3:");
  Serial.println(rango_MQ3);

  Serial.print("adc_MQ135:");
  Serial.print(adc_MQ135); Serial.print("\t");
  Serial.print("voltaje_MQ135:");
  Serial.print(voltaje_MQ135); Serial.print("\t");
  Serial.print("Rs_MQ135:");
  Serial.print(Rs_MQ135); Serial.print("\t");
  Serial.print("CO2:");
  Serial.print(co2);
  Serial.print("mg/L"); Serial.print("\t");
  Serial.print("rango_MQ135:");
  Serial.println(rango_MQ135);

  Serial.println();

    // Delay a second
    delay(1000);
}
