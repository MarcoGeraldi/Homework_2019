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
			signal_out.Set(!signal_in[0].Read());
			return;
		}
	}
}

OR::OR(const std::vector<signal_input> & _signal_in)
{
	this->signal_in = _signal_in;

	//The OUTPUT is LOW by default

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

NOT::NOT(const signal_input & _signal_in_not)
{
	this->signal_in_not = _signal_in_not;
	// simply invert the input signal
	this->signal_out.Set(!signal_in_not.Read());
}