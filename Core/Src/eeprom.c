//
// Created by iz on 07.01.2021.
//

#include "eeprom.h"
#include "stm32f1xx_hal.h"

I2C_HandleTypeDef hi2c1;



void get_alarm(uint8_t *buff,uint16_t addr_rozetka)
{
    HAL_I2C_Mem_Read(&hi2c1,ADDRESS_EEPROM , addr_rozetka, I2C_MEMADD_SIZE_16BIT, (uint8_t *) buff, 5, HAL_MAX_DELAY);
    while (HAL_I2C_IsDeviceReady(&hi2c1, ADDRESS_EEPROM, 1, HAL_MAX_DELAY) != HAL_OK);
}

void set_alarm(uint8_t *buff,uint16_t addr_rozetka)
{
    HAL_I2C_Mem_Write(&hi2c1,ADDRESS_EEPROM , addr_rozetka, I2C_MEMADD_SIZE_16BIT, (uint8_t *) buff, 5, HAL_MAX_DELAY);
    while (HAL_I2C_IsDeviceReady(&hi2c1, ADDRESS_EEPROM, 1, HAL_MAX_DELAY) != HAL_OK);
}


void get_all_alarm(struct eventRozetka *array_event_rozetka )
{
    uint8_t buff_rozetka[5];
    uint16_t list_addr[8]={
            ADDR_ROZETKA_1,
            ADDR_ROZETKA_2,
            ADDR_ROZETKA_3,
            ADDR_ROZETKA_4,
            ADDR_ROZETKA_5,
            ADDR_ROZETKA_6,
            ADDR_ROZETKA_7,
            ADDR_ROZETKA_8,
    };
    int i;
    for (i = 0; i < 8; i++) {
        HAL_I2C_Mem_Read(&hi2c1,ADDRESS_EEPROM , list_addr[i], I2C_MEMADD_SIZE_16BIT, (uint8_t *) buff_rozetka, 5, HAL_MAX_DELAY);
        while (HAL_I2C_IsDeviceReady(&hi2c1, ADDRESS_EEPROM, 1, HAL_MAX_DELAY) != HAL_OK);
        array_event_rozetka[i].hour1 = buff_rozetka[0] > 24 ? 0 : buff_rozetka[0];
        array_event_rozetka[i].min1 = buff_rozetka[1] > 60 ? 0 : buff_rozetka[1];
        array_event_rozetka[i].type = buff_rozetka[2] > 2 ? 0 : buff_rozetka[2];
        array_event_rozetka[i].hour2 = buff_rozetka[3] > 24 ? 0 : buff_rozetka[3];
        array_event_rozetka[i].min2 = buff_rozetka[4] > 60 ? 0 : buff_rozetka[4];

    }
}
void disable_all_alarm()
    {
        uint8_t buff_rozetka[5];
        uint16_t list_addr[8]={
                ADDR_ROZETKA_1,
                ADDR_ROZETKA_2,
                ADDR_ROZETKA_3,
                ADDR_ROZETKA_4,
                ADDR_ROZETKA_5,
                ADDR_ROZETKA_6,
                ADDR_ROZETKA_7,
                ADDR_ROZETKA_8,
        };
        int i;
        for (i = 0; i < 8; i++) {
            HAL_I2C_Mem_Read(&hi2c1,ADDRESS_EEPROM , list_addr[i], I2C_MEMADD_SIZE_16BIT, (uint8_t *) buff_rozetka, 5, HAL_MAX_DELAY);
            while (HAL_I2C_IsDeviceReady(&hi2c1, ADDRESS_EEPROM, 1, HAL_MAX_DELAY) != HAL_OK);
            buff_rozetka[2]=0;
            HAL_I2C_Mem_Write(&hi2c1,ADDRESS_EEPROM , list_addr[i], I2C_MEMADD_SIZE_16BIT, (uint8_t *) buff_rozetka, 5, HAL_MAX_DELAY);
            while (HAL_I2C_IsDeviceReady(&hi2c1, ADDRESS_EEPROM, 1, HAL_MAX_DELAY) != HAL_OK);

        }

    }