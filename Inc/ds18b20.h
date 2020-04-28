/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-08     Ares       the first version
 */
#ifndef APPLICATIONS_DS18B20_H_
#define APPLICATIONS_DS18B20_H_
#include "gpio.h"



//////IO操作函数
//#define DS18B20_DQ_OUT PBout(12) //数据端口 PB12
//#define DS18B20_DQ_IN  PBin(12)  //数据端口 PB12

////IO操作函数


uint8_t DS18B20_Init(void);          //初始化DS18B20
void DS18B20_Start(void);       //开始温度转换
void DS18B20_Write_Byte(uint8_t dat);//写入一个字节
uint8_t DS18B20_Read_Byte(void);     //读出一个字节
uint8_t DS18B20_Read_Bit(void);      //读出一个位
uint8_t DS18B20_Check(void);         //检测是否存在DS18B20
void DS18B20_Rst(void);         //复位DS18B20


short ds18b20_get_temperature(void);

#endif /* APPLICATIONS_DS18B20_H_ */
