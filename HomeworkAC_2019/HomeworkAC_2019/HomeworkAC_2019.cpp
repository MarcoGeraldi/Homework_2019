//HomeworkAC_2019

// Marco Geraldi	<248825>
// Margherita Gili	<249774>

#include "HomeworkAC_2019.h"
#include "signal.h"
#include "gate.h"
#include "filecheck.hpp"
#include "btree.hpp"

int main(){ 
	
	string input = "(a AND (NOT b)) OR (c XOR (NOT (e NAND d)))";
	//cout << getValue(input);
	//cout << "value is " << getValue(input) << endl;
/*
	filename_circuitDescr = "descrizione.txt";
	check_circuitDescr();
	checkSignals();*/

	/*btree * head;
	head= builtTree(input);
	std::cout << "testa:" << head->value << std::endl
		<< "genitore di OR:" << head->right->right->right->parent->value << endl
		<< "genitore di a: " << head->left->parent->value << endl
		<< "genitore di AND: " << head->right->right->parent->value << endl;*/
	/*std::vector <calculatepath> cammino;
	btree *head;
	head = builtTree(input);
	cammino = Path(head);*/

	//vector <string> c, min, max;
	/*c = coniLogici(input);
	for (int i = 0; i < c.size(); i++)
	{
		std::cout << "gli input associati all'output sono: "
			<< c[i] << std::endl;
	}*/
	//min = findMin(input);
	//max = findMax(input);
	/*for (int i = 0; i < min.size(); i++)
	{
		std::cout << "gli input con percorso minore sono: "
			<< min[i] << std::endl;
	}*/
	/*for (int i = 0; i < max.size(); i++)
	{
		std::cout << "gli input con percorso maggiore sono: "
			<< max[i] << std::endl;
	}*/
	return 0;
}



 
