/*
* Copyright (c) 2014-2015 Alibaba Group. All rights reserved.
*
* Alibaba Group retains all right, title and interest (including all
* intellectual property rights) in and to this computer program, which is
* protected by applicable intellectual property laws.  Unless you have
* obtained a separate written license from Alibaba Group., you are not
* authorized to utilize all or a part of this computer program for any
* purpose (including reproduction, distribution, modification, and
* compilation into object code), and you must immediately destroy or
* return to Alibaba Group all copies of this computer program.  If you
* are licensed by Alibaba Group, your rights to utilize this computer
* program are limited by the terms of that license.  To obtain a license,
* please contact Alibaba Group.
*
* This computer program contains trade secrets owned by Alibaba Group.
* and, unless unauthorized by Alibaba Group in writing, you agree to
* maintain the confidentiality of this computer program and related
* information and to not disclose this computer program and related
* information to any other person or entity.
*
* THIS COMPUTER PROGRAM IS PROVIDED AS IS WITHOUT ANY WARRANTIES, AND
* Alibaba Group EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED,
* INCLUDING THE WARRANTIES OF MERCHANTIBILITY, FITNESS FOR A PARTICULAR
* PURPOSE, TITLE, AND NONINFRINGEMENT.
*/
#include "c_types.h"
#include "alink_export.h"
#include "alink_json.h"
#include <stdio.h>
#include <string.h>
#include "alink_export_rawdata.h"
#include "esp_common.h"
#include "user_config.h"
#include "user_uart.h" // user uart handler head

#define wsf_deb  os_printf
#define wsf_err os_printf
#define PASS_THROUGH 
//#define SUB_DEV_ENABLE // 子设备功能sample 未实现
/* 设备信息：根据网页注册信息导出的电子表格更新对应信息 */
/* device info */
#define DEV_NAME "P440"
#define DEV_CATEGORY "LIVING"
#define DEV_TYPE "AIRPURIFIER"
#ifdef PASS_THROUGH
#define DEV_MODEL "HEKR_LIVING_AIRPURIFIER_P440"
#define ALINK_KEY "EGuPnDiE9Kuk5yPWBStv"
#define ALINK_SECRET "gw9KbFDj4TpZmlgGac8oc5UJljCEX4vxWrD2t1Vc"
#else
#define DEV_MODEL "HEKR_LIVING_AIRPURIFIER_P440_TEST"
#define ALINK_KEY "EkIOEjZ5BL4STlvNscbk"
#define ALINK_SECRET "5UqkiNBIdEpZ1Q2t6bjPMlneXxCxHxsdar71r2ac"
#endif
#define DEV_MANUFACTURE "HEKR"
/*sandbox key/secret*/
#define ALINK_KEY_SANDBOX "uf9dEc7i2LJBHxw91cgg"
#define ALINK_SECRET_SANDBOX "yjj6rpEzWeMxmzpfOXgIDqc4EhoOoAxjrVDfyWcB"
/*设备硬件信息:系统上电后读取的硬件/固件信息,此处为演示需要,直接定义为宏.产品对接时,需要调用自身接口获取*/
#define DEV_SN "1234567890"
#define DEV_VERSION "1.0.0;APP2.0;OTA1.0"
#define DEV_MAC "19:FE:34:A2:C7:1A"	//"AA:CC:CC:CA:CA:01" // need get from device
#define DEV_CHIPID "3D0044000F47333139373030"	// need get from device
/*alink-sdk 信息 */

extern void alink_sleep(int);
/*do your job here*/
/*这里是一个虚拟的设备,将5个设备属性对应的值保存到全局变量,真实的设备需要去按照实际业务处理这些属性值 */

P440_DEV virtual_device;
u8 cloud_connect_status = 0x02;

char device_status_change = 1;

int sample_running = ALINK_TRUE;

/* 根据不同系统打印剩余内存,用于平台调试 */
int ICACHE_FLASH_ATTR print_mem_callback(void *a, void *b)
{
	int ret = 0;
	ret = system_get_free_heap_size();
	os_printf("heap_size %d\n", ret);
	return ret;
}

