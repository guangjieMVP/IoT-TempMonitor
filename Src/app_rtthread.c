/**********************************************************************
  * @file:        xxx.c
  * @author:      guangjieMVP
  * @version:     v1.00.00
  * @date:        2020-xx-xx
  * @github:      https://github.com/guangjieMVP
  * @brief: 
************************************************************************/

#include <rtthread.h>
#include "usart.h"
#include "at_cmd.h"

#define UART1_THREAD_PRIORITY        5
#define UART1_THREAD_STACK_SIZE      1024
static struct rt_thread uart1_thread;        //线程控制块
static rt_uint8_t uart1_thread_stack[UART1_THREAD_STACK_SIZE];
static void uart1_thread_entry(void *para);

#define GPRS_THREAD_PRIORITY         6  
#define GPRS_THREAD_STACK_SIZE       512
static struct rt_thread gprs_thread;     //线程控制块
static rt_uint8_t gprs_thread_stack[GPRS_THREAD_STACK_SIZE];
static void gprs_thread_entry(void *para);
    
#define DS18B20_THREAD_PRIORITY      7
#define DS18B20_THREAD_STACK_SIZE    1024
static struct rt_thread ds18b20_thread;     //线程控制块
static rt_uint8_t ds18b20_thread_stack[DS18B20_THREAD_STACK_SIZE];
static void ds18b20_thread_entry(void *para);


#define LED_THREAD_PRIORITY         8
#define LED_THREAD_STACK_SIZE       512
static struct rt_thread led_thread;     //线程控制块
static rt_uint8_t led_thread_stack[LED_THREAD_STACK_SIZE];
static void led_thread_entry(void *para);


/* 邮箱控制块 */
struct rt_mailbox uart1_mb;
/* 用于放邮件的内存池 */
static char uart1_mb_pool[128];

/* 邮箱控制块 */
struct rt_mailbox gprs_mb;
/* 用于放邮件的内存池 */
static char gprs_mb_pool[12];

static rt_err_t ret;

void app_thread_Init(void)
{
	

       /* initialize usart1 mailbox */
    // ret = rt_mb_init(&gprs_mb,
    //                 "gprs_mb",                      /* 名称是 gprs_mb */
    //                 &gprs_mb_pool[0],                /* 邮箱用到的内存池是 gprs_mb_pool */
    //                 sizeof(gprs_mb_pool) / 4,        /* 邮箱中的邮件数目，因为一封邮件占 4 字节 */
    //                 RT_IPC_FLAG_FIFO);                /* 采用 FIFO 方式进行线程等待 */
    // if (ret != RT_EOK)
    // {
    //     rt_kprintf("init gprs mailbox failed.\n");
	// 	return;
    // }

    /* initialize usart1 mailbox */
    ret = rt_mb_init(&uart1_mb,
                    "uart1_mb",                      /* 名称是 uart1_mb */
                    &uart1_mb_pool[0],                /* 邮箱用到的内存池是 uart1_mb_pool */
                    sizeof(uart1_mb_pool) / 4,        /* 邮箱中的邮件数目，因为一封邮件占 4 字节 */
                    RT_IPC_FLAG_FIFO);                /* 采用 FIFO 方式进行线程等待 */
    if (ret != RT_EOK)
    {
        rt_kprintf("init usart1 mailbox failed.\n");
		return;
    }
	
	ret = rt_thread_init(&uart1_thread,
					   "uart1_thread",
						uart1_thread_entry,
						RT_NULL,
						uart1_thread_stack,
						UART1_THREAD_STACK_SIZE,
						UART1_THREAD_PRIORITY,
                        20);
    if (ret == RT_EOK)
	{
        rt_thread_startup(&uart1_thread);
    }

    ret = rt_thread_init(&gprs_thread,
					   "gprs_thread",
						gprs_thread_entry,
						RT_NULL,
						gprs_thread_stack,
						GPRS_THREAD_STACK_SIZE,
						GPRS_THREAD_PRIORITY,
                        20);
    if (ret == RT_EOK)
	{
        rt_thread_startup(&gprs_thread);
    }
	
	ret = rt_thread_init(&led_thread,
					   "led_thread",
						led_thread_entry,
						RT_NULL,
						led_thread_stack,
						LED_THREAD_STACK_SIZE,
						LED_THREAD_PRIORITY,
                        20);
    if (ret == RT_EOK)
	{
        rt_thread_startup(&led_thread);
    }
	
	ret = rt_thread_init(&ds18b20_thread,
                        "ds18b20_thread",
                        ds18b20_thread_entry,
                        RT_NULL,
                        ds18b20_thread_stack,
                        DS18B20_THREAD_STACK_SIZE,
                        DS18B20_THREAD_PRIORITY,
                        20);

     if (ret == RT_EOK)
    {
        rt_thread_startup(&ds18b20_thread);           //startup ds18b20 thread
    }
 
}


