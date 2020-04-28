#include "ds18b20.h"
 
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>

void DS18B20_Output_Init(void);
void DS18B20_Input_Init(void);

#define DS18B20_IO_IN()     DS18B20_Input_Init()    //{GPIOB->MODER&=~(3<<(12*2));GPIOB->MODER|=0<<12*2;}
#define DS18B20_IO_OUT()    DS18B20_Output_Init()  // {GPIOB->MODER&=~(3<<(12*2));GPIOB->MODER|=1<<12*2;}

#define DS18B20_DQ_OUT(X)  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, X) //数据端口 PB12
#define DS18B20_DQ_IN  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)  //数据端口 PB12

void DS18B20_Output_Init(void)
{
    GPIO_InitTypeDef gpioInitStruct = {0};

    gpioInitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    gpioInitStruct.Pin   = GPIO_PIN_12;
    gpioInitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOB, &gpioInitStruct);
}

void DS18B20_Input_Init(void)
{
    GPIO_InitTypeDef gpioInitStruct = {0};

    gpioInitStruct.Mode  = GPIO_MODE_INPUT;
    gpioInitStruct.Pin   = GPIO_PIN_12;
    gpioInitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOB, &gpioInitStruct);
}

extern void rt_hw_us_delay(rt_uint32_t us);
//复位DS18B20
void DS18B20_Rst(void)
{
    DS18B20_IO_OUT();   //设置为输出
    DS18B20_DQ_OUT(0);  //拉低DQ
    rt_hw_us_delay(750);      //拉低750us
    DS18B20_DQ_OUT(1);  //DQ=1
    rt_hw_us_delay(15);       //15US
}

//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
uint8_t DS18B20_Check(void)
{
    uint8_t retry = 0;
    DS18B20_IO_IN();    //设置为输入
    while (DS18B20_DQ_IN && retry < 200)
    {
        retry++;
        rt_hw_us_delay(1);
    };
    if(retry >= 200)
        return 1;
    else
        retry=0;
    while (!DS18B20_DQ_IN && retry < 240)
    {
        retry++;
        rt_hw_us_delay(1);
    };

    if(retry >= 240)
        return 1;
    return 0;
}

//从DS18B20读取一个位
//返回值：1/0
uint8_t DS18B20_Read_Bit(void)
{
    uint8_t data;

    DS18B20_IO_OUT();   //设置为输出
    DS18B20_DQ_OUT(0);
    rt_hw_us_delay(2);
    DS18B20_DQ_OUT(1);
    DS18B20_IO_IN();    //设置为输入
    rt_hw_us_delay(12);
    if(DS18B20_DQ_IN)
        data = 1;
    else
        data=0;
    rt_hw_us_delay(50);
    return data;
}

//从DS18B20读取一个字节
//返回值：读到的数据
uint8_t DS18B20_Read_Byte(void)
{
    uint8_t i,j,dat;

    dat = 0;
    for ( i = 1 ; i <= 8; i++)
    {
        j = DS18B20_Read_Bit();
        dat = (j << 7) | (dat >> 1);
    }
    return dat;
}

//写一个字节到DS18B20
//dat：要写入的字节
void DS18B20_Write_Byte(uint8_t dat)
 {
    uint8_t j;
    uint8_t testb;
    DS18B20_IO_OUT();     //设置为输出
    for (j = 1; j <= 8; j++)
    {
        testb = dat&0x01;
        dat = dat >> 1;
        if(testb)       // 写1
        {
            DS18B20_DQ_OUT(0);
            rt_hw_us_delay(2);
            DS18B20_DQ_OUT(1);
            rt_hw_us_delay(60);
        }
        else            //写0
        {
            DS18B20_DQ_OUT(0);
            rt_hw_us_delay(60);
            DS18B20_DQ_OUT(1);
            rt_hw_us_delay(2);
        }
    }
}

//开始温度转换
void DS18B20_Start(void)
{
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
}

//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在
uint8_t DS18B20_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOB时钟

    GPIO_Initure.Pin   = GPIO_PIN_12;           //PB12
    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull  = GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_LOW;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //初始化

    DS18B20_Rst();
    return DS18B20_Check();
}

void user_ds18b20_init(void)
{
	if(!DS18B20_Init())
	{
		rt_kprintf("ds18b20\r\n");
	}
	else
	{
		rt_kprintf("no ds18b20\r\n");
	}
}

//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250）
short DS18B20_Get_Temp(void)
{
    uint8_t temp;
    uint8_t TL,TH;
    short tem;

    DS18B20_Start();           //开始转换
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);   // skip rom
    DS18B20_Write_Byte(0xbe);   // convert
    TL = DS18B20_Read_Byte();     // LSB
    TH = DS18B20_Read_Byte();     // MSB
    if(TH > 7)
    {
        TH = ~TH;
        TL = ~TL;
        temp = 0;//温度为负
    }
    else
        temp = 1;//温度为正
    tem = TH; //获得高八位
    tem <<= 8;
    tem += TL;//获得底八位
    tem = (double)tem * 0.625;//转换
    if(temp)
        return tem; //返回温度值
    else
        return -tem;
}

short ds18b20_get_temperature(void)
{
	return DS18B20_Get_Temp();
}

