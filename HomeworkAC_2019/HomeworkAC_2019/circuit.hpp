#pragma once
#include "btree.hpp"
#include "stringParser.hpp"


struct Instance {
	std::string label_circuitFrom;
	std::vector <std::string> from_circuit;
	std::vector <std::string> to_circuit;
};


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

	void setInstance(std::string & _instance);
	bool getComposed();

	std::vector <std::vector <signal_output>> simulation(const std::string & file_name, const long int &_clk=0);

	std::vector<Instance> getInstance();

	std::vector <signal_input> getInput();
	std::vector <signal_output> getOutput();
	std::vector <flipflop> getFF();
	
	void update(const std::vector <signal_output> &_output, const std::vector <flipflop> &_FF);
	void createTree();

private:
	std::string label;
	std::vector < signal_input> input;
	std::vector <signal_output> output;
	std::vector <std::vector <signal_output>> simulation_output;

	std::vector <flipflop> FF;

	std::vector <std::string> circuit_instance;

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
	bool isComposted;
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

 inline void circuit::setInstance(std::string & _instance)
 {
	 this->isComposted = true;
	 std::string s;
	 s = _instance;
	 circuit_instance.push_back(s);
 }

 inline bool circuit::getComposed()
 {
	 return this->isComposted;
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

 inline std::vector<Instance> circuit::getInstance()
 {
	 std::vector <Instance> to_return;
	 Instance t_instance;

	 for (size_t i = 0; i < circuit_instance.size(); i++)
	 {
		 std::string name, cuttedString;
		 std::vector <std::string> dividedByComma;
		 std::stringstream s_stream(circuit_instance[i]);
		 s_stream >> name;
		 t_instance.label_circuitFrom = name;

		 int pos_Open = circuit_instance[i].find("(");
		 int pos_Close = circuit_instance[i].find(")");

		 cuttedString = circuit_instance[i].substr(pos_Open + 1, pos_Close - (pos_Open +1));
		 std::stringstream s_divided(cuttedString);

		 while (std::getline (s_divided, cuttedString, ','))
		 {
			 //got a vector that contains all the associated input in order to elaborate them
			 dividedByComma.push_back(cuttedString);
		 }
		 for (size_t j = 0; j < dividedByComma.size(); j++)
		 {
			 int pos_equal = dividedByComma[j].find("=");
			 int pos_point = dividedByComma[j].find(".");
			 t_instance.from_circuit.push_back(dividedByComma[j].substr(pos_point + 1, pos_equal - (pos_point+1)));
			 t_instance.to_circuit.push_back(dividedByComma[j].substr(pos_equal + 1, dividedByComma[j].size() - pos_equal));
		 }
		 to_return.push_back(t_instance);
	 }
	 return to_return;
 }

 inline std::vector<signal_input> circuit::getInput()
 {
	 return this->input;
 }

 inline std::vector<signal_output> circuit::getOutput()
 {
	 return this->output;
 }

 inline std::vector<flipflop> circuit::getFF()
 {
	 return this->FF;
 }

 inline void circuit::update(const std::vector<signal_output>& _output, const std::vector<flipflop>& _FF)
 {
	 this->output = _output;
	 this->FF = _FF;
 }

 inline void circuit::createTree()
 {
	 //the circuit can be sequential but for sure there are no FF
	 for (size_t i = 0; i < output.size(); i++)
	 {

		 std::vector <calculatepath> t_path;
		 std::vector <std::string > min;
		 std::vector <std::string > max;
		 std::vector <std::string > coni;

		 Paths t_paths;

		 //for each output get the label of it
		 t_paths.label = output[i].getLabel();

		 btree *_head;
		 //create the tree
		 _head = builtTree(output[i].getParse());
		 //calculate the path
		 t_path = Path(_head);
		 //calculate min, max, coni
		 min = findMin(t_path);
		 max = findMax(t_path);
		 coni = coniLogici(t_path);

		 //save it in the struct so every output has its name, its min,max and coni
		 t_paths.min_Path.push_back(min);
		 t_paths.max_Path.push_back(max);
		 t_paths.coni_Logici.push_back(coni);
		 t_paths.paths.push_back(t_path);

		 delete_tree(_head);

		 circuit_output.push_back(t_paths);
	 }

	 for (size_t i = 0; i < FF.size(); i++)
	 {
		 std::vector <calculatepath> f_path;
		 std::vector <std::string > f_min;
		 std::vector <std::string > f_max;
		 std::vector <std::string > f_coni;

		 Paths t_paths;

		 //get the name of the FF in order to associate it min, max and coni
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

	 if (FF.size() > 0)
	 {
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

			 //if an output contains FF we have to add also the path of the FF in order to find min, max and coni
			 for (size_t i = 0; i < t_path.size(); i++)
			 {
				 for (size_t j = 0; j < circuit_FF.size(); j++)
				 {
					 if (t_path[i].label == circuit_FF[j].label)
					 {
						 for (size_t k = 0; k < circuit_FF[j].paths.size(); k++)
						 {
							 for (size_t l = 0; l < circuit_FF[j].paths[k].size(); l++)
							 {
								 calculatepath temp;
								 //add to the FF paths the path between FF and the output
								 circuit_FF[j].paths[k][l].path += t_path[i].path;
								 temp.label = circuit_FF[j].paths[k][l].label;
								 temp.path = circuit_FF[j].paths[k][l].path;
								 t_path.push_back(temp);
							 }
						 }
						 t_path.erase(t_path.begin() + i);
					 }
				 }
			 }
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
 }


 