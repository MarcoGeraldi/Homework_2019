#include "flipflop.h"

flipflop::flipflop(const std::string & _FF_label, const std::string & _FF_to_parse, const long int & _clkNum)
{
	this->clkNum = _clkNum;
	this->FF_to_parse = _FF_to_parse;
	this->FF_label = _FF_label;
	this->Q.Set(D.Read());
	this->_Q.Set(!D.Read());

}

flipflop::flipflop(const std::string & _FF_label, const int & _value, const long int & _clkNum)
{
	this->FF_label = _FF_label;
	this->clkNum = _clkNum;
	this->value = _value;
	this->D.Set(value);
	this->Q.Set(D.Read());
	this->_Q.Set(!D.Read());
}

std::string flipflop::FF_getParse()
{
	return this->FF_to_parse;
}

std::string flipflop::FF_getLabel()
{
	return this->FF_label;
}

 int flipflop::FF_Read()
{
	return this->Q.Read();
}

 int flipflop::notFF_Read()
 {
	 return this->_Q.Read();
 }
