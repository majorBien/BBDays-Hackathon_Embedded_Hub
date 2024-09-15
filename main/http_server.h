/*
 * http_server.h
 *
 *  Created on: 14 wrz 2024
 *      Author: majorbien
 */


#ifndef MAIN_HTTP_SERVER_H_
#define MAIN_HTTP_SERVER_H_


extern char json_buffer[];


typedef enum http_server_message
{
	HTTP_MSG_WIFI_CONNECT_INIT = 0,
	HTTP_MSG_WIFI_CONNECT_SUCCESS,
	HTTP_MSG_WIFI_CONNECT_FAIL,
	HTTP_MSG_FIRMWARE_UPDATE_SUCCESSFUL,
	HTTP_MSG_FIRMWARE_UPDATE_FAILED,
	HTTP_MSG_FIRMWARE_UPATE_INITIALIZED,
	HTTP_MSG_USER_LOGIN_DONE,
	HTTP_MGS_USER_LOGIN_FAIL,
	HTTP_MSG_USER_REGISTER_DONE,
	HTTP_MGS_USER_REGISTER_FAIL,
	HTTP_MGS_FIRST_AID_KIT_STATUS
} http_server_message_e;


typedef struct http_server_queue_message
{
	http_server_message_e msgID;
} http_server_queue_message_t;



void http_server_start(void);

void http_server_stop(void);

#endif /* MAIN_HTTP_SERVER_H_ */
