#include "flipflop.h"

flipflop::flipflop(const int & _clk, const std::vector<signal_input>& _D)
{
	this->D = _D;
	this->clk = _clk;

	//The output is setted by default to -1 (X) 

	//for each clock cycle set the output Q as the input D
	//if the signal input is longer than clk the remaining part of the signal will stay NOT_DEFINED
	for (int i = 0; i <clk; i++)
	{
		Q[i].Set(D[i].Read());
	}
}

std::vector<signal_output> flipflop::Read()
{
	return this->Q;
}

signal_output flipflop::Read(const int & _index)
{
	return this->Q[_index];
}
 
