#ifndef MENU_H
#define MENU_H
#include <Arduino.h>

extern void get_config(uint8_t * folder,uint8_t * volume,uint8_t * lightpower,uint8_t* flicker);
extern void set_config(uint8_t folder,uint8_t volume,uint8_t lightpower,uint8_t flicker);
extern void menu(uint8_t * folder,uint8_t * volume,uint8_t * lightpower,uint8_t* flicker);

#endif //MENU_H
