#include "esphome.h"
#include "esphome/core/log.h"
#define SENSOR_CNT 36

class Ivt495Twin : public PollingComponent, public UARTDevice {
Sensor *xsensor1 {nullptr};  //Framledning
Sensor *xsensor2 {nullptr};  //Ute
Sensor *xsensor3 {nullptr};  //Tappvarmvatten Topp
Sensor *xsensor4 {nullptr};  //Varmvatten Mitt
Sensor *xsensor5 {nullptr};  //Värmevatten Botten
Sensor *xsensor6 {nullptr};  //Rumstemp
Sensor *xsensor7 {nullptr};  //Hetgas
Sensor *xsensor8 {nullptr};  //Extra Acc-Tank
Sensor *xsensor9 {nullptr};  //Tryckvakt
Sensor *xsensor10 {nullptr}; //Högtryck
Sensor *xsensor11 {nullptr}; //Lågtryck
Sensor *xsensor12 {nullptr}; //Semester aktiv?
Sensor *xsensor13 {nullptr}; //Kompressor aktiv
Sensor *xsensor14 {nullptr}; //SV1 Öppna
Sensor *xsensor15 {nullptr}; //SV1 Stäng
Sensor *xsensor16 {nullptr}; //P1 Rad
Sensor *xsensor17 {nullptr}; //Fläkt
Sensor *xsensor18 {nullptr}; //Larm aktiv
Sensor *xsensor19 {nullptr}; //Extern P2
Sensor *xsensor20 {nullptr}; //LLT GT1
Sensor *xsensor21 {nullptr}; //LL GT1
Sensor *xsensor22 {nullptr}; //BV GT1
Sensor *xsensor23 {nullptr}; //UL GT1
Sensor *xsensor24 {nullptr}; //LL GT3:2
Sensor *xsensor25 {nullptr}; //ULT GT3:2
Sensor *xsensor26 {nullptr}; //UL GT3:2
Sensor *xsensor27 {nullptr}; //LL GT3:3
Sensor *xsensor28 {nullptr}; //BV GT3:3
Sensor *xsensor29 {nullptr}; //SV3 BV Förskj
Sensor *xsensor30 {nullptr}; //Effekt ink vit VV behov
Sensor *xsensor31 {nullptr}; //Tillskotstimer VV behov
Sensor *xsensor32 {nullptr}; //Tappv prio
Sensor *xsensor33 {nullptr}; //Tillskott i %/10
Sensor *xsensor34 {nullptr}; //Tillskott RAD
Sensor *xsensor35 {nullptr}; //Tillskott Tillägg
Sensor *xsensor36 {nullptr}; //Default SV2 Open

String inputString;
boolean stringComplete = false;
char inChar;
int counter;
int commaPosition;
float ivt[50];
/*
 #define LOG_IVT(prefix, type, obj) \
   if ((obj) != nullptr) { \
     ESP_LOGCONFIG(TAG, "%s%s '%s'", prefix, LOG_STR_LITERAL(type), (obj)->get_name().c_str()); \
   }
*/

public:
  Ivt495Twin(UARTComponent *parent, Sensor *sensor1, Sensor *sensor2, Sensor *sensor3, Sensor *sensor4, Sensor *sensor5, Sensor *sensor6, Sensor *sensor7, Sensor *sensor8, Sensor *sensor9, Sensor *sensor10, Sensor *sensor11, Sensor *sensor12, Sensor *sensor13, Sensor *sensor14, Sensor *sensor15, Sensor *sensor16, Sensor *sensor17, Sensor *sensor18, Sensor *sensor19, Sensor *sensor20, Sensor *sensor21, Sensor *sensor22, Sensor *sensor23, Sensor *sensor24, Sensor *sensor25, Sensor *sensor26, Sensor *sensor27, Sensor *sensor28, Sensor *sensor29, Sensor *sensor30, Sensor *sensor31, Sensor *sensor32, Sensor *sensor33, Sensor *sensor34, Sensor *sensor35, Sensor *sensor36) : UARTDevice(parent) , xsensor1(sensor1) , xsensor2(sensor2), xsensor3(sensor3), xsensor4(sensor4) ,xsensor5(sensor5), xsensor6(sensor6), xsensor7(sensor7), xsensor8(sensor8), xsensor9(sensor9), xsensor10(sensor10), xsensor11(sensor11), xsensor12(sensor12), xsensor13(sensor13), xsensor14(sensor14) ,xsensor15(sensor15), xsensor16(sensor16), xsensor17(sensor17), xsensor18(sensor18), xsensor19(sensor19), xsensor20(sensor20), xsensor21(sensor21), xsensor22(sensor22), xsensor23(sensor23), xsensor24(sensor24) ,xsensor25(sensor25), xsensor26(sensor26), xsensor27(sensor27), xsensor28(sensor28), xsensor29(sensor29), xsensor30(sensor30), xsensor31(sensor31), xsensor32(sensor32), xsensor33(sensor33), xsensor34(sensor34) ,xsensor35(sensor35), xsensor36(sensor36){}
  Ivt495Twin(UARTComponent *parent);

void setup() override {
    this->set_update_interval(60000);
}

void update() override {

}

void loop() override {
  
  if(available() > 1){
    ESP_LOGD("TAG", "'%s' - Setting", "Loop()");
    delay(100);
    fetchSerial();
    splitString();
    publishState();
  }
  delay(100);

}

void fetchSerial (){
  
  while(available() > 0)
  {
    inChar = read();
    if(inChar != 32){
      inputString += inChar;
    }
    delay(2);
  }
  inChar = '0';
  Serial.println(inputString);
  stringComplete = true;
  
}

void splitString ()
{ 
  commaPosition = inputString.indexOf(';');
  while(commaPosition >= 0)
  {
    commaPosition = inputString.indexOf(';');
    if(commaPosition != -1)
    {
      ivt[counter] = stringToFloat(inputString.substring(0,commaPosition));
      inputString = inputString.substring(commaPosition+1, inputString.length());    
    }
    else
    {
      if(inputString.length() > 0){
        ivt[counter] = stringToFloat(inputString.substring(0,commaPosition));
      }
    }
    while(ivt[counter] > (float)1200){
      ivt[counter] = ivt[counter] /(float)10;
    }
    counter++;
  }
  counter = 0;
  inputString = "";
  commaPosition = 0;
}

float stringToFloat(String input){
  String stringTemp = input;
  char stfarray[stringTemp.length() + 1];
  stringTemp.toCharArray(stfarray, sizeof(stfarray));
  float stf = atof(stfarray);
  stringTemp = "";
  memset(stfarray, 0, sizeof stfarray);
  return stf; 
}

