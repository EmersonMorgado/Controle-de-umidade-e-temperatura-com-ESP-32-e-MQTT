

// GPIOS pins-------------------------------------------------------

#define  INPUT_E1  	ADC1_CHANNEL_6
#define  INPUT_E2	ADC1_CHANNEL_7
#define  INPUT_E3	ADC1_CHANNEL_0
#define  INPUT_E4	ADC1_CHANNEL_3

#define  OUTPUT_K1 	32
#define  OUTPUT_K2 	33
#define  OUTPUT_K3 	16
#define  OUTPUT_K4 	12
#define  LED_STATUS 2
#define  BUZZER 	13

#include "st_buzzer.h"
#include "st_temperatura.h"
#include "st_umidade.h"

void setup_gpio(void)
{
	gpio_set_direction(OUTPUT_K1, GPIO_MODE_OUTPUT);
	gpio_set_direction(OUTPUT_K2, GPIO_MODE_OUTPUT);
	gpio_set_direction(OUTPUT_K3, GPIO_MODE_OUTPUT);
	gpio_set_direction(OUTPUT_K4, GPIO_MODE_OUTPUT);
	gpio_set_direction(LED_STATUS, GPIO_MODE_OUTPUT);

	adc1_config_channel_atten(INPUT_E1,ADC_ATTEN_DB_11);
	adc1_config_channel_atten(INPUT_E2,ADC_ATTEN_DB_11);
	adc1_config_channel_atten(INPUT_E3,ADC_ATTEN_DB_11);
	adc1_config_channel_atten(INPUT_E4,ADC_ATTEN_DB_11);

	config_buzzer();
}

double get_tensao_adc( int input){
	int adc = adc1_get_raw(input);
	double tensao = 3.3;
	double resolucao = (tensao/4095.0);
	return tensao - (resolucao*adc);
}

void get_adc(){

	int adc1 = adc1_get_raw(INPUT_E1);
	printf("\n Sensor1 %i ", adc1);
	double tensao = 3.24;
	double resolucao = (tensao/4095.0);
	double tensao_medida = tensao - (resolucao*adc1);
	printf("\n Tensao saida E1 %.2f", tensao_medida);

	int adc2 = adc1_get_raw(INPUT_E2);
	printf("\n Sensor2 %i ", adc2);
	tensao_medida = tensao - (resolucao*adc2);
	printf("\n Tensao saida E2 %2f", tensao_medida);

	int adc3 = adc1_get_raw(INPUT_E3);
	printf("\n Sensor3 %i ", adc3);
	tensao_medida = tensao - (resolucao*adc3);
	printf("\n Tensao saida E3 %2f", tensao_medida);

	int adc4 = adc1_get_raw(INPUT_E4);
	printf("\n Sensor4 %i ", adc4);
	tensao_medida = tensao - (resolucao*adc4);
	printf("\n Tensao saida E4 %2f", tensao_medida);
}

void get_sensor(void *pvParameter){
	while(true){
		double sensors[4];
		sensors[0] = get_temperature(INPUT_E1, 10000.0);
		sensors[1] = get_temperature(INPUT_E2, 10000.0);
		sensors[2] = get_umidade_solo(INPUT_E3);
		sensors[3] = get_umidade_solo(INPUT_E4);
		char* msg[100];
		char* topic[100];
		printf("\n ---------------------------------");

		sprintf((char*)msg, "%.1f", sensors[0]);
		sprintf((char*)topic, "/E1/t_in");
		report_system((char*)msg,topic);
		printf("\n Sensor t in : %.1f C", sensors[0]);

		sprintf((char*)msg, "%.1f", sensors[1]);
		sprintf((char*)topic, "/E2/t_out");
		report_system((char*)msg,topic);
		printf("\n Sensor t out : %.1f C", sensors[1]);

		sprintf((char*)msg, "%.0f", sensors[2]);
		sprintf((char*)topic, "/E3");
		report_system((char*)msg,topic);
		printf("\n Sensor E3 : %.0f ohm", sensors[2]);

		sprintf((char*)msg, "%.0f", sensors[3]);
		sprintf((char*)topic, "/E4");
		report_system((char*)msg,topic);
		printf("\n Sensor E4 : %.0f ohm", sensors[3]);

		vTaskDelay(120000 / portTICK_PERIOD_MS);
		//vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}






