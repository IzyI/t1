#include <string.h>
#include <stdio.h>
#include <LCD1602.h>
#include <ds3231.h>
#include "stm32f1xx_hal.h"
#include <utils.h>
#include <eeprom.h>
#include <menu.h>


char bufer[100];

void reset_state(
        int val_state_display,
        int val_type_menu,
        int val_state_menu,
        int val_cursor_menu,
        int val_length_menu) {
    state_display = val_state_display;      // состояние дисплея
    type_menu = val_type_menu;              // тип  меню
    state_menu = val_state_menu;            // вычеслем  положение экрана который мы выводим
    cursor_menu = val_cursor_menu;          // вычеслем курсор (>)  в меню
    length_menu = val_length_menu;          // длина текущего меню
}

char *menu1Strings[] = {
        "DATE",
        "ROZETKA 1",
        "ROZETKA 2",
        "ROZETKA 3",
        "ROZETKA 4",
        "ROZETKA 5",
        "ROZETKA 6",
        "ROZETKA 7",
        "ROZETKA 8",
        "CLEAR ALL"
};


uint16_t global_addr_rozetka;

struct eventRozetka global_event_rozetka = {0, 0, 0, 0, 0};

char *menu2Strings[] = {
        "SUN.", "MON.", "TUE.", "WED.", "THU.", "FRI.", "SAT.", ""
};

void print_input_menu(int input1, int input2, char *menuStrings[]) {
    lcd_clear();
    lcd_put_cur(0, 1);
    snprintf(bufer, 100, "%s ", menuStrings[input1]);
    lcd_send_string(bufer);
    lcd_put_cur(1, 1);
    snprintf(bufer, 100, "%s", menuStrings[input2]);
    lcd_send_string(bufer);
}

void set_input_menu_rozetka(uint16_t addr_rozetka) {
    uint8_t buff_rozetka[5];
    global_addr_rozetka = addr_rozetka;
    get_alarm(buff_rozetka, addr_rozetka);
    global_event_rozetka.hour1 = buff_rozetka[0] > 24 ? 0 : buff_rozetka[0];
    global_event_rozetka.min1 = buff_rozetka[1] > 60 ? 0 : buff_rozetka[1];
    global_event_rozetka.type = buff_rozetka[2] > 2 ? 0 : buff_rozetka[2];
    global_event_rozetka.hour2 = buff_rozetka[3] > 24 ? 0 : buff_rozetka[3];
    global_event_rozetka.min2 = buff_rozetka[4] > 60 ? 0 : buff_rozetka[4];
}

void set_global_event_rozetka_in_eeprom() {
    uint8_t buff_rozetka[5];
    buff_rozetka[0] = global_event_rozetka.hour1;
    buff_rozetka[1] = global_event_rozetka.min1;
    buff_rozetka[2] = global_event_rozetka.type;
    buff_rozetka[3] = global_event_rozetka.hour2;
    buff_rozetka[4] = global_event_rozetka.min2;
    if (global_addr_rozetka==ADDR_ROZETKA_1){set_alarm(buff_rozetka, ADDR_ROZETKA_1);}
    else if (global_addr_rozetka==ADDR_ROZETKA_2){ set_alarm(buff_rozetka, ADDR_ROZETKA_2);}
    else if (global_addr_rozetka==ADDR_ROZETKA_3){ set_alarm(buff_rozetka, ADDR_ROZETKA_3);}
    else if (global_addr_rozetka==ADDR_ROZETKA_4){ set_alarm(buff_rozetka, ADDR_ROZETKA_4);}
    else if (global_addr_rozetka==ADDR_ROZETKA_5){ set_alarm(buff_rozetka, ADDR_ROZETKA_5);}
    else if (global_addr_rozetka==ADDR_ROZETKA_6){ set_alarm(buff_rozetka, ADDR_ROZETKA_6);}
    else if (global_addr_rozetka==ADDR_ROZETKA_7){ set_alarm(buff_rozetka, ADDR_ROZETKA_7);}
    else if (global_addr_rozetka==ADDR_ROZETKA_8){ set_alarm(buff_rozetka, ADDR_ROZETKA_8);}
}

void print_input_menu_rozetka() {
    lcd_clear();
    lcd_put_cur(0, 0);
    snprintf(
            bufer, 100, "TIM_UP  %02d:%02d %s ",
            global_event_rozetka.hour1,
            global_event_rozetka.min1,
            global_event_rozetka.type == 0 ? "No" : "oK");
    lcd_send_string(bufer);
    lcd_put_cur(1, 0);
    snprintf(bufer, 100, "TIM_DOW %02d:%02d", global_event_rozetka.hour2, global_event_rozetka.min2);
    lcd_send_string(bufer);
}

