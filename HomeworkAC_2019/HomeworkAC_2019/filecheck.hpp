#pragma once
  
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "signal.h"


std::string filename_inputSignal;
std::string filename_circuitDescr;
std::fstream stream_inputSignal;
std::fstream stream_circuitDescr;

std::vector <std::string> all_input, all_output, all_assign, all_FF, all_instance, all_name;

std::vector <std::vector<int> > vect_matrix;

std::vector <int> _positionEndmodule, _positionModule;
std::vector <int> _positionOpenBracket, _positionCloseBracket;
std::vector <int> _positionInput, _positionOutput, _positionAssign, _positionClk, _positionFF;

std::vector <bool> isSequential;

//INPUT FILE

/*
	- LEGGERE LA PRIMA RIGA E VEDERE SE E' VUOTA
	- CONTROLLARE CHE IL NUMERO DI SEGNALI SIA PARI AL NUMERO DI VALORI
	- CONTROLLARE CHE OGNI VALORE SIA 1 O 0
	- CONTROLLARE CHE IL CLK SIA PARI O MINORE DEL NUMERO DI RIGHE
*/

bool open_inputFile() {
	bool isOk = true;
	std::vector <int> vect_signal;
	std::string line;

	stream_inputSignal.open(filename_inputSignal, std::fstream::in); //open file


	//check for opening errors
	if (!stream_inputSignal.is_open()) {
		std::cerr << "ERROR: can't open " << filename_inputSignal << std::endl;
		return false;
	}
	else
	{
		//if the file is empty return an error
		if (stream_inputSignal.eof()) {
			std::cerr << "ERROR: " << filename_inputSignal << " is empty." << std::endl;
			return false;
		}
		int lineIndex = 1; //first line
		//open the file until the end
		while (!stream_inputSignal.eof()) {

			if (std::getline(stream_inputSignal, line)) {
				
				for (size_t i = 0; i < line.size(); i++) {

					//check if the line is correct
					if (line[i] == '0' || line[i] == '1') {
						
						vect_signal.push_back((int)line[i]-48); // if correct add a signal to the vector
					}

					else if (line[i] == ' '); //skip to the next character

					else {
						std::cerr << "ERROR: syntax error in " << filename_inputSignal << " at line " << lineIndex << std::endl;
						vect_signal.clear(); // remove all the elements added until now
						isOk = false;
					}
				}

				vect_matrix.push_back(vect_signal); //organize all the signal in a matrix, where a is vect_matrix [x][1]
				vect_signal.clear();
				lineIndex++;
			}
		}
		if (vect_matrix.size()>1 && isOk==true)
		{
			for (size_t i = 0; i < vect_matrix.size()-1; i++)
			{
				if (vect_matrix[i].size() != vect_matrix[i+1].size())
				{
					std::cerr << "ERROR: input's signals not defined correctly" << std::endl;
					isOk = false;
				}
			}
		}
	}
	stream_inputSignal.close();
	return isOk;
}

//CIRCUIT'S DESCRIPTION

/*
	TODO:
	CONTROLLI SUI FLIFLOP
	SALVARE FLIPFLOP
	ERRORI DI SINTASSI
	SALVARE VARIABILI:
		NOME DEL CIRCUITO 
		INPUT 
		OUTPUT
		ASSIGN STRING
		FFSTRING
		
*/

void checkString_Save(std::string &_temp, const std::string & _word_to_compare, std::vector <int> &_position, int & _index, bool _ok, std::vector <std::string> & _save_line) {

	std::string divided_string;

	if (_temp.find(_word_to_compare) != std::string::npos)//look for the output in order to use them if the file is correct
	{
		std::istringstream _stream_temp(_temp); //save the line in a stream
		_stream_temp >> divided_string; //save the sentence divided by space in a temporary value

		if (divided_string.compare(_word_to_compare) == 0) //if the word input is written correct then it saves the position
		{
			_position.push_back(_index);
			_save_line.push_back(_temp); //save the line
		}

		else
		{
			std::cerr << "ERROR: syntax error at line " << _index + 1 << std::endl;
			_ok = false;
		}
	}
}