#ifdef PASS_THROUGH
/* device response server command,用户需要自己实现这个函数,处理服务器下发的指令*/
/* this sample save cmd value to virtual_device*/
static int ICACHE_FLASH_ATTR execute_cmd(const char *rawdata, int len)
{
	int ret = 0, i = 0;

	if (len < LUA_DATA_EXTENT_LEN || len > CUS_UART_TX_MAX ) return -1;
	if(rawdata[0] != LUA_DATA_HEADER || rawdata[len-1] != LUA_DATA_END)return -1;
	
	//for debug
	for (i = 0; i < len; i++) {
		wsf_deb("%2x ", rawdata[i]);
	}
	wsf_deb("\n");

	for (i = 0; i < len; i++) {
		switch (i) {
		case LUA_CMD_POWER_ONOFF_BIT:
			if (virtual_device.OnOff_Power != rawdata[i] && rawdata[i] != 0x00) {
		
				cus_wifi_data_handler(CUS_CMD_POWER_ONOFF, rawdata[i]);
			}
			break;
		case LUA_CMD_IONS_ONOFF_BIT:
			if (virtual_device.OnOff_Ions != rawdata[i] && rawdata[i] != 0x00) {
				
				cus_wifi_data_handler(CUS_CMD_IONS_ONOFF, rawdata[i]);
			}
			break;
		case LUA_CMD_WORKMODE_BIT:
			if (virtual_device.WorkMode != rawdata[i] && rawdata[i] != 0x00) {
				
				cus_wifi_data_handler(CUS_CMD_WORKMODE, rawdata[i]);

			}
			break;
		case LUA_CMD_SPEED_SET_BIT:
			if (virtual_device.Ventilation_Speed != rawdata[i] && rawdata[i] != 0x00) {
				
				cus_wifi_data_handler(CUS_CMD_SPEED_SET, rawdata[i]);
			}
			break;
		case LUA_CMD_TIMER_ON_BIT:
			if (virtual_device.TimeMeter_PowerOn != rawdata[i] && rawdata[i] != 0x00) {
				
				cus_wifi_data_handler(CUS_CMD_TIMER, rawdata[i]);
			}
			break;
		case LUA_CMD_TIMER_OFF_BIT:
			if (virtual_device.TimeMeter_PowerOff != rawdata[i] && rawdata[i] != 0x00) {
				//virtual_device.TimeMeter_PowerOff = rawdata[i];
			}
			break;
		case LUA_CMD_AIR_ONOFF_BIT:
			if (virtual_device.OnOff_AirQuality != rawdata[i] && rawdata[i] != 0x00) {
				
				cus_wifi_data_handler(CUS_CMD_AIR_ONOFF, rawdata[i]);
			}
			break;
		default:
			break;
		}
	}
	

	return ret;
}

/*获取设备信息,需要用户实现 */
static int ICACHE_FLASH_ATTR get_device_status(char *rawdata, int len)
{
	/* do your job here */

	uint8_t datalen = LUA_CUS_DATA_LEN + LUA_DATA_END_LEN + LUA_DATA_LEN_LEN;

	rawdata[0] = LUA_DATA_HEADER;
	rawdata[1] = datalen;
	rawdata[LUA_CMD_POWER_ONOFF_BIT] = virtual_device.OnOff_Power;
	rawdata[LUA_CMD_IONS_ONOFF_BIT] = virtual_device.OnOff_Ions;
	rawdata[LUA_CMD_AIRQUALITY_BIT] = virtual_device.Status_AirQuality;
	rawdata[LUA_CMD_WORKMODE_BIT] = virtual_device.WorkMode;
	rawdata[LUA_CMD_SPEED_SET_BIT] = virtual_device.Ventilation_Speed;

	rawdata[LUA_CMD_TIMER_ON_BIT] = virtual_device.TimeMeter_PowerOn;
	rawdata[LUA_CMD_TIMER_OFF_BIT] = virtual_device.TimeMeter_PowerOff;

	rawdata[LUA_CMD_FILTER_LIFE_BIT] = virtual_device.LifeTime_Filter;///

	rawdata[LUA_CMD_ERROR_BIT] = virtual_device.ErrorCode;
	rawdata[LUA_CMD_AIR_ONOFF_BIT] = virtual_device.OnOff_AirQuality;

	rawdata[datalen] = LUA_DATA_END;
	//for debug
	uint8_t i;
	for(i=0;i<len;i++)
		wsf_deb("%2x ", rawdata[i]);
	wsf_deb("\n ");

	/* do your job end */
	return 1;
}

