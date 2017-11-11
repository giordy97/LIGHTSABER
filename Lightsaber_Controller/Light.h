#ifndef LIGHT_H
#define LIGHT_H
#include <Arduino.h>

extern uint8_t fade_OffToOn(uint8_t lightpower);
extern uint16_t fade_OnToOff(uint8_t lightpower);
extern void LockUp(uint8_t lightpower);
extern void Flicker(uint8_t lightpower);
extern void CristalPulse();
extern uint8_t clashFlash(uint8_t lightpower);

#endif //LIGHT_H
