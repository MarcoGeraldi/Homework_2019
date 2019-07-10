#pragma once
#include "btree.hpp"
#include "stringParser.hpp"

class circuit
{

public:
	circuit(const std::string &_label, const std::vector < signal_input> &_input, const std::vector<signal_output> &_output, const bool & _isSequential=false);
	circuit(const std::string &_label, const std::vector < signal_input> &_input, const std::vector<signal_output> &_output, const std::vector<flipflop> &_fliflop );

	std::string printPaths();
	std::string printMin();
	std::string printMax();
	std::string printConiLogici();
	std::string getLabel();

	std::vector <std::vector <signal_output>> simulation(const std::string & file_name, const long int &_clk=0);

private:
	std::string label;
	std::vector < signal_input> input;
	std::vector <signal_output> output;
	std::vector <std::vector <signal_output>> simulation_output;
	//std::vector <std::vector <signal_input>> simulation_input;

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
	long double clk;

};

circuit::circuit(	const std::string & _label, 
					const std::vector<signal_input>& _input, 
					const std::vector<signal_output>& _output, 
					const bool & _isSequential) 
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
	 std::stringstream print_stream;

	 print_stream <<
		 getLabel() << std::endl <<
		 printMin() <<
		 printMax() <<
		 printConiLogici() << std::endl;
	 
	 
		 return print_stream.str();
 }

 inline std::string circuit::printMin()
{
	 std::stringstream min_stream;
	 for (size_t i = 0; i < circuit_output.size(); i++)
	 {
		 for (size_t j = 0; j < circuit_output[i].min_Path.size(); j++)
		 {
			 min_stream << "label: " << circuit_output[i].label << std::endl;

			 for (int k = 0; k < circuit_output[i].min_Path[j].size(); k++)
			 {
				 min_stream << "min: " << circuit_output[i].min_Path[j][k] << std::endl;
			 }

		 }
	 }
	 return min_stream.str();
 }

 inline std::string circuit::printMax()
 {
	 std::stringstream max_stream;
	 for (size_t i = 0; i < circuit_output.size(); i++)
	 {
		 for (size_t j = 0; j < circuit_output[i].max_Path.size(); j++)
		 {
			 max_stream << "label: " << circuit_output[i].label << std::endl;

			 for (int k = 0; k < circuit_output[i].max_Path[j].size(); k++)
			 {
				 max_stream << "max: " << circuit_output[i].max_Path[j][k] << std::endl;
			 }

		 }
	 }
	 return max_stream.str();
 }

 inline std::string circuit::printConiLogici()
 {
	 std::stringstream coniLogici_stream;
	 for (size_t i = 0; i < circuit_output.size(); i++)
	 {
		 for (size_t j = 0; j < circuit_output[i].coni_Logici.size(); j++)
		 {
			 coniLogici_stream << "label: " << circuit_output[i].label << std::endl;

			 for (int k = 0; k < circuit_output[i].coni_Logici[j].size(); k++)
			 {
				 coniLogici_stream << "coni logici: " << circuit_output[i].coni_Logici[j][k] << std::endl;
			 }

		 }
	 }
	 return coniLogici_stream.str();
 }

 inline std::string circuit::getLabel()
 {
	 return this->label;
 }

 inline std::vector<std::vector<signal_output>> circuit::simulation(const std::string & file_name, const long int &_clk)
 {
	 filename_inputSignal = file_name;
	 open_inputFile();
	 clk = _clk;

		
	 if (isSequential==false)
	 {
		 //combinatorio circuits and it only needs the first line written in the input file
		 if (input.size()<=vect_matrix[0].size())
		 {
			 if (input.size() < vect_matrix[0].size()) std::cerr << "WARNING: too many input defined in the file" << std::endl;

			 for (size_t i = 0; i < input.size(); i++)
			 {
				 //each input has its own label and now also its own value
				 input[i].Set(vect_matrix[0][i]);
			 }

			 std::vector<signal_output> vect_output;

			 for (size_t i = 0; i < output.size(); i++)
			 {
				 //takes the input and give back the value of the output
				 signal_output t_output(output[i].getLabel(), getValue(output[i].getParse(), input));
				 vect_output.push_back(t_output);
			 }
			 simulation_output.push_back(vect_output);
		 }
		 else
		 {
			 std::cerr << "ERROR: input's value are too few" << std::endl;
		 }
	 }
	 else
	 {
		 if (clk <= vect_matrix.size())
		 {
			 for (int i = 0; i < clk; i++)
			 {
				 std::vector <signal_input> t_vect;

				 for (int j = 0; j < input.size(); j++)
				 {
					 signal_input t_input(input[j].getLabel(), vect_matrix[i][j]);
					 t_vect.push_back(t_input);
				 }
				 std::vector <flipflop> vect_FF;

				 for (int j = 0; j < FF.size(); j++)
				 {
					 flipflop t_FF(FF[j].FF_getLabel(), getValue(FF[j].FF_getParse(), t_vect, FF), clk);
					 vect_FF.push_back(t_FF);
				 }
			
				 std::vector<signal_output> vect_output;
				 for (size_t j = 0; j < output.size(); j++)
				 {
					 //takes the input and give back the value of the output
					 signal_output t_output(output[j].getLabel(), getValue(output[j].getParse(), t_vect, vect_FF));
					 vect_output.push_back(t_output);
				 }
				 simulation_output.push_back(vect_output); 
			 }
			 for (int i = 0; i < simulation_output.size(); i++)
			 {
				 for (size_t j = 0; j < simulation_output[i].size(); j++)
				 {
					 std::cout << "label: " << simulation_output[i][j].getLabel() << " valore: " << simulation_output[i][j].getValue() << std::endl;
				 }
			 }
		 }
		 else
		 {
			 std::cerr << "ERROR: the clock is too big respect to input's lines" << std::endl;
		 }
	 }
	 return simulation_output;
 }

 