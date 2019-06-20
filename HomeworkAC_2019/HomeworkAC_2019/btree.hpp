#pragma once
#include "stringParser.hpp"
#include <algorithm>


struct calculatepath {
	int path;
	string label;
};


struct btree {
	std::string value;
	char colour = 'B'; //da cambiare in white and black
	btree * left, *right, *parent;
};

btree * newNode(std::string & _value_to_assign) {
	btree * temp = new btree;
	temp->left = nullptr;
	temp->right = nullptr;
	temp->parent = nullptr;
	temp->value = _value_to_assign;
	return temp;
}

btree * addChildren(btree * _node, btree * _left_child, btree * _right_child=nullptr) {

	_node->left = _left_child;
	_node->right = _right_child;
	_left_child->parent = _node;
	_right_child->parent = _node;
	return _node;
}

/*per costruire l'albero riutilizzo parte del codice del parser, cerco la parentesi più interna e ritaglio la frase al suo interno
e la divido per spazi salvandola in un vettore.

se dentro la parentesi c'è un NOT allora la dimensione del vettore sarà due, credo due nodi uno con NOT e uno con il segnale
e li collego insieme facendo diventare il segnale il figlio di NOT.

se dentro invece c'è una qualsiasi altra porta logica, allora la dimensione del vettore sarà 3 e crero 3 nodi e faccio diventare
la porta logica il genitore e i due segnali diventano i sui figli.

ogni qualvolta trovo una parentesi e creo il pezzo di albero sostituisco l'intera parentesi con un carattere speciale (*)
che mi ricorda che quel nodo non lo devo creare da zero ma devo riutilizzare un nodo già esistente salvato in un vettore chiamato special
che contiene l'indirizzo del nodo a cui devo puntare e che aggiorno ogni volta (aggiungendo o eliminando).

parto sempre dalla parentesi più interna partendo da sx e poi a andando a dx nel caso
*/

