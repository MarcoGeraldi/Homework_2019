#pragma once
#include "stringParser.hpp"

struct btree {
	std::string value;
	btree * left, *right;
};

btree * newNode(std::string & value_to_assign) {
	btree * temp = new btree;
	temp->left = nullptr;
	temp->right = nullptr;
	temp->value = value_to_assign;
	return temp;
}

btree * addChildren(btree * _node, btree * left_child=nullptr, btree * right_child=nullptr) {

	_node->left = left_child;
	_node->right = right_child;
	return _node;
}

btree * builtTree(std::string & _input_string) {

	std::string new_input = _input_string;
	btree * head = nullptr;
	vector<size_t> positionOpenB;
	vector<size_t> positionCloseB;
	vector <btree *> special;

	int innerCounter = getPositions(_input_string, "(").size();
	
	std::string special_character = "*";

	for (int i = 0; i < innerCounter+1; i++)//has to do it as many times as the size of the bracktes plus one 
	{
		positionOpenB = getPositions(new_input, "(");
		positionCloseB = getPositions(new_input, ")");

		int counterOpen = 0;
		int counterClose = 0;
		int flag = 0;

		if (positionOpenB.size() == 0 || positionCloseB.size() == 0) //there are no more brackets
		{
			istringstream stream_temp(new_input);
			std::string string_temp;
			vector <std::string> vect_temp;
			while (getline(stream_temp, string_temp,' ')) //divide the sentence by spaces
			{
				vect_temp.push_back(string_temp);
			}
			if (vect_temp[0]==special_character && vect_temp[2]==special_character)
			{
				btree * root = newNode(vect_temp[1]);
				btree * right_child = special.back();

				//delete the last two element added
				special.erase(special.end() - 1);

				btree * left_child = special.back();
				special.erase(special.end() - 1);

				root = addChildren(root, left_child, right_child);
				
				return root;
			}
			else if (!(vect_temp[0] == special_character && vect_temp[2] == special_character) && vect_temp[0] == special_character)
			{
				btree * root = newNode(vect_temp[1]);
				btree * right_child = newNode(vect_temp[2]);

				root = addChildren(root, head, right_child);

				return root;
			}
			else if (!(vect_temp[0] == special_character && vect_temp[2] == special_character) && vect_temp[2] == special_character)
			{
				btree * root = newNode(vect_temp[1]);
				btree * left_child = newNode(vect_temp[0]);

				root = addChildren(root, left_child, head);

				return root;
			}
			else //there are no special character, then there were no brackets that were replace
			{
				btree * root = newNode(vect_temp[1]);
				btree * left_child = newNode(vect_temp[0]);
				btree * right_child = newNode(vect_temp[2]);

				root=addChildren(root, left_child, right_child);

				return root;
			}
		}
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
			//std::cout << "frase tagliata: " << s << std::endl;
			
			stringstream stream_temp(s);
			vector<string> vect_temp;
			string string_temp;

			while (getline(stream_temp, string_temp, ' ')) //divide the string by space
			{
				vect_temp.push_back(string_temp);
			}
			
			if (!(vect_temp[0] == special_character && vect_temp[2] == special_character) && vect_temp[0] == special_character)
			{
				btree * general_node = newNode(vect_temp[1]);
				btree * right_child = newNode(vect_temp[2]);

				head=addChildren(general_node, head, right_child);
				special.push_back(head);

				//replace the inner bracket with the resolution of the boolean operator
				new_input.replace(positionOpenB[counterOpen], positionCloseB[counterClose] - positionOpenB[counterOpen] + 1, special_character);

				positionCloseB.erase(positionCloseB.begin() + counterClose);
				positionOpenB.erase(positionOpenB.begin() + counterOpen);
			}
			else if (!(vect_temp[0] == special_character && vect_temp[2] == special_character) && vect_temp[2] == special_character)
			{
				btree * general_node = newNode(vect_temp[1]);
				btree * left_child = newNode(vect_temp[0]);

				head=addChildren(general_node, left_child, head);
				special.push_back(head);

				//replace the inner bracket with the resolution of the boolean operator
				new_input.replace(positionOpenB[counterOpen], positionCloseB[counterClose] - positionOpenB[counterOpen] + 1, special_character);

				positionCloseB.erase(positionCloseB.begin() + counterClose);
				positionOpenB.erase(positionOpenB.begin() + counterOpen);
			}
			else if (vect_temp[0]==special_character && vect_temp[2] == special_character)
			{
				btree * general_node = newNode(vect_temp[1]);

				btree * right_child = special.back();
				//delete the last two element added in the vector
				special.erase(special.end() - 1);
				
				btree * left_child = special.back();
				special.erase(special.end() - 1);

				head = addChildren(general_node, left_child, right_child);
				special.push_back(head); //save all the pointers into a vector in order to reuse them if you find two s

				//replace the inner bracket with the resolution of the boolean operator
				new_input.replace(positionOpenB[counterOpen], positionCloseB[counterClose] - positionOpenB[counterOpen] + 1, special_character);

				positionCloseB.erase(positionCloseB.begin() + counterClose);
				positionOpenB.erase(positionOpenB.begin() + counterOpen);

			}
			else //there are no special character, then there were no brackets that were replace
			{
				btree * general_node = newNode(vect_temp[1]);
				btree * left_child = newNode(vect_temp[0]);
				btree * right_child = newNode(vect_temp[2]);

				head= addChildren(general_node, left_child, right_child);
				special.push_back(head); //save all the pointers into a vector in order to reuse them if you find two special characters

				//replace the inner bracket with the resolution of the boolean operator

				new_input.replace( positionOpenB[counterOpen], positionCloseB[counterClose] - positionOpenB[counterOpen] +1, special_character);

				positionCloseB.erase(positionCloseB.begin() + counterClose);
				positionOpenB.erase(positionOpenB.begin() + counterOpen);
				//std::cout << "Nuova frase " << new_input << std::endl;
			}
		}
	}	
}
