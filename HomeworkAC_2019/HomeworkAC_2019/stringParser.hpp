#pragma once

#include "gate.h" 
#include "flipflop.h"

std::vector<size_t> positionOpen;
std::vector<size_t> positionClose;
std::vector<size_t> positionGate;
std::vector<size_t> positionSignal;

//returns position of occurrencies
std::vector<size_t> getPositions(const std::string &_inputString, const std::string &to_find) {

	std::vector <size_t> positions;
	//positions.clear();

	bool found = false;

	for (size_t i = 0; i < _inputString.length(); i++)
	{
		if (_inputString.substr(i, to_find.length()) == to_find)
		{
			found = true;
			positions.push_back(i);
		}

	}

	return positions;
}

//from a simple string, return output value

//Solve a sentence with FF
signal_output solve(std::vector <std::string> &_toSolve, const std::vector<signal_input> &_inputSignals, const std::vector<flipflop> &_flipflops) {


	std::vector <signal_input> signals = _inputSignals;
	std::vector <flipflop> flipflops = _flipflops;

	signal_input sig1;
	signal_input sig2;
	signal_output out;
	//The sentence is empty
	if (_toSolve.size() == 0) {
		std::cerr << "ERROR: can't solve the output" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	//there is NOT in the sentence
	else if (_toSolve.size()==2)
	{
		//if are already solved or are directly numbers
		if (isdigit(_toSolve[1][0]))
		{
			//cout << "right signal is bool " << _toSolve[1] << endl;
			sig1.Set(_toSolve[1]);
		}

		for (size_t i = 0; i < signals.size(); i++)
		{
			// if they are input signals
			if (signals[i].getLabel() == _toSolve[1])
			{
				//cout << "right signal is char " << signals[i].Read() << endl;
				sig1.Set(signals[i].Read());
			}

		}
		for (size_t i = 0; i < flipflops.size(); i++)
		{
			// if they are input signals
			if (flipflops[i].FF_getLabel() == _toSolve[1])
			{
				//cout << "right signal is char " << signals[i].Read() << endl;
				sig1.Set(flipflops[i].FF_Read());
			}
		}
		
		if (_toSolve[0] == "NOT") { NOT not_gate(sig1);	out.Set(not_gate.Read()); }
		else {
			std::cerr << "ERROR: can't solve the sentence" << std::endl;
			std::exit(EXIT_FAILURE);
		}
		
	}
	//there are boolean operators except NOT	
	else if (_toSolve.size() == 3)	
	{
		//if are already solved or are directly numbers
		if (isdigit(_toSolve[0][0]))
		{
			//cout << "Left signal is bool " << _toSolve[0] << endl;
			sig1.Set(_toSolve[0]);

		}

		if (isdigit(_toSolve[2][0]))
		{
			//cout << "right signal is bool " << _toSolve[2] << endl;
			sig2.Set(_toSolve[2]);
		}

		for (size_t i = 0; i < signals.size(); i++)
		{
			// if they are input signals
			if (signals[i].getLabel() == _toSolve[0])
			{
				//cout << "Left signal is char " << signals[i].Read() << endl;
				sig1.Set(signals[i].Read());
			}

			if (signals[i].getLabel() == _toSolve[2])
			{
				//cout << "right signal is char " << signals[i].Read() << endl;
				sig2.Set(signals[i].Read());
			}
		}

		for (size_t i = 0; i < flipflops.size(); i++)
		{
			// if left signal is a FF
			if (flipflops[i].FF_getLabel()==_toSolve[0])
			{
				sig1.Set(flipflops[i].FF_Read());
			}
			// if right signal is a FF
			if (flipflops[i].FF_getLabel() == _toSolve[2])
			{
				sig2.Set(flipflops[i].FF_Read());
			}
		}

		std::vector<signal_input> sig;
		sig.push_back(sig1);
		sig.push_back(sig2);
	
		if (_toSolve[1] == "AND")	{ AND and_gate(sig);	out.Set(and_gate.Read()); }
		else if (_toSolve[1] == "OR")	{ OR or_gate(sig);		out.Set(or_gate.Read()); }
		else if (_toSolve[1] == "NOR")	{ NOR nor_gate(sig);	out.Set(nor_gate.Read()); }
		else if (_toSolve[1] == "XOR")	{ XOR xor_gate(sig);	out.Set(xor_gate.Read()); }
		else if (_toSolve[1] == "NAND")	{ NAND nand_gate(sig);	out.Set(nand_gate.Read()); }
		else if (_toSolve[1] == "XNOR")	{ XNOR xnor_gate(sig);	out.Set(xnor_gate.Read()); }
		else {
			std::cerr << "ERROR: can't solve the sentence" << std::endl;
			std::exit(EXIT_FAILURE);
		}
	}
	//there is ony one signal and it's the flipflop output
	else if (_toSolve.size() == 1) {
		for (size_t i = 0; i < signals.size(); i++)
		{
			if (_toSolve[0]==signals[i].getLabel())
			{
				out.Set(signals[i].Read());
				return out;
			}
		}

		for (size_t i = 0; i < flipflops.size(); i++)
		{
			if (_toSolve[0] == flipflops[i].FF_getLabel())
			{
				out.Set(flipflops[i].FF_Read());
				return out;
			}
			 
		}
		std::cerr << "ERROR: " << _toSolve[0] << " not defined" << std::endl;
	}
	// in any other case
	else {
		std::cerr << "ERROR: can't solve the sentence" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	return out;
}
//Solve a sentence without FF
signal_output solve(std::vector <std::string> &_toSolve, const std::vector<signal_input> &_inputSignals) {


	std::vector <signal_input> signals = _inputSignals;


	signal_input sig1;
	signal_input sig2;
	signal_output out;
	//The sentence is empty
	if (_toSolve.size() == 0) {
		std::cerr << "ERROR: can't solve the output" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	//there is NOT in the sentence
	else if (_toSolve.size() == 2)
	{
		//if are already solved or are directly numbers
		if (isdigit(_toSolve[1][0]))
		{
			//cout << "right signal is bool " << _toSolve[1] << endl;
			sig1.Set(_toSolve[1]);
		}

		for (size_t i = 0; i < signals.size(); i++)
		{
			// if they are input signals
			if (signals[i].getLabel() == _toSolve[1])
			{
				//cout << "right signal is char " << signals[i].Read() << endl;
				sig1.Set(signals[i].Read());
			}

		}
		
		if (_toSolve[0] == "NOT") { NOT not_gate(sig1);	out.Set(not_gate.Read()); }
		else {
			std::cerr << "ERROR: can't solve the sentence" << std::endl;
			std::exit(EXIT_FAILURE);
		}

	}
	//there are boolean operators except NOT
	else if (_toSolve.size() == 3)
	{
		//if are already solved or are directly numbers
		if (isdigit(_toSolve[0][0]))
		{
			//cout << "Left signal is bool " << _toSolve[0] << endl;
			sig1.Set(_toSolve[0]);

		}

		if (isdigit(_toSolve[2][0]))
		{
			//cout << "right signal is bool " << _toSolve[2] << endl;
			sig2.Set(_toSolve[2]);
		}

		for (size_t i = 0; i < signals.size(); i++)
		{
			// if they are input signals
			if (signals[i].getLabel() == _toSolve[0])
			{
				//cout << "Left signal is char " << signals[i].Read() << endl;
				sig1.Set(signals[i].Read());
			}

			if (signals[i].getLabel() == _toSolve[2])
			{
				//cout << "right signal is char " << signals[i].Read() << endl;
				sig2.Set(signals[i].Read());
			}
		}		 

		std::vector<signal_input> sig;
		sig.push_back(sig1);
		sig.push_back(sig2);

		if (_toSolve[1] == "AND") { AND and_gate(sig);		out.Set(and_gate.Read());	}
		else if (_toSolve[1] == "OR")  { OR or_gate(sig);		out.Set(or_gate.Read());	}
		else if (_toSolve[1] == "NOR") { NOR nor_gate(sig);		out.Set(nor_gate.Read());	}
		else if (_toSolve[1] == "XOR") { XOR xor_gate(sig);		out.Set(xor_gate.Read());	}
		else if (_toSolve[1] == "NAND") { NAND nand_gate(sig);	out.Set(nand_gate.Read());	}
		else if (_toSolve[1] == "XNOR") { XNOR xnor_gate(sig);	out.Set(xnor_gate.Read());  }
		else {
			std::cerr << "ERROR: can't solve the sentence" << std::endl;
			std::exit(EXIT_FAILURE);
		}
	}
	else {
		std::cerr << "ERROR: can't solve the sentence" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	return out;
}


std::string getSig(const std::string & inputString, const std::vector<signal_input> &_inputSignals) {

	std::string _inputString = inputString;
	std::vector <signal_input> signals = _inputSignals;


	//get the positions of opened and closed brackets
	positionOpen = getPositions(_inputString, "(");
	positionClose = getPositions(_inputString, ")");


	size_t counterOpen = 0;
	size_t counterClose = 0;
	int flag = 0;

	//there are bracktes left
	if (positionOpen.size() != 0 && positionClose.size() != 0) {

		while (counterOpen < positionOpen.size() - 1 && flag == 0)
		{
			//find the inner brackets
			if (positionOpen[counterOpen] < positionClose[counterClose] && positionOpen[counterOpen + 1] < positionClose[counterClose])
			{
				counterOpen++;
			}
			else
			{
				flag = 1;
			}
		}

		//take the sentence inside the brackets
		std::string s = _inputString.substr(positionOpen[counterOpen] + 1, positionClose[counterClose] - positionOpen[counterOpen] - 1);
		
		std::stringstream ss(s);
		std::vector<std::string> tokens;
		std::string token;

		 //divide the string by spaces
		while(std::getline(ss, token, ' '))
		{
			tokens.push_back(token);
		}

		//solve the sentence
		signal_output t_out = solve(tokens, signals);

		std::stringstream s_out;
		s_out << t_out.Read();

		//replace the inner bracket with the resolution of the boolean operator
		_inputString.replace(positionOpen[counterOpen], positionClose[counterClose] - positionOpen[counterOpen] + 1, s_out.str());

		positionClose.erase(positionClose.begin() + counterClose);
		positionOpen.erase(positionOpen.begin() + counterOpen);

		return _inputString;
	}
	else
	{
		std::stringstream ss(_inputString);
		std::vector<std::string> tokens;
		std::string token;

		while (std::getline(ss, token, ' ')) //divide the string by space
		{
			tokens.push_back(token);
		}

		signal_output t_out = solve(tokens, signals); //solve the sentence

		std::stringstream s_out;
		s_out << t_out.Read();

		_inputString.replace(_inputString.begin(), _inputString.end(), s_out.str());

		return _inputString;
	}
}
std::string getSig(const std::string & inputString, const std::vector<signal_input> &_inputSignals, const std::vector<flipflop> &_flipflops) {

	std::string _inputString = inputString;
	std::vector <signal_input> signals = _inputSignals;

	std::vector <flipflop> flipflops = _flipflops;


	//get the positions of opened and closed brackets
	positionOpen = getPositions(_inputString, "(");
	positionClose = getPositions(_inputString, ")");

	 

	size_t counterOpen = 0;
	size_t counterClose = 0;
	int flag = 0;

	//there are bracktes left
	if (positionOpen.size() != 0 && positionClose.size() != 0) {

		while (counterOpen < positionOpen.size() - 1 && flag == 0)
		{
			//find the inner brackets
			if (positionOpen[counterOpen] < positionClose[counterClose] && positionOpen[counterOpen + 1] < positionClose[counterClose])
			{
				counterOpen++;
			}
			else
			{
				flag = 1;
			}
		}

		//take the sentence inside the brackets
		std::string s = _inputString.substr(positionOpen[counterOpen] + 1, positionClose[counterClose] - positionOpen[counterOpen] - 1);

		std::stringstream ss(s);
		std::vector<std::string> tokens;
		std::string token;

		//divide the string by spaces
		while (std::getline(ss, token, ' '))
		{
			tokens.push_back(token);
		}

		//solve the sentence
		signal_output t_out = solve(tokens, signals, flipflops);

		std::stringstream s_out;
		s_out << t_out.Read();

		//replace the inner bracket with the resolution of the boolean operator
		_inputString.replace(positionOpen[counterOpen], positionClose[counterClose] - positionOpen[counterOpen] + 1, s_out.str());

		positionClose.erase(positionClose.begin() + counterClose);
		positionOpen.erase(positionOpen.begin() + counterOpen);

		return _inputString;
	}
	else
	{
		std::stringstream ss(_inputString);
		std::vector<std::string> tokens;
		std::string token;

		while (std::getline(ss, token, ' ')) //divide the string by space
		{
			tokens.push_back(token);
		}

		signal_output t_out = solve(tokens, signals, flipflops); //solve the sentence

		std::stringstream s_out;
		s_out << t_out.Read();

		_inputString.replace(_inputString.begin(), _inputString.end(), s_out.str());

		return _inputString;
	}
}

int	getValue(const std::string & _input, const std::vector<signal_input> &_inputSignals) {

	std::vector <signal_input> signals = _inputSignals;

	std::string input = _input;
	int counter_p = getPositions(input, "(").size();
	for (int i = 0; i < counter_p + 1; i++)
	{
		input = getSig(input, signals);
		//cout << input << endl;
	}
	return stoi(input);
}
int	getValue(const std::string & _input, const std::vector<signal_input> &_inputSignals, const std::vector<flipflop> &_flipflops) {

	std::vector <signal_input> signals = _inputSignals;

	std::vector <flipflop> flipflops = _flipflops;

	std::string input = _input;
	int counter_p = getPositions(input, "(").size();

	for (int i = 0; i < counter_p + 1; i++)
	{
		input = getSig(input, signals, flipflops);
		//cout << input << endl;
	}
	return stoi(input);
}
 