bool check_circuitDescr() 
{
	int index = 0;
	bool isOk = true;
	std::string line, temp;

	stream_circuitDescr.open(filename_circuitDescr, std::fstream::in);

	if (!stream_circuitDescr.is_open())
	{
		std::cerr << "error while opening " << filename_circuitDescr << std::endl;
		return false;
	}

	while (!stream_circuitDescr.eof())
	{
		std::getline(stream_circuitDescr, line); //read line by line

		if (line.find("module") != std::string::npos)
		{
			if (line.substr(0, 7) == "module " || line.substr(0, 7) == "module") //the line should contain the word module 
			{
				_positionModule.push_back(index); //save the position of module found in the file
				std::string t_name;
				int m_tPos = line.find("module");
				int p_tPos = line.find("(");
				t_name = line.substr(m_tPos+7 , p_tPos - (m_tPos+7));//save the name after module and before the open bracket

				all_name.push_back(t_name); //save the name
			}
		}
		if (line.find("endmodule") != std::string::npos)
		{
			if (line.substr(0, 10) == "endmodule " || line.substr(0, 10) == "endmodule")
			{
				_positionEndmodule.push_back(index); //save the position of the endmodule found in the file
			}
			else
			{
				isOk = false;
				std::cerr << "ERROR: syntax error at line " << index + 1 << std::endl;
			}
		}

		checkString_Save(line, "input", _positionInput, index, isOk, all_input);
		checkString_Save(line, "output", _positionOutput, index, isOk, all_output);
		checkString_Save(line, "assign", _positionAssign, index, isOk, all_assign);

		if (line.find(")") != std::string::npos && line.find(";") != std::string::npos)
		{
			_positionCloseBracket.push_back(index);
		}
		if (line.find("(") != std::string::npos && line.find("module") != std::string::npos)
		{		
			_positionOpenBracket.push_back(index);
		}
		if (line.find("FF") != std::string::npos && line.find("assign") == std::string::npos)//look for a flipflop
		{
			std::istringstream _stream_temp(line); //save the line in a stream
			std::string _flipFlop;

			_stream_temp >> temp; //save the first word in a temporary value that should be FF number
			_flipFlop = temp;

			if (_flipFlop.size()>=3)
			{
				for (size_t j = 0; j < _flipFlop.size() - 2; j++)
				{
					if (_flipFlop[0] == 'F' && _flipFlop[1] == 'F' && isdigit(_flipFlop[j + 2]) != 0)
						//the first and the second words have to be F and then just numbers
					{
						_positionFF.push_back(index);
						all_FF.push_back(line);
						isOk = true;
					}
					else
					{
						isOk = false;
						std::cerr << "ERROR: syntax error at line " << index + 1 << std::endl;
					}
				}
			}
			else
			{
				isOk = false;
				std::cerr << "ERROR: syntax error at line " << index + 1 << std::endl;
			}
		}

		if (line.find("clk") != std::string::npos) //look for the word clock 
		{

			std::istringstream _stream_temp(line); //save the line in a stream
			_stream_temp >> temp; //save the sentence divided by space in a temporary value

			if (temp.compare("clk") == 0) //if the word input is written correct then it saves the position
			{
				_positionClk.push_back(index);
			}

			else
			{
				std::cerr << "ERROR: syntax error at line " << index + 1 << std::endl;
				isOk = false;
			}
		}
		index++;
	}
	stream_circuitDescr.close();

	if (_positionEndmodule.size() == 0) //there are no endmodule
	{
		std::cerr << "ERROR: no endmodule found" << std::endl;
		isOk= false;
	}

	if (_positionModule.size() == 0) //no module found
	{
		std::cerr << "ERROR: no module found" << std::endl;
		isOk= false;
	}
	
	if (_positionEndmodule.size()!=_positionModule.size()) //module and endmodule are not matching
	{
		std::cerr << "ERROR: there are more module than endmodule or viceversa" << std::endl;
		isOk= false;
	}
	
	if (_positionCloseBracket.size() == 0) //there are no closed brackets
	{
		std::cerr << "ERROR: no closed brackets found" << std::endl;
		isOk= false;
	}
	 
	if (_positionOpenBracket.size() == 0)//no opened brackets found
	{
		 std::cerr << "ERROR: no opened brackets found" << std::endl;
		 isOk= false;
	}
	
	if (_positionCloseBracket.size()!= _positionOpenBracket.size()) //the brackets are not matching
	{
		std::cerr << "ERROR: there aren't as many opened brackets as closed brackets" << std::endl;
		isOk= false;
	}
	
	if (_positionAssign.size()==0) //no assign found
	{
		std::cerr << "ERROR: no assign found" << std::endl;
		isOk= false;
	}
	
	if (_positionInput.size() == 0) //no input found
	{
		std::cerr << "ERROR: no input's signals found" << std::endl;
		isOk= false;
	}
	
	if (_positionOutput.size() == 0) //no output found
	{
		std::cerr << "ERROR: no output's signals found" << std::endl;
		isOk= false;
	}

	if (isOk==true) //until now there are no error of sintax
	{
		isSequential.resize(_positionEndmodule.size(), false); //it has to be as big as many circuits are in the file

		for (size_t i = 0; i < _positionModule.size(); i++) 
		{
			if (_positionModule[i]>_positionEndmodule[i]  ) //module has to be before endmodule
			{
				std::cerr	<< "ERROR: module was found after endmodule or viceversa in the circuit number: " 
							<< i+1
							<< std::endl;
				isOk= false;
			}
			
			if (_positionOpenBracket[i]>_positionCloseBracket[i] ) //opened bracket before the closed one
			{
				std::cerr	<< "ERROR: the bracket aren't in the right place in the circuit number: " 
							<< i + 1
							<< std::endl;
				isOk= false;
			}
			
			if (_positionClk.size() > 0) //if there a clock
			{

				int innerCounter = 0;

				for (size_t counterClk = 0; counterClk < _positionClk.size(); counterClk++)
				{
					if (_positionClk[counterClk] > _positionModule[i] && _positionClk[counterClk]<_positionEndmodule[i]) //for each circuit
					{
						innerCounter++;

						if (innerCounter>1) //just one clock in a circuit
						{
							std::cerr << "ERROR: too many clock defined in the circuit number " << i + 1 << std::endl;
							isOk = false;
						}

						else
						{
							isSequential[i] = true;
						}
					}
				}
			}

			for (size_t counterInput=0		; counterInput		<	_positionInput.size();		counterInput++)
			{
				if (_positionInput[counterInput]>_positionModule[i] &&_positionInput[counterInput]<_positionEndmodule[i]) //for each circuit
				{
					if (_positionInput[counterInput]<_positionOpenBracket[i] || _positionInput[counterInput]>_positionCloseBracket[i])
					//control that input's signals are written inside the brackets
					{
						std::cerr	<< "ERROR: the input's signals are not inside the brackets in the circuit number: "
									<< i + 1	
									<< std::endl;
						isOk=false;
					}
				}
			}

			for (size_t counterOutput=0	; counterOutput		<	 _positionOutput.size();	counterOutput++)
			{
				if (_positionOutput[counterOutput]>_positionModule[i] &&_positionOutput[counterOutput] < _positionEndmodule[i]) //for each circuit
				{
					if (_positionOutput[counterOutput]<_positionOpenBracket[i] || _positionOutput[counterOutput]>_positionCloseBracket[i])
								//control that output's signals are written inside the brackets
					{
						std::cerr	<< "ERROR: the output's signals are not inside the brackets in the circuit number: "
									<< i+1
									<< std::endl;
						isOk=false;
					}
				}
			}
			
			for (size_t counterAssign=0	; counterAssign		<	 _positionAssign.size();	counterAssign++)
			{
				if (_positionAssign[counterAssign] > _positionModule[i] &&_positionAssign[counterAssign] < _positionEndmodule[i])//for each circuit
				{
					if (_positionAssign[counterAssign] < _positionCloseBracket[i])
						//control that the word assign is written after the closed bracket but before the endmodule
					{
						std::cerr << "ERROR: the assign is not in the right place in the circuit number: "
							<< i + 1
							<< std::endl;
						isOk = false;
					}
				}
			}
			for (size_t counterFF = 0; counterFF < _positionFF.size(); counterFF++)
			{
				if (_positionFF[counterFF] > _positionModule[i] && _positionFF[counterFF] < _positionEndmodule[i])//for each circuit
				{
					if (_positionFF[counterFF] < _positionCloseBracket[i])
						//control that the fliflop is written after the closed bracket but before the endmodule
					{
						std::cerr << "ERROR: the flipflop is not in the right place in the circuit number: "
							<< i + 1
							<< std::endl;
						isOk = false;
					}
				}
			}
		}	
		 
	}
	return isOk;
}

