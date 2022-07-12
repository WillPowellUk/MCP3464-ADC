#include "MCP3464.h"
#include "Arduino.h"

/////////Interrupt Wrapper for Static to Non Static Conversion/////////
static MCP3464* adcList[numberOfAdcs] = {nullptr};
int instanceCount = 0;

template<unsigned ADC> 
static void dataReady_Wrapper()
{
  (adcList[ADC])->dataReadyInterrupt();
}

static void (*const dataReady_Handler[numberOfAdcs])() = 
{
  dataReady_Wrapper<0>
  // add as more stimulators appear
};
/////////////////////////////////////////////////////////////////////


MCP3464::MCP3464()
{ 
  instanceIndex = instanceCount;
  instanceCount++;
  adcList[instanceIndex] = this;
  Ch = 0;
  attachInterrupt(dataReadyPin, dataReady_Handler[instanceIndex], FALLING); 
}


void MCP3464::dataReadyInterrupt()
{
  dataReady = true;
}


unsigned short MCP3464::read()
{
  // blocking wait for new data to be ready
  while(!dataReady);
  
  // SPI full duplex transfer
  digitalWrite(adcChipSelectPin,LOW); 
  SPI.transfer(readConversionData);
  adcReading = (SPI.transfer(0) << 8);
  adcReading += SPI.transfer(0);
  digitalWrite(adcChipSelectPin, HIGH);

  nextCh();
  
  return (adcReading+32768);
}


void MCP3464::init()
{
  //SPI initialisation
  pinMode (adcChipSelectPin, OUTPUT); 
  // set the ChipSelectPins high initially (CS active Low): 
  digitalWrite(adcChipSelectPin, HIGH);  
  // initialise SPI:
  SPI.begin();

  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0)); 
  //set frequency to be 20 MHz,  bit order to Most Sig. first, set SPI mode to 0. 
  //See Arduino SPI library documentation for details

  //SPI.setBitOrder(MSBFIRST);         // Not strictly needed but just to be sure.
  //SPI.setDataMode(SPI_MODE0);        // Not strictly needed but just to be sure.
  //SPI.setFrequency(20000000);        // The maximum frequency of the ADC is 20MHz

  unsigned char data[5];   

  // ADC initialisation 
  // Reset, initial config and tests connection
  writeData(&fullResetFastCmd, 1);
  
  // Write the initial config
  writeData(&initialConfigData[0], 7);
  
  // Check we can talk to the device
  readData(readConfigBytes, &data[0], 5);
  if ((data[1] != initialConfigData[1]) || (data[2] != initialConfigData[2]) || (data[3] != initialConfigData[3]) || (data[4] != initialConfigData[4]))
  {
    delay(1000);
    Serial.println();
    Serial.println("ADC Init fail");
    Serial.println();
    return;
  }
  
  // Set multiplexer of ADC to Ch0
  writeData(&muxRegisters[0][0], 2);
  
  // Start the first ADC conversion
  writeData(&startConversionFastCmd, 1);

  delay(1000);
  Serial.println("ADC OK");
}


// sets new multiplexer channel
void MCP3464::nextCh()
{ 
  Ch++;
  
  if (Ch >= numOfCh)
  {
    Ch = 0;
  }
  
  // Write the new ADC channel to multiplexer
  writeData(&muxRegisters[Ch][0], 2);

  // reset flag and wait for interrupt to flag for new adc reading
  dataReady = false;

  // Start the next conversion (single conversion mode)
  writeData(&startConversionFastCmd, 1);

}


void MCP3464::readData(unsigned char cmd, unsigned char *data, unsigned char len)
{
  digitalWrite(adcChipSelectPin,LOW); // chip Select pin low to read/write to ADC.

  // Write command byte
  *data = SPI.transfer(cmd);
  data++;
  len--;
  
  // Read remaining bytes
  while (len > 0)
  {
    *data = SPI.transfer(0);
    data++;
    len--;
  }

  digitalWrite(adcChipSelectPin,HIGH); // chip Select pin high to stop read/write of ADC
}


void MCP3464::writeData(const unsigned char *data, unsigned char len)
{
  digitalWrite(adcChipSelectPin,LOW); // chip Select pin low to read/write to ADC.
 
  // Write bytes
  while (len > 0)
  {
  (void)SPI.transfer(*data);
  data++;
  len--;
  }
 
  digitalWrite(adcChipSelectPin,HIGH); // chip Select pin high to stop read/write of ADC
}
