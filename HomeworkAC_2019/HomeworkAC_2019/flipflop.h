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
 
#include "signal.h"

class flipflop
{
private:
	long int clkNum;  //number of clock cycles
	signal_output	 Q; //data out
	signal_output	_Q; // !Q
	signal_input	 D; //data in

	int value;
	int clkNeeded;

	std::string FF_to_parse;
	std::string FF_label;

	std::vector<std::string> FF_found;

public:
	flipflop(const std::string & _FF_label, const std::string & _FF_to_parse,	const long int & _clkNum=0);
	flipflop(const std::string & _FF_label, const int &_value = SIGNAL_NOT_DEFINED, const long int & _clkNum = 0);

	std::string FF_getParse();
	std::string FF_getLabel();
	int FF_Read();
	int notFF_Read();

	int getClkN();
	void setClkN(const int & _need);

	std::vector <std::string> getFound();

	void calculateClk();

	void setUndefined();
 };