bool checkInput(std::vector <std::string> & _to_check) {

	for (size_t i = 0; i < _to_check.size(); i++)
	{
		for (size_t j = 0; j <_to_check[i].length(); j++)//for each string that contains input
		{
			//input can contains just letters, numbers, commas and square brackets
			if (isdigit(_to_check[i][j])==0 && isalpha(_to_check[i][j])==0 && _to_check[i][j]!=',' && _to_check[i][j]!='[' && _to_check[i][j]!=']' && _to_check[i][j]!=' ' && _to_check[i][j]!='\t')
			{
				std::cerr << "ERROR: the input is not formatted correctly" << std::endl;
				 
				return false;
			}
		}
	}
	
	for (size_t i = 0; i < _to_check.size(); i++)
	{		
		std::string inputLine;
		int position = _to_check[i].find("input");
		inputLine = _to_check[i].substr(position + 5, _to_check[i].size());
		std::stringstream inputLine_stream(inputLine);
		std::vector<std::string> inputs;
		std::string token;
		//divide the string by commas
		while (getline(inputLine_stream, token, ','))
		{
			inputs.push_back(token);
		}

		//check each input
		for (size_t k = 0; k < inputs.size(); k++)
		{
			if (inputs[k] == " " || inputs[k] == "\t"  )
			{
				std::cerr << "ERROR: input missing" << std::endl;
				return false;
			}

			//if there is a space or tab in the input definition giva an error
			std::vector < int >  t_pos ;		
			for (size_t l = 0; l < inputs[k].length(); l++)
			{
				if (isalpha(inputs[k][l]))
				{ 
					t_pos.push_back(l);
					if (t_pos.size()>1 && (t_pos[t_pos.size()-1] - t_pos[t_pos.size()-2])>1)
					{
						std::cerr << "ERROR: syntax error " << std::endl;
						return false;
					}
				}
			}
			t_pos.clear();


			//if the argument are not corrected
			if (inputs[k].find("[") != std::string::npos && inputs[k].find("]") != std::string::npos)
			{
				std::string checkString;
				int positionSquare_open = inputs[k].find("[");
				int positionSquare_close = inputs[k].find("]");

				//check square parenthesis order
				if (positionSquare_close<positionSquare_open)
				{
					std::cerr << "ERROR: check input array parenthesis" << std::endl;
					return false;
				}
				//check the inner part of square brackets
				checkString = inputs[k].substr(positionSquare_open+1, positionSquare_close-positionSquare_open-1);
				if (checkString.size() > 0)
				{
					for (size_t j = 0; j < checkString.size(); j++)
					{
						if (isdigit(checkString[j]) == 0)
						{
							std::cerr << "ERROR 3: syntax error in an input line" << std::endl;
							return false;
						}
					}
				}
				else
				{
					std::cerr << "ERROR: empty brackets" << std::endl;
					return false;
				}
			}
		}	
	}
	return true;
}

