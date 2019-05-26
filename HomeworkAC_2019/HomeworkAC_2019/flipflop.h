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
#include <vector>


class flipflop
{
private:
	int clk;  //number of clock cycles
	std::vector<signal_output>	 Q; //data out
	std::vector<signal_input>	 D; //data in
public:
	flipflop(const int & _clk, const std::vector<signal_input> &_D);//constructor

	std::vector<signal_output> Read(); //returns the output signal for each clock cycle
	signal_output Read(const int &_index); //returns the output for one specific clock cycle

 };

