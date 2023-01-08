# IVT_490_esphome
Serial connection to the IVT490 and IVT495Twin 
I've used a D1 mini Pro, you should be able to use any esp8622 or esp32, just change the platform and the pin that you will use.

I use the power from the Rego card, side contact,  to power the the D1 mini pro. 


Since moving from MS code to use more of esphome I've converted the project to use esphome and native Home Assistant sensors instead of MQTT and compiled with MS code. 

The're are 36 different readings that come from the serial device, all is collected and placed in a sensor for each value. Easy modification can be done if you wish not to have certain readings.


I've enabled debugging on UART so that you can check if your serial connection is working. 


This code should work esphome  2022.12.3. Older and newer should also work.



