//HomeworkAC_2019

// Marco Geraldi	<248825>
// Margherita Gili	<249774>

#include "HomeworkAC_2019.h"
#include "signal.h"
#include "gate.h"
#include "filecheck.hpp"

#include <string>
#include <vector>  
#include <iostream>
#include <sstream>
#include <algorithm>
 
using namespace std;

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
	signal_input b("b", 0);
	signal_input c("c", 1);
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
		sig1.Set(_toSolve[2]);
	}

	for (size_t i = 0; i<signals.size() ; i++)
	{
		// if they are input signals
		if (signals[i].getLabel()==_toSolve[0])
		{
			//cout << "Left signal is char " << signals[i].Read() << endl;
			sig1.Set(signals[i].Read());
		}
		
		if (signals[i].getLabel()== _toSolve[2])
		{
			//cout << "right signal is char " << signals[i].Read() << endl;
			sig2.Set(signals[i].Read());
		}

		
	}

	vector<signal_input> sig;
	sig.push_back(sig1);
	sig.push_back(sig2);

	if (_toSolve[1] == "AND") { AND and_gate(sig);	out.Set(and_gate.Read()); }
	
	return out;
}

string getSig(  const string & inputString) {
	string _inputString = inputString;
	int counter = 0;
	vector<size_t> positionOpen;
	vector<size_t> positionClose;
	vector<size_t> positionGate;
	vector<size_t> positionSignal;
	

	positionOpen = getPositions(_inputString, "(");
	positionClose = getPositions(_inputString, ")");


		//go to the most inner parentesi
		size_t i, j;
		for (i = 0, j = 0; (positionOpen[i] < positionClose[j]) && (positionOpen.size() > i + 1); i++)
		{
			;
		}

		cout << i << endl;

		//get the inner part 
		string s = _inputString.substr(positionOpen[i] + 1, positionClose[j] - positionOpen[i] - 1);
		stringstream ss(s);
		vector<string> tokens;
		string token;

		while (getline(ss, token, ' '))
		{
			tokens.push_back(token);
		}

		signal_output t_out = solve(tokens);

		stringstream s_out;
		s_out << t_out.Read();
		_inputString.replace(positionOpen[i], positionClose[j] - positionOpen[i] + 1, s_out.str());



	
		j++;
	return _inputString;
	
}

int main(){ 
	/*
	string input = "(a AND b) AND (d AND c)";
	input=getSig(input);
	cout << input << endl;
	input = getSig(input);
	cout << input << endl; 
	*/
	filename_circuitDescr = "descrizione.txt";
	check_circuitDescr();

	return 0;
}



 