void check_state_main_menu(int state_menu) {
    if (state_menu == 0) {
        print_input_menu(SET_DATE, ROZETKA_1, menu1Strings);
    } else if (state_menu == 1) {
        print_input_menu(ROZETKA_2, ROZETKA_3, menu1Strings);
    } else if (state_menu == 2) {
        print_input_menu(ROZETKA_4, ROZETKA_5, menu1Strings);
    } else if (state_menu == 3) {
        print_input_menu(ROZETKA_6, ROZETKA_7, menu1Strings);
    } else if (state_menu == 4) {
        print_input_menu(ROZETKA_8, CLEAR_ALL, menu1Strings);
    } else {
        __NOP();
    }
}

void check_state_menu_rozetka(int state_menu) {
    if (state_menu == 0) {
        print_input_menu_rozetka();
    } else {
        __NOP();
    }

}


void check_cursor_menu(int cursor_menu) {
    if (cursor_menu == 0) {
        lcd_put_cur(0, 0);
        snprintf(bufer, 10, ">");
        lcd_send_string(bufer);
    } else {
        lcd_put_cur(1, 0);
        snprintf(bufer, 10, ">");
        lcd_send_string(bufer);
    }
}

void date_menu_check_cursor(int cursor_menu) {
    if (cursor_menu == 0) {
        lcd_put_cur(0, 0);
    } else if (cursor_menu == 1) {
        lcd_put_cur(0, 9);
    } else if (cursor_menu == 2) {
        lcd_put_cur(0, 12);
    } else if (cursor_menu == 3) {
        lcd_put_cur(0, 15);
    } else if (cursor_menu == 4) {
        lcd_put_cur(1, 9);
    } else if (cursor_menu == 5) {
        lcd_put_cur(1, 12);
    } else if (cursor_menu == 6) {
        lcd_put_cur(1, 15);
    }
}

void rozetka_menu_check_cursor(int cursor_menu) {
    if (cursor_menu == 0) {
        lcd_put_cur(0, 9);
    } else if (cursor_menu == 1) {
        lcd_put_cur(0, 12);
    } else if (cursor_menu == 2) {
        lcd_put_cur(0, 15);
    } else if (cursor_menu == 3) {
        lcd_put_cur(1, 9);
    } else if (cursor_menu == 4) {
        lcd_put_cur(1, 12);
    }
}

void print_datee_menu(_RTC rtc_check) {
    lcd_clear();
    getDaysOfWeek(rtc_check.DaysOfWeek);
    lcd_put_cur(0, 8);
    snprintf(
            bufer,
            20,
            "%02d.%02d.%02d",
            rtc_check.Date,
            rtc_check.Month,
            rtc_check.Year
    );
    lcd_send_string(bufer);
    lcd_put_cur(1, 0);
    snprintf(bufer, 20, "TIME:   %02d:%02d:%02d", rtc_check.Hour, rtc_check.Min, rtc_check.Sec);
    lcd_send_string(bufer);


}


void encoder_rol_menu(int direct, int *cursor_menu, int length_menu, int *state_menu) {
    if (direct == 0) {
        if (*cursor_menu == 1) {
            if (*state_menu != length_menu) {
                *cursor_menu = 0;
            }
            //  change state_menu
            if (direct == 0 && length_menu != *state_menu) {
                (*state_menu)++;
            } else if (direct == 1 && *state_menu != 0) {
                (*state_menu)--;
            } else {
                __NOP();
            }
        } else {
            *cursor_menu = 1;
        };
    } else {
        if (*cursor_menu == 1) {
            *cursor_menu = 0;
        } else {
            if (*state_menu != 0) {
                *cursor_menu = 1;
            }

            //  change state_menu
            if (direct == 0 && length_menu != *state_menu) {
                (*state_menu)++;
            } else if (direct == 1 && *state_menu != 0) {
                (*state_menu)--;
            } else {
                __NOP();
            }
        };
    };
};

void print_vertical_menu(int type_menu, int state_menu, int cursor_menu, int *change_main) {
    if (type_menu == 1 && *change_main == 1) {
        check_state_main_menu(state_menu);
        check_cursor_menu(cursor_menu);
    } else {
        __NOP();
    }
    *change_main = 0;
}

void print_horizontal_menu(int type_menu, int state_menu, int cursor_menu, int *change_main, _RTC rtc_check) {
    if (type_menu == 1 && *change_main == 1) {
        print_datee_menu(rtc_check);
        date_menu_check_cursor(cursor_menu);
    } else if (type_menu == 2 && *change_main == 1) {
//        check_state_menu_rozetka(state_menu);
        print_input_menu_rozetka();
        rozetka_menu_check_cursor(cursor_menu);
    } else {
        __NOP();
    }

    *change_main = 0;
}

void encoder_rol_menu_horizontal(int direct, int *cursor_menu, int length_menu) {
    if (direct == 1) {
        if (*cursor_menu < length_menu) {
            (*cursor_menu)++;
        }
    } else {
        if (*cursor_menu > 0) {
            (*cursor_menu)--;
        }
    };
}

