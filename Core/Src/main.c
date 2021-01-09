/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <LCD1602.h>
#include <ds3231.h>
#include <utils.h>
#include <menu.h>
#include <eeprom.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C2_Init(void);
static void MX_TIM1_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

_RTC rtc_new = { .Year = 12, .Month = 12, .Date = 12, .DaysOfWeek = SUNDAY, .Hour = 12, .Min = 12, .Sec = 12 };
_RTC rtc_old = { .Year = 21, .Month = 21, .Date = 21, .DaysOfWeek = WEDNESDAY, .Hour = 21, .Min = 21, .Sec = 21 };
_RTC rtc_check = { .Year = 12, .Month = 12, .Date = 12, .DaysOfWeek = SUNDAY, .Hour = 12, .Min = 12, .Sec = 12 };

typedef enum {
    MENU_MAIN_LEN = 4,
    MAIN_DATE_LEN = 6,
    MENU_ALARM_LEN=4,
} dlina_menu;

int flag_irq0;
int time_irq0;
int flag_irq2;
int time_irq2;

struct eventRozetka array_event_rozetka[8];

//--------------------------------

uint32_t count;
uint8_t direct;
int flgu = 0;
char trans_str[1000];
//--------------------------------

#define GPIO_ROZETKA_1 G


void  GPIO_WritePin_Rozetka(int d,int c){
    if(d==0){
        if(c==1){
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);

        } else{
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
        }

    }
    else if(d==1){
        if(c==1){
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
        } else{
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
        }
    }
    else if(d==2){
        if(c==1){
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        } else{
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        }
    }
    else if(d==3){
        if(c==1){
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
        } else{
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
        }
    }
    else if(d==4){
        if(c==1){
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
        } else{
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
        }
    }
    else if(d==5){
        if(c==1){
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
        } else{
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        }
    }
    else if(d==6){
        if(c==1){
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
        } else{
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
        }
    }
    else if(d==7){
        if(c==1){
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
        } else{
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
        }
    }

};
void check_timer_handler(_RTC *rtc_new){
    int i;
    for (i = 0; i < 8; i++) {
        if (array_event_rozetka[i].type==1){
            int min_start=get_all_minut(array_event_rozetka[i].hour1,array_event_rozetka[i].min1);
            int min_stop=get_all_minut(array_event_rozetka[i].hour2,array_event_rozetka[i].min2);
            int min_check=get_all_minut(rtc_new->Hour, rtc_new->Min);
//            snprintf(trans_str, 100, "min_start %d - min_stop %d  - min_check %d ", min_start,min_stop,min_check);
//            send_string_uart(trans_str,flgu);
            if(check_time(min_start, min_stop,min_check)==1){
                GPIO_WritePin_Rozetka(i,1);
                snprintf(trans_str, 100, "%d - enable_rozetka", i);
            }
            else{
                GPIO_WritePin_Rozetka(i,0);
                snprintf(trans_str, 100, "%d - disable_rozetka", i);
            }
        }
        else{
            GPIO_WritePin_Rozetka(i,0);
            snprintf(trans_str, 100, "%d - disable_rozetka", i);
        }
//        send_string_uart(trans_str,flgu);
    }
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_1);
    lcd_init();
    HAL_Delay(2);
    DS3231_Init(&hi2c2);
    change_main = 0;
    reset_state(1, 0, 0, 0, 2);

    //DS3231_SetTime(&rtc);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

//    char wmsg[] = "We love STM32!";
//    char rmsg[20];
//    HAL_I2C_Mem_Write(&hi2c1, 0xA0, 0x40, I2C_MEMADD_SIZE_16BIT, (uint8_t *) wmsg, strlen(wmsg), HAL_MAX_DELAY);
//    while (HAL_I2C_IsDeviceReady(&hi2c1, 0xA0, 1, HAL_MAX_DELAY) != HAL_OK);
//    HAL_I2C_Mem_Read(&hi2c1, 0xA0, 0x40, I2C_MEMADD_SIZE_16BIT, (uint8_t *) rmsg, strlen(wmsg), HAL_MAX_DELAY);
//    while (HAL_I2C_IsDeviceReady(&hi2c1, 0xA0, 1, HAL_MAX_DELAY) != HAL_OK);
//    snprintf(trans_str, 100, "| %s |", rmsg);
//    HAL_UART_Transmit(&huart1, (uint8_t*)trans_str, strlen(trans_str), 1000);

