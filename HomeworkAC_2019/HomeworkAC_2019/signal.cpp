#include "signal.h"

bool signal::Read()
{
	return this->value;
}

void signal::Set(const bool &_value)
{
	this->value = _value;
}

output_signal::output_signal(const char & _label, const bool & _value)
{
	this->label = _label;
	this->value = _value;
	this->kind = SIGNAL_OUTPUT;
}

input_signal::input_signal(const char & _label, const bool & _value)
{
	this->label = _label;
	this->value = _value;
	this->kind = SIGNAL_INPUT;
}
