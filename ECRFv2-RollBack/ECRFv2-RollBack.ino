#include "ELECHOUSE_CC1101_SRC_DRV.h"

// Binary TX
String first_signal = "PUT_YOUR_FIRST_SIGNAL_HERE"; // Example: 1000100010001110111010001000111010001000100011101000100010001000100010001000100011101000100010001110 [Pause: 12315  samples] 1000100010001110111010001000111010001000100011101000100010001000100010001000100011101000100010001110 [Pause: 12315  samples] 1000100010001110111010001000111010001000100011101000100010001000100010001000100011101000100010001110
String second_signal = "PUT_YOUR_SECOND_SIGNAL_HERE";
String third_signal = "PUT_YOUR_THIRD_SIGNAL_HERE";
int samplepulse = 400; // Change this with your signal information
int timeframe = 1000; // Change this if you have a lower or higher timeframe

// Radio config
float frequency = 434.26;
int mod = 2;
float datarate = 5;
float deviation = 0;

// Radio Pins
int RXPin = 26;
int RXPin0 = 4;
int TXPin0 = 2;
int TXPin = 25;

// Global
long data_to_send[2000];
int count_binconvert=0;
String bindata;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);

  // Config CC1101
  configcc1101();

  // TX first signal
  bindata = first_signal;
  binarytoraw();
  txrawdata();
  delay(timeframe);
  
  // TX second signal
  bindata = second_signal;
  binarytoraw();
  txrawdata();
  delay(timeframe);

  // TX third signal
  bindata = third_signal;
  binarytoraw();
  txrawdata();
}

void binarytoraw() {
  
  for (int i=0; i<2000; i++){
    data_to_send[i]=0;
  }

  bindata.replace(" ","");
  bindata.replace("\n","");
  bindata.replace("Pause:","");
      
  count_binconvert=0;
  String lastbit_convert="1";
  Serial.println("");
  Serial.println(bindata);

  for (int i = 0; i<bindata.length()+1; i++){
    if (lastbit_convert != bindata.substring(i, i+1)){
      if (lastbit_convert == "1"){
        lastbit_convert="0";
      }else if (lastbit_convert == "0"){
        lastbit_convert="1";
      }
      count_binconvert++;
    }
    
    if (bindata.substring(i, i+1)=="["){
      data_to_send[count_binconvert]= bindata.substring(i+1,bindata.indexOf("]",i)).toInt();
      lastbit_convert="0";
      i+= bindata.substring(i,bindata.indexOf("]",i)).length();
    }else{
      data_to_send[count_binconvert]+=samplepulse;
    }
  }

  for (int i = 0; i<count_binconvert; i++){
    Serial.print(data_to_send[i]);
    Serial.print(",");
  }
}

void txrawdata() {
  
  for (int i = 0; i<count_binconvert; i+=2){
    digitalWrite(25,HIGH);
    delayMicroseconds(data_to_send[i]);
    digitalWrite(25,LOW);
    delayMicroseconds(data_to_send[i+1]);
  }
}

void configcc1101() {
  ELECHOUSE_cc1101.addSpiPin(14, 12, 13, 27, 1);
  pinMode(25,OUTPUT);
  ELECHOUSE_cc1101.setModul(1);
  ELECHOUSE_cc1101.Init();
  ELECHOUSE_cc1101.setModulation(mod);
  ELECHOUSE_cc1101.setMHZ(frequency);
  ELECHOUSE_cc1101.setDeviation(deviation);
  ELECHOUSE_cc1101.setDRate(datarate);
  ELECHOUSE_cc1101.SetTx();
}

void loop() {
  // put your main code here, to run repeatedly:

}
