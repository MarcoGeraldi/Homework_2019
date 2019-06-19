//HomeworkAC_2019

// Marco Geraldi	<248825>
// Margherita Gili	<249774>

#include "HomeworkAC_2019.h"
#include "signal.h"
#include "gate.h"
#include "filecheck.hpp"
#include "btree.hpp"

int main(){ 
	
	string input = "((l NAND m) AND (c OR d) ) OR (d XOR e)";
	//cout << "value is " << getValue(input) << endl;

	//filename_circuitDescr = "descrizione.txt";
	//check_circuitDescr();
	btree * head;
	head= builtTree(input);
	std::cout << "testa:" << head->value << std::endl
		<< "dx di testa:" << head->right->value << endl
		<< "sx di XOR:" << head->right->left->value << endl
		<< "sx di AND: " << head->left->left->value << endl
		<< "dx di NAND: " << head->left->left->right->value << endl;
	return 0;
}



 