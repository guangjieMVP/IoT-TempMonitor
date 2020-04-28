/**********************************************************************
  * @file:        xxx.c
  * @author:      guangjieMVP
  * @version:     v1.00.00
  * @date:        2020-xx-xx
  * @github:      https://github.com/guangjieMVP
  * @brief: 
************************************************************************/
#include <rtthread.h>

#include "at_cmd.h"
#include "usart.h"
#include "stdio.h"

void send_at_cmd(unsigned char *cmd)
{
	unsigned char cmd_buffer[300];
	sprintf(cmd_buffer, "%s\r\n", cmd);
	usart_write(&huart1, cmd_buffer, rt_strlen(cmd_buffer));
}

//订阅主题
void subscribe_topic(void)
{
	unsigned char cmd_buffer[300];
	sprintf(cmd_buffer, "%s\r\n", AT_SUBSCRIBE_TOPIC);
	usart_write(&huart1, cmd_buffer, rt_strlen(cmd_buffer));
}

//推送主题
void publish_topic(unsigned int topic_len)
{
	unsigned char cmd_buffer[300];
	sprintf(cmd_buffer, "%s,%d,0\r\n", AT_PUBLISH_TOPIC, topic_len);
	usart_write(&huart1, cmd_buffer, rt_strlen(cmd_buffer));
}