#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include "signal.h"

std::string filename_inputSignal;
std::string filename_circuitDescr;
std::fstream stream_inputSignal;
std::fstream stream_circuitDescr;

std::vector <std::string> _description;

std::vector < std::vector <signal_input>> matrix_input;
std::vector < std::vector <signal_output>> matrix_output;
std::vector <std::string> all_input, all_output, all_assign, all_FF;

std::vector <int> _positionEndmodule, _positionModule;
std::vector <int> _positionOpenBracket, _positionCloseBracket;
std::vector <int> _positionInput, _positionOutput, _positionAssign, _positionClk, _positionFF;

bool isSequential = false; // to check if the circuit is sequential

//INPUT FILE

/*
	- LEGGERE LA PRIMA RIGA E VEDERE SE E' VUOTA
	- CONTROLLARE CHE IL NUMERO DI SEGNALI SIA PARI AL NUMERO DI VALORI
	- CONTROLLARE CHE OGNI VALORE SIA 1 O 0
	- CONTROLLARE CHE IL CLK SIA PARI O MINORE DEL NUMERO DI RIGHE
*/

bool open_inputFile() {
	bool isOk = true;
	std::vector <bool> vect_signal;
	std::vector <std::vector<bool> > vect_matrix;
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

		//open the file until the end
		while (!stream_inputSignal.eof()) {

			if (std::getline(stream_inputSignal, line)) {

				for (size_t i = 0; i < line.size(); i++) {

					//check if the line is correct
					if (line[i] == '0' || line[i] == '1') {
						vect_signal.push_back(bool(line[i])); // if correct add a signal to the vector
					}

					else if (line[i] == ' '); //skip to the next character

					else {
						std::cerr << "ERROR: syntax error in " << filename_inputSignal << " , line " << i << std::endl;
						vect_signal.clear(); // remove all the elements added until now
						isOk = false;
					}
				}

				vect_matrix.push_back(vect_signal); //organize all the signal in a matrix, where a is vect_matrix [x][1]

				//if the number of elements of each vector is different from the previous one give an error
				if (vect_matrix.size() > 0 && (vect_matrix[vect_matrix.size()] != vect_matrix[vect_matrix.size() - 1])) {
					vect_matrix.clear(); // remove all the elements added until now
					std::cerr << "ERROR: signal vectors are not definited correctly in " << filename_inputSignal << std::endl;
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
	SALVARE VARIABILI
	AMARE LA PROPRIA PAPERINA
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
		if (line.find("FF") != std::string::npos)//look for a flipflop
		{
			std::istringstream _stream_temp(line); //save the line in a stream
			std::string _flipFlop;

			_stream_temp >> temp; //save the first word in a temporary value that should be FF number
			_flipFlop = temp;

			for (int j = 0; j < _flipFlop.length() - 2; j++)
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

	//for (int i = 0; i < _description.size(); i++)
	//{
	//	if (_description[i].substr(0, 7) == "module " || _description[i].substr(0, 7) == "module") //the line should contain the word module 
	//	{
	//		_positionModule.push_back(i); //save the position of module found in the file
	//	}

	//	if (_description[i].substr(0, 10) == "endmodule " || _description[i].substr(0, 10) == "endmodule")
	//	{
	//		_positionEndmodule.push_back(i); //save the position of the endmodule found in the file
	//	}

	//	if (_description[i].find("input") != std::string::npos) //look for the word input 
	//	{
	//		std::istringstream _stream_temp(_description[i]); //save the line in a stream
	//		std::vector <std::string> _checkInput;

	//		while (_stream_temp >> temp) //save the sentence divided by space in a temporary value
	//		{
	//			_checkInput.push_back(temp);
	//		}

	//		//da rifare
	//		int j = 0;
	//		for (int i = 0; i < _checkInput.size(); i++)
	//		{
	//			if (_checkInput[0] == "input" && isalpha(_checkInput[2 * i + 1][j]) != 0 && _checkInput[2 * i] == ","&& isalpha(_checkInput[_checkInput.size()][j]) != 0)
	//				//if the word input is written correct and also the input's signals are correct then it saves the position
	//			{
	//				_positionInput.push_back(i);
	//			}

	//			else
	//			{
	//				std::cerr << "ERROR: syntax error at line " << i + 1 << std::endl;
	//				isOk = false;
	//			}
	//		}
	//	}

	//	if (_description[i].find("output") != std::string::npos)//look for the output in order to use them if the file is correct
	//	{
	//		std::istringstream _stream_temp(_description[i]); //save the line in a stream
	//		_stream_temp >> temp; //save the sentence divided by space in a temporary value

	//		if (temp.compare("output") == 0) //if the word input is written correct then it saves the position
	//		{
	//			_positionOutput.push_back(i);
	//		}

	//		else
	//		{
	//			std::cerr << "ERROR: syntax error at line " << i + 1 << std::endl;
	//			isOk = false;
	//		}
	//	}

	//	if (_description[i].find("assign") != std::string::npos) //look for the word assing
	//	{
	//		std::istringstream _stream_temp(_description[i]); //save the line in a stream
	//		_stream_temp >> temp; //save the sentence divided by space in a temporary value

	//		if (temp.compare("assign") == 0) //if the word input is written correct then it saves the position
	//		{
	//			_positionAssign.push_back(i);
	//		}

	//		else
	//		{
	//			std::cerr << "ERROR: syntax error at line " << i + 1 << std::endl;
	//			isOk = false;
	//		}
	//	}

	//	if (_description[i].find("clk") != std::string::npos) //look for the word clock 
	//	{

	//		std::istringstream _stream_temp(_description[i]); //save the line in a stream
	//		_stream_temp >> temp; //save the sentence divided by space in a temporary value

	//		if (temp.compare("clk") == 0) //if the word input is written correct then it saves the position
	//		{
	//			_positionClk.push_back(i);
	//		}

	//		else
	//		{
	//			std::cerr << "ERROR: syntax error at line " << i + 1 << std::endl;
	//			isOk = false;
	//		}
	//	}

	//	if (_description[i].find("(") != std::string::npos)
	//	{
	//		_positionOpenBracket.push_back(i);
	//	}

	//	if (_description[i].find(")") != std::string::npos) //look for a closed bracket in each line
	//	{
	//		_positionCloseBracket.push_back(i); //save the position of the closed bracket
	//	}

	//	if (_description[i].find("FF") != std::string::npos)//look for a flipflop
	//	{
	//		std::istringstream _stream_temp(_description[i]); //save the line in a stream
	//		std::string _flipFlop;

	//		_stream_temp >> temp; //save the first word in a temporary value that should be FF number
	//		_flipFlop = temp;

	//		for (int j = 0; j < _flipFlop.length() - 2; j++)
	//		{
	//			if (_flipFlop[0] == 'F' && _flipFlop[1] == 'F' && isdigit(_flipFlop[j + 2]) != 0)
	//				//the first and the second words have to be F and then just numbers
	//			{
	//				_positionFF.push_back(i);
	//				isOk = true;
	//			}

	//			else
	//			{
	//				isOk = false;
	//				std::cerr << "ERROR: syntax error at line " << i + 1 << std::endl;
	//			}
	//		}
	//	}
	//}

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

		for (int i = 0; i < _positionModule.size(); i++) 
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

				for (int counterClk = 0; counterClk < _positionClk.size(); counterClk++)
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
							isSequential = true; //da rifare
						}
					}
				}
			}

			for (int counterInput=0		; counterInput		<	_positionInput.size();		counterInput++)
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

			for (int counterOutput=0	; counterOutput		<	 _positionOutput.size();	counterOutput++)
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
			
			for (int counterAssign=0	; counterAssign		<	 _positionAssign.size();	counterAssign++)
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
			for (int counterFF = 0; counterFF < _positionFF.size(); counterFF++)
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
	bool okay=true;
	
	for (int i = 0; i < _to_check.size(); i++)
	{
		for (int j = 0; j <_to_check[i].length(); j++)//for each string that contains input
		{
			//input can contains just letters, numbers, commas and square brackets
			if (isdigit(_to_check[i][j])==0 && isalpha(_to_check[i][j])==0 && _to_check[i][j]!=',' && _to_check[i][j]!='[' && _to_check[i][j]!=']' && _to_check[i][j]!=' ' && _to_check[i][j]!='\t')
			{
				std::cerr << "ERROR: the input is not formatted correctly" << std::endl;
				okay = false;
			}
		}
	}
	for (int i = 0; i < _to_check.size(); i++)
	{
		for (int j = 0; j < _to_check[i].length(); j++)
		{
			int length;
			length = _to_check[i].length() - 1;
			if (okay)
			{
				//after a comma there has to be a space and inside the brackets there has to be a number
				if (_to_check[i][j] == ',' && _to_check[i][j + 1] != ' ')
				{
					std::cerr << "ERROR: no space found after the comma" << std::endl;
					okay= false;
				}
				else if (_to_check[i][j] == '[' && isdigit(_to_check[i][j + 1]) == 0 && _to_check[i][j + 2] == ']')
				{
					std::cerr << "ERROR: there is inside the square brackets an unallowed character" << std::endl;
					okay = false;
				}
				else if (_to_check[i][j] == '[' && _to_check[i][j+1] == ']')
				{
					std::cerr << "ERROR: no number found inside the square brackets" << std::endl;
					okay = false;
				}
				//DA RIFARE
				/*else if (isalpha(_to_check[i][length]) != 0 || _to_check[i][length] != ' ' || _to_check[i][length] !=']')
				{
					std::cerr << "the input's string doesn't end correctly" << std::endl;
					okay = false;
				}*/
			}

		}
	}
	return okay;
}

