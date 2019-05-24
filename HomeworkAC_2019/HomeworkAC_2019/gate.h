#pragma once

#include "signal.h"
#include <vector>

//Generic Gate
class gate{
protected:
	//Each Gate has many input signal and only 1 output signal
	std::vector<signal_input> signal_in;
	signal_output signal_out;

public:
	 
};

//AND Gate
class AND : public gate {
public: 
	AND();
};

//OR Gate
class OR : public gate {
public: 
	OR();
};

//NOT Gate
class NOT : public gate {
private:
	//For now, OR gates can handle only 1 input
	signal_input signal_in_not;
public:
	NOT();
};