static unsigned int delta_time = 0;

/*主动上报设备状态,需要用户自己实现*/
int ICACHE_FLASH_ATTR alink_device_post_raw_data(void)
{
	/* do your job here */
	uint8_t len = LUA_CUS_DATA_LEN + LUA_DATA_EXTENT_LEN;
	int ret = 0;
	
	char rawdata[LUA_CUS_DATA_LEN + LUA_DATA_EXTENT_LEN] = { 0 };
	if (device_status_change) {
		wsf_deb("[%s][%d|  Available memory:%d.\n", __FILE__, __LINE__, system_get_free_heap_size());

		delta_time = system_get_time() - delta_time;
		wsf_deb("%s %d \n delta_time = %d \n", __FUNCTION__, __LINE__, delta_time / 1000);
		get_device_status(rawdata, len);

		ret = alink_post_device_rawdata(rawdata, len);
		device_status_change = 0;

		if (ret) {
			wsf_err("post failed!\n");
		}
		else {
			wsf_deb("dev post raw data success!\n");
		}
		
	}
	/* do your job end */
	return ret;
}

/*透传方式服务器查询指令回调函数*/

int ICACHE_FLASH_ATTR rawdata_get_callback(const char *in_rawdata, int in_len, char *out_rawdata, int *out_len)
{
	//TODO: 下发指令到MCU
	int ret = 0;
	wsf_deb("%s %d \n", __FUNCTION__, __LINE__);
	//ret=alink_device_post_raw_data(); //  此例是假设能同步获取到虚拟设备数据, 实际应用中,处理服务器指令是异步方式,需要厂家处理完毕后主动上报一次设备状态
	
	cus_wifi_data_handler(CUS_CMD_POST,0x00);
	
	// do your job end!

	return ret;
}

/*透传方式服务器下发指令回调函数*/
/*实际应用中,处理服务器指令是异步方式,需要厂家处理完毕后主动上报一次设备状态*/
int ICACHE_FLASH_ATTR rawdata_set_callback(char *rawdata, int len)
{
	// TODO: 
	//get cmd from server, do your job here!
	int ret = 0;
	wsf_deb("%s %d \n", __FUNCTION__, __LINE__);
	ret = execute_cmd(rawdata, len);
	
	delta_time = system_get_time();
	// do your job end!

	return ret;
}

#else

char *device_attr[DEV_ATTRI_NUM] = { "OnOff_Power", "OnOff_Ions","Status_AirQuality",
"WorkMode","Ventilation_Speed","TimeMeter_PowerOn", "TimeMeter_PowerOff",
"LifeTime_Filter","ErrorCode", "OnOff_AirQuality" };   // this is a demo json package data, real device need to update this package
													   /*
													   const char *main_dev_params =
													   "{\"OnOff_Power\": { \"value\": \"%d\" }, \"OnOff_Ions\": { \"value\": \"%d\" }, \"OnOff_Ozone\": { \"value\": \"%d\" },\"OnOff_Disinfection_UltraViolet\": { \"value\": \"%d\"}, \"Status_FilterMesh\": { \"value\": \"%d\"}, \"Status_AirQuality\": { \"value\": \"%d\" }, \"AirQuality_PM25\": { \"value\": \"%d\" }, \"WorkMode\": { \"value\": \"%d\" },\"Ventilation_Speed\": { \"value\": \"%d\" }, \"Ventilation_Direction_UpDown\": { \"value\": \"%d\" }, \"Temperature_Now\": { \"value\": \"%d\" },\"Humidity_Now\": { \"value\": \"%d\" }, \"Humidity_Target\": { \"value\": \"%d\" }, \"Time_Now\": { \"value\": \"%d\" }, \"AlarmClock_PowerOn\": { \"value\": \"%d\" }, \"AlarmClock_PowerOff\": { \"value\": \"%d\" }, \"TimeMeter_PowerOn\": { \"value\": \"%d\" }, \"TimeMeter_PowerOff\": { \"value\": \"%d\" }, \"Duration_Runtime\": { \"value\": \"%d\" }, \"LifeTime_Filter\": { \"value\": \"%d\" }, \"Status_TVOC\": { \"value\": \"%d\" }, \"ErrorCode\": { \"value\": \"%d\" }}";
													   */

