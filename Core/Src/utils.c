#include <string.h>
#include <stdio.h>
#include <LCD1602.h>
#include <ds3231.h>
#include "stm32f1xx_hal.h"

I2C_HandleTypeDef hi2c2;
UART_HandleTypeDef huart1;

void send_uart(char *arr) {
    strcat(arr, "\n\r");
    HAL_UART_Transmit(&huart1, (uint8_t *) arr,
                      strlen(arr),
                      HAL_MAX_DELAY);
}

void send_string_uart(char *str, int flag) {
    if (flag) {
        char bufer[100];
        snprintf(bufer, 100, str);
        send_uart(bufer);
    }
};



void getDaysOfWeek(int i) {
    lcd_put_cur(0, 0);
    switch (i) {
        case 1:
            lcd_send_string("MON.:");
            break;
        case 2:
            lcd_send_string("TUE.:");
            break;
        case 3:
            lcd_send_string("WED.:");
            break;
        case 4:
            lcd_send_string("THU.:");
            break;
        case 5:
            lcd_send_string("FRI.:");
            break;
        case 6:
            lcd_send_string("SAT.:");
            break;
        case 7:
            lcd_send_string("SUN.:");
            break;

    }
}


void check_rtc(_RTC *rtc_new, _RTC *rtc_old) {
    char s[3];
    if (rtc_new->Sec != rtc_old->Sec) {
        lcd_put_cur(1, 14);
        snprintf(s, 3, "%02d", rtc_new->Sec);
        lcd_send_string(s);
    }
    if (rtc_new->Min != rtc_old->Min) {
        lcd_put_cur(1, 11);
        snprintf(s, 3, "%02d", rtc_new->Min);
        lcd_send_string(s);
    }
    if (rtc_new->Hour != rtc_old->Hour) {
        lcd_put_cur(1, 8);
        snprintf(s, 3, "%02d", rtc_new->Hour);
        lcd_send_string(s);
    }
    if (rtc_new->Date != rtc_old->Date) {
        lcd_put_cur(0, 8);
        snprintf(s, 3, "%02d", rtc_new->Date);
        lcd_send_string(s);
    }
    if (rtc_new->Month != rtc_old->Month) {
        lcd_put_cur(0, 11);
        snprintf(s, 3, "%02d", rtc_new->Month);
        lcd_send_string(s);
    }
    if (rtc_new->Year != rtc_old->Year) {
        lcd_put_cur(0, 14);
        snprintf(s, 3, "%02d", rtc_new->Year);
        lcd_send_string(s);
    }
    if (rtc_new->DaysOfWeek != rtc_old->DaysOfWeek) {
        getDaysOfWeek(rtc_new->DaysOfWeek);
    }
    rtc_new = rtc_old;
}


void print_rtc(_RTC rtc_new) {
    lcd_put_cur(1, 0);
    char s[20];
    snprintf(s, 20, "TIME:   %02d:%02d:%02d", rtc_new.Hour, rtc_new.Min, rtc_new.Sec);
    lcd_send_string(s);

    lcd_put_cur(0, 8);
    snprintf(
            s,
            20,
            "%02d.%02d.%02d",
            rtc_new.Date,
            rtc_new.Month,
            rtc_new.Year
    );
    lcd_send_string(s);
    getDaysOfWeek(rtc_new.DaysOfWeek);
}

int get_all_minut(int hour, int min)
{
    return hour*60+min;
}

int check_time(int min_start, int min_stop,int min_check)
{
    if(min_start<min_stop){
        if ((min_start<min_check) && (min_check<=min_stop)){
            return 1;
        } else{
            return 0;
        };
    }else{
        if ((min_start<min_check)||(min_check<=min_stop)){
            return 1;
        } else{
            return 0;
        };
    }


}