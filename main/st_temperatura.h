
int get_adc_crescente( int input){
	int adc = adc1_get_raw(input);
	return (4095 - adc);
}

double get_temperature(int INPUT, double ntc){
	// Parâmetros do termistor
	const double beta = 4100.0;
	//Betas  3450, 3600, 3900, 4100
	const double r0 = ntc;
	const double t0 = 273.0 + 25;
	double rx = r0 * exp(-beta/t0);

	// Parâmetros do circuito
	const double vcc = 3.240;
	const double R = 3300.0;

	// Numero de amostras na leitura
	const int nAmostras = 10;
	int soma = 0;
	for (int i = 0; i < nAmostras; i++) {
		soma += adc1_get_raw(INPUT);
	    vTaskDelay(10 / portTICK_PERIOD_MS);
	  }
	// Determina a resistência do termistor
	double v = (vcc*soma)/(nAmostras*4095.0);
	double rt = (vcc*R)/v - R;

	// Calcula a temperatura
	double t = beta / log(rt/rx);
	return t-273.0-5;
}
