

double get_tensao_adc( int input);

double get_umidade_solo( int input){

	double v_solo = get_tensao_adc(input);
	double v_resistor = 3.25 - v_solo;
	double i_resistor  = v_resistor / 3300;
	return v_solo / i_resistor;  //Resistividade do solo.

}
