/*
 * http_server.c
 *
 *  Created on: 14 wrz 2024
 *      Author: majorbien
 */

#include "http_server.h"


#include "esp_http_server.h"
#include "esp_log.h"

#include "tasks_settings.h"
#include "wifi.h"

#include <string.h> 
#include <cJSON.h> 
#include "lora.h"

char json_buffer[256]; 

static SemaphoreHandle_t json_mutex;

static const char TAG[] = "http_server";

static httpd_handle_t http_server_handle = NULL;

static TaskHandle_t task_http_server_monitor = NULL;

static QueueHandle_t http_server_monitor_queue_handle;

static esp_err_t http_server_json_handler1(httpd_req_t *req);
//static esp_err_t http_server_json_handler2(httpd_req_t *req);
//static esp_err_t http_server_json_handler3(httpd_req_t *req);




static void http_server_monitor(void *parameter)
{
	http_server_queue_message_t msg;

	for (;;)
	{
		if (xQueueReceive(http_server_monitor_queue_handle, &msg, portMAX_DELAY))
		{
			switch (msg.msgID)
			{
				case HTTP_MSG_WIFI_CONNECT_INIT:
					ESP_LOGI(TAG, "HTTP_MSG_WIFI_CONNECT_INIT");

					break;

				case HTTP_MSG_WIFI_CONNECT_SUCCESS:
					ESP_LOGI(TAG, "HTTP_MSG_WIFI_CONNECT_SUCCESS");

					break;

				case HTTP_MSG_WIFI_CONNECT_FAIL:
					ESP_LOGI(TAG, "HTTP_MSG_WIFI_CONNECT_FAIL");

					break;

				case HTTP_MSG_FIRMWARE_UPDATE_SUCCESSFUL:
					ESP_LOGI(TAG, "HTTP_MSG_OTA_UPDATE_SUCCESSFUL");

					break;

				case HTTP_MSG_FIRMWARE_UPDATE_FAILED:
					ESP_LOGI(TAG, "HTTP_MSG_OTA_UPDATE_FAILED");

					break;

				case HTTP_MSG_FIRMWARE_UPATE_INITIALIZED:
					ESP_LOGI(TAG, "HTTP_MSG_OTA_UPATE_INITIALIZED");

					break;
				case HTTP_MSG_USER_LOGIN_DONE:
					ESP_LOGI(TAG, "HTTP_MSG_USER_LOGIN_DONE");

					break;
					
				case HTTP_MGS_USER_LOGIN_FAIL:
					ESP_LOGI(TAG, "HTTP_MGS_USER_LOGIN_FAIL");

					break;
				case HTTP_MSG_USER_REGISTER_DONE:
					ESP_LOGI(TAG, "HTTP_MSG_USER_REGISTER_DONE");

					break;
				case HTTP_MGS_USER_REGISTER_FAIL:
					ESP_LOGI(TAG, "HTTP_MGS_USER_REGISTER_FAIL");

					break;
				case HTTP_MGS_FIRST_AID_KIT_STATUS:
					ESP_LOGI(TAG, "HTTP_MGS_FIRST_AID_KIT_STATUS");

					break;
				default:
					break;
			}
		}
	}
}






static httpd_handle_t http_server_configure(void)
{

	httpd_config_t config = HTTPD_DEFAULT_CONFIG();

	xTaskCreatePinnedToCore(&http_server_monitor, "http_server_monitor", HTTP_SERVER_MONITOR_STACK_SIZE, NULL, HTTP_SERVER_MONITOR_PRIORITY, &task_http_server_monitor, HTTP_SERVER_MONITOR_CORE_ID);

	http_server_monitor_queue_handle = xQueueCreate(3, sizeof(http_server_queue_message_t));

	config.core_id = HTTP_SERVER_TASK_CORE_ID;

	config.task_priority = HTTP_SERVER_TASK_PRIORITY;

	config.stack_size = HTTP_SERVER_TASK_STACK_SIZE;

	config.max_uri_handlers = 20;

	config.recv_wait_timeout = 10;
	config.send_wait_timeout = 10;

	ESP_LOGI(TAG,
			"http_server_configure: Starting server on port: '%d' with task priority: '%d'",
			config.server_port,
			config.task_priority);

	if (httpd_start(&http_server_handle, &config) == ESP_OK)
	{
		ESP_LOGI(TAG, "http_server_configure: Registering URI handlers");


		
		httpd_uri_t json_post1 = {
			.uri = "/Index/index",   
			.method = HTTP_POST,
			.handler = http_server_json_handler1,
			.user_ctx = NULL
		};
		httpd_register_uri_handler(http_server_handle, &json_post1);
	/*	
		httpd_uri_t json_post2 = {
    			.uri = "/login",
   				 .method = HTTP_POST,
   				 .handler = http_server_json_handler2,
   				 .user_ctx = NULL
		};
		httpd_register_uri_handler(http_server_handle, &json_post2);


		httpd_uri_t json_post3 = {
    			.uri = "/index",
   				 .method = HTTP_POST,
   				 .handler = http_server_json_handler3,
   				 .user_ctx = NULL
		};		

		httpd_register_uri_handler(http_server_handle, &json_post3);
*/

		return http_server_handle;
	}

	return NULL;
}


void http_server_start(void)
{
	json_mutex = xSemaphoreCreateMutex();
	if (http_server_handle == NULL)
	{
		http_server_handle = http_server_configure();
	}
}

void http_server_stop(void)
{
	if (http_server_handle)
	{
		httpd_stop(http_server_handle);
		ESP_LOGI(TAG, "http_server_stop: stopping HTTP server");
		http_server_handle = NULL;
	}
	if (task_http_server_monitor)
	{
		vTaskDelete(task_http_server_monitor);
		ESP_LOGI(TAG, "http_server_stop: stopping HTTP server monitor");
		task_http_server_monitor = NULL;
	}
}

BaseType_t http_server_monitor_send_message(http_server_message_e msgID)
{
	http_server_queue_message_t msg;
	msg.msgID = msgID;
	return xQueueSend(http_server_monitor_queue_handle, &msg, portMAX_DELAY);
}
extern char json_buffer[];  // Globalny bufor do danych JSON

static esp_err_t http_server_json_handler1(httpd_req_t *req)
{
	ESP_LOGI(TAG, "JSON data requested to send:");

	// Zabezpiecz dostęp do bufora json_buffer za pomocą mutexa
	xSemaphoreTake(json_mutex, portMAX_DELAY);

	// Sprawdź, czy w buforze JSON są dane
	if (strlen(json_buffer) > 0) {
		// Ustaw typ odpowiedzi na JSON
		httpd_resp_set_type(req, "application/json");

		// Wyślij dane JSON do klienta
		httpd_resp_send(req, json_buffer, strlen(json_buffer));
		ESP_LOGI(TAG, "Sent JSON: %s", json_buffer);
	} else {
		// Jeśli bufor jest pusty, zwróć odpowiedni komunikat
		const char *resp_str = "{\"error\": \"No data received from LoRa\"}";
		httpd_resp_send(req, resp_str, strlen(resp_str));
		ESP_LOGW(TAG, "No data in JSON buffer.");
	}

	xSemaphoreGive(json_mutex);  // Zwolnij mutex po odczycie danych

	return ESP_OK;
}