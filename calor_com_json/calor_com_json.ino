#include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include <Adafruit_I2CDevice.h>


Adafruit_AMG88xx amg;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
//AMG88xx_PIXEL_ARRAY_SIZE = 64 (64 pixeles)

String TX_buffer="";

void setup() {
    Serial.begin(9600);
    //Serial.println(F("AMG88xx pixels"));
    bool status;
    
    // default settings
    status = amg.begin();
    if (!status) {
        Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
        while (1);
    }
    
    delay(100); // let sensor boot up
}


void loop() { 
    //read all the pixels
    amg.readPixels(pixels);

    // Cargando el Buffer TX
    byte trama=0;
    
    for(int i=0; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++)
    {
      //Serial.println(pixels[i-1]);
      if(i==0){
        TX_buffer = "[" + (String)trama + ",";
        }
      else if( i%8 != 0 ){
        TX_buffer += (String)pixels[i-1];
        TX_buffer += ",";
        }
      else if( i%8 == 0 )
      {
        TX_buffer += (String)pixels[i-1];
        TX_buffer += "]";
        trama++;
        
        //Serial.println(TX_buffer);
        String json_data = "{\"Sensor_id\":\"AMG8833\",\"Value\":" + (String)TX_buffer + "}";
        Serial.println(json_data);
    
        TX_buffer = "[" + (String)trama + ",";
      }
    }
    Serial.println("");

    //delay a second
    delay(100);
}
