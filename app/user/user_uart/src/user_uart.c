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

#include "c_types.h"
#include <stdio.h>
#include <string.h>
#include "esp_common.h"
#include "user_uart.h"
//#include "user_light.h"


#include "../../../include/driver/uart.h"

xQueueHandle xQueueCusUart;

uint8_t _ID = 0;

void debug_print_hex_data(char*buf, int len)
{
	int i = 0;
	printf("\n_____________[%d]__________\n", len);
	for (i = 0; i<len; i++)
	{
		printf("%X ", *(buf + i));
	}
	printf("\n____________________________\n");
	return;
}

int uart0_write_data(u8 *data, int len)
{
	int i = 0;
	for (i = 0; i <len; i++)
	{
		uart0_write_char(*(data + i));
	}
	return i;

}

#ifdef PASS_THROUGH

void ICACHE_FLASH_ATTR cus_wifi_data_handler(u8 cmd, int value)
{

	ESP_DBG(("cmd:%d,value:%d", cmd, value));

	uint8_t i;
	frame_t frame;

	for (i = 0; i < FRAME_DOWN_DATA_LEN-FRAME_EXTENAL_LEN; i++)
		frame.data[i] = 0;

	frame.crc = 0;
	frame.head = FRAME_HEADER;
	frame.crc += frame.head;
	frame.len = FRAME_DOWN_DATA_LEN;
	frame.crc += frame.len;
	frame.type = FRAME_CUS_DOWN_TYPE;
	frame.crc += frame.type;
	frame.id = _ID++;
	frame.crc += frame.id;

	switch (cmd) {

	case CUS_CMD_POST:

		frame.data[0] = CUS_CMD_POST;

		break;

	case CUS_CMD_POWER_ONOFF:
		frame.data[0] = CUS_CMD_POWER_ONOFF;
		frame.data[CUS_CMD_POWER_ONOFF_BIT] = value;
		break;
	case CUS_CMD_WORKMODE:
		frame.data[0] = CUS_CMD_WORKMODE;
		frame.data[CUS_CMD_WORKMODE_BIT] = value;
		break;
	case CUS_CMD_IONS_ONOFF:
		frame.data[0] = CUS_CMD_IONS_ONOFF;
		frame.data[CUS_CMD_IONS_ONOFF_BIT] = value;
		break;
	case CUS_CMD_SPEED_SET:
		frame.data[0] = CUS_CMD_SPEED_SET;
		frame.data[CUS_CMD_SPEED_SET_BIT] = value;
		break;
	case CUS_CMD_AIR_ONOFF:
		frame.data[0] = CUS_CMD_AIR_ONOFF;
		frame.data[CUS_CMD_AIR_ONOFF_BIT] = value;
		break;
	case CUS_CMD_TIMER:
		frame.data[0] = CUS_CMD_TIMER;
		frame.data[CUS_CMD_TIMER_BIT] = value;
		break;
	default:
		ESP_DBG(("CMD UNKNOW"));
		return;

	}
	frame.crc += frame.data[0];
	frame.crc += value;

	uart0_write_data((uint8_t *)&frame, frame.len);

	return;
}

#else


