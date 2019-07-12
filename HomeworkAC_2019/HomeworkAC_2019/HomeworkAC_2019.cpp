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
#include "HomeworkAC_2019.h"

int main(){ 
	
	std::vector<signal_input> s;
	signal_input s1("s1", 1);
	signal_input s2("s2",1);

	s.push_back(s1);
	s.push_back(s2);

	std::string input1 = "NOT 0";
	std::string input2 = "NOT 1"; 

	std::cout << input1 << " = " << getSig(input1, s)<< std::endl;

	std::cout << input2 << " = " << getSig(input2, s) << std::endl;
	 
	/*for (int i = 0; i < myCircuits.size(); i++) {
		std::cout << myCircuits[i].printPaths();
	}*/
	/*filename_circuitDescr = "descrizione.txt";
	std::vector <circuit> myCircuits = saveData();
	myCircuits[2].simulation("input.txt", 6);
	*/
	//std::cout << myCircuits[i].printPaths() << std::endl;
	return 0;
}

std::vector <circuit> saveData() {
	
	std::vector <circuit> circuit_list;

	//if the file is formatted correctly
	if (checkSignals())
	{
		std::vector <signal_input> t_input;
		std::vector <signal_output> t_output;
		std::vector <flipflop> t_FF;

		for (size_t i = 0; i < _positionModule.size(); i++)
		{

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
						token.erase(remove(token.begin(), token.end(), ' '), token.end());

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
							label.erase(remove(label.begin(), label.end(), ' '), label.end());

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
			//get output
			for (size_t j = 0; j < all_output.size(); j++)
			{

				if (_positionOutput[j] > _positionModule[i] && _positionOutput[j] < _positionEndmodule[i])
				{
					std::string output_string;
					int position = all_output[j].find("output");
					output_string = all_output[j].substr(position + 6, all_output[j].size());
					std::stringstream outputLine_stream(output_string);
					std::string token;

					//divide the string by commas
					while (getline(outputLine_stream, token, ','))
					{
						token.erase(remove(token.begin(), token.end(), ' '), token.end());

						signal_output t_signal(token);
						t_output.push_back(t_signal);
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

					for (size_t j = 0; j < t_output.size(); j++)
					{
						if (t_output[j].getLabel()==label_output)
						{
							t_output[j].setParse(assign_string);
						}
					}
				}
			}

			//get FF
			if (isSequential[i]==true)
			{
				for (size_t j = 0; j < all_FF.size(); j++)
				{
					if (_positionFF[j]> _positionModule[i] && _positionFF[j]< _positionEndmodule[i])
					{
						std::string label_FF, string_FF;
						int pos_FF = all_FF[j].find("FF");
						int pos_equal = all_FF[j].find("=");
						label_FF = all_FF[j].substr(pos_FF, pos_equal - pos_FF - 1);
						string_FF = all_FF[j].substr(pos_equal + 2, all_FF[j].length() - pos_equal);
						flipflop FF(label_FF, string_FF);
						t_FF.push_back(FF);
					}
				}

				if (all_FF.size() > 0)
				{
					circuit t_circuit(t_name, t_input, t_output, t_FF);

					for (size_t j = 0; j < all_instance.size(); j++)
					{
						if (_positionInstance[j] > _positionModule[i] && _positionInstance[j] < _positionEndmodule[i])
						{
							t_circuit.setInstance(all_instance[j]);
						}
					}
					circuit_list.push_back(t_circuit);
				}
				
				if (all_FF.size()==0)
				{
					circuit t_circuit(t_name, t_input, t_output, isSequential[i]);

					for (size_t j = 0; j < all_instance.size(); j++)
					{
						if (_positionInstance[j] > _positionModule[i] && _positionInstance[j] < _positionEndmodule[i])
						{
							t_circuit.setInstance(all_instance[j]);
						}
					}
					circuit_list.push_back(t_circuit);
				}
			}
			else
			{
				circuit t_circuit(t_name, t_input, t_output, isSequential[i]);

				for (size_t j = 0; j < all_instance.size(); j++)
				{
					if (_positionInstance[j] > _positionModule[i] && _positionInstance[j] < _positionEndmodule[i])
					{
						t_circuit.setInstance(all_instance[j]);
					}
				}
				circuit_list.push_back(t_circuit);
				
			}
			t_input.clear();
			t_output.clear();
			t_FF.clear();
		}
		
		for (size_t count_circuit = 0; count_circuit < circuit_list.size(); count_circuit++)
		{
			bool isCom = false;
			std::vector < Instance> t_instance;

			t_output.clear();
			t_FF.clear();

			//circuit is composed so we need all the new input and output
			if (circuit_list[count_circuit].getComposed())
			{
				t_instance=circuit_list[count_circuit].getInstance();
				isCom = true;
			}

			//save the output of the composed circuit
			std::vector <signal_output> vett = circuit_list[count_circuit].getOutput();

			for (size_t countInstance = 0; countInstance < t_instance.size(); countInstance++)
			{

				for (size_t countCC = 0; countCC < circuit_list.size(); countCC++)
				{
					//look for the name of the circuit that is written before the instance
					if (circuit_list[countCC].getLabel() == t_instance[countInstance].label_circuitFrom)
					{
						//save the output of the circuit that will create the composed one
						std::vector <signal_output> vect_output = circuit_list[countCC].getOutput();

						std::string output_parse;
						int pos_output;
						std::string FF_parse;
						int pos_FF;

						for (size_t countOutput = 0; countOutput < vect_output.size(); countOutput++)
						{
							for (size_t countFrom = 0; countFrom < t_instance[countInstance].from_circuit.size(); countFrom++)
							{
								//look in every output if the label is the same in the instance
								if (vect_output[countOutput].getLabel() == t_instance[countInstance].from_circuit[countFrom])
								{
									output_parse = vect_output[countOutput].getParse();
									pos_output = countFrom;
								}
							}
							//look through the string to parse
							for (size_t countFrom = 0; countFrom < t_instance[countInstance].from_circuit.size(); countFrom++)
							{
								int pos;
								do
								{
									//find the position of the input in order to replace it
									pos = output_parse.find(t_instance[countInstance].from_circuit[countFrom]);
									int lenght = t_instance[countInstance].from_circuit[countFrom].size();

									//check if the word is the one that I'm looking for
									if (pos > 0 && pos != (output_parse.size()-1))
									{
										if ((output_parse[pos - 1] == ' ' && output_parse[lenght + pos] == ' ') ||
											(output_parse[pos - 1] == '(' && output_parse[lenght + pos] == ' ') ||
											(output_parse[pos - 1] == ' ' && output_parse[lenght + pos] == ')'))
										{
											output_parse = output_parse.replace(pos, lenght, t_instance[countInstance].to_circuit[countFrom]);
										}
									}
									else if (pos == 0)
									{
										if (output_parse[lenght + pos] == ' ' || output_parse[lenght + pos] == ')')
										{
											output_parse = output_parse.replace(pos, lenght, t_instance[countInstance].to_circuit[countFrom]);
										}
									}
									else if (pos == (output_parse.size()-1))
									{
										if (output_parse[pos - 1] == ' ' || output_parse[pos - 1] == '(')
										{
											output_parse = output_parse.replace(pos, lenght, t_instance[countInstance].to_circuit[countFrom]);
										}
									}
								} while (pos >= 0);
							}

							int isFF = 1;
							for (size_t k = 0; k < vett.size(); k++)
							{
								if (vett[k].getLabel() == t_instance[countInstance].to_circuit[pos_output])
								{
									//update the output and their string and then update
									vett[k].setParse(output_parse);
									
									isFF = 0;
								}
							}

							//no output was found than it has to be a FF
							if (isFF == 1)
							{
								flipflop FF_temp(t_instance[countInstance].to_circuit[pos_output], output_parse);
								t_FF.push_back(FF_temp);
							}
						}

						std::vector <flipflop> vect_FF = circuit_list[countCC].getFF();

						for (size_t countFF = 0; countFF < vect_FF.size(); countFF++)
						{
							for (size_t countFrom = 0; countFrom < t_instance[countInstance].from_circuit.size(); countFrom++)
								{
									//look in every FF to find if the label is the same in the instance
								if (vect_FF[countFF].FF_getLabel() == t_instance[countInstance].from_circuit[countFrom])
								{
									FF_parse = vect_FF[countFF].FF_getParse();
									pos_FF = countFrom;
								}
							}
					
							//look through the string to parse
							for (size_t countFrom = 0; countFrom < t_instance[countInstance].from_circuit.size(); countFrom++)
							{
								int pos;
								do
								{
									//find the position of the input in order to replace it
									pos = FF_parse.find(t_instance[countInstance].from_circuit[countFrom]);
									int lenght = t_instance[countInstance].from_circuit[countFrom].size();

									//check if the word is the one that I'm looking for
									if (pos > 0 && pos != (FF_parse.size()-1))
									{
										if ((FF_parse[pos - 1] == ' ' && FF_parse[lenght + pos] == ' ') ||
											(FF_parse[pos - 1] == '(' && FF_parse[lenght + pos] == ' ') ||
											(FF_parse[pos - 1] == ' ' && FF_parse[lenght + pos] == ')'))
										{
											FF_parse = FF_parse.replace(pos, lenght, t_instance[countInstance].to_circuit[countFrom]);
										}
									}
									else if (pos == 0)
									{
										if (FF_parse[lenght + pos] == ' ' || FF_parse[lenght + pos] == ')')
										{
											FF_parse = FF_parse.replace(pos, lenght, t_instance[countInstance].to_circuit[countFrom]);
										}
									}
									else if (pos == (FF_parse.size()-1))
									{
										if (FF_parse[pos - 1] == ' ' || FF_parse[pos - 1] == '(')
										{
											FF_parse = FF_parse.replace(pos, lenght, t_instance[countInstance].to_circuit[countFrom]);
										}
									}
								} while (pos >= 0);
							}								

							int is_FF = 1;
							for (size_t k = 0; k < vett.size(); k++)
							{
								if (vett[k].getLabel() == t_instance[countInstance].to_circuit[pos_FF])
								{
									//resave all the output and update them
									vett[k].setParse(FF_parse);
							
									is_FF = 0;
								}
							}
	
							//no output was found than it has to be a FF
							if (is_FF == 1)
							{
								flipflop FF_temp(t_instance[countInstance].to_circuit[pos_FF], FF_parse);
								t_FF.push_back(FF_temp);
							}
						}

						t_output = vett;
					}
				}
			}

			if (isCom)
			{
				circuit_list[count_circuit].update(t_output, t_FF);
			}
		}
		
		for (size_t i = 0; i < circuit_list.size(); i++)
		{
			circuit_list[i].createTree();
		}
		return circuit_list;
	}
	else
	{
		std::exit(EXIT_FAILURE);
	}
}


void menu() {

	filename_circuitDescr = "descrizione.txt";
	std::vector <circuit> myCircuits = saveData();
	std::string what_to_do;
	int which_circuit;

	for (size_t i = 0; i < myCircuits.size(); i++)
	{
	}
}

 
