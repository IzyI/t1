//пример использования функции strcat
 
#include <string.h>
#include <stdio.h>


#include <stdio.h>
#include <stdint.h>



//void get_alarm(int rozetka, int tip,int day)
//{
//    if (i=)
//}
int get_all_minut(int hour, int min)
{
    return hour*60+min;
}

int check_time(int min_start, int min_stop,int min_check)
{   if ((min_start<min_check) && (min_check<=min_stop)){
        printf("TRUE\n");
        return 1;
    } else{
        printf("FALSE\n");
        return 0;
    };
}

#define _EEPROM_PSIZE     16;
#define		_EEPROM_ADDRESS               0xA0;
int main(void)
{
    int min_start=get_all_minut(1,10);
    int min_stop=get_all_minut(2,10);
    int min_check=get_all_minut(2,11);
    check_time(min_start, min_stop,min_check);


}