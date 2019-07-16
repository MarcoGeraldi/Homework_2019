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

int main(int argc, char* argv[]) {


	filename_circuitDescr = argv[1];
	std::vector <circuit> myCircuits = saveData();
	std::vector<std::vector <signal_output>> simulation;

	/////////////////////////////////////////////////////////
	///////////////////	Title ///////////////////////////////
	/////////////////////////////////////////////////////////
	std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	std::cout << "+++++++++++++++++      Homework 2019: Digital Circuits Simulator.      +++++++++++++++++" << std::endl;
	std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl << std::endl;
	std::cout << "----------------------------------------------------------------------------------------" << std::endl << std::endl;

	/////////////////////////////////////////////////////////
	/////////////// Select Circuit  /////////////////////////
	/////////////////////////////////////////////////////////
	std::string inputString;
	bool checkString = true;
	int circuitSelected = 0;
	while (1) {
		do
		{

			std::cout << "*******************************************************" << std::endl;
			std::cout << "****************** Select Circuit *********************" << std::endl;
			std::cout << "*******************************************************" << std::endl;

			checkString = true;
			std::cout << "Found " << myCircuits.size() << " circuits." << std::endl << "Select one to continue." << std::endl;
			for (size_t i = 0; i < myCircuits.size(); i++)
			{
				std::cout << "[" << i+1 << "] " << myCircuits[i].getLabel() << std::endl;
			}

			std::cout << "[0] EXIT" << std::endl;

			// Get circuit to test
			std::cin >> inputString;

			

			//check if inputstring is a number
			for (size_t i = 0; i < inputString.size(); i++) { if (!isdigit(inputString[i])) checkString = false; }
			if (checkString) {
				circuitSelected = stoi(inputString)-1;

				if (stoi(inputString)==0)
				{
					std::exit(EXIT_SUCCESS);
				}
				//check circuitSelected
				if (circuitSelected > myCircuits.size())
				{
					std::cout << "Circuit Selected not found." << std::endl;
				}
			}
			else {
				std::cout << "Circuit Selected not found." << std::endl;
#ifdef ENABLE_CLEARSCREEN
				system("PAUSE");
				system("CLS");
#endif // ENABLE_CLEARSCREEN
			}

		} while (circuitSelected > myCircuits.size() || checkString == false);

		// continue to the next screen
#ifdef ENABLE_CLEARSCREEN
		system("CLS");
#endif // ENABLE_CLEARSCREEN
		bool thisCircuit = true;
		while (thisCircuit) {
			/////////////////////////////////////////////////////////
			/////////////// Select Option  //////////////////////////
			/////////////////////////////////////////////////////////
			int optionSelected = -1; // initialize it to a not possible value
			do
			{

				std::cout << "*******************************************************" << std::endl;
				std::cout << "****************** Select Option **********************" << std::endl;
				std::cout << "*******************************************************" << std::endl;

				checkString = true;

				std::cout << "Circuit [" << circuitSelected << "] " << myCircuits[circuitSelected].getLabel() << " selected" << std::endl;
				std::cout << "Select one option" << std::endl
					<< "[1] Simulate Circuit" << std::endl
					<< "[2] Analyze Circuit" << std::endl
					<< "[9] Select another circuit" << std::endl
					<< "[0] EXIT" << std::endl;

				// Get option to test
				std::cin >> inputString;
				std::cout << std::endl;

				//check if inputstring is a number

				if (inputString != "0" && inputString != "1" && inputString != "2"&& inputString != "92")
				{
					checkString = false;
					std::cout << "option not found." << std::endl;

#ifdef ENABLE_CLEARSCREEN
					system("PAUSE");
					system("CLS");
#endif // ENABLE_CLEARSCREEN
				}
#ifdef ENABLE_CLEARSCREEN 
				system("CLS");
#endif // ENABLE_CLEARSCREEN


				if (checkString)
				{
					for (size_t i = 0; i < inputString.size(); i++)	if (!isdigit(inputString[i])) checkString = false;
					optionSelected = stoi(inputString);
				}

			} while (optionSelected != 0 && optionSelected != 1); //simulazione o analisi 

			/////////////////////////////////////////////////////////
			/////////////// Simulation & Analysis ///////////////////
			/////////////////////////////////////////////////////////

			//simulation
			if (optionSelected == 1)
			{
				//simulation

				//simulazione:	1 file di  input
				//				2 clk
				//				stampa output
				std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
				std::cout << "+++++++++++++++++++++++++++++++  Circuit Simulation.  ++++++++++++++++++++++++++++++++++" << std::endl;
				std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl << std::endl;
				std::cout << "----------------------------------------------------------------------------------------" << std::endl << std::endl;
				std::cout << "Inser input filename: " << std::endl;
				std::cin >> filename_inputSignal;

				int clk = 1;
				if (myCircuits[circuitSelected].getSequential())
				{
					//check for clock
					do
					{
						checkString = true;
						std::cout << "The circuit selected is sequential. Insert clock frequency." << std::endl << "Insert \"0\" to skip" << std::endl;
						std::cin >> inputString;
						for (size_t i = 0; i < inputString.size(); i++)	if (!isdigit(inputString[i])) checkString = false;
						if (checkString) clk = stoi(inputString);
						else std::cout << "the clock frequency inserted is not valid, please try again." << std::endl;
					} while (checkString == false);

				}
				simulation = myCircuits[circuitSelected].simulation(filename_inputSignal, clk);

#ifdef ENABLE_CLEARSCREEN
				system("CLS");
#endif // ENABLE_CLEARSCREEN

				//get simulation values
				int simulation_optionSelected = -1; // initialize it to a not possible value
				do
				{
					checkString = true;
					std::cout << "Simulation finished. please select one of the following options:" << std::endl
						<< "[1] Print on screen" << std::endl
						<< "[2] Print on file" << std::endl
						<< "[9] Select another circuit" << std::endl
						<< "[0] EXIT" << std::endl;

					// Get option to test
					std::cin >> inputString;
					std::cout << std::endl;

					//check if inputstring is a number
					if (inputString != "0" && inputString != "1"&& inputString != "2" && inputString != "9")
					{
						checkString = false;
						std::cout << "option not found." << std::endl;
					}
#ifdef ENABLE_CLEARSCREEN
					system("PAUSE");
					system("CLS");
#endif // ENABLE_CLEARSCREEN


					if (checkString)
					{
						for (size_t i = 0; i < inputString.size(); i++)	if (!isdigit(inputString[i])) checkString = false;
						simulation_optionSelected = stoi(inputString);
					}

				} while (simulation_optionSelected != 0 && simulation_optionSelected != 1);

				//print values
				switch (simulation_optionSelected)
				{
					//print on screen
				case 1:
					for (int i = 0; i < simulation.size(); i++)
					{
						for (size_t j = 0; j < simulation[i].size(); j++)
						{
							std::cout << "label: " << simulation[i][j].getLabel() << " valore: " << simulation[i][j].getValue() << std::endl;
						}
					}
#ifdef ENABLE_CLEARSCREEN
					system("PAUSE");
					system("CLS");
#endif // ENABLE_CLEARSCREEN
					break;
					//print on file -- da fare
				case 2:
#ifdef ENABLE_CLEARSCREEN
					system("PAUSE");
					system("CLS");
#endif // ENABLE_CLEARSCREEN
					break;
					//exit
				case 0:
#ifdef ENABLE_CLEARSCREEN
					system("PAUSE");
					system("CLS");
#endif // ENABLE_CLEARSCREEN
					break;
					//select another circuit
				case 9:
#ifdef ENABLE_CLEARSCREEN
					system("PAUSE");
					system("CLS");
#endif // ENABLE_CLEARSCREEN
					break;
				default:
					break;
				}
			}

			//analysis
			if (optionSelected == 2)
			{
				//analisi:	1 path minimo
				//			2 path max
				//			3 coni logici
				//			4 tutto
				//			5 potenza -> file di potenza, file di input e clk

				std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
				std::cout << "++++++++++++++++++++++++++++++++  Circuit Analysis.  +++++++++++++++++++++++++++++++++++" << std::endl;
				std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl << std::endl;
				std::cout << "----------------------------------------------------------------------------------------" << std::endl << std::endl;

				checkString = true;
				int analysis_optionSelected = -1;
				do
				{
					std::cout << "Select one option" << std::endl
						<< "[1] Print Path Min" << std::endl
						<< "[2] Print Path Max" << std::endl
						<< "[3] Print Logical Cones" << std::endl
						<< "[4] Print All Paths" << std::endl
						<< "[5] Print Power Dissipated" << std::endl
						<< "[9] Select a new circuit" << std::endl
						<< "[0] EXIT" << std::endl;

					// Get option to test
					std::cin >> inputString;
					std::cout << std::endl;

					//check if inputstring is a number

					if (inputString != "0" && inputString != "1" &&
						inputString != "2" && inputString != "3" &&
						inputString != "4" && inputString != "5" && inputString != "9")
					{
						checkString = false;
						std::cout << "option not found." << std::endl;
					}
#ifdef ENABLE_CLEARSCREEN
					system("PAUSE");
					system("CLS");
#endif // ENABLE_CLEARSCREEN
					if (checkString)
					{
						for (size_t i = 0; i < inputString.size(); i++)	if (!isdigit(inputString[i])) checkString = false;
						analysis_optionSelected = stoi(inputString);
					}
				} while (analysis_optionSelected != 0 && analysis_optionSelected != 1 &&
					analysis_optionSelected != 2 && analysis_optionSelected != 3 &&
					analysis_optionSelected != 4);

				myCircuits[circuitSelected].createTree();

				switch (analysis_optionSelected)
				{
					//print path min
				case 1:
					std::cout << myCircuits[circuitSelected].printMin();
#ifdef ENABLE_CLEARSCREEN
					system("PAUSE");
					system("CLS");
#endif // ENABLE_CLEARSCREEN
					break;

					//print path max
				case 2:
					std::cout << myCircuits[circuitSelected].printMax();
#ifdef ENABLE_CLEARSCREEN
					system("PAUSE");
					system("CLS");
#endif // ENABLE_CLEARSCREEN
					break;

					//print Logical cones
				case 3:
					std::cout << myCircuits[circuitSelected].printConiLogici();
#ifdef ENABLE_CLEARSCREEN
					system("PAUSE");
					system("CLS");
#endif // ENABLE_CLEARSCREEN
					break;

					//print All
				case 4:
					std::cout << myCircuits[circuitSelected].printPaths();
#ifdef ENABLE_CLEARSCREEN
					system("PAUSE");
					system("CLS");
#endif // ENABLE_CLEARSCREEN
					break;

					//EXIT
				case 0:
					std::exit(EXIT_SUCCESS);
					break;

					//select another circuit
				case 9:
#ifdef ENABLE_CLEARSCREEN
					thisCircuit = false;
					system("CLS");
#endif // ENABLE_CLEARSCREEN
					break;

					//print power
				case 5:

					std::cout << "*******************************************************" << std::endl;
					std::cout << "************** Calculating Power Dissipated ***********" << std::endl;
					std::cout << "*******************************************************" << std::endl;

					std::cout << "Insert power definitions filename: " << std::endl;
					std::cin >> filename_powerDescr;
					double powerLoss = 0;
					//there circuit were not simulated 
					if (simulation.size() == 0)
					{
						std::cout << "The circuit has never been simulated, so in order to calculate power loss insert input filename: " << std::endl;
						std::cin >> filename_inputSignal;
						int clk = 1;
						if (myCircuits[circuitSelected].getSequential())
						{
							//check for clock
							do
							{
								checkString = true;
								std::cout << "The circuit selected is sequential. Insert clock frequency." << std::endl << "Insert \"0\" to skip" << std::endl;
								std::cin >> inputString;
								for (size_t i = 0; i < inputString.size(); i++)	if (!isdigit(inputString[i])) checkString = false;
								if (checkString) clk = stoi(inputString);
								else std::cout << "the clock frequency inserted is not valid, please try again." << std::endl;
							} while (checkString == false);
						}
						myCircuits[circuitSelected].simulation(filename_inputSignal, clk);
					}

					powerLoss = myCircuits[circuitSelected].getPower();
					std::cout << "Total power dissipation: " << powerLoss << std::endl;
#ifdef ENABLE_CLEARSCREEN
					system("PAUSE");
					system("CLS");
#endif // ENABLE_CLEARSCREEN
					break;

				}
			}

			//select another circuit
			if (optionSelected == 9)
			{
				thisCircuit = false;
#ifdef ENABLE_CLEARSCREEN 
				system("CLS");
#endif // ENABLE_CLEARSCREEN
			}

			//exit
			if (optionSelected==0)
			{
				std::exit(EXIT_SUCCESS);
			}

		}
	}

	return 0;
}