const char *main_dev_params =
"{\"OnOff_Power\": { \"value\": \"%d\" }, \"OnOff_Ions\": { \"value\": \"%d\" }, \"Status_AirQuality\": { \"value\": \"%d\" },\"WorkMode\": { \"value\": \"%d\"}, \"Ventilation_Speed\": { \"value\": \"%d\"},\"TimeMeter_PowerOn\": { \"value\": \"%d\"}, \"TimeMeter_PowerOff\": { \"value\": \"%d\"},\"LifeTime_Filter\": { \"value\": \"%d\"}, \"ErrorCode\": { \"value\": \"%d\"}, \"OnOff_AirQuality\": { \"value\": \"%d\"}}";


/*设备上报数据,需要客户根据具体业务去实现*/
#define buffer_size (512)
static int ICACHE_FLASH_ATTR alink_device_post_data(alink_down_cmd_ptr down_cmd)
{
	alink_up_cmd up_cmd;
	int ret = ALINK_ERR;
	//char buffer[1024];
	char *buffer = NULL;
	//      static int count=0;
	if (device_status_change) {

		wsf_deb("##[%s][%s|%d]Malloc %u. Available memory:%d.\n", __FILE__, __FUNCTION__, __LINE__,
			buffer_size, system_get_free_heap_size());

		buffer = (char *)malloc(buffer_size);
		if (buffer == NULL) {
			wsf_deb("##[%s][%s|%d]Malloc error!!\n", __FILE__, __FUNCTION__, __LINE__);
			return -1;
		}

		memset(buffer, 0, buffer_size);

		sprintf(buffer, main_dev_params, virtual_device.OnOff_Power,
			virtual_device.OnOff_Ions, virtual_device.Status_AirQuality, virtual_device.WorkMode,
			virtual_device.Ventilation_Speed, virtual_device.TimeMeter_PowerOn,
			virtual_device.TimeMeter_PowerOff, virtual_device.LifeTime_Filter,
			virtual_device.ErrorCode, virtual_device.OnOff_AirQuality);

		printf(buffer);//////////////

		up_cmd.param = buffer;
		if (down_cmd != NULL) {
			up_cmd.target = down_cmd->account;
			up_cmd.resp_id = down_cmd->id;
		}
		else {
			up_cmd.target = NULL;
			up_cmd.resp_id = -1;
		}
		ret = alink_post_device_data(&up_cmd);

		if (ret == ALINK_ERR) {
			wsf_err("post failed!\n");
			alink_sleep(2000);
		}
		else {
			wsf_deb("dev post data success!\n");
			device_status_change = 0;
		}

		if (buffer)
			free(buffer);
		wsf_deb("##[%s][%s][%d]  Free |Aviable Memory|%5d| \n", __FILE__, __FUNCTION__, __LINE__,
			system_get_free_heap_size());

		stack_free_size();
	}
	return ret;

}

/* do your job end */

