#pragma once

#include "filecheck.hpp"
#include "circuit.h"

std::vector <circuit> saveData() {

std::vector <circuit> circuit_to_return;

	for (int i = 0; i < _positionModule.size(); i++)
	{
		std::vector <signal_input> t_input;
		std::vector <signal_output> t_output;
		//get Name
		std::string t_name;
		t_name = all_name[i];

		//get Inputs
		for (int input_counter = 0; input_counter < all_input.size(); input_counter++)
		{
			if (_positionInput[input_counter]>_positionModule[i] && _positionInput[input_counter]< _positionEndmodule[i])
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
						int positionSquare_open = all_input[input_counter].find("[");
						int positionSquare_close = all_input[input_counter].find("]");

						size_string= all_input[input_counter].substr(positionSquare_open + 1, positionSquare_close - positionSquare_open - 1);
						size_input = std::stoi(size_string);

						label = all_input[i].substr(all_input[input_counter][0], positionSquare_open -1);
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
		//get Outputs
		/*for (int output_counter = 0; output_counter < all_output.size(); output_counter++)
		{
			if (_positionOutput[output_counter] > _positionModule[i] && _positionOutput[output_counter] < _positionEndmodule[i])
			{
				std::string output_string;
				int position = all_output[output_counter].find("output");
				output_string = all_output[output_counter].substr(position + 6, all_output[output_counter].size());
				std::stringstream outputLine_stream(output_string);
				std::string token;
				while (getline(outputLine_stream, token, ','))
				{
					signal_output t_signal(token);
					t_output.push_back(t_signal);
				}
			}
		}*/
		// get output and paths
		for (int j = 0; j < all_assign.size(); j++)
		{
			if (_positionAssign[j] > _positionModule[i] && _positionAssign[j] < _positionEndmodule[i])
			{
				std::string assign_string, label_output;
				int position = all_assign[j].find("assign");
				int pos_equal = all_assign[j].find("=");
				label_output = all_assign[j].substr(position + 7, pos_equal-position-1);
				assign_string = all_assign[j].substr(pos_equal, all_assign[j].length());
				signal_output t_signal(label_output, assign_string);
				t_output.push_back(t_signal);
			}
		}
		circuit t_circuit(t_name, t_input, t_output);
		circuit_to_return.push_back(t_circuit);
	}
	return circuit_to_return;
}