/************************************************************************
  * @file:     xxx.c
  * @author:   guangjieMVP
  * @github:   https://github.com/guangjieMVP
  * @version:  v1.0.0
  * @date:     2020-xx-xx
  * @brief:               
**************************************************************************/
#include "led.h"


void LED_Toggle(void)
{
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

void LED_TurnOn(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, RESET);
}


void LED_TurnOff(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, SET);
}