bool checkOutput(std::vector <std::string> & _to_check) 
{

	for (size_t i = 0; i < _to_check.size(); i++)
	{

		for (size_t j = 0; j < _to_check[i].length(); j++)//for each string that contains output
		{
			//output can just contain letters and commas 
			if (isalpha(_to_check[i][j]) == 0 && _to_check[i][j] != ',' &&  _to_check[i][j] != ' ' && _to_check[i][j] != '\t')
			{
				std::cerr << "ERROR: the output is not formatted correctly" << std::endl;
				return false;
			}
			
		}

		for (size_t i = 0; i < _to_check.size(); i++)
		{
			std::string outputLine;
			int position = _to_check[i].find("output");
			outputLine = _to_check[i].substr(position + 6, _to_check[i].size());
			std::stringstream outputLine_stream(outputLine);
			std::vector<std::string> outputs;
			std::string token;
			//divide the string by commas
			while (getline(outputLine_stream, token, ','))
			{
				outputs.push_back(token);
			}

			//check each output
			for (size_t k = 0; k < outputs.size(); k++)
			{
				if (outputs[k] == " " || outputs[k] == "\t")
				{
					std::cerr << "ERROR: output missing" << std::endl;
					return false;
				}

				//if there is a space or tab in the output definition giva an error
				std::vector < int >  t_pos;
				for (size_t l = 0; l < outputs[k].length(); l++)
				{
					if (isalpha(outputs[k][l]))
					{
						t_pos.push_back(l);
						if (t_pos.size() > 1 && (t_pos[t_pos.size() - 1] - t_pos[t_pos.size() - 2]) > 1)
						{
							std::cerr << "ERROR: syntax error " << std::endl;
							return false;
						}
					}
				}
				t_pos.clear();

			}
		}
	}
	return true;
}

