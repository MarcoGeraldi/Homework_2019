#pragma once

//This class emulates the behavior of a D-TYPE FlipFlop

/*
			 ______________
			|			   |
	------	|	D		Q  | ------
			|			   |
	------	|	CLK		   |
			|______________|
					   
*/
 
#include "signal.h";
class flipflop
{
private:
	long int clkNum;  //number of clock cycles
	std::vector<signal_output>	 Q; //data out
	std::vector<signal_input>	 D; //data in

	std::string FF_to_parse;
	std::string FF_label;
public:
	flipflop(const std::string & _FF_label, const std::string & _FF_to_parse,	const long int & _clkNum=0);

	std::string FF_getParse();
	std::string FF_getLabel();
	std::vector <signal_output> FF_Read();
 };

