#include "flipflop.h"

flipflop::flipflop(const std::string & _FF_label, const std::string & _FF_to_parse, const long int & _clkNum)
{
	this->clkNum = _clkNum;
	this->FF_to_parse = _FF_to_parse;
	this->FF_label = _FF_label;
}

std::string flipflop::FF_getParse()
{
	return this->FF_to_parse;
}

std::string flipflop::FF_getLabel()
{
	return this->FF_label;
}

std::vector<signal_output> flipflop::FF_Read()
{
	return this->Q;
}
