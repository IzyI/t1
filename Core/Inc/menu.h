/*
 * menu.h
 *
 *  Created on: Jan 21, 2020
 *      Author: Controllerstech
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

int state_display;  // состояние дисплея
int type_menu;      // тип  меню
int state_menu;     // номер экрана который мы выводим во время прокуртки меню
int cursor_menu;    // вычеслем курсор (>)  в меню
int length_menu;    // длина текущего меню



int change_main;    // сообщаем что сделали изменение

typedef enum {
    SET_DATE, // изменение  даты и времени
    ROZETKA_1,
    ROZETKA_2,
    ROZETKA_3,
    ROZETKA_4,
    ROZETKA_5,
    ROZETKA_6,
    ROZETKA_7,
    ROZETKA_8,
    CLEAR_ALL // сбросить все
} menu1;

struct eventRozetka {
    uint8_t hour1; // от 0 до 24
    uint8_t min1; // от 0 до 60
    uint8_t type; // 0 вклчить, 1 выключить
    uint8_t hour2; // от 0 до 24
    uint8_t min2; // от 0 до 60
};



void print_vertical_menu(int type_menu, int state_menu, int cursor_menu, int *change_menu);

void print_horizontal_menu(int type_menu, int state_menu, int cursor_menu, int *change_menu, _RTC rtc_check);

void encoder_rol_menu(int direct, int *cursor_menu, int length_menu, int *state_menu);

void encoder_rol_menu_horizontal(int direct, int *cursor_menu, int length_menu);

void add_date_rtc_check_for_cursor(int direct, int cursor_menu, _RTC *rtc_check);

void reset_state(
        int val_state_display,
        int val_type_menu,
        int val_state_menu,
        int val_cursor_menu,
        int val_length_menu);

void set_input_menu_rozetka(uint16_t addr_rozetka);

void add_date_rozetka_check_for_cursor(int direct, int cursor_menu);
void set_global_event_rozetka_in_eeprom();
#endif /* INC_MENU_H_ */
