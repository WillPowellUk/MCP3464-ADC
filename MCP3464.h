#pragma once
#include <SPI.h>

// change if more than one MCP3464 per arduino
#define numberOfAdcs 1

// SPI Pins
#define adcChipSelectPin 5
#define dataReadyPin 17

/**********************
 *
 * The MCP3464 class is for sending and receiving data through SPI to the MCP3464 ADC 
 *
 ***********************/
class MCP3464{
protected:
    static constexpr const float vRef = 1.65;
    int Ch;
    signed short adcReading;

    // method to write register data to ADC
    void writeData(const unsigned char *data, unsigned char len);
    // method for reading ADC registers - for confirming ADC is initialised properly and for debugging
    void readData(unsigned char cmd, unsigned char *data, unsigned char len);
    // switches multiplexer to next channel
    void nextCh();

public:
    int numOfCh = 8;
    volatile bool dataReady = false;
    int instanceIndex = 0;
    // constructor
    MCP3464();
    //interrupt ISR to turn data ready flag true
    void dataReadyInterrupt();
    // method to initialise SPI and ADC
    void init();
    // method to read current adc channel
    unsigned short read();
};


// Datasheet for MCP3464: https://www.mouser.co.uk/datasheet/2/268/20006180C_MCP3461_2_4_Two_Four_Eight_Channel_LowNo-2297874.pdf
// Register definitions
#define REG_ADCDATA 0x00
#define REG_CONFIG0 0x01
#define REG_CONFIG1 0x02
#define REG_CONFIG2 0x03
#define REG_CONFIG3 0x04
#define REG_IRQ 0x05
#define REG_MUX 0x06
#define REG_SCAN 0x07
#define REG_TIMER 0x08
#define REG_OFFSETCAL 0x09
#define REG_GAINCAL 0x0A
#define REG_LOCK 0x0D

// ADC Commands and Registers
const unsigned char initialConfigData[7] = 
{
 0b01000110, // Control byte: Device address 1, incremental write starting at address 1 (Config0 register)
 0b11000010, // Config0: No total shutdown, external clock (up to 20MHz clock input), 0uA on ADC inputs, ADC standby mode
 0b00000000, // Config1: No clock prescaler, 32 oversampling ratio, 00 (reserved bits)
 0b10001111, // Config2: 1x Boost, 16x Gain, auto-zeroing enabled, reserved bits set to 1
 0b10000000, // Config3: One-shot conversion then standby mode, 16-bit two's complement ADC data (SGN + 15 bit), CRC disabled, offset and gain disabled
 0b00000110, // IRQ Register: Fast commands enabled, IRQ active low enabled (connected to dataReadyPin)
 0b00000001 // Mux register: Channel 0 on +, REFIN- on -};
};

// Fast commands
const unsigned char startConversionFastCmd = 0b01101000; // Fast command to start a conversion
const unsigned char fullResetFastCmd = 0b01111000; // Fast command to reset device
const unsigned char readConversionData = 0b01000011; // Device address 1, incremental read starting at register address 0 (ADC Data register)
const unsigned char readConfigBytes = 0b01000111; // Device address 1, incremental read starting at register address 1 (Config0)
const unsigned char readIRQ = 0b01010101; // static read of IRQ register

// Mux Register locations for channels
 const unsigned char muxRegisters[8][2] = 
{
   {
    0b01011010, // Control byte: Device address 1, incremental write starting at address 6 (Mux register)
    0b00001100, // Channel 0 on +, REFIN- on -
   },
   {
    0b01011010, // Control byte: Device address 1, incremental write starting at address 6 (Mux register)
    0b00011100, // Channel 1 on +, REFIN- on -
   },
   {
    0b01011010, // Control byte: Device address 1, incremental write starting at address 6 (Mux register)
    0b00101100, // Channel 2 on +, REFIN- on -
   },
   {
    0b01011010, // Control byte: Device address 1, incremental write starting at address 6 (Mux register)
    0b00111100, // Channel 3 on +, REFIN- on -
   },
   {
    0b01011010, // Control byte: Device address 1, incremental write starting at address 6 (Mux register)
    0b01001100, // Channel 4 on +, REFIN- on -
   },
   {
    0b01011010, // Control byte: Device address 1, incremental write starting at address 6 (Mux register)
    0b01011100, // Channel 5 on +, REFIN- on -
   },
   {
    0b01011010, // Control byte: Device address 1, incremental write starting at address 6 (Mux register)
    0b01101100, // Channel 6 on +, REFIN- on -
   },
   {
    0b01011010, // Control byte: Device address 1, incremental write starting at address 6 (Mux register)
    0b01111100, // Channel 7 on +, REFIN- on -
   },

};
