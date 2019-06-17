#pragma once
#include "gate.h"

struct btree {
	gate newGate;
	struct btree	*left;
	struct btree	*right; 
	struct btree	*parent;
	signal p_signal;
};

btree *newNode (gate _newGate)
{
	btree *node = new btree();
	node->left = nullptr;
	node->right = nullptr;
	node->parent = nullptr;
	node->newGate = _newGate;
	node->p_signal.Set( _newGate.Read() );
	return node;
};

btree *newNode (signal_input _signal) 
{
	btree *node = new btree();
	node->left = nullptr;
	node->right = nullptr;
	node->parent = nullptr;
	node->p_signal = _signal;
	return node;
};