/*get json cmd from server 服务器下发命令,需要设备端根据具体业务设定去解析处理*/
int ICACHE_FLASH_ATTR main_dev_set_device_status_callback(alink_down_cmd_ptr down_cmd)
{

	json_value *jptr;
	json_value *jstr;
	json_value *jstr_value;
	int value = 0, i = 0;
	char *value_str = NULL;

	wsf_deb("%s %d \n", __FUNCTION__, __LINE__);
	wsf_deb("%s %d\n%s\n", down_cmd->uuid, down_cmd->method, down_cmd->param);

	jptr = json_parse(down_cmd->param, strlen(down_cmd->param));

	for (i = 0; i < DEV_ATTRI_NUM; i++)
	{
		jstr = json_object_object_get_e(jptr, device_attr[i]);
		jstr_value = json_object_object_get_e(jstr, "value");
		value_str = json_object_to_json_string_e(jstr_value);

		if (value_str) {
			value = atoi(value_str);
			cus_wifi_handler_alinkdata2mcu(i, value);

			ESP_DBG(("OnOff_Power:0x%X,OnOff_Ions:0x%X,WorkMode:0x%X,Ventilation_Speed:0x%X,LifeTime_Filter:0x%X", virtual_device.OnOff_Power, \
				virtual_device.OnOff_Ions, virtual_device.WorkMode, virtual_device.Ventilation_Speed, virtual_device.LifeTime_Filter));
		}
	}


	for (i = 0; i < DEV_ATTRI_NUM; i++)
	{
		jstr = json_object_object_get_e(jptr, device_attr[i]);
		jstr_value = json_object_object_get_e(jstr, "value");
		value_str = json_object_to_json_string_e(jstr_value);
		if (value_str) {
			value = atoi(value_str);
			switch (i) {
			case 0:
				if (virtual_device.OnOff_Power != value) {
					virtual_device.OnOff_Power = value;
				}
				break;
			case 1:
				if (virtual_device.OnOff_Ions != value) {
					virtual_device.OnOff_Ions = value;
				}
				break;
			case 2:
				if (virtual_device.Status_AirQuality != value) {
					virtual_device.Status_AirQuality = value;
				}
				break;
			case 3:
				if (virtual_device.WorkMode != value) {
					virtual_device.WorkMode = value;
				}
				break;
			case 4:
				if (virtual_device.Ventilation_Speed != value) {
					virtual_device.Ventilation_Speed = value;
				}
				break;
			case 5:
				if (virtual_device.TimeMeter_PowerOn != value) {
					virtual_device.TimeMeter_PowerOn = value;
				}
				break;
			case 6:
				if (virtual_device.TimeMeter_PowerOff != value) {
					virtual_device.TimeMeter_PowerOff = value;
				}
				break;
			case 7:
				if (virtual_device.LifeTime_Filter != value) {
					virtual_device.LifeTime_Filter = value;
				}
				break;
			case 8:
				if (virtual_device.ErrorCode != value) {
					virtual_device.ErrorCode = value;
				}
				break;
			case 9:
				if (virtual_device.OnOff_AirQuality != value) {
					virtual_device.OnOff_AirQuality = value;
				}
				break;
			default:
				break;
			}
		}
	}

	json_value_free(jptr);
	device_status_change = 1;   // event send current real device status to the alink server

	return 0;		// alink_device_post_data(down_cmd);
					/* do your job end! */
}

/*服务器查询设备状态,需要设备上报状态*/
int ICACHE_FLASH_ATTR main_dev_get_device_status_callback(alink_down_cmd_ptr down_cmd)
{
	wsf_deb("%s %d \n", __FUNCTION__, __LINE__);
	wsf_deb("%s %d\n%s\n", down_cmd->uuid, down_cmd->method, down_cmd->param);
	device_status_change = 1;

	return 0;		//alink_device_post_data(down_cmd);
}

#endif //PASS_THROUGH

/*alink-sdk 状态查询回调函数*/
int ICACHE_FLASH_ATTR alink_handler_systemstates_callback(void *dev_mac, void *sys_state)
{
	char uuid[33] = { 0 };
	char *mac = (char *)dev_mac;
	enum ALINK_STATUS *state = (enum ALINK_STATUS *)sys_state;
	switch (*state) {
	case ALINK_STATUS_INITED:
		break;
	case ALINK_STATUS_REGISTERED:
		sprintf(uuid, "%s", alink_get_uuid(NULL));
		wsf_deb("ALINK_STATUS_REGISTERED, mac %s uuid %s \n", mac, uuid);
		break;
	case ALINK_STATUS_LOGGED:
		sprintf(uuid, "%s", alink_get_uuid(NULL));
		wsf_deb("ALINK_STATUS_LOGGED, mac %s uuid %s\n", mac, uuid);
		break;
	default:
		break;
	}
	return 0;
}

