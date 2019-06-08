#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

std::string filename_inputSignal;
std::string filename_circuitDescr;
std::fstream stream_inputSignal;
std::fstream stream_circuitDescr;

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

bool check_inputFile() {

	std::vector <bool> vect_signal;
	std::string line;
	while (!stream_inputSignal.eof())
	{
		std::getline(stream_inputSignal, line);

		for (size_t i = 0; i < line.size(); i++)
		{
			if (line[i] == '0' || line[i] == '1')
			{
				vect_signal.push_back(bool(line[i]));
			}
		}

	}
}