bool checkAssign(std::vector <std::string> & _to_check) {
	for (size_t i = 0; i < _to_check.size(); i++)
	{
		for (size_t j = 0; j < _to_check[i].length(); j++)//for each string that contains input
		{
			//input can contains just letters, numbers, commas and square brackets
			if (isalpha(_to_check[i][j]) == 0 &&
				_to_check[i][j] != '(' && 
				_to_check[i][j] != ')' && 
				_to_check[i][j] != ' ' && 
				_to_check[i][j] != '\t' && 
				_to_check[i][j]!='=' && 
				isdigit(_to_check[i][j]) == 0 &&
				_to_check[i][j] != '[' &&
				_to_check[i][j] != ']')
			{
				std::cerr << "ERROR: syntax error at line " << _positionAssign[i]+1 << std::endl;
				return false;
			}
		}
	}
	return true;
}

bool checkFF(std::vector <std::string> &_to_check) {

	for (size_t i = 0; i < _to_check.size(); i++)
	{
		for (size_t j = 0; j < _to_check[i].length(); j++)//for each string that contains input
		{
			//input can contains just letters, number, commas and square brackets
			if (isalpha(_to_check[i][j]) == 0 && _to_check[i][j] != '(' && _to_check[i][j] != ')' &&
				_to_check[i][j] != ' ' && _to_check[i][j] != '\t' && _to_check[i][j] != '=' && isdigit(_to_check[i][j])==0 &&
				_to_check[i][j] != '[' && _to_check[i][j] != ']')
			{
				std::cerr << "ERROR: syntax error at line " << _positionFF[i] + 1 << std::endl;
				return false;
			}
		}
	}
	for (size_t j = 0; j < _positionModule.size(); j++)
	{
		for (size_t i = 0; i < _positionFF.size(); i++)
		{
			if (_positionFF[i] > _positionModule[j] && _positionFF[i] < _positionEndmodule[j])//for each circuit that contains a FF
			{
				if (isSequential[j] == false)
				{
					std::cerr << "ERROR: clock not defined in the circuit named: " << all_name[j] << std::endl;
					return false;
				}
			}
		}
	}
	return true;
}

bool checkSignals() {
	if (
		check_circuitDescr() &&
		checkInput(all_input) &&
		checkOutput(all_output) &&
		checkAssign(all_assign) &&
		checkFF(all_FF)
		)	return true;

	else return false;
	
}
