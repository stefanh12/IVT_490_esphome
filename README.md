# IVT_490_esphome
Serial connection to the IVT490 and IVT495Twin 


Since moving from MS code to use more of esphome I've converted the project to use esphome and native Home Assistant sensors instead of MQTT and compiled with MS code. 

The're are 36 different readings that come from the serial device, all is collected and placed in a sensor for each value. Easy modification can be done if you wish not to have certain readings.


I've enabled debugging on UART so that you can check if your serial connection is working. 
