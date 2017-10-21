#ifndef MENU_H
#define MENU_H
#include <Arduino.h>

extern void get_config(uint8_t * numfont,uint8_t * folder,uint8_t * volume,uint8_t * lightpower);
extern void set_config(uint8_t numfont,uint8_t folder,uint8_t volume,uint8_t lightpower);
extern void menu(uint8_t * numfont,uint8_t * folder,uint8_t * volume,uint8_t * lightpower);

#endif //MENU_H
