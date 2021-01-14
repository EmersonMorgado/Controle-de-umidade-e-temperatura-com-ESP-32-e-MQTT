

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void check_wifi_connection(void *pvParameter)
{
	while(1){
		ESP_ERROR_CHECK( esp_wifi_connect() );
		vTaskDelay(30000 / portTICK_PERIOD_MS);
	}
}