void add_date_rtc_check_for_cursor(int direct, int cursor_menu, _RTC *rtc_check) {
    if (cursor_menu == 0) {
        if (direct == 1) {
            if ((rtc_check->DaysOfWeek + 1) <= 7) {
                rtc_check->DaysOfWeek++;
                DS3231_SetTime(rtc_check);
            }

        } else {
            if (rtc_check->DaysOfWeek > 1 && rtc_check->DaysOfWeek <= 7) {
                rtc_check->DaysOfWeek--;
                DS3231_SetTime(rtc_check);
            }
        }
    } else if (cursor_menu == 1) {
        int b = (int) rtc_check->Month % 2;  // b = 1
        int max_day;
        if (rtc_check->Month == 2) {
            max_day = 28;
        } else {
            if (b == 0) {
                max_day = 30;
            } else {
                max_day = 31;
            }
        }
        if (direct == 1) {
            if ((rtc_check->Date) < max_day) {
                rtc_check->Date++;
                DS3231_SetTime(rtc_check);
            }

        } else {
            if (rtc_check->Date > 1 && rtc_check->Date <= max_day) {
                rtc_check->Date--;
                DS3231_SetTime(rtc_check);
            }
        }
    } else if (cursor_menu == 2) {
        if (direct == 1) {
            if ((rtc_check->Month + 1) <= 12) {
                rtc_check->Month++;
            }

        } else {
            if (rtc_check->Month > 1 && rtc_check->Month <= 12) {
                rtc_check->Month--;
            }
        }

        if (rtc_check->Month == 2) {
            if (rtc_check->Date == 30 || rtc_check->Date == 31) {
                rtc_check->Date = 28;
            }

        } else {
            if (((int) rtc_check->Month % 2) == 0 && rtc_check->Date == 31) {
                rtc_check->Date = 30;
            }
        }
        DS3231_SetTime(rtc_check);
    } else if (cursor_menu == 3) {
        if (direct == 1) {
            if ((rtc_check->Year + 0) < 99) {
                rtc_check->Year++;
                DS3231_SetTime(rtc_check);
            }

        } else {
            if (rtc_check->Year > 0 && rtc_check->Year <= 99) {
                rtc_check->Year--;
                DS3231_SetTime(rtc_check);
            }
        }
    } else if (cursor_menu == 4) {
        if (direct == 1) {
            if ((rtc_check->Hour) < 23) {
                rtc_check->Hour++;
                DS3231_SetTime(rtc_check);
            }

        } else {
            if (rtc_check->Hour > 0 && rtc_check->Hour <= 23) {
                rtc_check->Hour--;
                DS3231_SetTime(rtc_check);
            }
        }
    } else if (cursor_menu == 5) {
        if (direct == 1) {
            if ((rtc_check->Min) < 59) {
                rtc_check->Min++;
                DS3231_SetTime(rtc_check);
            }

        } else {
            if (rtc_check->Min > 0 && rtc_check->Min <= 59) {
                rtc_check->Min--;
                DS3231_SetTime(rtc_check);
            }
        }
    } else if (cursor_menu == 6) {
        if (direct == 1) {
            if ((rtc_check->Sec) < 59) {
                rtc_check->Sec++;
                DS3231_SetTime(rtc_check);
            }

        } else {
            if (rtc_check->Sec > 0 && rtc_check->Sec <= 59) {
                rtc_check->Sec--;
                DS3231_SetTime(rtc_check);
            }
        }
    }
}

void add_date_rozetka_check_for_cursor(int direct, int cursor_menu) {
    if (cursor_menu == 0) {
        if (direct == 1) {
            if ((global_event_rozetka.hour1) < 23) {
                global_event_rozetka.hour1++;
            }

        } else {
            if (global_event_rozetka.hour1 > 0 && global_event_rozetka.hour1 <= 23) {
                global_event_rozetka.hour1--;
            }
        }
    } else if (cursor_menu == 1) {
        if (direct == 1) {
            if (global_event_rozetka.min1 < 59) {
                global_event_rozetka.min1++;
            }

        } else {
            if (global_event_rozetka.min1 > 0 && global_event_rozetka.min1 <= 59) {
                global_event_rozetka.min1--;
            }
        }
    }
    else if (cursor_menu == 2) {
        if (direct == 1) {
            if (global_event_rozetka.type < 1) {
                global_event_rozetka.type++;
            }

        } else {
            if (global_event_rozetka.type > 0 && global_event_rozetka.type <= 1) {
                global_event_rozetka.type--;
            }
        }
    }
    else if (cursor_menu == 3) {
        if (direct == 1) {
            if ((global_event_rozetka.hour2) < 23) {
                global_event_rozetka.hour2++;
            }

        } else {
            if (global_event_rozetka.hour2 > 0 && global_event_rozetka.hour2 <= 23) {
                global_event_rozetka.hour2--;
            }
        }
    }
    else if (cursor_menu == 4) {
        if (direct == 1) {
            if (global_event_rozetka.min2 < 59) {
                global_event_rozetka.min2++;
            }

        } else {
            if (global_event_rozetka.min2 > 0 && global_event_rozetka.min2 <= 59) {
                global_event_rozetka.min2--;
            }
        }
    }
}