void ICACHE_FLASH_ATTR cus_wifi_handler_alinkdata2mcu(u8 dat_index, int dat_value)
{
	ESP_DBG(("index[%x],data_value[%x]", dat_index, dat_value));
	// here handler user own uart protocol...
	uint8_t i;
	frame_t frame;

	for (i = 0; i < FRAME_DOWN_DATA_LEN - FRAME_EXTENAL_LEN; i++)
		frame.data[i] = 0;

	frame.crc = 0;
	frame.head = FRAME_HEADER;
	frame.crc += frame.head;
	frame.len = FRAME_DOWN_DATA_LEN;
	frame.crc += frame.len;
	frame.type = FRAME_CUS_DOWN_TYPE;
	frame.crc += frame.type;
	frame.id = _ID++;
	frame.crc += frame.id;

	switch (dat_index) {

	case CMD_MCU_STATUS_QUERY:

		frame.data[0] = CUS_CMD_POST;

		break;

	case CMD_POWER_ONOFF_BIT:
		frame.data[0] = CUS_CMD_POWER_ONOFF;
		frame.data[CUS_CMD_POWER_ONOFF_BIT] = dat_value;
		break;
	case CMD_WORKMODE_BIT:
		frame.data[0] = CUS_CMD_WORKMODE;
		frame.data[CUS_CMD_WORKMODE_BIT] = dat_value;
		break;
	case CMD_IONS_ONOFF_BIT:
		frame.data[0] = CUS_CMD_IONS_ONOFF;
		frame.data[CUS_CMD_IONS_ONOFF_BIT] = dat_value;
		break;
	case CMD_SPEED_SET_BIT:
		frame.data[0] = CUS_CMD_SPEED_SET;
		frame.data[CUS_CMD_SPEED_SET_BIT] = dat_value;
		break;
	case CMD_AIR_ONOFF_BIT:
		frame.data[0] = CUS_CMD_AIR_ONOFF;
		frame.data[CUS_CMD_AIR_ONOFF_BIT] = dat_value;
		break;
	case CMD_TIMER_ON_BIT:
		frame.data[0] = CUS_CMD_TIMER;
		frame.data[CUS_CMD_TIMER_BIT] = dat_value;
		break;
	default:
		ESP_DBG(("CMD UNSUPPORT"));
		return;

	}
	frame.crc += frame.data[0];
	frame.crc += dat_value;

	uart0_write_data((uint8_t *)&frame, frame.len);


	return;
}


#endif


/*
	模块状态查询应答
 */
void ICACHE_FLASH_ATTR cus_uart_data_echo()
{
	ESP_DBG((" "));

	uint8_t i;
	frame_t frame;

	for (i = 0; i < FRAME_STATUS_ECHO_DATA_LEN - 5; i++)
		frame.data[i] = 0;

	frame.crc = 0;
	frame.head = FRAME_HEADER;
	frame.crc += frame.head;
	frame.len = FRAME_STATUS_ECHO_DATA_LEN;
	frame.crc += frame.len;
	frame.type = FRAME_CUS_QUERY_TYPE;
	frame.crc += frame.type;
	frame.id = _ID++;
	frame.crc += frame.id;

	i = 0;
	frame.data[i] = FRAME_CMD_STATUS_QUERY;  //work model, 01:STA,02:AP,03:STA+AP
	frame.crc += frame.data[i++];
	//add mode status
	frame.data[i] = wifi_get_opmode();  //work model, 01:STA,02:AP,03:STA+AP
	frame.crc += frame.data[i++];
	//add AP connected status
	if (wifi_station_get_connect_status()) {
		frame.data[i] = 0x01;
	}
	else {
		frame.data[i] = 0x02;
	}
	frame.crc += frame.data[i++];
	//add CLOUD connected status
	frame.data[i] = cloud_connect_status;
	frame.crc += frame.data[i++];
	//add RSSI
	int rssi = wifi_station_get_rssi();
	frame.data[i] = ((128 + rssi) / 128.0 * 4 + 1);
	frame.crc += frame.data[i++];

	frame.data[i++] = 0;//保留
	frame.data[i] = frame.crc;

	uint8_t *p = (uint8_t *)&frame;

	uart0_write_data(p, frame.len);

	return;
}

/*
	串口错误应答
*/
void ICACHE_FLASH_ATTR cus_uart_error_echo(u8 error, u8 id)
{
	ESP_DBG(("cus_uart_error_echo"));

	frame_t frame;

	frame.crc = 0;
	frame.head = FRAME_HEADER;
	frame.crc += frame.head;
	frame.len = FRAME_ERROR_ECHO_DATA_LEN;
	frame.crc += frame.len;
	frame.type = FRAME_CUS_ERRECHO_TYPE;
	frame.crc += frame.type;
	frame.id = id;
	frame.crc += frame.id;

	frame.data[0] = error;
	frame.crc += error;
	frame.data[1] = 0;//保留

	uart0_write_data((uint8_t *)&frame, frame.len);

	return;
}