std::vector <circuit> saveData() {
	
	std::vector <circuit> circuit_list;
	bool isOk = true;
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
		 
			//checks for input errors
			for (size_t k = 0; k < t_input.size(); k++)
			{
				if (t_input[k].getLabel()[0] == 'F' &&  t_input[k].getLabel()[1] == 'F')
				{
					std::cerr << "ERROR: " << t_input[k].getLabel() << " can't be defined as an  input" << std::endl;
					isOk = false;
				}

				if (isdigit(t_input[k].getLabel()[0]))
				{
					std::cerr << "ERROR: " << t_input[k].getLabel() << " can't be defined as an  input because begin with a number or it's a number itself" << std::endl;
					isOk = false;
				}
			}

			//check for output errors
			for (size_t j = 0; j < t_output.size(); j++)
			{
				for (size_t k = 0; k < t_input.size(); k++)
				{
					if (t_output[j].getLabel()==t_input[k].getLabel())
					{
						std::cerr << "ERROR: " << t_output[j].getLabel() << " is defined as an input and as an output" << std::endl;
						isOk = false;
					}

					
				}
				if (t_output[j].getLabel()[0]=='F' && t_output[j].getLabel()[1] == 'F')
				{
					std::cerr << "ERROR: " << t_output[j].getLabel() << " can't be defined as an output" << std::endl;
					isOk = false;
				}
				
				if (isdigit(t_output[j].getLabel()[0]))
				{
					std::cerr << "ERROR: " << t_output[j].getLabel() << " can't be defined as an output because begin with a number or it's a number itself" << std::endl;
					isOk = false;
				}
				
				 
			}

			std::vector <std::string> to_check;

			// get output and assign
			for (size_t j = 0; j < all_assign.size(); j++)
			{	
				//outputPath t_temp;
				if (_positionAssign[j] > _positionModule[i] && _positionAssign[j] < _positionEndmodule[i])
				{
					std::string assign_string, label_output;
					int position = all_assign[j].find("assign");
					int pos_equal = all_assign[j].find("=");
					if (pos_equal<0)
					{
						std::cerr << "ERROR: equal not found" << std::endl;
						isOk = false;
					}
					label_output = all_assign[j].substr(position + 7, pos_equal - (position + 7) - 1);  // output label
					assign_string = all_assign[j].substr(pos_equal + 2, all_assign[j].length() - (pos_equal)); // string output
					
					for (size_t j = 0; j < t_output.size(); j++)
					{
						if (t_output[j].getLabel()==label_output)
						{
							t_output[j].setParse(assign_string);
						}
						to_check.push_back(label_output);
					}
				}
			}
			int found = 0;
			for (size_t j = 0; j < to_check.size(); j++)
			{
				for (size_t k = 0; k < t_output.size(); k++)
				{
					if (t_output[k].getLabel()==to_check[j] )
					{
						found = 1;
					}
				}
				if (found==0)
				{
					std::cerr << "ERROR: the output: " << to_check[j] <<" doesn't exist" << std::endl;
					isOk = false;
				}
				found = 0;
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
						if (pos_equal<0)
						{
							std::cerr << "ERROR: equal not found" << std::endl;
							isOk = false;
						}
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
					for (size_t i = 0; i < t_output.size(); i++)
					{
						//search for input or output not definited correctly
						std::string ss_assign = t_output[i].getParse();
						ss_assign.erase(remove(ss_assign.begin(), ss_assign.end(), ')'), ss_assign.end());
						ss_assign.erase(remove(ss_assign.begin(), ss_assign.end(), '('), ss_assign.end());

						std::vector < std::string > tokens;
						std::string token;
						std::stringstream to_split(ss_assign);
						while (std::getline(to_split, token, ' '))
						{
							tokens.push_back(token);
						}

						for (size_t k = 0; k < tokens.size(); k++)
						{
							bool isInput = false;
							bool isGate = false;
							bool isOutput = false;
							bool isFlipFlop = false;

							for (size_t e = 0; e < t_input.size(); e++)
							{
								if (t_input[e].getLabel() == tokens[k])
								{
									isInput = true;
								}
							}

							for (size_t e = 0; e < t_output.size(); e++)
							{
								if (t_output[e].getLabel() == tokens[k])
								{
									isOutput = true;
								}
							}

							for (size_t e = 0; e < t_FF.size(); e++)
							{
								if (t_FF[e].FF_getLabel() == tokens[k])
								{
									isFlipFlop = true;
								}
							}
							if (tokens[k] == "NOR" || tokens[k] == "OR" || tokens[k] == "AND" ||
								tokens[k] == "XOR" || tokens[k] == "XNOR" || tokens[k] == "NAND" || tokens[k] == "NOT")
							{
								isGate = true;
							}

							if (isOutput)
							{
								std::cerr << "ERROR: can't solve because " << tokens[k] << " is an output" << std::endl;
								isOk = false;
							}

							if (!isInput && !isGate && !isFlipFlop)
							{
								std::cerr << "ERROR: can't solve because " << tokens[k] << " not found" << std::endl;
								isOk = false;
							}

						}
					}
					for (size_t i = 0; i < t_FF.size(); i++)
					{
						//search for input or output not definited correctly
						std::string ss_assign = t_FF[i].FF_getParse();
						ss_assign.erase(remove(ss_assign.begin(), ss_assign.end(), ')'), ss_assign.end());
						ss_assign.erase(remove(ss_assign.begin(), ss_assign.end(), '('), ss_assign.end());

						std::vector < std::string > tokens;
						std::string token;
						std::stringstream to_split(ss_assign);
						while (std::getline(to_split, token, ' '))
						{
							tokens.push_back(token);
						}

						for (size_t k = 0; k < tokens.size(); k++)
						{
							bool isInput = false;
							bool isGate = false;
							bool isOutput = false;
							bool isFlipFlop = false;

							for (size_t e = 0; e < t_input.size(); e++)
							{
								if (t_input[e].getLabel() == tokens[k])
								{
									isInput = true;
								}
							}

							for (size_t e = 0; e < t_output.size(); e++)
							{
								if (t_output[e].getLabel() == tokens[k])
								{
									isOutput = true;
								}
							}

							for (size_t e = 0; e < t_FF.size(); e++)
							{
								if (t_FF[e].FF_getLabel() == tokens[k])
								{
									isFlipFlop = true;
								}
							}
							if (tokens[k] == "NOR" || tokens[k] == "OR" || tokens[k] == "AND" ||
								tokens[k] == "XOR" || tokens[k] == "XNOR" || tokens[k] == "NAND" || tokens[k] == "NOT")
							{
								isGate = true;
							}

							if (isOutput)
							{
								std::cerr << "ERROR: can't solve because " << tokens[k] << " is an output" << std::endl;
								isOk = false;
							}

							if (!isInput && !isGate && !isFlipFlop)
							{
								std::cerr << "ERROR: can't solve because " << tokens[k] << " not found" << std::endl;
								isOk = false;
							}

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
					for (size_t i = 0; i < t_output.size(); i++)
					{
						//search for input or output not definited correctly
						std::string ss_assign = t_output[i].getParse();
						ss_assign.erase(remove(ss_assign.begin(), ss_assign.end(), ')'), ss_assign.end());
						ss_assign.erase(remove(ss_assign.begin(), ss_assign.end(), '('), ss_assign.end());

						std::vector < std::string > tokens;
						std::string token;
						std::stringstream to_split(ss_assign);
						while (std::getline(to_split, token, ' '))
						{
							tokens.push_back(token);
						}

						for (size_t k = 0; k < tokens.size(); k++)
						{
							bool isInput = false;
							bool isGate = false;
							bool isOutput = false;

							for (size_t e = 0; e < t_input.size(); e++)
							{
								if (t_input[e].getLabel() == tokens[k])
								{
									isInput = true;
								}
							}

							for (size_t e = 0; e < t_output.size(); e++)
							{
								if (t_output[e].getLabel() == tokens[k])
								{
									isOutput = true;
								}
							}

							if (tokens[k] == "NOR" || tokens[k] == "OR" || tokens[k] == "AND" ||
								tokens[k] == "XOR" || tokens[k] == "XNOR" || tokens[k] == "NAND" || tokens[k] == "NOT")
							{
								isGate = true;
							}

							if (isOutput)
							{
								std::cerr << "ERROR: can't solve because " << tokens[k] << " is an output" << std::endl;
								isOk = false;
							}

							if (!isInput && !isGate)
							{
								std::cerr << "ERROR: can't solve because " << tokens[k] << " not found" << std::endl;
								isOk = false;
							}

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
				for (size_t i = 0; i < t_output.size(); i++)
				{
					//search for input or output not definited correctly
					std::string ss_assign = t_output[i].getParse();
					ss_assign.erase(remove(ss_assign.begin(), ss_assign.end(), ')'), ss_assign.end());
					ss_assign.erase(remove(ss_assign.begin(), ss_assign.end(), '('), ss_assign.end());

					std::vector < std::string > tokens;
					std::string token;
					std::stringstream to_split(ss_assign);
					while (std::getline(to_split, token, ' '))
					{
						tokens.push_back(token);
					}

					for (size_t k = 0; k < tokens.size(); k++)
					{
						bool isInput = false;
						bool isGate = false;
						bool isOutput = false;

						for (size_t e = 0; e < t_input.size(); e++)
						{
							if (t_input[e].getLabel() == tokens[k])
							{
								isInput = true;
							}
						}

						for (size_t e = 0; e < t_output.size(); e++)
						{
							if (t_output[e].getLabel() == tokens[k])
							{
								isOutput = true;
							}
						}

						if (tokens[k] == "NOR" || tokens[k] == "OR" || tokens[k] == "AND" ||
							tokens[k] == "XOR" || tokens[k] == "XNOR" || tokens[k] == "NAND" || tokens[k] == "NOT")
						{
							isGate = true;
						}

						if (isOutput)
						{
							std::cerr << "ERROR: can't solve because " << tokens[k] << " is an output" << std::endl;
							isOk = false;
						}

						if (!isInput && !isGate)
						{
							std::cerr << "ERROR: can't solve because " << tokens[k] << " not found" << std::endl;
							isOk = false;
						}

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
		
		if (isOk == false)
		{
			std::cerr << "ERROR: the file is not formatted correctly" << std::endl;
			std::exit(EXIT_FAILURE);
		}
		return circuit_list;
	}

	std::cerr << "ERROR: the file is not formatted correctly" << std::endl;
	std::exit(EXIT_FAILURE);
}
 
 
