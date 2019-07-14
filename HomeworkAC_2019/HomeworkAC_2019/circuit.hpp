#pragma once
#include "btree.hpp"
#include "stringParser.hpp"

struct Instance {
	std::string label_circuitFrom;
	std::vector <std::string> from_circuit;
	std::vector <std::string> to_circuit;
};
struct powerMatrix {
	std::vector < std::vector < int > > vect_and;
	std::vector < std::vector < int	> > vect_or;
	std::vector < std::vector < int	> > vect_nor;
	std::vector < std::vector < int	> > vect_xor;
	std::vector < std::vector < int > > vect_nand;
	std::vector < std::vector < int > > vect_xnor;
	std::vector < std::vector < int > > vect_not;
	std::vector < std::vector < int > > vect_FF;
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


	std::vector <std::vector <signal_output>> simulation(const std::string & file_name, const long int &_clk=0);

	std::vector<Instance> getInstance();

	std::vector <signal_input> getInput();
	std::vector <signal_output> getOutput();
	std::vector <flipflop> getFF();
	
	void update(const std::vector <signal_output> &_output, const std::vector <flipflop> &_FF);
	void createTree();
	void setInstance(std::string & _instance);

	bool getComposed();

	double getPower();

private:
	std::string label;
	std::vector <signal_input> input;
	std::vector <signal_output> output;
	std::vector <std::vector <signal_output>> simulation_output;
	std::vector <std::vector <signal_input>> simulation_input;
	std::vector <std::vector<flipflop>> simulation_FF;

	std::vector <flipflop> FF;

	std::vector <std::string> circuit_instance;

	std::vector <std::string> min, max;

	//struct that keeps the name and all the paths
	struct Paths {
		std::string label;

		std::vector <std::string> min_Path;
		std::vector <std::string> max_Path;
		std::vector <std::vector <std::string>> coni_Logici;
		std::vector <std::vector <calculatepath>> paths;
	};

	std::vector <Paths> circuit_output, circuit_FF;

	double power;
	bool isSequential;
	bool isComposted;
	long double clk;

	double t_power(std::vector < std::vector< int> > _vect, const double & _high, const double & _low);
	void ClkNedd( flipflop & _flipf);
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

inline double circuit::t_power(std::vector < std::vector< int> > _vect, const double & _high, const double & _low) {
	 double tt_power = 0;
	 if (_vect.size() > 0) {
		 for (size_t i = 0; i < _vect.size(); i++)
		 {
			 if (_vect[i][0] == SIGNAL_HIGH)
			 {
				 tt_power += _high;
			 }
			 for (size_t j = 1; j < _vect[i].size(); j++)
			 {
				 // if rising
				 if ((_vect[i][j] - _vect[i][j-1]) > 0)
				 {
					 tt_power += _high;
				 }

				 //if lower
				 if ((_vect[i][j] - _vect[i][j - 1]) < 0)
				 {

					 tt_power += _low;
				 }
			 }
		 }
	 }
	 return tt_power;
 }

inline void circuit::ClkNedd (flipflop & _flipf)
{
	int simple = 0;
	int max = 1;
	int flag = 0;

	for (size_t i = 0; i < _flipf.getFound().size(); i++)
	{
		for (size_t j = 0; j < FF.size(); j++)
		{
			if (_flipf.getFound()[i] == FF[j].FF_getLabel())
			{
				//FF is simple so does not contains other FF
				if (FF[j].getClkN() == 1)
				{
					simple++;
				}
				//found a FF that is not simple, so we have to check if the not simple FF contains simple or not simple FF
				else
				{
					if (max < FF[j].getClkN())
					{
						max = FF[j].getClkN();
						flag = 1;
					}
				}
			}
		}
	}

	if (_flipf.getFound().size()>0)
	{
		//FF contains only simple FF so the clk needed is 1
		if (_flipf.getFound().size() == simple)
		{
			_flipf.setClkN(2);
		}
		if (flag==1)
		{
			_flipf.setClkN(max + 1);
		}
	}

}

