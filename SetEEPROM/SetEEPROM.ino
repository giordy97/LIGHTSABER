#include <EEPROM.h>

void setup() {

pinMode(13, OUTPUT);  digitalWrite(13, LOW);
riparti:
     //SETTING MAIN CONFIG:
     EEPROM[0] = 'C';
     EEPROM[1] = 4 ; //NUMFONT
     EEPROM[2] = 3 ; //FOLDER
     EEPROM[3] = 15; //VOLUME
     EEPROM[4] = 50; // LIGHTPOWER   
     EEPROM[5] = 0;
     EEPROM[6] = 0;

    if( EEPROM[0] == 'C' && EEPROM[1] == 4 && EEPROM[2] == 3 && EEPROM[3] == 15 &&EEPROM[4] == 50);
    else goto riparti;


     
}

void loop() {
  
    digitalWrite(13,HIGH);
    delay(1000);
    digitalWrite(13,LOW);
    delay(1000);
}
