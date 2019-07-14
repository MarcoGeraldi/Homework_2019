#include "flipflop.h"

flipflop::flipflop(const std::string & _FF_label, const std::string & _FF_to_parse, const long int & _clkNum)
{
	this->clkNum = _clkNum;
	this->FF_to_parse = _FF_to_parse;
	this->FF_label = _FF_label;
	this->Q.Set(D.Read());
	this->_Q.Set(!D.Read());

	calculateClk();
}

flipflop::flipflop(const std::string & _FF_label, const int & _value, const long int & _clkNum)
{
	this->FF_label = _FF_label;
	this->clkNum = _clkNum;
	this->value = _value;
	this->D.Set(value);
	this->Q.Set(D.Read());
	this->_Q.Set(!D.Read());
}

std::string flipflop::FF_getParse()
{
	return this->FF_to_parse;
}

std::string flipflop::FF_getLabel()
{
	return this->FF_label;
}

 int flipflop::FF_Read()
{
	return this->Q.Read();
}

 int flipflop::notFF_Read()
 {
	 return this->_Q.Read();
 }

 int flipflop::getClkN()
 {
	 return this->clkNeeded;
 }

 void flipflop::setClkN(const int & _need)
 {
	 this->clkNeeded = _need;
 }

 std::vector<std::string> flipflop::getFound()
 {
	 return this->FF_found;
 }

 void flipflop::calculateClk()
 {
	 std::string token;
	 std::string to_find = { "FF" };
	 std::string cut;

	 std::string use = FF_to_parse;
	
	 for (size_t i = 0; i <use.length(); i++)
	 {
		 //look for FF
		 if (FF_to_parse.substr(i, to_find.length()) == "FF")
		 {
			 cut= use.substr(i, use.size()-i);
			 std::stringstream s(cut);
			 int pos_space = cut.find(" ");
			 int pos_bracket = cut.find(")");

			 //the first found was space
			 if (pos_space > 0 && pos_bracket > 0)
			 {
				 if (pos_space<pos_bracket)
				 {
					 std::getline(s, token, ' ');
					 FF_found.push_back(token);
				 }
				 else
				 {
					 std::getline(s, token, ')');
					 FF_found.push_back(token);
				 }
			 }
			 else if(pos_space>0 && pos_bracket<0)
			 {
				 std::getline(s, token, ' ');
				 FF_found.push_back(token);
			 }
			 else if (pos_space<0 && pos_bracket>0)
			 {
				 std::getline(s, token, ')');
				 FF_found.push_back(token);
			 }
			 else if (pos_bracket<0 && pos_space<0)
			 {
				 FF_found.push_back(cut);
			 }
		 }
	 }
 
	 if (FF_found.size()==0)
	 {
		 clkNeeded = 1;
	 }
 }

 void flipflop::setUndefined()
 {
	 this->Q.Set(SIGNAL_NOT_DEFINED);
	 this->_Q.Set(SIGNAL_NOT_DEFINED);
 }