bool checkOutput(std::vector <std::string> & _to_check) 
{
	bool okay=true;
	for (int i = 0; i < _to_check.size(); i++)
	{

		for (int j = 0; j < _to_check[i].length(); j++)//for each string that contains input
		{
			//output can just contain letters and commas
			if (isalpha(_to_check[i][j]) == 0 && _to_check[i][j] != ',' &&  _to_check[i][j] != ' ' && _to_check[i][j] != '\t')
			{
				std::cerr << "ERROR: the output is not formatted correctly" << std::endl;
				okay = false;
			}
			if (_to_check[i][j] == ',' && _to_check[i][j + 1] != ' ')
			{
				std::cerr << "no space found after the comma" << std::endl;
				okay = false;
			}

			//DA RIFARE
			////there has to be a comma after an output
			//if (isalpha(_to_check[i][j])!=0 && _to_check[i][j+1]!=',' && _to_check[i][j - 1] == ' ' && _to_check[i][j + 2] == ' ')
			//{
			//	std::cerr << "no comma found after the output's signal" << std::endl;
			//	okay = false;
			//}
			////output's string has to end with a character or a space
			//if (isalpha(_to_check[i][_to_check[i].length()-1]) != 0 || _to_check[i][_to_check[i].length()-1] != ' ')
			//{
			//	std::cerr << "ERROR: the output's string doesn't end correctly" << std::endl;
			//	okay = false;
			//}
		}
	}
	return okay;
}

bool checkAssign(std::vector <std::string> & _to_check) {
	bool okay = true;
	for (int i = 0; i < _to_check.size(); i++)
	{
		for (int j = 0; j < _to_check[i].length(); j++)//for each string that contains input
		{
			//input can contains just letters, numbers, commas and square brackets
			if (isalpha(_to_check[i][j]) == 0 && _to_check[i][j] != '(' && _to_check[i][j] != ')' && _to_check[i][j] != ' ' && _to_check[i][j] != '\t' && _to_check[i][j]!='=')
			{
				okay = false;
			}

			else //there are no unwanted characters
			{
				//the string has to have spaces before the opened brackets and after the closed brackets
				if( (_to_check[i][j]=='(' && _to_check[i][j-1]!=' ') || (_to_check[i][j]==')' && _to_check[i][j+1]!=' '))
				{
					okay = false;
				}
			}
		}
	}
	return okay;
}

void checkSignals() {
	bool valid = true;
	if (check_circuitDescr)
	{
		valid = checkOutput(all_output);
		if (valid==false)
		{
			std::cout << "errore";
		}
	}
}