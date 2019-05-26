#include "signal.h"

bool signal::Read()
{
	return this->value;
}

void signal::Set(const bool &_value)
{
	this->value = _value;
}

signal_output::signal_output(const char & _label, const bool & _value)
{
	this->label = _label;
	this->value = _value;
	this->kind = SIGNAL_OUTPUT;
}

signal_output::signal_output()
{
	this->kind = SIGNAL_OUTPUT;
	this->label = 'X';
	this->value = SIGNAL_LOW;
}

signal_input::signal_input(const char & _label, const bool & _value)
{
	this->label = _label;
	this->value = _value;
	this->kind = SIGNAL_INPUT;
}

signal_input::signal_input()
{
	this->kind = SIGNAL_INPUT;
	this->label = 'X';
	this->value = SIGNAL_LOW;
}
