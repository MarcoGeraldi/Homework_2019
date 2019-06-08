#include "signal.h"

int signal::Read()
{
	return this->value;
}

std::string signal::getLabel()
{
	return label;
}

void signal::Set(const bool &_value)
{
	this->value = _value;
}

void signal::Set(const std::string & _value)
{ 
	std::istringstream s_value(_value);
	int b_value;
	s_value >> b_value;
	value >> b_value;
}

void signal::operator=(const signal & to_assign)
{
	this->value = to_assign.value;
}

 

signal_output::signal_output(const std::string & _label, const bool & _value)
{
	this->label = _label;
	this->value = _value;
	this->kind = SIGNAL_OUTPUT;
}

signal_output::signal_output()
{
	this->label = 'X';
	this->value = SIGNAL_NOT_DEFINED;
	this->kind = SIGNAL_OUTPUT;
	
}

signal_input::signal_input(const std::string & _label, const bool & _value)
{
	this->label = _label;
	this->value = _value;
	this->kind = SIGNAL_INPUT;
}

signal_input::signal_input()
{
	this->label = 'X';
	this->value = SIGNAL_NOT_DEFINED;
	this->kind = SIGNAL_INPUT;
}

signal operator!(const signal & to_invert)
{
	signal inverted;
	inverted.value = !to_invert.value;
	return inverted;
}
