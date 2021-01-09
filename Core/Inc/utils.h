/*
 * utils.h
 *
 *  Created on: Jan 21, 2020
 *      Author: Controllerstech
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_



void getDaysOfWeek(int i);   // initialize lcd
void check_rtc(_RTC *rtc_new, _RTC *rtc_old);  // send command to the lcd
void print_rtc(_RTC rtc_new);  // send data to the lcd
int get_all_minut(int hour, int min);
int check_time(int min_start, int min_stop,int min_check);
void send_uart(char *arr);
void send_string_uart(char *str, int flag);
#endif /* INC_UTILS_H_ */