  void publishState(){
    
    //Framledning
    if (xsensor1 != nullptr)   
      xsensor1->publish_state((ivt[1] / 10 ));

    //Ute
    if (xsensor2 != nullptr)   
      xsensor2->publish_state((ivt[2]/10));

    //Tappvarmvatten Topp
    if (xsensor3 != nullptr)   
      xsensor3->publish_state((ivt[3]/10));

    //Varmvatten Mitt
    if (xsensor4 != nullptr)   
      xsensor4->publish_state((ivt[4]/10));

    //Värmevatten Botten
    if (xsensor5 != nullptr)   
      xsensor5->publish_state((ivt[5]/10));

    //Rumstemp
    if (xsensor6 != nullptr)   
      xsensor6->publish_state((ivt[6]/10));

    //Hetgas
    if (xsensor7 != nullptr)   
      xsensor7->publish_state((ivt[7]/10));

    //Extra Acc-Tank
    if (xsensor8 != nullptr)   
      xsensor8->publish_state((ivt[8]/10));

    //Tryckvakt
    if (xsensor9 != nullptr)   
      xsensor9->publish_state((bool)(int)ivt[9]);

    //Högtryck
    if (xsensor10 != nullptr)   
      xsensor10->publish_state((bool)(int)ivt[10]);

    //Lågtryck
    if (xsensor11 != nullptr)   
      xsensor11->publish_state((bool)(int)ivt[11]);

    //Semester aktiv?
    if (xsensor12 != nullptr)   
      xsensor12->publish_state((bool)(int)ivt[12]);

    //Kompressor aktiv
    if (xsensor13 != nullptr)   
      xsensor13->publish_state((bool)(int)ivt[13]);

    //SV1 Öppna
    if (xsensor14 != nullptr)   
      xsensor14->publish_state((bool)(int)ivt[14]);

    //SV1 Stäng
    if (xsensor15 != nullptr)   
      xsensor15->publish_state((bool)(int)ivt[15]);

    //P1 Rad
    if (xsensor16 != nullptr)   
      xsensor16->publish_state((int)ivt[16]);

    //Fläkt
    if (xsensor17 != nullptr)   
      xsensor17->publish_state((bool)(int)ivt[17]);

    //Larm aktiv
    if (xsensor18 != nullptr)   
      xsensor18->publish_state((bool)(int)ivt[18]);

    //Extern P2
    if (xsensor19 != nullptr)   
      xsensor19->publish_state((int)ivt[19]);

    //LLT GT1
    if (xsensor20 != nullptr)   
      xsensor20->publish_state((ivt[20]/10));

    //LL GT1
    if (xsensor21 != nullptr)   
      xsensor21->publish_state((ivt[21]/10));

    //BV GT1
    if (xsensor22 != nullptr)   
      xsensor22->publish_state((ivt[22]/10));

    //UL GT1
    if (xsensor23 != nullptr)   
      xsensor23->publish_state((ivt[23]/10));

    //LL GT3:2
    if (xsensor24 != nullptr)   
      xsensor24->publish_state((ivt[24]/10));

    //ULT GT3:2
    if (xsensor25 != nullptr)   
      xsensor25->publish_state((ivt[25]/10));

    //UL GT3:2
    if (xsensor26 != nullptr)   
      xsensor26->publish_state((ivt[26]/10));

    //LL GT3:3
    if (xsensor27 != nullptr)   
      xsensor27->publish_state((ivt[27]/10));

    //BV GT3:3
    if (xsensor28 != nullptr)   
      xsensor28->publish_state((ivt[28]/10));

    //SV3 BV Förskj
    if (xsensor29 != nullptr)   
      xsensor29->publish_state((ivt[29]/10));

    //Effekt ink vit VV behov
    if (xsensor30 != nullptr)   
      xsensor30->publish_state((ivt[30]/10));

    //Tillskotstimer VV behov
    if (xsensor31 != nullptr)   
      xsensor31->publish_state((ivt[31]/10));

    //Tappv prio
    if (xsensor32 != nullptr)   
      xsensor32->publish_state((bool)(int)ivt[32]);

    //Tillskott i %/10
    if (xsensor33 != nullptr)   
      xsensor33->publish_state((int)ivt[33]);

    //Tillskott RAD
    if (xsensor34 != nullptr)   
      xsensor34->publish_state((int)ivt[34]);

    //Tillskott Tillägg
    if (xsensor35 != nullptr)   
      xsensor35->publish_state((ivt[35]/10));

    //Default SV2 Open
    if (xsensor36 != nullptr)   
      xsensor36->publish_state((bool)(int)ivt[36]);


  }
};