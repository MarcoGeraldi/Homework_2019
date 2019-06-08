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

std::vector < std::vector <signal_input>> matrix_input;
std::vector < std::vector <signal_output>> matrix_output;

//CHECK FOR OPENING
bool openFile(std::fstream _fileStream, std::string & _fileName) {
	bool error = false;
	_fileStream.open(_fileName, std::fstream::in);
	if (!_fileStream.is_open())
	{
		std::cerr << "error while opening " << _fileName << std::endl;
		error = true;
	}

	return error;
}

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
	-verificare se circuito è combinatorio o sequenziale
	-verificare che ci sia module e endmodule
	-nel sequenziale deve esserci anche il clock
*/

bool check_circuitDescr() {
	bool isOk = true;
	std::vector <std::string> _description;
	std::vector <std::istringstream> s_description;
	std::vector <int> _positionEndmodule, _positionModule;
	std::vector <int> _positionInput, _positionOutput, _positionAssign, _positionClk;
	std::vector <int> _positionOpenBracket, _positionCloseBracket;
	std::string line;
	std::istringstream s_line;
	stream_circuitDescr.open(filename_circuitDescr, std::fstream::in);
	if (!stream_circuitDescr.is_open())
	{
		std::cerr << "error while opening " << filename_circuitDescr << std::endl;
		return false;
	}


	while (!stream_circuitDescr.eof())
	{
		std::getline(stream_circuitDescr, line); //read line by line
		//s_line >> line;
		_description.push_back(line); //save each line in a vector
		//s_description.push_back(s_line);
	}
	stream_circuitDescr.close();

	for (int i = 0; i < _description.size(); i++)
	{
		if (_description[i].substr(0,7) == "module " || _description[i].substr(0, 7) == "module") //the line should contain the word module 
		{
			std::cout << "module found" << std::endl;
			_positionModule.push_back(i); //save the position of module found in the file
		}

		if (_description[i].substr(0,10) == "endmodule " || _description[i].substr(0, 10) == "endmodule")
		{
			std::cout << "endmodule found" << std::endl;
			_positionEndmodule.push_back(i); //save the position of the endmodule found in the file
		}		

		if (_description[i].find("input")!=std::string::npos) //look for the input 
		{
			_positionInput.push_back(i);
		}

		if (_description[i].find("output")!=std::string::npos)//look for the output in order to use them if the file is correct
		{
			_positionOutput.push_back(i);
		}

		if (_description[i].find("assign")!=std::string::npos) //look for the word assing
		{
			_positionAssign.push_back(i);
		}
		
		if (_description[i].find("clk")!=std::string::npos) //look for the word clock 
		{
			_positionClk.push_back(i);
		}


		if (_description[i].find("(") != std::string::npos)
		{
			_positionOpenBracket.push_back(i);
		}

		if (_description[i].find(")") != std::string::npos) //look for a closed bracket in each line
		{
			_positionCloseBracket.push_back(i); //save the position of the closed bracket
		}

	}

	if (_positionEndmodule.size() == 0) //if there are no endmodule
	{
		std::cerr << "ERROR: no endmodule found" << std::endl;
		isOk= false;
	}

	if (_positionModule.size() == 0) //no module found
	{
		std::cerr << "ERROR: no module found" << std::endl;
		isOk= false;
	}
	
	if (_positionEndmodule.size()!=_positionModule.size()) //if module and endmodule are not matching
	{
		std::cerr << "ERROR: there are more module than endmodule or viceversa" << std::endl;
		isOk= false;
	}
	
	if (_positionCloseBracket.size() == 0) //if there are no brackets
	{
		std::cerr << "ERROR: no closed brackets found" << std::endl;
		isOk= false;
	}
	 
	if (_positionOpenBracket.size() == 0)
	{
		 std::cerr << "ERROR: no opened brackets found" << std::endl;
		 isOk= false;
	}
	
	if (_positionCloseBracket.size()!= _positionOpenBracket.size()) //if the brackets are not matching
	{
		std::cerr << "ERROR: there aren't as many opened brackets as closed brackets" << std::endl;
		isOk= false;
	}
	
	if (_positionAssign.size()==0) //no assign foung
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
			if (_positionModule[i]>_positionEndmodule[i] && _positionModule.size() == _positionEndmodule.size() )
			{
				std::cerr	<< "ERROR: module was found after endmodule or viceversa in the circuit number: " 
							<< i+1
							<< std::endl;
				return false;
			}
			
			if (_positionOpenBracket[i]>_positionCloseBracket[i] && _positionCloseBracket.size() == _positionOpenBracket.size())
			{
				std::cerr	<< "ERROR: the bracket aren't in the right place in the circuit number: " 
							<< i + 1
							<< std::endl;
				return false;
			}
			
			for (int k = 0; k < _positionInput.size(); k++)
			{
				if (_positionInput[k]<=_positionEndmodule[i]) //for each circuit
				{
					if (_positionInput[k]<_positionOpenBracket[i] && _positionInput[k]>_positionCloseBracket[i])
					//control that input's signals are written inside the brackets
					{
							std::cerr << "ERROR: the input's signals are not inside the brackets" << std::endl;
							isOk=false;
					}
				}
			}

			for (int k = 0; k < _positionOutput.size(); k++)
			{
				if (_positionOutput[k] <= _positionEndmodule[i]) //for each circuit
				{
					if (_positionOutput[k]<_positionOpenBracket[i] && _positionOutput[k]>_positionCloseBracket[i])
								//control that output's signals are written inside the brackets
					{
								std::cerr << "ERROR: the output's signals are not inside the brackets" << std::endl;
								isOk=false;
					}
				}
			}
				
			for (int k = 0; k < _positionAssign.size(); k++)
			{
						
				if (!(_positionAssign[k]>_positionCloseBracket[i] && _positionAssign[k]<_positionEndmodule[i]))
					//control that the word assign is written after the closed bracket but before the endmodule
				{
					std::cerr << "ERROR: the assign is not in the right place" << std::endl;
					isOk= false;
				}						
			}
		}
	}
	return isOk;
}


