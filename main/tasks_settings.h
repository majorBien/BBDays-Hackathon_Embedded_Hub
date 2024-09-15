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
#define BLUETOOTH_TASK_STACK_SIZE			4096
#define BLUETOOTH_TASK_PRIORITY				4
#define BLUETOOTH_TASK_CORE_ID				1


#endif /* MAIN_TASKS_SETTINGS_H_ */
