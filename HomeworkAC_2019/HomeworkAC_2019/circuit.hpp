#pragma once
#include "btree.hpp"
#include "stringParser.hpp"

class circuit
{

public:
	circuit(const std::string &_label, const std::vector < signal_input> &_input, const std::vector<signal_output> &_output, const bool & _isSequential=false);
	circuit(const std::string &_label, const std::vector < signal_input> &_input, const std::vector<signal_output> &_output, const std::vector<flipflop> &_fliflop );

	std::string printPaths();


private:
	std::string label;
	std::vector < signal_input> input;
	std::vector <signal_output> output;
	std::vector <flipflop> FF;

	//struct that keeps the name and all the paths
	struct Paths {
		std::string label;

		std::vector <std::vector <std::string>> min_Path;
		std::vector <std::vector <std::string>> max_Path;
		std::vector <std::vector <std::string>> coni_Logici;
		std::vector <std::vector <calculatepath>> paths;
	};

	std::vector <Paths> circuit_output, circuit_FF;


	long double power;
	bool isSequential;

};

circuit::circuit(	const std::string & _label, 
					const std::vector<signal_input>& _input, 
					const std::vector<signal_output>& _output, 
					const bool & _isSequential
				) 
{
	power = 0;

	this->label = _label;
	this->input = _input;
	this->output = _output;

	this->isSequential = _isSequential;
	

	for (size_t i = 0; i < output.size(); i++)
  	{
  		
  		std::vector <calculatepath> t_path;
  		std::vector <std::string > min;
  		std::vector <std::string > max;
   		std::vector <std::string > coni;
  
		Paths t_paths;
		t_paths.label = output[i].getLabel();

  		btree *_head;
  		_head = builtTree(output[i].getParse());
  		t_path = Path(_head);
  
  		min = findMin(t_path); 
  		max = findMax(t_path);
  		coni = coniLogici(t_path);
  
  		t_paths.min_Path.push_back(min);
		t_paths.max_Path.push_back(max);
		t_paths.coni_Logici.push_back(coni);
		t_paths.paths.push_back(t_path);
  		delete_tree(_head);

		circuit_output.push_back(t_paths);
  	}
}

 circuit::circuit(		const std::string & _label, 
						const std::vector<signal_input>& _input, 
						const std::vector<signal_output>& _output, 
						const std::vector<flipflop>& _fliflop)
{
	power = 0;
	isSequential = true;

	this->label = _label;
	this->input = _input;
	this->output = _output;
	this->FF = _fliflop;


	for (size_t i = 0; i < FF.size(); i++)
	{
		std::vector <calculatepath> f_path;
		std::vector <std::string > f_min;
		std::vector <std::string > f_max;
		std::vector <std::string > f_coni;

		Paths t_paths;

		t_paths.label = FF[i].FF_getLabel();

		btree * F_head;
		F_head = builtTree(FF[i].FF_getParse());
		f_path = Path(F_head);

		f_min = findMin(f_path);
		f_max = findMax(f_path);
		f_coni = coniLogici(f_path);

		t_paths.min_Path.push_back(f_min);
		t_paths.max_Path.push_back(f_max);
		t_paths.coni_Logici.push_back(f_coni);
		t_paths.paths.push_back(f_path);
		delete_tree(F_head);
		circuit_FF.push_back(t_paths);
	}

	for (size_t i = 0; i < output.size(); i++)
	{
		std::vector <calculatepath> t_path;
		std::vector <std::string > min;
		std::vector <std::string > max;
		std::vector <std::string > coni;

		Paths t_paths;
		t_paths.label = output[i].getLabel();

		btree *_head;
		_head = builtTree(output[i].getParse());
		t_path = Path(_head);

		min = findMin(t_path);
		max = findMax(t_path);
		coni = coniLogici(t_path);

		t_paths.min_Path.push_back(min);
		t_paths.max_Path.push_back(max);
		t_paths.coni_Logici.push_back(coni);
		t_paths.paths.push_back(t_path);
		delete_tree(_head);

		circuit_output.push_back(t_paths);
	}
}

 inline std::string circuit::printPaths()
 {
	 std::stringstream to_return_stream;

	 
	 for (size_t i = 0; i < circuit_output.size(); i++)
	 {
		 for (size_t j = 0; j < circuit_output[i].min_Path.size(); j++)
		 {
			 to_return_stream << "label: " << circuit_output[i].label << std::endl;

			 for (int k = 0; k < circuit_output[i].min_Path[j].size() ; k++)
			 {
				 to_return_stream  << "min: " << circuit_output[i].min_Path[j][k] << std::endl;
			 }

		 }
	 }
		 return to_return_stream.str();
 }
