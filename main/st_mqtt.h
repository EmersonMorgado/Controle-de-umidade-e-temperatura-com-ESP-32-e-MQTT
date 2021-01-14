#include <sys/_stdint.h>


int MQTT_CONNECT = 0;
int SYSTEMA_FIRST_START = 0;

char* ALARM_ACCOUNT = "0001";
char* TOPIC_PUB = "topico0";
char* TOPIC_SUB = "topico1";
char* ST_TAG_MQTT = "MQTT_SYS";

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event);
void report_system(char* MSG, char* TOPIC);

const esp_mqtt_client_config_t mqtt_cfg = {
			.uri = "mqtt://mqtt.emersonmorgado.com.br:1883",
			.transport = MQTT_TRANSPORT_OVER_TCP,

		};
esp_mqtt_client_handle_t client;

static void set_mqtt_init()
{
	client = esp_mqtt_client_init(&mqtt_cfg);
	esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
	esp_mqtt_client_start(client);
}

void report_system(char* MSG, char* TOPIC)
{
	//printf("%s \n",MSG);
	if (MQTT_CONNECT){
		esp_mqtt_client_publish(client, TOPIC, MSG, 0, 1, 0);
	}
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;

    char MSG[100];
	strcpy(MSG,"0001");
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            MQTT_CONNECT = 1;
        	if (SYSTEMA_FIRST_START){
        		strcat(MSG,"System reconnected.");
        		report_system(MSG,TOPIC_PUB);
        	}else{
        		SYSTEMA_FIRST_START = 1;
        		strcat(MSG,"Started system , firmware: ");
        		strcat(MSG, APP_VERSION);
        		report_system(MSG,TOPIC_PUB);
        	}
        	ESP_LOGI(ST_TAG_MQTT, "MQTT_EVENT_CONNECTED");
            esp_mqtt_client_subscribe(client, TOPIC_SUB, 0);

            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(ST_TAG_MQTT, "MQTT_EVENT_DISCONNECTED");
            MQTT_CONNECT = 0;
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(ST_TAG_MQTT, "CONTROLLER SUBSCRIBED, msg_id=%d", event->msg_id);
            strcat(MSG,"System subscribed.");
            report_system(MSG,TOPIC_PUB);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(ST_TAG_MQTT, "CONTROLLER UNSUBSCRIBED, msg_id=%d", event->msg_id);
            strcat(MSG,"System unsubscribed.");
            report_system(MSG,TOPIC_PUB);
            break;
        case MQTT_EVENT_PUBLISHED:
            //ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(ST_TAG_MQTT, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            char* MSG_[50];
            strncpy((char*)MSG_, event->data, event->data_len);
            printf("MSG=%s\n",(char*)MSG);
            //remote_command((char*)MSG_, event->data_len, event->client);

            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(ST_TAG_MQTT, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_ESP_TLS) {
                ESP_LOGI(ST_TAG_MQTT, "Last error code reported from esp-tls: 0x%x", event->error_handle->esp_tls_last_esp_err);
                ESP_LOGI(ST_TAG_MQTT, "Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err);
            } else if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED) {
                ESP_LOGI(ST_TAG_MQTT, "Connection refused error: 0x%x", event->error_handle->connect_return_code);
            } else {
                ESP_LOGW(ST_TAG_MQTT, "Unknown error type: 0x%x", event->error_handle->error_type);
            }
            break;
        default:
            ESP_LOGI(ST_TAG_MQTT, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(ST_TAG_MQTT, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