/* fill device info 这里设备信息需要修改对应宏定义,其中DEV_MAC和DEV_CHIPID 需要用户自己实现接口函数*/
void ICACHE_FLASH_ATTR alink_fill_deviceinfo(struct device_info *deviceinfo)
{
	uint8 macaddr[6];
	//fill main device info here
	strcpy(deviceinfo->name, DEV_NAME);
	strcpy(deviceinfo->sn, DEV_SN);
	strcpy(deviceinfo->key, ALINK_KEY);
	strcpy(deviceinfo->model, DEV_MODEL);
	strcpy(deviceinfo->secret, ALINK_SECRET);
	strcpy(deviceinfo->type, DEV_TYPE);
	strcpy(deviceinfo->version, DEV_VERSION);
	strcpy(deviceinfo->category, DEV_CATEGORY);
	strcpy(deviceinfo->manufacturer, DEV_MANUFACTURE);
	strcpy(deviceinfo->key_sandbox, ALINK_KEY_SANDBOX);
	strcpy(deviceinfo->secret_sandbox, ALINK_SECRET_SANDBOX);

	if (wifi_get_macaddr(0, macaddr)) {
		wsf_deb("macaddr=%02x:%02x:%02x:%02x:%02x:%02x\n", MAC2STR(macaddr));
		snprintf(deviceinfo->mac, sizeof(deviceinfo->mac), "%02x:%02x:%02x:%02x:%02x:%02x", MAC2STR(macaddr));
	}
	else
		strcpy(deviceinfo->mac, DEV_MAC);
	//	if ((macaddr[4] == 0xc7) && (macaddr[5] == 0x18))	// the mac  18:fe:34:a2:c7:18   binding CHIPID  "3D0044000F47333139373030" 
	//	{
	//		strcpy(deviceinfo->cid, DEV_CHIPID);
	//	} else {
	snprintf(deviceinfo->cid, sizeof(deviceinfo->cid), "%024d", system_get_chip_id());
	//	}
	wsf_deb("DEV_MODEL:%s \n", DEV_MODEL);
}


//#define GET_ALIGN_STRING_LEN(str)    ((strlen(str) + 3) & ~3)
/*ALINK_CONFIG_LEN 2048, len need < ALINK_CONFIG_LEN */
static int ICACHE_FLASH_ATTR write_config(unsigned char *buffer, unsigned int len)
{
	int res = 0, pos = 0;

	if (buffer == NULL) {
		return ALINK_ERR;
	}
	if (len > ALINK_CONFIG_LEN)
		len = ALINK_CONFIG_LEN;

	res = spi_flash_erase_sector(LFILE_START_ADDR / 4096);	//one sector is 4KB 
	if (res != SPI_FLASH_RESULT_OK) {
		wsf_err("erase flash data fail\n");
	}
	else {
		wsf_err("erase flash data %d Byte\n", res);
	}
	os_printf("write data:\n");


	res = spi_flash_write((LFILE_START_ADDR), (uint32 *)buffer, len);
	if (res != SPI_FLASH_RESULT_OK) {
		wsf_err("write data error\n");
		return ALINK_ERR;
	}
	wsf_deb("write key(%s) success.", buffer);
	return ALINK_OK;
}

/*ALINK_CONFIG_LEN 2048, len need < ALINK_CONFIG_LEN */
static int ICACHE_FLASH_ATTR read_config(unsigned char *buffer, unsigned int len)
{

	int res = 0;
	int pos = 0;
	res = spi_flash_read(LFILE_START_ADDR, (uint32 *)buffer, len);
	if (res != SPI_FLASH_RESULT_OK) {
		wsf_err("read flash data error\n");
		return ALINK_ERR;
	}
	os_printf("read data:\n");
	return ALINK_OK;
}

int ICACHE_FLASH_ATTR alink_get_debuginfo(info_type type, char *status)
{
	int used;
	switch (type) {
	case MEMUSED:
		used = 100 - ((system_get_free_heap_size() * 100) / (96 * 1024));
		sprintf(status, "%d%%", used);
		break;
	case WIFISTRENGTH:
		sprintf(status, "%ddB", wifi_station_get_rssi());
		break;
	default:
		status[0] = '\0';
		break;
	}
	return 0;
}
int esp_ota_firmware_update(char * buffer, int len)
{
	os_printf("esp_ota_firmware_update \n");
	return upgrade_download(buffer, len);
}