btree * builtTree(std::string & _input_string) {

	std::string new_input = _input_string;
	btree * head = nullptr;
	vector<size_t> positionOpenB;
	vector<size_t> positionCloseB;
	vector <btree *> special;

	//get how many brackets are in the initial sentence in order to know how many times the tree's code has to run
	int innerCounter = getPositions(_input_string, "(").size();
	
	//need the special character to highlight the place where there was a bracket before
	std::string special_character = "*";

	//has to do it as many times as the size of the bracktes plus one 
	for (int i = 0; i < innerCounter+1; i++)
	{
		//everytime the string newinput is upload and need to know how many brackets are in there
		positionOpenB = getPositions(new_input, "(");
		positionCloseB = getPositions(new_input, ")");

		int counterOpen = 0;
		int counterClose = 0;
		int flag = 0;

		//there are no brackets so the tree is almost finished, only the roots is missing
		if (positionOpenB.size() == 0 || positionCloseB.size() == 0) 
		{
			istringstream stream_temp(new_input);
			std::string string_temp;
			vector <std::string> vect_temp;

			//divide the sentence by spaces
			while (getline(stream_temp, string_temp,' '))
			{
				vect_temp.push_back(string_temp);
			}

			//if in the sentence there is a NOT then it enters in this if
			if (vect_temp.size()==2)
			{
				//there was a brackets before so no need of a new node but it's saved in the special vector
				if (vect_temp[1]==special_character)
				{
					btree * root = newNode(vect_temp[0]);
					btree * right_child = special.back();

					root->right = right_child; // connects the two nodes
					right_child->parent = root;
					return root;
				}

				else
				{
					btree * root = newNode(vect_temp[0]); //contains NOT
					btree * right_child = newNode(vect_temp[1]); //contains the signal

					root->right = right_child; //connects the two nodes
					right_child->parent = root;
					return root;
				}
			}

			//there are boolean operators different from NOT
			else
			{
				//the boolean operator was between two bracktes
				if (vect_temp[0] == special_character && vect_temp[2] == special_character)
				{
					btree * root = newNode(vect_temp[1]);

					//the right one was the last saved in the special vector
					btree * right_child = special.back(); 

					//delete the last element added because it was just saved in the right_child
					special.erase(special.end() - 1);

					//the left one was the first saved in special 
					btree * left_child = special.back();
					special.erase(special.end() - 1);

					root = addChildren(root, left_child, right_child);

					return root;
				}
				//there were brackets just on the  left on the boolean operator
				else if (!(vect_temp[0] == special_character && vect_temp[2] == special_character) && vect_temp[0] == special_character)
				{
					btree * root = newNode(vect_temp[1]);

					//the right child is the signal on the boolean operator's right
					btree * right_child = newNode(vect_temp[2]);

					//the left child is alread a node and it's saved in special
					root = addChildren(root, special.back(), right_child);

					return root;
				}
				//only on the right of the boolean operator there were brackets
				else if (!(vect_temp[0] == special_character && vect_temp[2] == special_character) && vect_temp[2] == special_character)
				{
					btree * root = newNode(vect_temp[1]);
					btree * left_child = newNode(vect_temp[0]);

					root = addChildren(root, left_child, special.back());

					return root;
				}
				else //there are no special character, then there were no brackets that were replace
				{
					//create three new nodes
					btree * root = newNode(vect_temp[1]);
					btree * left_child = newNode(vect_temp[0]);
					btree * right_child = newNode(vect_temp[2]);
					
					//the two signals become the boolean operator's children
					root = addChildren(root, left_child, right_child);

					return root;
				}
			}
		}

		//there are still bracktes to solve
		else
		{
			while (counterOpen < positionOpenB.size() - 1 && flag == 0)
			{
				//find the inner brackets
				if (positionOpenB[counterOpen] < positionCloseB[counterClose] && positionOpenB[counterOpen + 1] < positionCloseB[counterClose])
				{
					counterOpen++;
				}
				else
				{
					flag = 1;
				}
			}

			//take the sentence inside the brackets
			string s = new_input.substr(positionOpenB[counterOpen] + 1, positionCloseB[counterClose] - positionOpenB[counterOpen] - 1);
			
			stringstream stream_temp(s);
			vector<string> vect_temp;
			string string_temp;

			//divide the string by spaces
			while (getline(stream_temp, string_temp, ' '))
			{
				vect_temp.push_back(string_temp);
			}

			// in the sentece there is NOT
			if (vect_temp.size()==2)
			{
				//on the right of NOT there was a bracktes that was replace
				if (vect_temp[1]==special_character)
				{
					btree * general_node = newNode(vect_temp[0]);

					//no need to create a new node but takes it from special
					btree * right_child = special.back();
					general_node->right = right_child;
					right_child->parent = general_node;
					
					//delete the node cause it was already connected to its parent
					special.erase(special.end() - 1);

					//save the new part of tree in special
					special.push_back(general_node);

					//replace the inner bracket with the resolution of the boolean operator
					new_input.replace(positionOpenB[counterOpen], positionCloseB[counterClose] - positionOpenB[counterOpen] + 1, special_character);

					//delete the positionn of the brackets solved
					positionCloseB.erase(positionCloseB.begin() + counterClose);
					positionOpenB.erase(positionOpenB.begin() + counterOpen);
				}

				else
				{
					//there are just signals near NOT so it creates two new node
					btree * general_node = newNode(vect_temp[0]);
					btree * right_child = newNode(vect_temp[1]);

					//connects them
					general_node->right = right_child;
					right_child->parent = general_node;
					special.push_back(general_node);
					
					//replace the inner bracket with the resolution of the boolean operator
					new_input.replace(positionOpenB[counterOpen], positionCloseB[counterClose] - positionOpenB[counterOpen] + 1, special_character);

					//delete the positions of the brackets solved
					positionCloseB.erase(positionCloseB.begin() + counterClose);
					positionOpenB.erase(positionOpenB.begin() + counterOpen);
				}
			}

			//there are just boolean operators exept NOT inside the sentence
			else
			{
				//on the boolean operator's left there were brackets
				if (!(vect_temp[0] == special_character && vect_temp[2] == special_character) && vect_temp[0] == special_character)
				{
					btree * general_node = newNode(vect_temp[1]);
					btree * right_child = newNode(vect_temp[2]);

					head = addChildren(general_node, special.back(), right_child);

					//delete the node that was just saved in head and save it in special
					special.erase(special.end() - 1);
					special.push_back(head);

					//replace the inner bracket with the resolution of the boolean operator
					new_input.replace(positionOpenB[counterOpen], positionCloseB[counterClose] - positionOpenB[counterOpen] + 1, special_character);

					positionCloseB.erase(positionCloseB.begin() + counterClose);
					positionOpenB.erase(positionOpenB.begin() + counterOpen);
				}
				//on the boolean operator's right there were brackets
				else if (!(vect_temp[0] == special_character && vect_temp[2] == special_character) && vect_temp[2] == special_character)
				{
					btree * general_node = newNode(vect_temp[1]);
					btree * left_child = newNode(vect_temp[0]);

					head = addChildren(general_node, left_child, special.back());

					//delete the node that was just saved in head and save it in special
					special.erase(special.end() - 1);
					special.push_back(head);

					//replace the inner bracket with the resolution of the boolean operator
					new_input.replace(positionOpenB[counterOpen], positionCloseB[counterClose] - positionOpenB[counterOpen] + 1, special_character);

					positionCloseB.erase(positionCloseB.begin() + counterClose);
					positionOpenB.erase(positionOpenB.begin() + counterOpen);
				}
				// the boolean operator was between two brackets
				else if (vect_temp[0] == special_character && vect_temp[2] == special_character)
				{
					btree * general_node = newNode(vect_temp[1]);

					btree * right_child = special.back();

					//delete the last two element added in the vector
					special.erase(special.end() - 1);

					btree * left_child = special.back();
					special.erase(special.end() - 1);

					head = addChildren(general_node, left_child, right_child);
					
					//save all the pointers into a vector in order to reuse them if you find two special characters
					special.push_back(head); 

					//replace the inner bracket with the resolution of the boolean operator
					new_input.replace(positionOpenB[counterOpen], positionCloseB[counterClose] - positionOpenB[counterOpen] + 1, special_character);

					positionCloseB.erase(positionCloseB.begin() + counterClose);
					positionOpenB.erase(positionOpenB.begin() + counterOpen);

				}

				//there are no special characters, then there were no brackets that were replace
				else 
				{
					//create three new node in order to connect them
					btree * general_node = newNode(vect_temp[1]);
					btree * left_child = newNode(vect_temp[0]);
					btree * right_child = newNode(vect_temp[2]);

					head = addChildren(general_node, left_child, right_child);

					//save all the pointers into a vector in order to reuse them if you find two special characters
					special.push_back(head);

					//replace the inner bracket with the resolution of the boolean operator
					new_input.replace(positionOpenB[counterOpen], positionCloseB[counterClose] - positionOpenB[counterOpen] + 1, special_character);

					positionCloseB.erase(positionCloseB.begin() + counterClose);
					positionOpenB.erase(positionOpenB.begin() + counterOpen);
				}
			}
		}
	}	
}

