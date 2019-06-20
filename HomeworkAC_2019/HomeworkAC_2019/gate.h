#pragma once

#include "signal.h"
#include <vector>
#include <iterator>

//Generic Gate
class gate{
protected:
	//Each Gate has many input signal and only 1 output signal
	std::vector<signal_input> signal_in;
	signal_output signal_out;

public:
	gate();
	bool Read();
};

//AND Gate
class AND : public gate {
public: 
	AND(const  std::vector<signal_input> & _signal_in);
};

//NAND GATE
class NAND : public gate {
public:
	NAND(const  std::vector<signal_input> & _signal_in);
};

//OR Gate
class OR : public gate {
public: 
	OR(const std::vector<signal_input> & _signal_in);
};

//XOR GATE
class XOR : public gate {
public:
	XOR (const std::vector<signal_input> & _signal_in);
};

//NOR GATE
class NOR : public gate {
public:
	NOR(const std::vector<signal_input> & _signal_in);
};

//XNOR GATE
class XNOR : public gate {
public:
	XNOR(const std::vector<signal_input> & _signal_in);
};

//NOT Gate
class NOT : public gate {
private:
	signal_input signal_in;
 public:
	NOT(const signal_input &_signal_in);
};



