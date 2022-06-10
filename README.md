# MCP3464-ADC
Upon popular request, I have created a simple driver/library for the MCP3464 ADC.

This driver is ready to go if you are running arduino/ESP32.  

I have not made it portable for any microcontroller, however feel free to fork this repository and make it portable between chips.

# Software Setup
If you have arduino or ESP32, ensure the pins (ChipSelect, dataReady, MISO, MOSI, SCK are correct for your hardware configuration) then simply upload the .ino file.  
If not, the .ino file still gives you a starting guide on how to implement the driver.

# Hardware Setup
To maximise the resolution to 16 bit, the reference voltage must be 1.65V!

[Click here to see pin configuration](https://user-images.githubusercontent.com/64581094/173102912-ea3950ea-a83a-4504-9a87-7f997dcfe979.png)

>N.B. The driver is currently configured to run on the ESP32 with pins:
- dataReady = 17
- SDO = 19
- SDI = 23
- SCK = 18
- CS = 5