btree* returnParent(btree * _node) {
	//in order to remind the visited nodes
	_node->colour = 'N';
	return _node->parent;
}

int counterpath = 0;
int flag = 0;
std::vector <calculatepath> allpaths;

vector <calculatepath> Path(btree * _node){

	calculatepath temp;

		//the tree is empty
		if (_node == NULL)
		{
			std::cerr << "the tree is empyt" << std::endl;
			temp.path = -1;
			temp.label = "NULL";
			allpaths.push_back(temp);
			return allpaths;
		}

		//a leaf wa found
		if (_node->left == NULL && _node->right == NULL)
		{
			temp.path = counterpath;
			temp.label = _node->value;
			allpaths.push_back(temp);
			counterpath--;
			Path(returnParent(_node)); //goes back to its parent in order to visit also the other node
			return allpaths;
		}
		//the left node was never visited
		if (_node->left != NULL && _node->left->colour != 'N')
		{
			counterpath++;
			Path(_node->left); //keep visting the left part
			return allpaths;
		}
		//the right node was never visited
		if (_node->right != NULL && _node->right->colour != 'N')
		{
			counterpath++;
			Path(_node->right); //keep visiting the right part
			return allpaths;
		}
		//the node just has a right child that was visited
		if (_node->left == NULL && _node->right->colour == 'N')
		{
			counterpath--;
			Path(returnParent(_node));//goes back to the parent
			return allpaths;
		}
		//just the left child and it was visited
		if (_node->right == NULL && _node->left->colour == 'N')
		{
			counterpath--;
			Path(returnParent(_node));//goes back to parent
			return allpaths;
		}
		//the node has both children
		if (_node->left != NULL && _node->right != NULL)
		{
			//all of them were visited
			if ((_node->right->colour == 'N' && _node->left->colour == 'N'))
			{
				//we reached the root
				if (_node->parent == NULL)
				{
					return allpaths;
				}

				counterpath--;
				Path(returnParent(_node));
				return allpaths;
			}
		}
		//provare se funziona perchè ho aggiunto questo return sotto
		return allpaths;
}


vector <string> findMin(std::string & _input) {
	btree * head;
	head = builtTree(_input);
	vector <calculatepath> _to_find;
	_to_find = Path(head);
	vector<int> save_path;
	vector <string> min_input;

	for (int i = 0; i <_to_find.size(); i++)
	{
		save_path.push_back(_to_find[i].path);
	}
	//sort the path in order to find the min
	std::sort(save_path.begin(), save_path.end());

	for (int i = 0; i < _to_find.size(); i++)
	{
		if (_to_find[i].path==save_path[0])
		{
			min_input.push_back(_to_find[i].label);
		}
	}
	return min_input;
}

vector <string> findMax(std::string & _input) {
	btree * head;
	head = builtTree(_input);
	vector <calculatepath> _to_find;
	_to_find = Path(head);
	vector<int> save_path;
	vector <string> max_output;

	for (int i = 0; i < _to_find.size(); i++)
	{
		save_path.push_back(_to_find[i].path);
	}
	//sort the vector so the last element is the biggest
	std::sort(save_path.begin(), save_path.end());

	for (int i = 0; i < _to_find.size(); i++)
	{
		if (_to_find[i].path==save_path[save_path.size()-1])
		{
			max_output.push_back(_to_find[i].label);
		}
	}
	std::cout << "size find: " << _to_find.size() << " " << " size di max" << max_output.size() << endl;
	
	return max_output;
}

vector <string> coniLogici(std::string & _input) {
	btree * head;
	head = builtTree(_input);
	vector <calculatepath> _to_find;
	_to_find = Path(head);
	vector <string> input;
	
	for (int i = 0; i < _to_find.size(); i++)
	{
		input.push_back(_to_find[i].label);
	}
	
	return input;
}