 inline double circuit::getPower()
 {
	 std::vector <powerDef> powerLosses=checkPower();
	 int max=0;

	 for (size_t i = 0; i < output.size()-1; i++)
	 {
		 if (output[i].getClk()>output[i+1].getClk())
		 {
			 max = output[i].getClk();
		 }
	 }
	 
	 if (clk< max)
	 {
		 std::cerr << "ERROR: power is impossible to calculate because the clk given is not enough to set all output" << std::endl;
		 return 0;
	 }
	 
	 clk -= max;
	 if (clk>simulation_input.size())
	 {
		 std::cerr << "ERROR: the clk is bigger than the input given" << std::endl;
		 return 0;
	 }
	 for (size_t j = 0; j < FF.size(); j++)
	 {
		 powerMatrix FF_power;

		 //for each clk save the valu eof each FF and solve their string 
		 //in order to calculate the powerloss of each operation

		 for (size_t k = 0; k < clk; k++)
		 {

			 std::string new_parse = FF[j].FF_getParse();
			 int counter_p = getPositions(FF[j].FF_getParse(), "(").size();

			 std::vector < int > not_vect;
			 std::vector < int > and_vect;
			 std::vector < int > or_vect;
			 std::vector < int > xor_vect;
			 std::vector < int > nand_vect;
			 std::vector < int > nor_vect;
			 std::vector < int > xnor_vect;
			 std::vector <int  > FF_vect;

			 //solve the sentence
			 for (int i = 0; i < counter_p + 1; i++)
			 {
				 //get the positions of opened and closed brackets
				 positionOpen = getPositions(new_parse, "(");
				 positionClose = getPositions(new_parse, ")");


				 size_t counterOpen = 0;
				 size_t counterClose = 0;
				 int flag = 0;

				 //there are bracktes left
				 if (positionOpen.size() != 0 && positionClose.size() != 0)
				 {

					 while (counterOpen < positionOpen.size() - 1 && flag == 0)
					 {
						 //find the inner brackets
						 if (positionOpen[counterOpen] < positionClose[counterClose] && positionOpen[counterOpen + 1] < positionClose[counterClose])
						 {
							 counterOpen++;
						 }
						 else
						 {
							 flag = 1;
						 }
					 }

					 //take the sentence inside the brackets
					 std::string s = new_parse.substr(positionOpen[counterOpen] + 1, positionClose[counterClose] - positionOpen[counterOpen] - 1);

					 std::stringstream ss(s);
					 std::vector<std::string> tokens;
					 std::string token;

					 //divide the string by spaces
					 while (std::getline(ss, token, ' '))
					 {
						 tokens.push_back(token);
					 }

					 int out_now;
					 std::stringstream s_out;
					 
					 //solve the sentence
					
					 signal_output t_out = solve(tokens, simulation_input[k], simulation_FF[k]);

					 out_now = t_out.Read();
					 s_out << t_out.Read();
					 

					 //replace the inner bracket with the resolution of the boolean operator
					 new_parse.replace(positionOpen[counterOpen], positionClose[counterClose] - positionOpen[counterOpen] + 1, s_out.str());

					 positionClose.erase(positionClose.begin() + counterClose);
					 positionOpen.erase(positionOpen.begin() + counterOpen);

					 if (tokens[0] == "NOT")	not_vect.push_back(out_now);
					 if (tokens[1] == "AND")	and_vect.push_back(out_now);
					 if (tokens[1] == "NAND")	nand_vect.push_back(out_now);
					 if (tokens[1] == "OR")		or_vect.push_back(out_now);
					 if (tokens[1] == "XOR")	xor_vect.push_back(out_now);
					 if (tokens[1] == "NOR")	nor_vect.push_back(out_now);
					 if (tokens[1] == "XNOR")	xnor_vect.push_back(out_now);

					 for (size_t l = 0; l < simulation_FF[k].size(); l++)
					 {
						 if (tokens[0] == simulation_FF[k][l].FF_getLabel());
						 {
							 FF_vect.push_back(simulation_FF[k][l].FF_Read());
						 }
						 if (tokens.size() == 3)
						 {
							 if (tokens[2] == simulation_FF[k][l].FF_getLabel())
							 {
								 FF_vect.push_back(simulation_FF[k][l].FF_Read());
							 }
						 }
					 }

				 }
				 else
				 {
					 std::stringstream ss(new_parse);
					 std::vector<std::string> tokens;
					 std::string token;

					 while (std::getline(ss, token, ' ')) //divide the string by space
					 {
						 tokens.push_back(token);
					 }

					 int out_now;
					 std::stringstream s_out;

					 //solve the sentence
					signal_output t_out = solve(tokens, simulation_input[k], simulation_FF[k]);
					out_now = t_out.Read();
					s_out << t_out.Read();

					 new_parse.replace(new_parse.begin(), new_parse.end(), s_out.str());

					 if (tokens[0] == "NOT")	not_vect.push_back(out_now);
					 if (tokens[1] == "AND")	and_vect.push_back(out_now);
					 if (tokens[1] == "NAND")	nand_vect.push_back(out_now);
					 if (tokens[1] == "OR")		or_vect.push_back(out_now);
					 if (tokens[1] == "XOR")	xor_vect.push_back(out_now);
					 if (tokens[1] == "NOR")	nor_vect.push_back(out_now);
					 if (tokens[1] == "XNOR")	xnor_vect.push_back(out_now);

					 for (size_t l = 0; l < simulation_FF[k].size(); l++)
					 {
						 if (tokens[0] == simulation_FF[k][l].FF_getLabel());
						 {
							 FF_vect.push_back(simulation_FF[k][l].FF_Read());
						 }
						 if (tokens.size() == 3)
						 {
							 if (tokens[2] == simulation_FF[k][l].FF_getLabel())
							 {
								 FF_vect.push_back(simulation_FF[k][l].FF_Read());
							 }
						 }
					 }
				 }
			 }
			 
			 //save the value of each operations for each clk
			 if (not_vect.size() > 0)	FF_power.vect_not.push_back(not_vect);
			 if (and_vect.size() > 0)	FF_power.vect_and.push_back(and_vect);
			 if (nand_vect.size() > 0)	FF_power.vect_nand.push_back(nand_vect);
			 if (nor_vect.size() > 0)	FF_power.vect_nor.push_back(nor_vect);
			 if (or_vect.size() > 0)	FF_power.vect_or.push_back(or_vect);
			 if (xnor_vect.size() > 0)	FF_power.vect_xnor.push_back(xnor_vect);
			 if (xor_vect.size() > 0)	FF_power.vect_xor.push_back(xor_vect);
			 if (FF_vect.size() > 0)	FF_power.vect_FF.push_back(FF_vect);

		 }
		 //calculate the total powerloss
		 for (size_t k = 0; k < powerLosses.size(); k++)
		 {
			 if (powerLosses[k].gate == "NOT")	power += t_power(FF_power.vect_not, powerLosses[k].to1, powerLosses[k].to0);
			 if (powerLosses[k].gate == "AND")	power += t_power(FF_power.vect_and, powerLosses[k].to1, powerLosses[k].to0);
			 if (powerLosses[k].gate == "NAND") power += t_power(FF_power.vect_nand, powerLosses[k].to1, powerLosses[k].to0);
			 if (powerLosses[k].gate == "NOR")	power += t_power(FF_power.vect_nor, powerLosses[k].to1, powerLosses[k].to0);
			 if (powerLosses[k].gate == "OR")	power += t_power(FF_power.vect_or, powerLosses[k].to1, powerLosses[k].to0);
			 if (powerLosses[k].gate == "XNOR") power += t_power(FF_power.vect_xnor, powerLosses[k].to1, powerLosses[k].to0);
			 if (powerLosses[k].gate == "XOR")	power += t_power(FF_power.vect_xor, powerLosses[k].to1, powerLosses[k].to0);
			 if (powerLosses[k].gate == "FF")	power += t_power(FF_power.vect_FF, powerLosses[k].to1, powerLosses[k].to0);
		 }
	 }

	 int isFF = 0;

	 if (FF.size()>0)
	 {
		 isFF = 1;
	 }

	 for (size_t j = 0; j < output.size(); j++)
	 {
		 powerMatrix output_power;

		 for (size_t k = 0; k < clk; k++)
		 {
			 
			 std::string new_parse = output[j].getParse();
			 int counter_p = getPositions(output[j].getParse(), "(").size();
			 
			 std::vector < int > not_vect ;
			 std::vector < int > and_vect ;
			 std::vector < int > or_vect  ;
			 std::vector < int > xor_vect ;
			 std::vector < int > nand_vect;
			 std::vector < int > nor_vect ;
			 std::vector < int > xnor_vect;
			 std::vector <int> FF_vect;


			 for (int i = 0; i < counter_p + 1; i++)
			 {
				 //get the positions of opened and closed brackets
				 positionOpen = getPositions(new_parse, "(");
				 positionClose = getPositions(new_parse, ")");


				 size_t counterOpen = 0;
				 size_t counterClose = 0;
				 int flag = 0;

				 //there are bracktes left
				 if (positionOpen.size() != 0 && positionClose.size() != 0) 
				 {

					 while (counterOpen < positionOpen.size() - 1 && flag == 0)
					 {
						 //find the inner brackets
						 if (positionOpen[counterOpen] < positionClose[counterClose] && positionOpen[counterOpen + 1] < positionClose[counterClose])
						 {
							 counterOpen++;
						 }
						 else
						 {
							 flag = 1;
						 }
					 }

					 //take the sentence inside the brackets
					 std::string s = new_parse.substr(positionOpen[counterOpen] + 1, positionClose[counterClose] - positionOpen[counterOpen] - 1);

					 std::stringstream ss(s);
					 std::vector<std::string> tokens;
					 std::string token;

					 //divide the string by spaces
					 while (std::getline(ss, token, ' '))
					 {
						 tokens.push_back(token);
					 }

					 int out_now;
					 std::stringstream s_out;
					 //solve the sentence
					 if (isFF ==1)
					 {
						 signal_output t_out = solve(tokens, simulation_input[k], simulation_FF[k]);
						 out_now = t_out.Read();
						 s_out << t_out.Read();
					 }
					 else
					 {
						 signal_output t_out = solve(tokens, simulation_input[k]);
						 out_now = t_out.Read();
						 s_out << t_out.Read();
					 }
					
					 //replace the inner bracket with the resolution of the boolean operator
					 new_parse.replace(positionOpen[counterOpen], positionClose[counterClose] - positionOpen[counterOpen] + 1, s_out.str());

					 positionClose.erase(positionClose.begin() + counterClose);
					 positionOpen.erase(positionOpen.begin() + counterOpen);

					 if (tokens[0] == "NOT")	not_vect.push_back(out_now);
					 if (tokens[1] == "AND")	and_vect.push_back(out_now);
					 if (tokens[1] == "NAND")	nand_vect.push_back(out_now);
					 if (tokens[1] == "OR")		or_vect.push_back(out_now);
					 if (tokens[1] == "XOR")	xor_vect.push_back(out_now);
					 if (tokens[1] == "NOR")	nor_vect.push_back(out_now);
					 if (tokens[1] == "XNOR")	xnor_vect.push_back(out_now);
					 for (size_t l = 0; l < simulation_FF[k].size(); l++)
					 {
						 if (tokens[0] == simulation_FF[k][l].FF_getLabel());
						 {
							 FF_vect.push_back(simulation_FF[k][l].FF_Read());
						 }
						 if (tokens.size() == 3)
						 {
							 if (tokens[2] == simulation_FF[k][l].FF_getLabel())
							 {
								 FF_vect.push_back(simulation_FF[k][l].FF_Read());
							 }
						 }
					 }

				 }
				 else
				 {
					 std::stringstream ss(new_parse);
					 std::vector<std::string> tokens;
					 std::string token;

					 while (std::getline(ss, token, ' ')) //divide the string by space
					 {
						 tokens.push_back(token);
					 }

					 int out_now;
					 std::stringstream s_out;
					 //solve the sentence
					 if (isFF == 1)
					 {
						 signal_output t_out = solve(tokens, simulation_input[k], simulation_FF[k]);
						 out_now = t_out.Read();
						 s_out << t_out.Read();
					 }
					 else
					 {
						 signal_output t_out = solve(tokens, simulation_input[k]);
						 out_now = t_out.Read();
						 s_out << t_out.Read();
					 }

					 new_parse.replace(new_parse.begin(), new_parse.end(), s_out.str());

					 if (tokens[0] == "NOT")	not_vect.push_back(out_now);
					 if (tokens[1] == "AND")	and_vect.push_back(out_now);
					 if (tokens[1] == "NAND")	nand_vect.push_back(out_now);
					 if (tokens[1] == "OR")		or_vect.push_back(out_now);
					 if (tokens[1] == "XOR")	xor_vect.push_back(out_now);
					 if (tokens[1] == "NOR")	nor_vect.push_back(out_now);
					 if (tokens[1] == "XNOR")	xnor_vect.push_back(out_now);

					for (size_t l = 0; l < simulation_FF[k].size(); l++)
					{
						if (tokens[0] == simulation_FF[k][l].FF_getLabel());
						{
							FF_vect.push_back(simulation_FF[k][l].FF_Read());
						}
						if (tokens.size()==3)
						{
							if (tokens[2] == simulation_FF[k][l].FF_getLabel())
							{
								FF_vect.push_back(simulation_FF[k][l].FF_Read());
							}
						}
					}
				}
			 }
			 if (not_vect.size()	> 0)	output_power.vect_not.push_back(not_vect);
			 if (and_vect.size()	> 0)	output_power.vect_and.push_back(and_vect);
			 if (nand_vect.size()	> 0)	output_power.vect_nand.push_back(nand_vect);
			 if (nor_vect.size()	> 0)	output_power.vect_nor.push_back(nor_vect);
			 if (or_vect.size()		> 0)	output_power.vect_or.push_back(or_vect);
			 if (xnor_vect.size()	> 0)	output_power.vect_xnor.push_back(xnor_vect);
			 if (xor_vect.size()	> 0)	output_power.vect_xor.push_back(xor_vect);
			 if (FF_vect.size()		> 0)	output_power.vect_FF.push_back(FF_vect);

		 } 	

		 for (size_t k = 0; k < powerLosses.size(); k++)
		 {
			 if (powerLosses[k].gate == "NOT")	power += t_power(output_power.vect_not, powerLosses[k].to1, powerLosses[k].to0);
			 if (powerLosses[k].gate == "AND")	power += t_power(output_power.vect_and, powerLosses[k].to1, powerLosses[k].to0);
			 if (powerLosses[k].gate == "NAND") power += t_power(output_power.vect_nand, powerLosses[k].to1, powerLosses[k].to0);
			 if (powerLosses[k].gate == "NOR")	power += t_power(output_power.vect_nor, powerLosses[k].to1, powerLosses[k].to0);
			 if (powerLosses[k].gate == "OR")	power += t_power(output_power.vect_or, powerLosses[k].to1, powerLosses[k].to0);
			 if (powerLosses[k].gate == "XNOR") power += t_power(output_power.vect_xnor, powerLosses[k].to1, powerLosses[k].to0);
			 if (powerLosses[k].gate == "XOR")	power += t_power(output_power.vect_xor, powerLosses[k].to1, powerLosses[k].to0);
			 if (powerLosses[k].gate == "FF")	power += t_power(output_power.vect_FF, powerLosses[k].to1, powerLosses[k].to0);
		 }
	 }

	 return this->power;
}
	 
