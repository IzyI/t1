//
// Created by iz on 07.01.2021.
//
// ROZETKA_1   |012|     3*12   = 36
// ROZETKA_2   |012|     3*12   = 36
// ROZETKA_3   |012|     3*12   = 36
// ROZETKA_4   |012|     3*12   = 36
// ROZETKA_5   |012|     3*12   = 36
// ROZETKA_6   |012|     3*12   = 36
// ROZETKA_7   |012|     3*12   = 36
// ROZETKA_8   |012|     3*12   = 36

//  256K is internally organized as 512 pages of 64-bytes each
//  b111111111111111 == 0x7fff

#ifndef T1_EEPROM_H
#define T1_EEPROM_H

#include <stdlib.h>
#include <stdbool.h>
#include "ds3231.h"
#include "menu.h"
#include "stm32f1xx_hal.h"

#define ADDR_ROZETKA_1   0x0
#define ADDR_ROZETKA_2   0x40
#define ADDR_ROZETKA_3   0x80
#define ADDR_ROZETKA_4   0xC0
#define ADDR_ROZETKA_5   0x100
#define ADDR_ROZETKA_6   0x140
#define ADDR_ROZETKA_7   0x180
#define ADDR_ROZETKA_8   0x1C0

#define ADDRESS_EEPROM   0xA0


void get_alarm(uint8_t *buff,uint16_t addr_rozetka);
void set_alarm(uint8_t *buff,uint16_t addr_rozetka);
void get_all_alarm(struct eventRozetka *array_event_rozetka );
void disable_all_alarm();

#endif //T1_EEPROM_H
