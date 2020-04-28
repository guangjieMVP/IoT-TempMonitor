/**********************************************************************
  * @file:        xxx.c
  * @author:      guangjieMVP
  * @version:     v1.00.00
  * @date:        2020-xx-xx
  * @github:      https://github.com/guangjieMVP
  * @brief: 
************************************************************************/
/***********************************************************************
 * @file:        xxx.c
 * @author:      guangjieMVP
 * @version:     v1.00.00
 * @date:        2020-xx-xx
 * @github:      https://github.com/guangjieMVP
 * @brief: 
***********************************************************************/
#ifndef _AT_CMD_H_
#define _AT_CMD_H_

#define AT_CHECK_SIM                         "AT+CPIN?"          //查询SIM是否正常
#define  AT_CHECK_SIM_RESPONSE               "+CPIN:READY"       //SIM卡正常应答

#define AT_RESIGSTER_BASE_STATION             "AT+CREG?"         //查询注册基站是否成功
#define AT_RESIGSTER_BASE_STATION_RESPONSE1   "+CREG: 1,1"       //注册基站成功返回值1
#define AT_RESIGSTER_BASE_STATION_RESPONSE2   "+CREG: 1,5"       //注册基站成功返回值2

#define AT_CHECK_GPRS_NET                     "AT+CGATT?"        //查询GPRS网络附着是否成功
#define AT_CHECK_GPRS_NET_RESPONSE            "+CGATT:1"         //成功返回值

#define AT_SUBSCRIBE_TOPIC       "AT+SUBSCRIBE=\"/sys/a1TAQrjlyNl/STM32G0_Node/thing/event/property/post\",0"      //订阅主题
#define AT_PUBLISH_TOPIC         "AT+PUBLISH=\"/sys/a1TAQrjlyNl/STM32G0_Node/thing/event/property/post\""         //发布主题

#define AT_SUBSCRIBE_TOPIC_OK    "Subscribe OK"

// #define SIM_OK             "sim ok"
// #define BASE_STATION_OK    "base station ok"
// #define GPRS_NET_OK        "gprs net ok"

void send_at_cmd(unsigned char *cmd);

void subscribe_topic(void);     //订阅主题
void publish_topic(unsigned int topic_len);    //推送主题

#endif