static u8 ICACHE_FLASH_ATTR cus_uart_data_handle(char *dat_in, int in_len, char *dat_out)
{
	ESP_DBG(("uart data handler.."));

	//接收到MCU发来的串口数据
	//根据串口协议，更新virtual device status

	if (*dat_in != FRAME_HEADER || in_len > FRAME_DOWN_DATA_LEN)
	{
		cus_uart_error_echo(FRAME_ERROR_DATA_RANGE, dat_in[FRAME_ID_OFFSET]);
		return false;
	}

	uint8_t crc = 0, i;
	for (i = 0; i < in_len - 1; i++)
	{
		crc += dat_in[i];
	}

	//尚未添加CRC校验
	printf("\ncrc:%d,recv_crc:%d\n", crc, dat_in[in_len - 1]);
	/*
	if (crc != dat_in[in_len - 1]){
		cus_uart_error_echo(FRAME_ERROR_CRC,dat_in[FRAME_ID_OFFSET]);
		return false;
	}
	*/
	uint8_t frame_type = dat_in[FRAME_HEAD_LEN + FRAME_LEN_LEN];
	char *cus_data = dat_in + FRAME_ID_OFFSET + FRAME_ID_LEN;//指向数据区-命令
	
	if (FRAME_CUS_POST_TYPE == frame_type || FRAME_CUS_DOWN_TYPE == frame_type) {

		if (*cus_data == 0)//状态查询响应消息或设备主动上报
		{
			if (virtual_device.OnOff_Power != cus_data[CUS_CMD_POWER_ONOFF_BIT]) {
				virtual_device.OnOff_Power = cus_data[CUS_CMD_POWER_ONOFF_BIT];
				device_status_change = 1;//更新设备状态，并上报设备状态至云
			}
			if (virtual_device.WorkMode != cus_data[CUS_CMD_WORKMODE_BIT]) {
				virtual_device.WorkMode = cus_data[CUS_CMD_WORKMODE_BIT];
				device_status_change = 1;//更新设备状态，并上报设备状态至云
			}
			if (virtual_device.OnOff_Ions != cus_data[CUS_CMD_IONS_ONOFF_BIT]) {
				virtual_device.OnOff_Ions = cus_data[CUS_CMD_IONS_ONOFF_BIT];
				device_status_change = 1;//更新设备状态，并上报设备状态至云
			}
			if (virtual_device.Ventilation_Speed != cus_data[CUS_CMD_SPEED_SET_BIT]) {
				virtual_device.Ventilation_Speed = cus_data[CUS_CMD_SPEED_SET_BIT];
				device_status_change = 1;//更新设备状态，并上报设备状态至云
			}
			if (virtual_device.OnOff_AirQuality != cus_data[CUS_CMD_AIR_ONOFF_BIT]) {
				virtual_device.OnOff_AirQuality = cus_data[CUS_CMD_AIR_ONOFF_BIT];
				device_status_change = 1;//更新设备状态，并上报设备状态至云
			}
			if (virtual_device.TimeMeter_PowerOn != cus_data[CUS_CMD_TIMER_BIT]) {
				virtual_device.TimeMeter_PowerOn = cus_data[CUS_CMD_TIMER_BIT];//////////
				device_status_change = 1;//更新设备状态，并上报设备状态至云
			}
			if (virtual_device.Status_AirQuality != cus_data[CUS_CMD_AIRQUALITY_BIT]) {
				virtual_device.Status_AirQuality = cus_data[CUS_CMD_AIRQUALITY_BIT];
				device_status_change = 1;//更新设备状态，并上报设备状态至云
			}
			if (virtual_device.LifeTime_Filter != ((cus_data[CUS_CMD_FILTER_LIFE_BIT] << 8) + cus_data[CUS_CMD_FILTER_LIFE_BIT + 1])) {
				virtual_device.LifeTime_Filter = ((cus_data[CUS_CMD_FILTER_LIFE_BIT] << 8) + cus_data[CUS_CMD_FILTER_LIFE_BIT + 1]);
				device_status_change = 1;//更新设备状态，并上报设备状态至云
			}

			//other params
			
			if (FRAME_CUS_POST_TYPE == frame_type)
				uart0_write_data(dat_in, in_len);//主动上报消息 模块反馈响应

		}
		else {
			if (FRAME_CUS_DOWN_TYPE == frame_type) {
				//APP下发命令响应,根据业务需求做相应处理




			}
			else {
				ESP_DBG(("command unsupport"));
				cus_uart_error_echo(FRAME_ERROR_UNCMD, dat_in[FRAME_ID_OFFSET]);
				return false;
			}

		} 
	}
	else if (FRAME_CUS_QUERY_TYPE == frame_type)//模块状态查询/设置
	{
		switch (*cus_data)
		{
		case FRAME_CMD_STATUS_QUERY://mode status query

			cus_uart_data_echo();

			break;

		case FRAME_CMD_RESTART://restart

			uart0_write_data(dat_in, in_len);//反馈响应
			///


			break;

		case FRAME_CMD_RESTORE://restore

			uart0_write_data(dat_in, in_len);//反馈响应

			need_factory_reset = 1;

			break;
		case FRAME_CMD_SMARTCONFIG://smartconfig

			uart0_write_data(dat_in, in_len);//反馈响应

			break;

		default:
			
			cus_uart_error_echo(FRAME_ERROR_FAIL, dat_in[FRAME_ID_OFFSET]);
			
			break;

		}

	}
	else if (FRAME_CUS_ERRECHO_TYPE == frame_type) {

		//下发至MCU的串口数据出错
		ESP_DBG(("data2mcu ERROR"));
	}
	else {

		ESP_DBG(("frame type unsupport"));
		cus_uart_error_echo(FRAME_ERROR_FAIL,dat_in[FRAME_ID_OFFSET]);
		return false;
	}

	return true;
}

