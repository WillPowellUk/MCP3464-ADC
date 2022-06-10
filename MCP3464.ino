#include "MCP3464.h"

void setup()
{
  Serial.begin(115200);

  MCP3464 adc;

  adc.init();
  
  while(true)
  {
    for(int i=0; i<adc.numOfCh; i++)
    {
      Serial.printf("Ch %i: %d \n", i+1, adc.read());
    }
    Serial.println();
    delay(500);
  }

}

void loop()
{

}
