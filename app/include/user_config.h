/*
 * ESPRSSIF MIT License
 *
 * Copyright (c) 2015 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__
#define RESTORE_KEEP_TIMER 0

#define USE_OPTIMIZE_PRINTF
extern  int need_factory_reset ;
#define LFILE_START_ADDR  0x1f8000// flash size is 0x200000
#define LFILE_SIZE 4096

#define USER_UART_CTRL_DEV_EN 	 (1)   // support allink uart smart device
#define USER_PWM_LIGHT_EN        (0)   // support alink pwm light  
#define USER_VIRTUAL_DEV_TEST	 (1)

#define CUS_GLOBAL_VER "AlinkV1.0.02-20160420"

#define ESP_DBG(a) os_printf("[dbg@%s,%d]",__FUNCTION__,__LINE__);os_printf a;os_printf("\r\n")   // for dbg print


//#define PASS_THROUGH 


#ifdef PASS_THROUGH

#define DEV_ATTRI_NUM			(10)
//LUA ������ض���
#define LUA_DATA_HEADER			0xAA
#define LUA_DATA_END			0x55
#define LUA_DATA_HEADER_LEN		0x1
#define LUA_DATA_END_LEN		0x1
#define LUA_DATA_LEN_LEN		0x1
#define LUA_DATA_EXTENT_LEN		(LUA_DATA_HEADER_LEN+LUA_DATA_END_LEN+LUA_DATA_LEN_LEN)

#define LUA_CUS_DATA_LEN		0x0A //lua �û����ݳ���
//����ֵƫ��λ��
#define LUA_CMD_POWER_ONOFF_BIT			0x02
#define LUA_CMD_IONS_ONOFF_BIT			0x03
#define LUA_CMD_AIRQUALITY_BIT			0x04
#define LUA_CMD_WORKMODE_BIT			0x05
#define LUA_CMD_SPEED_SET_BIT			0x06
#define LUA_CMD_TIMER_ON_BIT			0x07
#define LUA_CMD_TIMER_OFF_BIT			0x08
#define LUA_CMD_FILTER_LIFE_BIT			0x09
#define LUA_CMD_ERROR_BIT				0x0A
#define LUA_CMD_AIR_ONOFF_BIT			0x0B

#else

#define DEV_ATTRI_NUM			(9)

#define CMD_POWER_ONOFF_BIT			0x00
#define CMD_IONS_ONOFF_BIT			0x01
#define CMD_AIRQUALITY_BIT			0x02
#define CMD_WORKMODE_BIT			0x03
#define CMD_SPEED_SET_BIT			0x04
#define CMD_TIMER_ON_BIT			0x05
#define CMD_FILTER_LIFE_BIT			0x06
#define CMD_ERROR_BIT				0x07
#define CMD_AIR_ONOFF_BIT			0x08

#define CMD_MCU_STATUS_QUERY		0xff//

#endif

//48Э����ض���
#define FRAME_HEAD_LEN	1	
#define FRAME_HEADER	0x48
#define FRAME_LEN_LEN	1	
#define	FRAME_TYPE_LEN	1
#define	FRAME_CHECK_LEN	1
#define	FRAME_ID_LEN	1
#define FRAME_ID_OFFSET (FRAME_HEAD_LEN + FRAME_LEN_LEN + FRAME_TYPE_LEN)
#define FRAME_EXTENAL_LEN (FRAME_ID_OFFSET + FRAME_ID_LEN + FRAME_CHECK_LEN)

#define FRAME_CUS_POST_TYPE		0x01//�����ϱ���Ϣ����
#define FRAME_CUS_DOWN_TYPE		0x02//�·���Ϣ����
#define FRAME_CUS_QUERY_TYPE	0xFE//ģ���ѯ��Ϣ����
#define FRAME_CUS_ERRECHO_TYPE	0xFF//����Ӧ����Ϣ����


#define FRAME_CMD_STATUS_QUERY		0x01//ģ��״̬��ѯ
#define FRAME_CMD_RESTART			0x02//ģ������
#define FRAME_CMD_RESTORE			0x03//�ָ���������
#define FRAME_CMD_SMARTCONFIG		0x04//����smartconfig


//�û�Э�����,���ݲ�ͬ�豸Э���޸�
#define FRAME_DOWN_DATA_LEN				0x10//����48�·���Ϣ�ܳ�
#define	FRAME_ERROR_ECHO_DATA_LEN		0x07//�������Ӧ����Ϣ�ܳ�
#define	FRAME_STATUS_ECHO_DATA_LEN		0x0B//����ģ���ѯ��Ϣ�ܳ�
//����
#define CUS_CMD_POST					0x00
#define CUS_CMD_POWER_ONOFF				0x01
#define CUS_CMD_WORKMODE				0x02
#define CUS_CMD_IONS_ONOFF				0x03
#define CUS_CMD_SPEED_SET				0x04
#define CUS_CMD_AIR_ONOFF				0x05
#define CUS_CMD_TIMER					0x06
//����ֵƫ��λ��
#define CUS_CMD_POWER_ONOFF_BIT			0x01
#define CUS_CMD_WORKMODE_BIT			0x02
#define CUS_CMD_IONS_ONOFF_BIT			0x03
#define CUS_CMD_SPEED_SET_BIT			0x04
#define CUS_CMD_AIR_ONOFF_BIT			0x05
#define CUS_CMD_TIMER_BIT				0x06
#define CUS_CMD_AIRQUALITY_BIT			0x07
#define CUS_CMD_FILTER_LIFE_BIT			0x08

//48Э�����ȡֵ��
#define FRAME_ERROR_FAIL				0x01
#define FRAME_ERROR_CRC					0x02
#define FRAME_ERROR_DATA_RANGE			0x03
#define FRAME_ERROR_UNCMD				0x04


#define CUS_CMD_KEEP_SAME				0x00


typedef struct p440_dev {   // real device update this Structure parameters
	u8 OnOff_Power;//��Դ����
	u8 OnOff_Ions;//����(������)����
	u8 Status_AirQuality;//����
	u8 WorkMode;//����ģʽ
	u8 Ventilation_Speed;//����
	u8 TimeMeter_PowerOn;//������ʱ����
	u8 TimeMeter_PowerOff;//������ʱ�ػ�
	u16 LifeTime_Filter;//
	u8 ErrorCode;//
	u8 OnOff_AirQuality;//

}P440_DEV;


extern P440_DEV virtual_device;

extern u8 cloud_connect_status;

 
typedef struct
{
	uint8_t head;
	uint8_t len;
	uint8_t type;
	uint8_t id;
	uint8_t data[FRAME_DOWN_DATA_LEN-5];
	uint8_t crc;

}__attribute__((packed))frame_t;



#endif

