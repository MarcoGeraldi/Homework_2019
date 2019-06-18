#pragma once

#include "signal.h"
#include "gate.h"

#include <string>
#include <vector>  
#include <iostream>
#include <sstream>

using namespace std;

vector<size_t> positionOpen;
vector<size_t> positionClose;
vector<size_t> positionGate;
vector<size_t> positionSignal;

//returns position of occurrencies
vector<size_t> getPositions(const string &_inputString, const string &to_find) {

	vector <size_t> positions;
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
signal_output solve(const vector < string > &_toSolve) {
	vector <signal> signals;
	signal_input a("a", 1);
	signal_input b("b", 1);
	signal_input c("c", 0);
	signals.push_back(a);
	signals.push_back(b);
	signals.push_back(c);


	signal_input sig1;
	signal_input sig2;
	signal_output out;

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

	vector<signal_input> sig;
	sig.push_back(sig1);
	sig.push_back(sig2);

	if (_toSolve[1] == "AND") { AND and_gate(sig);	out.Set(and_gate.Read()); }
	if (_toSolve[1] == "OR") { OR or_gate(sig);	out.Set(or_gate.Read()); }
	/*
	std::cout << "sig1: " << sig1.Read() << std::endl
		<< "sig2: " << sig2.Read() << std::endl
		<< "out: " << out.Read() << std::endl;
	*/

	return out;
}

string getSig(const string & inputString) {

	string _inputString = inputString;

	//get the positions of opened and closed brackets
	positionOpen = getPositions(_inputString, "(");
	positionClose = getPositions(_inputString, ")");


	int counterOpen = 0;
	int counterClose = 0;
	int flag = 0;

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
		string s = _inputString.substr(positionOpen[counterOpen] + 1, positionClose[counterClose] - positionOpen[counterOpen] - 1);

		stringstream ss(s);
		vector<string> tokens;
		string token;

		while (getline(ss, token, ' ')) //divide the string by space
		{
			tokens.push_back(token);
		}

		signal_output t_out = solve(tokens); //solve the sentence

		stringstream s_out;
		s_out << t_out.Read();

		//replace the inner bracket with the resolution of the boolean operator
		_inputString.replace(positionOpen[counterOpen], positionClose[counterClose] - positionOpen[counterOpen] + 1, s_out.str());

		positionClose.erase(positionClose.begin() + counterClose);
		positionOpen.erase(positionOpen.begin() + counterOpen);


		return _inputString;
	}
	else
	{
		stringstream ss(_inputString);
		vector<string> tokens;
		string token;

		while (getline(ss, token, ' ')) //divide the string by space
		{
			tokens.push_back(token);
		}

		signal_output t_out = solve(tokens); //solve the sentence

		stringstream s_out;
		s_out << t_out.Read();

		_inputString.replace(_inputString.begin(), _inputString.end(), s_out.str());

		return _inputString;
	}
}

int	getValue(const string & _input) {

	string input = _input;
	int counter_p = getPositions(input, "(").size();

	for (size_t i = 0; i < counter_p + 1; i++)
	{
		input = getSig(input);
		//cout << input << endl;
	}

	return stoi(input);
}