#pragma once
 
#include <sstream>
#include <vector>
#include <iostream>
//SIGNAL CLASS:

/*
There are two main types of signal: input or output

FF has general output signal '-1'
*/
#define SIGNAL_INPUT 1
#define SIGNAL_OUTPUT 0

#define SIGNAL_HIGH 1
#define SIGNAL_LOW 0
#define SIGNAL_NOT_DEFINED -1

//Generic signal
class signal
{
protected:
	//KIND: Select between INPUT or OUTPUT signal
	bool kind;
	//VALUE: Actual state of the signal, can be HIGH/LOW - OR - NOT_DEFINED
	int value;
	//LABEL: Associate each signal with a char
	std::string label;

public:
	 
	//READ FUNCTION: returns the value of the signal
	int Read();
	std::string getLabel();
	//SET FUNCTION: Set the value High or low
	void Set(const int & _value);
	void Set(const std::string &_value);
	
	void operator=	(const signal & to_assign);
	friend signal operator! (const signal & to_invert);

};

//output signal
class signal_output : public signal
{
public:
	signal_output(const std::string &_label,  const bool &_value = 0);
	signal_output(const std::string &_label, const std::string &_to_parse, const bool &_value =0 );//to_parse: in order to get paths
	signal_output();

	std::string getParse();
	int getValue();

private:
	std::string to_parse;
};
 
//input signal
class signal_input :public signal {
public:
	signal_input(const std::string &_label, const bool &_value = 0);
	signal_input();
};