//    uint8_t buff_rozetka[10];
//    get_alarm(buff_rozetka, ROZETKA_1);
//    int i;
//    for (i = 0; i < 8; i++) {
//        snprintf(trans_str, 100, "- %d %d,%d",i, array_event_rozetka[i].hour1,array_event_rozetka[i].hour2);
//        send_uart(trans_str);
//
//    }
//    send_string_uart("==========", flgu);
//    buff_rozetka[0]=buff_rozetka[0] > 24 ? 0 : buff_rozetka[1];
//    for (i = 0; i < 1; i++) {
//        snprintf(trans_str, 100, "%d. %d = %c ", i,buff_rozetka[i],buff_rozetka[i]);
//        send_uart(trans_str);
//
//    }



    while (1) {
        if (state_display == 0) {
            // обновляем время
            DS3231_GetTime(&rtc_new);
            check_rtc(&rtc_new, &rtc_old);
        } else if (state_display == 1) {
            // устанавливаем время и переодим в стартовый state 0
            DS3231_GetTime(&rtc_old);
            DS3231_GetTime(&rtc_new);
            lcd_clear();
            print_rtc(rtc_new);
            state_display = 0;
            get_all_alarm(array_event_rozetka);
        } else if (state_display == 2) {
            // входим в меню где есть вертикальная прокрутка
            print_vertical_menu(type_menu, state_menu, cursor_menu, &change_main);
        } else if (state_display == 3) {
            // входим в меню
            print_horizontal_menu(type_menu, state_menu, cursor_menu, &change_main, rtc_check);
        } else {
            //ничего не делаем
            __NOP();
        };



        // snprintf(trans_str, 63, "-Encoder %lu %s\n", count, TIM3->CR1 & TIM_CR1_DIR ? "DOWN" : "UP");
        // HAL_UART_Transmit(&huart1, (uint8_t*)trans_str, strlen(trans_str), 1000);

        // send_uart(trans_str);
        // lcd_put_cur(0, 0);
//          HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        // char answer[]="1111";
        // send_uart(answer,sizeof(answer));


        if (flag_irq2 && (HAL_GetTick() - time_irq2) > 200) {
            __HAL_GPIO_EXTI_CLEAR_IT(EXTI_PR_PR2);  // очищаем бит EXTI_PR
            NVIC_ClearPendingIRQ(EXTI2_IRQn); // очищаем бит NVIC_ICPRx
            HAL_NVIC_EnableIRQ(EXTI2_IRQn);   // включаем внешнее прерывание
            flag_irq2 = 0;
        }
        if (flag_irq0 && (HAL_GetTick() - time_irq0) > 200) {
            __HAL_GPIO_EXTI_CLEAR_IT(EXTI_PR_PR0);  // очищаем бит EXTI_PR
            NVIC_ClearPendingIRQ(EXTI0_IRQn); // очищаем бит NVIC_ICPRx
            HAL_NVIC_EnableIRQ(EXTI0_IRQn);   // включаем внешнее прерывание
            flag_irq0 = 0;
        }
        check_timer_handler(&rtc_new);
        send_string_uart("---------------\n", flgu);
        HAL_Delay(100);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 72-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xffff-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim2.Init.Period = 59;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 15;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA3 PA4 PA5 PA6
                           PA7 PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB3
                           PB4 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {


    if (GPIO_Pin == GPIO_PIN_2) {
        HAL_NVIC_DisableIRQ(EXTI2_IRQn);

        send_string_uart("GPIO_PIN_2 ", flgu);
        if (state_display == 0) {
            send_string_uart("entry MAIN", flgu);
            reset_state(2, 1, 0, 0, MENU_MAIN_LEN);
        }

        else if (state_display == 2 && cursor_menu == 0 && state_menu == 0 && type_menu == 1) {
            send_string_uart("entry DATE", flgu);
            DS3231_GetTime(&rtc_check);
            reset_state(3, 1, 0, 0, MAIN_DATE_LEN);
            lcd_cursor_enable();
        }
        else if (state_display == 2 && cursor_menu == 1 && state_menu==0 && type_menu == 1) {
            send_string_uart("entry ROZETKA 1", flgu);
            set_input_menu_rozetka(ADDR_ROZETKA_1);
            reset_state(3, 2, 0, 0, MENU_ALARM_LEN);
            lcd_cursor_enable();
        }
        else if (state_display == 2 && cursor_menu == 0 && state_menu==1 && type_menu == 1) {
            send_string_uart("entry ROZETKA 2", flgu);
            set_input_menu_rozetka(ADDR_ROZETKA_2);
            reset_state(3, 2, 0, 0, MENU_ALARM_LEN);
            lcd_cursor_enable();
        }
        else if (state_display == 2 && cursor_menu == 1 && state_menu==1 && type_menu == 1) {
            send_string_uart("entry ROZETKA 3", flgu);
            set_input_menu_rozetka(ADDR_ROZETKA_3);
            reset_state(3, 2, 0, 0, MENU_ALARM_LEN);
            lcd_cursor_enable();
        }
        else if (state_display == 2 && cursor_menu == 0 && state_menu==2 && type_menu == 1) {
            send_string_uart("entry ROZETKA 4", flgu);
            set_input_menu_rozetka(ADDR_ROZETKA_4);
            reset_state(3, 2, 0, 0, MENU_ALARM_LEN);
            lcd_cursor_enable();
        }
        else if (state_display == 2 && cursor_menu == 1 && state_menu==2 && type_menu == 1) {
            send_string_uart("entry ROZETKA 5", flgu);
            set_input_menu_rozetka(ADDR_ROZETKA_5);
            reset_state(3, 2, 0, 0, MENU_ALARM_LEN);
            lcd_cursor_enable();
        }
        else if (state_display == 2 && cursor_menu == 0 && state_menu==3 && type_menu == 1) {
            send_string_uart("entry ROZETKA 6", flgu);
            set_input_menu_rozetka(ADDR_ROZETKA_6);
            reset_state(3, 2, 0, 0, MENU_ALARM_LEN);
            lcd_cursor_enable();
        }
        else if (state_display == 2 && cursor_menu == 1 && state_menu==3 && type_menu == 1) {
            send_string_uart("entry ROZETKA 7", flgu);
            set_input_menu_rozetka(ADDR_ROZETKA_7);
            reset_state(3, 2, 0, 0, MENU_ALARM_LEN);
            lcd_cursor_enable();
        }
        else if (state_display == 2 && cursor_menu == 0 && state_menu==4 && type_menu == 1) {
            send_string_uart("entry ROZETKA 8", flgu);
            set_input_menu_rozetka(ADDR_ROZETKA_8);
            reset_state(3, 2, 0, 0, MENU_ALARM_LEN);
            lcd_cursor_enable();
        }
        else if (state_display == 2 && cursor_menu == 1 && state_menu==4 && type_menu == 1) {
            send_string_uart("entry CLEAR ALL", flgu);
            disable_all_alarm();
            get_all_alarm(array_event_rozetka);
            reset_state(1, 0, 0, 0, 0);
            send_string_uart("Desktop", flgu);
        }
        else if (state_display == 3) {
            if (state_menu == 1) {
                state_menu = 0;
                lcd_cursor_blink_disable();
                send_string_uart("lcd_cursor_blink_enable", flgu);
            } else {
                state_menu = 1;
                lcd_cursor_blink_enable();
                send_string_uart("lcd_cursor_blink_disable", flgu);
            }

        } else {
            __NOP();
        }

        change_main = 1;
        flag_irq2 = 1;
        time_irq2 = HAL_GetTick();

    } else if (GPIO_Pin == GPIO_PIN_0) {
        HAL_NVIC_DisableIRQ(EXTI0_IRQn);
        send_string_uart("GPIO_PIN_0 ", flgu);
        if (state_display == 2) {
            if (type_menu == 1) {
                send_string_uart("Desktop", flgu);

                reset_state(1, 0, 0, 0, 0);
            } else if (type_menu == 2 || type_menu == 3) {
                send_string_uart("exit week menu", flgu);
                reset_state(2, 1, 0, 0, MENU_MAIN_LEN);
            }
        } else if (state_display == 3) {
            if (type_menu == 1) {
                // выход в  меню 1------------------------------
                send_string_uart("exit DATE", flgu);
                reset_state(2, 1, 0, 0, MENU_MAIN_LEN);
                lcd_cursor_disable();
            }
            else if (type_menu == 2) {
                send_string_uart("exit ROZETKA", flgu);
                set_global_event_rozetka_in_eeprom();
                get_all_alarm(array_event_rozetka);
                reset_state(2, 1, 0, 0, MENU_MAIN_LEN);
                lcd_cursor_disable();
            }
        } else {
            __NOP();
        }
        change_main = 1;
        flag_irq0 = 1;
        time_irq0 = HAL_GetTick();

    } else {
        __NOP(); //ничего не делаем
    }
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim2) {
        // count = __HAL_TIM_GET_COUNTER(&htim2);
        direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2);

        //------------------------------------------------------------------------
        if (state_display == 2) {
            encoder_rol_menu(direct, &cursor_menu, length_menu, &state_menu);
        } else if (state_display == 3) {
            if (type_menu == 1) {
                if (state_menu == 0) {
                    encoder_rol_menu_horizontal(direct, &cursor_menu, length_menu);
                } else {
                    add_date_rtc_check_for_cursor(direct, cursor_menu, &rtc_check);
                }
            }
            if (type_menu == 2) {
                if (state_menu == 0) {
                    encoder_rol_menu_horizontal(direct, &cursor_menu, length_menu);
                } else {
                    add_date_rozetka_check_for_cursor(direct, cursor_menu);
                }
            }

        } else {
            __NOP();
        }
        change_main = 1;

        //------------------------------------------------------------------------

        snprintf(
                trans_str,
                100,
                "count-%lu direct-%u state_display-%u type_menu-%u state_menu-%u cursor_menu-%u length_menu-%u\n",
                count,
                direct,
                state_display,
                type_menu,
                state_menu,
                cursor_menu,
                length_menu
        );
        send_string_uart(trans_str,flgu);

    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