 inline std::vector<std::vector<signal_output>> circuit::simulation(const std::string & file_name, const long int &_clk)
 {
	 filename_inputSignal = file_name;
	 open_inputFile();
	 clk = _clk;

	 int innerCount = 0;
		
	 if (isSequential==false)
	 {
		 clk = 1;

		 //combinatorio circuits and it only needs the first line written in the input file
		 if (input.size()<=vect_matrix[0].size())
		 {
			 if (input.size() < vect_matrix[0].size()) std::cerr << "WARNING: too many input defined in the file" << std::endl;

			 for (size_t i = 0; i < input.size(); i++)
			 {
				 //each input has its own label and now also its own value
				 input[i].Set(vect_matrix[0][i]);
			 }

			 simulation_input.push_back(input);

			 std::vector<signal_output> vect_output;

			 for (size_t i = 0; i < output.size(); i++)
			 {
				 if (output[i].getParse().size()!=0)
				 {
					 //takes the input and give back the value of the output
					 signal_output t_output(output[i].getLabel(), getValue(output[i].getParse(), input));
					 vect_output.push_back(t_output);
				 }
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
		 int checkCLK=0;

		 if (clk <= vect_matrix.size())
		 {
			 //get the clk needed for each FF
			 for (size_t j = 0; j < FF.size(); j++)
			 {
				 ClkNedd(FF[j]);
			 }
			 //get the clk needed for each output that contains a FF 
			 for (size_t j = 0; j < output.size(); j++)
			 {
				 int max = 0;

				 for (size_t i = 0; i < output[j].get_FF().size(); i++)
				 {
					 for (size_t k = 0; k < FF.size(); k++)
					 {
						 if (output[j].get_FF()[i] == FF[k].FF_getLabel())
						 {
							 if (FF[k].getClkN() > max)
							 {
								 max = FF[k].getClkN();
							 }
						 }
					 }
				 }
				 output[j].setClk(max);
			 }

			 for (int i = 0; i < clk; i++)
			 {
				 std::vector <signal_input> t_vect;

				 for (int j = 0; j < input.size(); j++)
				 {
					 signal_input t_input(input[j].getLabel(), vect_matrix[i][j]);
					 t_vect.push_back(t_input);
				 }
				 simulation_input.push_back(t_vect);

				 std::vector <flipflop> vect_FF;

				 for (int j = 0; j < FF.size(); j++)
				 {
					 if (FF[j].FF_getParse().size()!=0)
					 {
						 if (FF[j].getClkN()>(i+1))
						 {
							 FF[j].setUndefined();
							 vect_FF.push_back(FF[j]);
						 }
						 else
						 {
							 flipflop t_FF(FF[j].FF_getLabel(), getValue(FF[j].FF_getParse(), t_vect, FF), clk);
							 vect_FF.push_back(t_FF);
						 }
						
					 }
				 }
				 simulation_FF.push_back(vect_FF);
			
				 std::vector<signal_output> vect_output;

				 for (size_t j = 0; j < output.size(); j++)
				 {
					 if (output[j].getParse().size()!=0)
					 {
						 if (output[j].getClk()>(i+1))
						 {
							 output[j].Set(SIGNAL_NOT_DEFINED);
							 vect_output.push_back(output[j]);
						 }
						 else
						 {
							 //takes the input and give back the value of the output
							 signal_output t_output(output[j].getLabel(), getValue(output[j].getParse(), t_vect, vect_FF));
							 vect_output.push_back(t_output);
						 }
					 }
				 }
				 simulation_output.push_back(vect_output); 
			 }
			/*for (int i = 0; i < simulation_output.size(); i++)
			 {
				 for (size_t j = 0; j < simulation_output[i].size(); j++)
				 {
					 std::cout << "label: " << simulation_output[i][j].getLabel() << " valore: " << simulation_output[i][j].getValue() << std::endl;
				 }
			 }*/
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
	 if (FF.size()==0)
	 {
		 //the circuit can be sequential but for sure there are no FF
		 for (size_t i = 0; i < output.size(); i++)
		 {
			 if (output[i].getParse().size() != 0)
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
				 t_paths.min_Path=min;
				 t_paths.max_Path=max;
				 t_paths.coni_Logici.push_back(coni);
				 t_paths.paths.push_back(t_path);

				 delete_tree(_head);

				 circuit_output.push_back(t_paths);
			 }
		 }
	 }

	 for (size_t e = 0; e < FF.size(); e++)
	 {
		 if (FF[e].FF_getParse().size()!=0)
		 {
			 std::vector <calculatepath> f_path;
			 std::vector <std::string > f_min;
			 std::vector <std::string > f_max;
			 std::vector <std::string > f_coni;

			 Paths t_paths;

			 //get the name of the FF in order to associate it min, max and coni
			 t_paths.label = FF[e].FF_getLabel();

			 btree * F_head;
			 F_head = builtTree(FF[e].FF_getParse());
			 if (F_head->left==NULL && F_head->right==NULL)
			 {
				 calculatepath tt;
				 tt.label = F_head->value;
				 tt.path = 0;
				 std::vector <calculatepath> vect;
				 vect.push_back(tt);
				 t_paths.min_Path.push_back(F_head->value);
				 t_paths.max_Path.push_back(F_head->value);
				 std::vector <std::string> t;
				 t.push_back(F_head->value);
				 t_paths.coni_Logici.push_back(t);
				 t_paths.paths.push_back(vect);
			 }
			 else
			 {
				 f_path = Path(F_head);

				 int flag = 0;
				 //if an output contains FF we have to add also the path of the FF in order to find min, max and coni
				 for (size_t i = 0; i < f_path.size(); i++)
				 {
					 if (flag == 1 && i != 0)
					 {
						 i = 0;
					 }
					 for (size_t j = 0; j < circuit_FF.size(); j++)
					 {
						 if (f_path[i].label == circuit_FF[j].label)
						 {
							 for (size_t k = 0; k < circuit_FF[j].paths.size(); k++)
							 {
								 for (size_t l = 0; l < circuit_FF[j].paths[k].size(); l++)
								 {
									 calculatepath temp;
									 //add to the FF paths the path between FF and the output
									 f_path[i].path += circuit_FF[j].paths[k][l].path;
									 temp.label = circuit_FF[j].paths[k][l].label;
									 temp.path = f_path[i].path;
									 f_path.push_back(temp);
								 }
							 }
							 f_path.erase(f_path.begin() + i);
							 flag = 1;
						 }
						 else
						 {
							 flag = 0;
						 }
					 }
				 }
				 f_min = findMin(f_path);
				 f_max = findMax(f_path);
				 f_coni = coniLogici(f_path);

				 t_paths.min_Path = f_min;
				 t_paths.max_Path = f_max;
				 t_paths.coni_Logici.push_back(f_coni);
				 t_paths.paths.push_back(f_path);
				 delete_tree(F_head);
			 }
			 circuit_FF.push_back(t_paths);
		 }
	 }

	 if (FF.size() > 0)
	 {
		 for (size_t e = 0; e < output.size(); e++)
		 {
			 if (output[e].getParse().size() != 0) 
			 {
				 std::vector <calculatepath> t_path;
				 std::vector <std::string > min;
				 std::vector <std::string > max;
				 std::vector <std::string > coni;

				 Paths t_paths;
				 t_paths.label = output[e].getLabel();

				 if (output[e].get_FF().size()==1)
				 {
					 for (size_t j = 0; j < circuit_FF.size(); j++)
					 {
						 if (output[e].get_FF()[0]==circuit_FF[j].label)
						 {
							 t_paths.paths = circuit_FF[j].paths;
							 t_paths.min_Path = circuit_FF[j].min_Path;
							 t_paths.max_Path = circuit_FF[j].max_Path;
							 t_paths.coni_Logici = circuit_FF[j].coni_Logici;
						 }
					 }
				 }
				 else
				 {
					 btree *_head;
					 _head = builtTree(output[e].getParse());
					 t_path = Path(_head);
					 int m_flag = 0;

					 //if an output contains FF we have to add also the path of the FF in order to find min, max and coni
					 for (size_t i = 0; i < t_path.size(); i++)
					 {
						 if (m_flag == 1 && i != 0)
						 {
							 i = 0;
						 }
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
								 m_flag = 1;
							 }
							 else
							 {
								 m_flag = 0;
							 }
						 }
					 }
					 min = findMin(t_path);
					 max = findMax(t_path);
					 coni = coniLogici(t_path);

					 t_paths.min_Path = min;
					 t_paths.max_Path = max;
					 t_paths.coni_Logici.push_back(coni);
					 t_paths.paths.push_back(t_path);
					 delete_tree(_head);
				 }
				
				 circuit_output.push_back(t_paths);
			 }
		 }
	 }
	
 }


 