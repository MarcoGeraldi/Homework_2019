//HomeworkAC_2019

// Marco Geraldi	<248825>
// Margherita Gili	<249774>

/*
TO DO LIST:
-creare un classe circuito combinatorio, sequenziale e combinato in grado di gestire un circuito.
la classe deve contenere input, output, path min e max, coni logici, se è sequenziale e se è composto.

-salvare i dati e finire i controlli su file, sulla stringa per il parser

-aggiungere analisi di potenza da aggiungere al parser, ogni gate deve avere un vettore che tiene memoria degli output

-
*/
#include "filecheck.hpp"
#include "btree.hpp"
#include "stringParser.hpp"

class circuit
{

public:
	circuit(const std::string &_label, const std::vector < signal_input> &_input, const std::vector<signal_output> &_output);


private:
	std::string label;
	std::vector < signal_input> input;
	std::vector <signal_output> output;

	std::vector <std::vector <std::string>> min_Path;
	std::vector <std::vector <std::string>> max_Path;
	std::vector <std::vector <std::string>> coni_Logici;

	std::vector <std::vector <calculatepath>> paths;

	long double power;


};

circuit::circuit(const std::string & _label, const std::vector<signal_input>& _input, const std::vector<signal_output>& _output){
	power = 0;

	this->label = _label;
	this->input = _input;
	this->output = _output;

	for (size_t i = 0; i < output.size(); i++)
	{
		std::vector <calculatepath> t_path;
		std::vector <std::string > min;
		std::vector <std::string > max;
		std::vector <std::string > coni;

		btree *_head;
		_head = builtTree(output[i].getParse());
		t_path = Path(_head);

		min = findMin(t_path);
		max = findMax(t_path);
		coni = coniLogici(t_path);

		min_Path.push_back(min);
		max_Path.push_back(max);
		coni_Logici.push_back(coni);
		paths.push_back(t_path);
		delete_tree(_head);
	}
}

std::vector <circuit> saveData() {
	std::vector <circuit> circuit_list;

	for (size_t i = 0; i < _positionModule.size(); i++)
	{

		std::vector <signal_input> t_input;
		std::vector <signal_output> t_output;
		//get Name
		std::string t_name;
		t_name = all_name[i];

		//get Inputs
		for (size_t input_counter = 0; input_counter < all_input.size(); input_counter++)
		{
			if (_positionInput[input_counter] > _positionModule[i] && _positionInput[input_counter] < _positionEndmodule[i])
			{
				std::string input_string;
				int position = all_input[input_counter].find("input");
				input_string = all_input[input_counter].substr(position + 5, all_input[input_counter].size());
				std::stringstream inputLine_stream(input_string);
				std::string token;

				//divide the string by commas
				while (getline(inputLine_stream, token, ','))
				{
					if (token.find("[") != std::string::npos) //if a input is a vector
					{
						int size_input;
						std::string size_string;
						std::string label;
						//find the positions of the brackets
						int positionSquare_open = all_input[input_counter].find("[");
						int positionSquare_close = all_input[input_counter].find("]");

						//find the size of the input's vector
						size_string = all_input[input_counter].substr(positionSquare_open + 1, positionSquare_close - positionSquare_open - 1);
						size_input = std::stoi(size_string);

						label = all_input[i].substr(all_input[input_counter][0], positionSquare_open - all_input[input_counter][0]);

						for (int j = 0; j < size_input; j++)
						{
							std::stringstream s_input;
							s_input << label << "[" << j << "]";
							signal_input t_signal(s_input.str());
							t_input.push_back(t_signal);
						}
					}
					else
					{
						signal_input t_signal(token);
						t_input.push_back(t_signal);
					}
				}
			}
		}

		// get output and paths
		for (size_t j = 0; j < all_assign.size(); j++)
		{
			//outputPath t_temp;
			if (_positionAssign[j] > _positionModule[i] && _positionAssign[j] < _positionEndmodule[i])
			{
				std::string assign_string, label_output;
				int position = all_assign[j].find("assign");
				int pos_equal = all_assign[j].find("=");
				label_output = all_assign[j].substr(position + 7, pos_equal - (position + 7) - 1);  // output label
				assign_string = all_assign[j].substr(pos_equal + 2, all_assign[j].length() - (pos_equal)); // string output
				signal_output t_oSignal(label_output, assign_string);
				t_output.push_back(t_oSignal);
			}
		}
		circuit t_circuit(t_name, t_input, t_output);
		circuit_list.push_back(t_circuit);
	}
	return circuit_list;
}

int main(){ 
	
	//string input = "(a OR (NOT c)) AND (NOT (b NAND (NOT d)))";
	
	filename_circuitDescr = "descrizione.txt";
	checkSignals();

	std::vector <circuit> myCircuits=saveData();
	return 0;
}



 
