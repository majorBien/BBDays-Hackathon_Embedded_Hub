/*
 * task_setting.h
 *
 *  Created on: 14 wrz 2024
 *      Author: majorbien
 */
 
#ifndef MAIN_TASKS_SETTINGS_H_
#define MAIN_TASKS_SETTINGS_H_


// LoraWAN Send task
#define LORA_SEND_STACK_SIZE				4096
#define LORA_SEND_PRIORITY					5
#define LORA_SEND_CORE_ID					0

//LoraWAN Receive task
#define LORA_RECEIVE_STACK_SIZE				4086
#define LORA_RECEIVE_PRIORITY				6
#define LORA_RECEIVE_CORE_ID				0	

// WiFi application task
#define WIFI_APP_TASK_STACK_SIZE			4096
#define WIFI_APP_TASK_PRIORITY				4
#define WIFI_APP_TASK_CORE_ID				1

// HTTP Server task
#define HTTP_SERVER_TASK_STACK_SIZE			8192
#define HTTP_SERVER_TASK_PRIORITY			3
#define HTTP_SERVER_TASK_CORE_ID			1

// HTTP Server Monitor task
#define HTTP_SERVER_MONITOR_STACK_SIZE		4096
#define HTTP_SERVER_MONITOR_PRIORITY		2
#define HTTP_SERVER_MONITOR_CORE_ID			1


#endif /* MAIN_TASKS_SETTINGS_H_ */
