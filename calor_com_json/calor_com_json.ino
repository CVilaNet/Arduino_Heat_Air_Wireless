#include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include <Adafruit_I2CDevice.h>


Adafruit_AMG88xx amg;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
//AMG88xx_PIXEL_ARRAY_SIZE = 64 (64 pixeles)

long randNumber;
String inputString;

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

    String amg_array = "[";

    //Serial.print("[");
    for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
      amg_array += pixels[i-1];
      if( i%64 != 0 ) amg_array += ", ";
    }

    amg_array += "]";
    //Serial.println(amg_array);

    String json_data = "{\"Sensor_id\":\"AMG88\",\"Value\":" + (String)amg_array + "}";
    Serial.println(json_data);  

    //delay a second
    delay(100);
}