void ICACHE_FLASH_ATTR user_uart_task(void *pvParameters)
{
	CusUartIntrPtr uartptrData;
	u32 sys_time_value = system_get_time();
	//char uart_beat_data[] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38 };  // test uart beat data "12345678"

	while (1)
	{
		if (xQueueReceive(xQueueCusUart, (void *)&uartptrData, (portTickType)500/*portMAX_DELAY*/)) // wait about 5sec 
		{
			ESP_DBG(("data uart recv.."));
			debug_print_hex_data(uartptrData.rx_buf, uartptrData.rx_len);

			if (uartptrData.rx_len>0x00) {
				cus_uart_data_handle(uartptrData.rx_buf, uartptrData.rx_len, NULL);
			}
		}

		if ((system_get_time() - sys_time_value) >= (60 * 1000 * 1000))  //about 1min, send data to uart0, demo beat data
		{
			ESP_DBG(("uart beat data***heap_size %d\n", system_get_free_heap_size()));
			
			//uart0_write_data(uart_beat_data, sizeof(uart_beat_data));//heatbeat sent to MCU
			
			sys_time_value = system_get_time();
		}
	}

	vTaskDelete(NULL);

}



void ICACHE_FLASH_ATTR user_uart_dev_start(void)
{
	uart_init_new();   // cfg uart0 connection device MCU, cfg uart1 TX debug output
	xQueueCusUart = xQueueCreate((unsigned portBASE_TYPE)CUS_UART0_QUEUE_LENGTH, sizeof(CusUartIntrPtr));
	xTaskCreate(user_uart_task, (uint8 const *)"uart", 256, NULL, tskIDLE_PRIORITY + 2, NULL);

	return;
}
