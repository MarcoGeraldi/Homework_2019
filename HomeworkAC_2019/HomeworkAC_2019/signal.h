#pragma once

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
	//KIND: Select between input or output signal
	bool kind;
	//VALUE: Actual state of the signal, can be HIGH - OR - NOT_DEFINED
	int value;
	//LABEL: Associate each signal with a char
	char label;

public:
	 
	//READ FUNCTION: returns the value of the signal
	int Read();
	//SET FUNCTION: Set the value High or low
	void Set(const bool & _value);
	
	void operator=	(const signal & to_assign);
	friend signal operator! (const signal & to_invert);

};

//output signal
class signal_output : public signal
{
public:
	signal_output( const char &_label,  const bool &_value = 0);
	signal_output();

};
 
//input signal
class signal_input :public signal {
public:
	signal_input(const char &_label, const bool &_value=0);
	signal_input();
};
