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



//////IO��������
//#define DS18B20_DQ_OUT PBout(12) //���ݶ˿� PB12
//#define DS18B20_DQ_IN  PBin(12)  //���ݶ˿� PB12

////IO��������


uint8_t DS18B20_Init(void);          //��ʼ��DS18B20
void DS18B20_Start(void);       //��ʼ�¶�ת��
void DS18B20_Write_Byte(uint8_t dat);//д��һ���ֽ�
uint8_t DS18B20_Read_Byte(void);     //����һ���ֽ�
uint8_t DS18B20_Read_Bit(void);      //����һ��λ
uint8_t DS18B20_Check(void);         //����Ƿ����DS18B20
void DS18B20_Rst(void);         //��λDS18B20


short ds18b20_get_temperature(void);

#endif /* APPLICATIONS_DS18B20_H_ */
