/**********************************************************************
  * @file:        xxx.c
  * @author:      guangjieMVP
  * @version:     v1.00.00
  * @date:        2020-xx-xx
  * @github:      https://github.com/guangjieMVP
  * @brief: 
************************************************************************/
/**********************************************************************
  * @file:        xxx.c
  * @author:      guangjieMVP
  * @version:     v1.00.00
  * @date:        2020-xx-xx
  * @github:      https://github.com/guangjieMVP
  * @brief: 
************************************************************************/
/**********************************************************************
  * @file:        xxx.c
  * @author:      guangjieMVP
  * @version:     v1.00.00
  * @date:        2020-xx-xx
  * @github:      https://github.com/guangjieMVP
  * @brief: 
************************************************************************/

#include <rtthread.h>
#include "jansson_app.h"
#include <jansson.h>
#include "usart.h"
#include "at_cmd.h"

/*
  {
     "deviceName" : "STM32G070RB",
	 "sensor" : { 
	       "temp" : 28.3
		   "humi" : 32.8      
	  }
	 
  
  }
*/
void janson_test(void)
{
	char *text;
    json_t *root = json_pack( "{sssfsf}", 
	                        "deviceName", "STM32G070RB",
	                        "temp", 28.3, 
	                        "humi" ,28.3 );
	if(root != NULL)
	{
		text = json_dumps(root, JSON_ENCODE_ANY);
		if(text != NULL)
		{
			rt_kprintf("%s\r\n", text);
			free(text);
		}
		else
		{
			rt_kprintf("failed to dumps root\r\n");
		}
		json_delete(root);
	}	
}

static void send_json(unsigned char *json_str)
{
    unsigned char json_buffer[500];
    sprintf(json_buffer, "%s\r\n", json_str);
	usart_write(&huart1, json_buffer, rt_strlen(json_buffer));
}

void send_temperature_json(double temperature)
{
    char *text;
    unsigned int str_len;
    static unsigned int prev_str_len;
	
//	printf("temperature = %f\r\n", temperature);
    json_t *root = json_pack( "{sssis{sf}ss}", 
	                        "version", "V1.0",
	                        "id", 12345, 
	                        "params" , 
							"CurrentTemperature", temperature,
	                        "method", "thing.event.property.post");
	if(root != NULL)
	{
		text = json_dumps(root, JSON_ENCODE_ANY);
		if(text != NULL)
		{
            str_len = rt_strlen(text);
//            if(str_len != prev_str_len)
//            {
                publish_topic(str_len);
//                prev_str_len = str_len;
//            } 
            rt_thread_mdelay(10);
            send_json(text);                     //send json data
			rt_kprintf("%s\r\n", text);
			free(text);
		}
		else
		{
			rt_kprintf("failed to dumps root\r\n");
		}
		json_delete(root);
	}	
}