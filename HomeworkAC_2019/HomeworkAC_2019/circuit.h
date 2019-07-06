#pragma once

#include <string>
#include <vector>
#include "signal.h"

class circuit
{
public:
	circuit(	const std::string &_name, 
				const std::vector<signal_input>	&_input_string, 
				const std::vector<signal_output>&_output_string
			);
 
private:
	bool isSequential;
	bool isComposed;
	long double power;

	//to_print Values
	std::string name;
	std::vector <signal_input> circuit_inputs;
	std::vector <signal_output> circuit_outputs;
	std::vector <std::string> pathMin;
	std::vector <std::string> pathMax;
	std::vector <std::string> coniLogici;


};

