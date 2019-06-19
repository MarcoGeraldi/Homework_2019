#include "gate.h"

gate::gate(){

}

bool gate::Read()
{
	return this->signal_out.Read();
}
 

AND::AND( const std::vector<signal_input>& _signal_in)
{
	this->signal_in = _signal_in;

	//Set the OUTPUT as the first input
	signal_out.Set(signal_in[0].Read());

	for (int i = 0; i < signal_in.size(); i++)
	{
		//if any of the signal is different from the first one the output should be the opposite of the first one
		if (signal_in[0].Read()!=signal_in[i].Read())
		{
			signal_out.Set(SIGNAL_LOW);
			return;
		}

	}
}

OR::OR(const std::vector<signal_input> & _signal_in)
{
	this->signal_in = _signal_in;

	//The OUTPUT is LOW by default
	signal_out.Set(SIGNAL_LOW);

	for (int i = 0; i < signal_in.size(); i++)
	{
		//if any of the signal is HIGH the output should be HIGH
		if (signal_in[i].Read()==SIGNAL_HIGH)
		{
			signal_out.Set(SIGNAL_HIGH);
			return;
		}
	}
}

NOT::NOT(const signal_input &_signal_in )
{
	this->signal_in = _signal_in;

	(signal_in.Read() == SIGNAL_HIGH) ?	this->signal_out.Set(SIGNAL_LOW) : this->signal_out.Set(SIGNAL_HIGH);
	
}

NAND::NAND(const std::vector<signal_input>& _signal_in)
{
	this->signal_in = _signal_in;

	AND AND_GATE(signal_in); 
	signal_input not_signal;
	not_signal.Set(AND_GATE.Read());
	NOT NOT_GATE(not_signal);

	this->signal_out.Set( NOT_GATE.Read());
}

//da sistemare non funziona
XOR::XOR(const std::vector<signal_input>& _signal_in)
{
	this->signal_out.Set(SIGNAL_LOW);
	for (size_t i = 0; i < _signal_in.size(); i++)
	{
		for (size_t j = 0; j < _signal_in.size(); j++)
		{
			//if any of the signal is different from another one set the output HIGH
			if (signal_in[i].Read() != signal_in[j].Read())
			{
				this->signal_out.Set(SIGNAL_HIGH);
				return;
			}
		}
	}
	return;
}

NOR::NOR(const std::vector<signal_input>& _signal_in)
{
	this->signal_in = _signal_in;

	OR OR_GATE(signal_in);
	signal_input not_signal;
	not_signal.Set(OR_GATE.Read());
	NOT NOT_GATE(not_signal);

	this->signal_out.Set(NOT_GATE.Read());
}

XNOR::XNOR(const std::vector<signal_input>& _signal_in)
{
	this->signal_in = _signal_in;

	NOR NOR_GATE(signal_in);
	signal_input not_signal;
	not_signal.Set(NOR_GATE.Read());
	NOT NOT_GATE(not_signal);

	this->signal_out.Set(NOT_GATE.Read());

}
