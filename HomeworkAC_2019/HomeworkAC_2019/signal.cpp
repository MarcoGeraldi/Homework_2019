#include "signal.h"

int signal::Read()
{
	return this->value;
}

std::string signal::getLabel()
{
	return label;
}

void signal::Set(const int &_value)
{
	this->value = _value;
}

void signal::Set(const std::string & _value)
{ 
	std::istringstream s_value(_value);
	int b_value;
	s_value >> b_value;
	this->value = b_value;

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

signal_output::signal_output(const std::string & _label, const std::string & _to_parse, const bool & _value)
{
	this->kind = SIGNAL_OUTPUT;
	this->value = _value;
	this->label = _label;
	this->to_parse = _to_parse;
	 
	FF_inside();
}


signal_output::signal_output()
{
	this->label = 'X';
	this->value = SIGNAL_NOT_DEFINED;
	this->kind = SIGNAL_OUTPUT;
	
}

std::string signal_output::getParse()
{
	return this->to_parse;
}

int signal_output::getValue()
{
	return value;
}

void signal_output::setParse(const std::string & parse)
{
	this->to_parse = parse;
	FF_inside();
}

void signal_output::FF_inside()
{
	std::string token;
	std::string to_find = { "FF" };
	std::string cut;

	std::string use = to_parse;

	for (size_t i = 0; i < use.length(); i++)
	{
		if (to_parse.substr(i, to_find.length()) == "FF")
		{
			cut = use.substr(i, use.size() - i);
			std::stringstream s(cut);
			int pos_space = cut.find(" ");
			int pos_bracket = cut.find(")");

			//the first found was space
			if (pos_space > 0 && pos_bracket > 0)
			{
				if (pos_space < pos_bracket)
				{
					std::getline(s, token, ' ');
					FF_found.push_back(token);
				}
				else
				{
					std::getline(s, token, ')');
					FF_found.push_back(token);
				}
			}
			else if (pos_space > 0 && pos_bracket < 0)
			{
				std::getline(s, token, ' ');
				FF_found.push_back(token);
			}
			else if (pos_space < 0 && pos_bracket>0)
			{
				std::getline(s, token, ')');
				FF_found.push_back(token);
			}
			else if (pos_bracket < 0 && pos_space < 0)
			{
				FF_found.push_back(cut);
			}
		}

	}
}

std::vector<std::string> signal_output::get_FF()
{
	return FF_found;
}

void signal_output::setClk(const int & _max)
{
	this->clkNeed = _max;
}

int signal_output::getClk()
{
	return clkNeed;
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