static char sim_ok          = 0;
static char base_station_ok = 0;
static char gprs_net_ok     = 0;
static char subscribe_ok    = 0;

/* usart1 message handle thread */
static void uart1_thread_entry(void *para)
{
    usart_buffer_t *uart1_buf;

    while(1)
    {
        if ( rt_mb_recv(&uart1_mb, (rt_uint32_t *)&uart1_buf, RT_WAITING_FOREVER) == RT_EOK )
        {
			rt_kprintf("uart1 msg : %s", uart1_buf->buffer);
            if( rt_strstr(uart1_buf->buffer, AT_CHECK_SIM_RESPONSE) != NULL)
            {
                sim_ok = 1;
                rt_kprintf("sim ok\r\n");
            }
            else if( rt_strstr(uart1_buf->buffer, AT_RESIGSTER_BASE_STATION_RESPONSE1) != NULL ||
                    rt_strstr(uart1_buf->buffer, AT_RESIGSTER_BASE_STATION_RESPONSE2) != NULL)
            {
                base_station_ok = 1;
				rt_kprintf("base station ok\r\n");
                // rt_mb_send(&gprs_mb, (rt_uint32_t)&base_station_ok);
            }
            else if( rt_strstr(uart1_buf->buffer, AT_CHECK_GPRS_NET_RESPONSE) != NULL )
            {
				rt_kprintf("gprs net ok\r\n");
               
                gprs_net_ok = 1;
            }
            else if( rt_strstr(uart1_buf->buffer, AT_SUBSCRIBE_TOPIC_OK) != NULL )
            {
                rt_kprintf("subscribe topic ok\r\n");
                subscribe_ok = 1;
            }
           
			rt_memset(uart1_buf->buffer, 0, USART_BUFF_SIZE);
			uart1_buf->len = 0;
        }
    }
}

static uint16_t led_delay_ms = 1000;

/* check GPRS module whether initialize successful */
static void gprs_thread_entry(void *para)
{
	char cnt = 0;
	
    while(1)
    {
//		rt_kprintf("gprs checking %d\r\n", cnt);
        if (!sim_ok) 
        {
            send_at_cmd(AT_CHECK_SIM);
            rt_thread_mdelay(150);
        }
        
        if (!base_station_ok)
        {
            send_at_cmd(AT_RESIGSTER_BASE_STATION);
            rt_thread_mdelay(150);
        }

        if (!gprs_net_ok)
        {
            send_at_cmd(AT_CHECK_GPRS_NET);
            rt_thread_mdelay(150);
        }

        if (sim_ok && base_station_ok && gprs_net_ok)      // gprs is working and connect server
        {
			cnt = 0;
            if(!subscribe_ok)
            {
                subscribe_topic();
                rt_thread_mdelay(150);        
            }
            else 
            {
                rt_thread_suspend(&gprs_thread);                 //suspend self
			    rt_schedule();                                    //thread schedule
            }
        }
		
		if( cnt++ >= 40 )
		{
			cnt = 0;
			led_delay_ms = 500;
			rt_thread_suspend(&gprs_thread);              //suspend self
            rt_schedule();                                //thread schedule
//			rt_kprintf("gprs moduele initialize failded\r\n");
		}
        rt_thread_mdelay(500);    
    }
}


/* led shine thread */
static void led_thread_entry(void *para)
{
    while(1)
	{
//	    rt_kprintf("Linux RT-Thread uCos-III\r\n");
        led_toggle();
        rt_thread_mdelay(led_delay_ms);
    }
}

/* get and report ds18b20's temperature thread */
static void ds18b20_thread_entry(void *para)
{
	short temperature;
	short prev_temp;
	float float_temp;
	
	while(1)
	{
//		rt_hw_interrupt_disable();
//		rt_hw_interrupt_enable();
//		rt_enter_critical();
		temperature = ds18b20_get_temperature();
        if(subscribe_ok)
        {
            if(temperature != prev_temp)
		    {
                float_temp = temperature / 10.0;
//				printf("float_temp = %f\r\n", float_temp);
                send_temperature_json(float_temp);
                prev_temp = temperature;
		    }
        }
		
		rt_kprintf("temperature = %d\r\n", temperature);
//		rt_exit_critical();
		rt_thread_mdelay(1000);
	}
}