int esp_ota_upgrade(void)
{
	os_printf("esp_ota_upgrade \n");
	system_upgrade_reboot();
	return 0;
}
extern int need_notify_app;
extern int  need_factory_reset;
int ICACHE_FLASH_ATTR alink_demo()
{
	struct device_info main_dev;

	memset(&main_dev, 0, sizeof(main_dev));
	alink_fill_deviceinfo(&main_dev);	// 必须根据PRD表格更新设备信息
	//alink_set_loglevel(ALINK_LL_DEBUG | ALINK_LL_INFO | ALINK_LL_WARN | ALINK_LL_ERROR);
	alink_set_loglevel(ALINK_LL_ERROR);
	//alink_enable_sandbox_mode();                                      // 线上环境需要注解此函数
	main_dev.sys_callback[ALINK_FUNC_SERVER_STATUS] = alink_handler_systemstates_callback;
	alink_set_callback(ALINK_FUNC_AVAILABLE_MEMORY, print_mem_callback);

	/* ALINK_CONFIG_LEN 2048 */
	alink_register_cb(ALINK_FUNC_READ_CONFIG, (void *)&read_config);
	alink_register_cb(ALINK_FUNC_WRITE_CONFIG, (void *)&write_config);
	alink_register_cb(ALINK_FUNC_GET_STATUS, alink_get_debuginfo);
	//alink_enable_sandbox(&main_dev);                                      // 线上环境需要注解此函数
	alink_register_cb(ALINK_FUNC_OTA_FIRMWARE_SAVE, esp_ota_firmware_update);
	alink_register_cb(ALINK_FUNC_OTA_UPGRADE, esp_ota_upgrade);
	/*start alink-sdk */
#ifdef PASS_THROUGH		//透传方式(设备与服务器采用raw data通讯)
	alink_start_rawdata(&main_dev, rawdata_get_callback, rawdata_set_callback);
#else // 非透传方式(设备与服务器采用json格式数据通讯)
	main_dev.dev_callback[ACB_GET_DEVICE_STATUS] = main_dev_get_device_status_callback;
	main_dev.dev_callback[ACB_SET_DEVICE_STATUS] = main_dev_set_device_status_callback;

	alink_start(&main_dev);	//register main device here
#endif //PASS_THROUGH

	os_printf("%s %d wait time=%d \n", __FUNCTION__, __LINE__, ALINK_WAIT_FOREVER);

	ESP_DBG(("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"));
	
	if (ALINK_OK == alink_wait_connect(NULL, ALINK_WAIT_FOREVER))	//wait main device login, -1 means wait forever
	{
		cloud_connect_status = 0x01;
		//char send_buf_alink_connOK[] = { 0x31, 0x31, 0x31, 0x31 }; // demo data to tell uart mcu dev, alink conn success
		//uart0_write_data(send_buf_alink_connOK, sizeof(send_buf_alink_connOK));
	}
	else
	{
		cloud_connect_status = 0x02;
		//char send_buf_alink_connFailed[] = { 0x32, 0x32, 0x32, 0x32 }; // demo data to tell uart mcu dev, alink conn success
		//uart0_write_data(send_buf_alink_connFailed, sizeof(send_buf_alink_connFailed));
	}
	if (need_notify_app) {
		need_notify_app = 0;
		uint8 macaddr[6];
		char mac[17 + 1];
		if (wifi_get_macaddr(0, macaddr)) {
			os_printf("macaddr=%02x:%02x:%02x:%02x:%02x:%02x\n", MAC2STR(macaddr));
			snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x", MAC2STR(macaddr));
			zconfig_notify_app(DEV_MODEL, mac, ""); // if not factory reset , 
		}
	}
	//printf("%s %d \n",__FUNCTION__,__LINE__);

	//printf("alink_demo heap_size %d\n",system_get_free_heap_size());
	//system_print_meminfo();


	/* 设备主动上报数据 */
	while (sample_running) {

		//os_printf("%s %d \n",__FUNCTION__,__LINE__);
#ifdef PASS_THROUGH
		alink_device_post_raw_data();
#else
		alink_device_post_data(NULL);
#endif //PASS_THROUGH

		if (need_factory_reset) {
			wsf_deb("key to factory_reset\n");
			need_factory_reset = 0;
			alink_factory_reset();
		}
		alink_sleep(1000);
	}

	/*  设备退出alink-sdk */
	alink_end();

	return 0;
}
