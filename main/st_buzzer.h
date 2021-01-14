

typedef enum{
	LEVEL_0		= 0,
	LEVEL_1 	= 15,
	LEVEL_2		= 25,
	LEVEL_3		= 35,
	LEVEL_4		= 100,
	LEVEL_5		= 128,
}buzzer_level;

int BUZZER_DUTY_CONF = LEVEL_1;

ledc_timer_config_t buzzer_timer = {
	    .speed_mode = LEDC_HIGH_SPEED_MODE,
		.duty_resolution = LEDC_TIMER_8_BIT,
	    .timer_num  = LEDC_TIMER_0,
	    .freq_hz    = 2048,
	    .clk_cfg    = LEDC_AUTO_CLK,
	};

ledc_channel_config_t buzzer_channel = {
		.channel    = LEDC_CHANNEL_0,
		.gpio_num   = BUZZER,
	    .speed_mode = LEDC_HIGH_SPEED_MODE,
	    .timer_sel  = LEDC_TIMER_0,
	    .duty       = LEVEL_0
	};

// BUZZER ---------------------------------------------------------------------------
void config_buzzer(){
	ledc_timer_config(&buzzer_timer);
	ledc_channel_config(&buzzer_channel);
}

void set_level_buzzer(int level){
	BUZZER_DUTY_CONF = level;
}

void set_buzzer_on(){

	ledc_channel_config_t buzzer_channel = {
		    .duty = BUZZER_DUTY_CONF
		};
	ledc_channel_config(&buzzer_channel);

}

void set_buzzer_off(){

	ledc_channel_config_t buzzer_channel = {
		    .duty = LEVEL_0
		};
	ledc_channel_config(&buzzer_channel);

}
