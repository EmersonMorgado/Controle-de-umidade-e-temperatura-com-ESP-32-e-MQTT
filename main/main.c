#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "esp_tls.h"
#include "mqtt_client.h"
#include "esp_log.h"
#include <driver/adc.h>
#include <stdlib.h>
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "lwip/sockets.h"
#include "driver/ledc.h"
#include <time.h>
#include <sys/_types.h>
#include <math.h>


const char *APP_VERSION = "Control Umid V0.01";
/*	0.1
 *  WiFi
 *  NVS
 *  OTA prepare
 *
 *  Server MQTT access
 *  Analog read
 *  send msg to Broker
 *
*/

#include "st_setup.h"
#include "st_mqtt.h"
#include "st_nvs.h"
#include "st_gpio.h"

// Protótipos de funções------------------------------------------------------------------------
static void set_init();

void app_main(void)
{
	set_init();
	printf("\n -------------- (( SilveTech Systems )) --------------");
	printf("\n firmware_ver:%s",APP_VERSION);
    st_init_print();
    printf("\n -----------------------------------------------------\n");
    set_mqtt_init();
    config_buzzer();
    xTaskCreate(&get_sensor, "read sensor", 8192, NULL, 5, NULL);
    while (true) {
    	//get_adc();
    	printf("\n-------------------");
    	vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}

//-------------------------------------------------------------------------------------------------------------
static void set_init(){
    nvs_flash_init();
    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    wifi_config_t sta_config = {
        .sta = {
            .bssid_set = false
        }
    };
    size_t size_key = 0;
    size_key = read_nvs_key("wifi_ssid",&sta_config.sta.ssid);
    size_key = read_nvs_key("wifi_pass",&sta_config.sta.password);
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );
}

