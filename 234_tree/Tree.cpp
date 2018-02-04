#include "Tree.h"
#include <iostream>

using namespace std;

/*Primarily for Remove cases, these tell us which
  child to traverse with*/
#define TRAVERSE_LEFT		0
#define TRAVERSE_MIN		1
#define TRAVERSE_MAX		2
#define TRAVERSE_RIGHT		3

/*TREE IMPLEMENTATIOS*/
Tree::Tree()
{
	root = NULL;
}

Tree::~Tree()
{
	Clear();
}

void Tree::Insert(const int data)
{
	int key = data;
	//If we end up splitting root, this
	//is where the result is stored
	Node* splitChild = NULL;

	//Special case: root is NULL
	if(!root)
	{
		root = new Node;
		root->Insert(key, data);
	}

	else
		Insert(key, data, root, splitChild);

	if(splitChild)
		root = splitChild;
}

void Tree::Insert(const int key, const int data, Node*& root, Node*& splitChild)
{
	if(!root)
		return;

	//Will be a child of root. Holds the split
	//child after the recursive call. This way, we
	//can easily integrate after a split
	Node* temp = NULL;

	if(root->Is4Node())
		SplitRoot(root, splitChild);
	else
		splitChild = NULL;

	//We can insert here! Note that if root was a 4-node
	//prior to now, this will fail, since it will have
	//children after a split
	if(root->IsLeaf())
		root->Insert(key, data);
	else
	{
		if(root->Is2Node())
		{
			//Go left
			if(key < root->keyes[0])
				Insert(key, data, root->left, temp);
			//Go right
			else
				Insert(key, data, root->right, temp);
		}

		else
		{
			//Go left
			if(key < root->keyes[0])
				Insert(key, data, root->left, temp);
			//Go middle
			else if(key < root->keyes[1])
				Insert(key, data, root->centMin, temp);
			//Go right
			else
				Insert(key, data, root->right, temp);
		}
	}

	//If we split, we must integrate
	if(temp)
		Integrate(root, temp);
}

void Tree::SplitRoot(Node* root, Node*& splitChild)
{
	if(!root)
		return;

	Node* left = new Node;
	left->left = root->left;
	left->right = root->centMin;
	//Use the smallest data for left
	left->Insert(root->keyes[0], root->data[0]);

	Node* right = new Node;
	right->left = root->centMax;
	right->right = root->right;
	//Use the highest data for right
	right->Insert(root->keyes[2], root->data[2]);

	//Move the data to the correct place
	root->keyes[0] = root->keyes[1];
	root->data[0] = root->data[1];
	//Now that we moved the data, we need to clear our
	//empty keyes
	root->keyes[1] = -1;
	//TODO: Clear data properly
	root->data[1] = 0;
	root->keyes[2] = -1;
	//TODO: Clear data properly
	root->data[2] = 0;

	//Lastly, fix our child pointers
	root->left = left;
	root->right = right;
	root->centMin = NULL;
	root->centMax = NULL;

	//Mark that we split root
	splitChild = root;
}

void Tree::Integrate(Node* root, Node*& toIntegrate)
{
	if(!root || !toIntegrate)
		return;

	if(root->Is2Node())
		IntegrateRoo2Node(root, toIntegrate);

	//Root must be a three-node
	else
		IntegrateRoo3Node(root, toIntegrate);

	//Note that root can't be a four-node at this point.
	//If root was a four-node, then it was already split
}

void Tree::IntegrateRoo2Node(Node* root, Node*& toIntegrate)
{
	if(!root || !toIntegrate)
		return;

	if(toIntegrate == root->left)
	{
		root->left = toIntegrate->left;
		root->centMin = toIntegrate->right;

		//Move the data to make way for the data we will
		//integrate
		root->keyes[1] = root->keyes[0];
		root->data[1] = root->data[0];
		//Now take in the new data
		root->keyes[0] = toIntegrate->keyes[0];
		root->data[0] = toIntegrate->data[0];

		//Since we've now got the data, we can get rid of toIntegrate
		delete toIntegrate;
		toIntegrate = NULL;
	}

	//Temp must be the right node
	else
	{
		root->centMin = toIntegrate->left;
		root->right = toIntegrate->right;

		//Since data is in the correct place already, we've no
		//need to move anything
		root->keyes[1] = toIntegrate->keyes[0];
		root->data[1] = toIntegrate->data[0];

		delete toIntegrate;
		toIntegrate = NULL;
	}
}

void Tree::IntegrateRoo3Node(Node* root, Node*& toIntegrate)
{
	if(!root || !toIntegrate)
		return;

	if(toIntegrate == root->left)
	{
		//Begin by moving the data to the correct places
		root->keyes[2] = root->keyes[1];
		root->data[2] = root->data[1];
		root->keyes[1] = root->keyes[0];
		root->data[1] = root->data[0];

		//Now take in the data from the left child
		root->keyes[0] = toIntegrate->keyes[0];
		root->data[0] = toIntegrate->data[0];

		//Next, move the centMin node to centMax
		root->centMax = root->centMin;

		//Now move the child nodes from toIntegrate
		root->centMin = toIntegrate->right;
		root->left = toIntegrate->left;

		delete toIntegrate;
		toIntegrate = NULL;
	}

	else if(toIntegrate == root->centMin)
	{
		//Just need to move the second data piece
		//to clear room for the middle child
		root->keyes[2] = root->keyes[1];
		root->data[2] = root->data[1];

		//Now we can integrate the middle data item
		root->keyes[1] = toIntegrate->keyes[0];
		root->data[1] = toIntegrate->keyes[0];

		//Now we take in the children
		root->centMax = toIntegrate->right;
		root->centMin = toIntegrate->left;

		//Now we can delete the toIntegrate
		delete toIntegrate;
		toIntegrate = NULL;
	}

	//Split child is the right child
	else
	{
		//Don't need to move data around in root
		//so we will just integrate
		root->keyes[2] = toIntegrate->keyes[0];
		root->data[2] = toIntegrate->data[0];

		//Take in toIntegrate's children
		root->centMax = toIntegrate->left;
		root->right = toIntegrate->right;

		delete toIntegrate;
		toIntegrate = NULL;
	}
}

bool Tree::Remove(const int key)
{
	//Use splitChild to tell us if root violates
	//234 tree properties
	Node* splitChild = NULL;
	bool result = Remove(key, root, splitChild);

	if(splitChild)
	{
		Node* temp = root->left;
		delete root;
		root = temp;
	}
	
	splitChild = NULL;

	return result;
}

bool Tree::Remove(const int key, Node*& root, Node*& splitChild)
{
	if(!root)
		return false;

	Node* temp = NULL;
	//When traversing (either to find the value or do
	//in order succession), we use this
	int toTraverse;
	//Describes what we are returning
	bool result;

	//We found our data!
	if(root->Contains(key))
	{
		//We can remove here
		if(root->IsLeaf())
		{
			int removeResult = root->Remove(key);
			//Since we will always Remove at this point,
			//if the result is 0, that means root was a 2-node.
			//Thus, we violate the 234 properties. Otherwise it
			//returns 1, meaning root was not a 2-node so we are
			//still okay
			splitChild = removeResult == 0 ? root : NULL;
		}

		//Otherwise, we must swap with in-order successor
		//and remove there
		else
		{
			root->SwapWithInOrderSuccessor(key, toTraverse);

			if(toTraverse == TRAVERSE_LEFT)
				result = Remove(key, root->left, temp);
			else if(toTraverse == TRAVERSE_MIN)
				result = Remove(key, root->centMin, temp);
			else if(toTraverse == TRAVERSE_MAX)
				result = Remove(key, root->centMax, temp);
			else
				result = Remove(key, root->right, temp);
		}

		result = true;
	}

	//We need to keep going
	else
	{
		toTraverse = root->FindChildToTraverse(key);

		if(toTraverse == TRAVERSE_LEFT)
			result = Remove(key, root->left, temp);
		else if(toTraverse == TRAVERSE_MIN)
			result = Remove(key, root->centMin, temp);
		else if(toTraverse == TRAVERSE_MAX)
			result = Remove(key, root->centMax, temp);
		else
			result = Remove(key, root->right, temp);
	}

	if(temp)
	{
		if(!AttemptFix(root, temp))
			splitChild = root;
	}

	return result;
}

bool Tree::AttemptFix(Node*& root, Node*& offendingChild)
{
	bool result = true;

	if(root->Is2Node())
	{
		//Our only option is to look at the right child
		if(offendingChild == root->left)
		{
			//We have a merge case
			if(root->right->Is2Node())
			{
				FixRoot2NodeLeftOffendingRight2Node(root, offendingChild);
				result = false;
			}

			else
				FixRoot2NodeLeftOffendingRight34Node(root, offendingChild);
		}

		//Have to look at the left child
		else
		{
			//We have a merge case
			if(root->left->Is2Node())
			{
				FixRoot2NodeRightOffendingLeft2Node(root, offendingChild);
				result = false;
			}

			else
				FixRoot2NodeRightOffendingLeft34Node(root, offendingChild);
		}
	}

	else if(root->Is3Node())
	{
		if(offendingChild == root->left)
		{
			if(root->centMin->Is2Node())
				FixRoot3NodeLeftOffendingCentMin2Node(root, offendingChild);
			else
				FixRoot3NodeLeftOffendingCentMin34Node(root, offendingChild);
		}

		else if(offendingChild == root->centMin)
		{
			if(!root->left->Is2Node())
				FixRoot3NodeCentMinOffendingLeft34Node(root, offendingChild);
			else if(!root->right->Is2Node())
				FixRoot3NodeCentMinOffendingRight34Node(root, offendingChild);
			else
				FixRoot3NodeCentMinOffendingLeftRight2Node(root, offendingChild);
		}

		else
		{
			if(root->centMin->Is2Node())
				FixRoot3NodeRightOffendingCentMin2Node(root, offendingChild);
			else
				FixRoot3NodeRightOffendingCentMin34Node(root, offendingChild);
		}
	}

	//Root is a 4 node
	else
	{
		if(offendingChild == root->left)
		{
			if(root->centMin->Is2Node())
				FixRoot4NodeLeftOffendingCentMin2Node(root, offendingChild);
			else
				FixRoot4NodeLeftOffendingCentMin34Node(root, offendingChild);
		}

		else if(offendingChild == root->centMin)
		{
			if(!root->left->Is2Node())
				FixRoot4NodeCentMinOffendingLeft34Node(root, offendingChild);
			else if(!root->centMax->Is2Node())
				FixRoot4NodeCentMinOffendingCentMax34Node(root, offendingChild);
			else
				FixRoot4NodeCentMinOffendingSibilingsNode(root, offendingChild);
		}

		else if(offendingChild == root->centMax)
		{
			if(!root->centMin->Is2Node())
				FixRoot4NodeCentMaxOffendingCentMin34Node(root, offendingChild);
			else if(!root->right->Is2Node())
				FixRoot4NodeCentMaxOffendingRight34Node(root, offendingChild);
			else
				FixRoot4NodeCentMaxOffendingSibilings2Node(root, offendingChild);
		}

		else
		{
			if(root->centMax->Is2Node())
				FixRoot4NodeRightOffendingCentMax2Node(root, offendingChild);
			else
				FixRoot4NodeRightOffendingCentMax34Node(root, offendingChild);
		}
	}

	return result;
}

void Tree::FixRoot2NodeLeftOffendingRight2Node(Node*& root, Node*& offendingChild)
{
	//First we bring down root's data into the non-offending
	//child
	//First move the data in right to clear some space
	root->right->keyes[1] = root->right->keyes[0];
	root->right->data[1] = root->right->data[0];
	//Now move root's data down into the child
	root->right->keyes[0] = root->keyes[0];
	root->right->data[0] = root->data[0];

	//With the data moved in, we now clear root
	root->keyes[0] = -1;
	//TODO: Properly clear data as needed
	root->data[0] = 0;

	//Now we move all child nodes into the correct place
	//Note that root->right/left are either leaves or not.
	//If leaves, the children are NULL. Otherwise, root->left->right
	//is NULL
	root->right->centMin = root->right->left;
	root->right->left = root->left->left;

	delete root->left;
	offendingChild = NULL;
	root->left = root->right;
	root->right = NULL;
}

void Tree::FixRoot2NodeLeftOffendingRight34Node(Node*& root, Node*& offendingChild)
{
	//Transfer case
	//Move the data in root into the offending child
	root->left->keyes[0] = root->keyes[0];
	root->left->data[0] = root->keyes[0];
	//Move the data in right up
	root->keyes[0] = root->right->keyes[0];
	root->data[0] = root->right->data[0];

	//Transfer child node over
	root->left->right = root->right->left;

	//Right is a 3-Node
	if(root->right->keyes[2] == -1)
	{
		root->right->keyes[0] = root->right->keyes[1];
		root->right->data[0] = root->right->data[1];
		root->right->keyes[1] = -1;
		//TODO: Clear data properly
		root->right->data[1] = 0;

		root->right->left = root->right->centMin;
		root->right->centMin = NULL;
	}

	//Right is a 4-node
	else
	{
		root->right->keyes[0] = root->right->keyes[1];
		root->right->data[0] = root->right->data[1];
		root->right->keyes[1] = root->right->keyes[2];
		root->right->data[1] = root->right->data[2];
		root->right->keyes[2] = -1;
		//TODO: Clear data properly
		root->right->data[2] = 0;

		root->right->left = root->right->centMin;
		root->right->centMin = root->right->centMax;
		root->right->centMax = NULL;
	}

	offendingChild = NULL;
}

void Tree::FixRoot2NodeRightOffendingLeft2Node(Node*& root, Node*& offendingChild)
{
	//We begin by moving root's data into
	//the left child
	root->left->keyes[1] = root->keyes[0];
	root->left->data[1] = root->data[0];
	//Now we clear root
	root->keyes[0] = -1;
	//TODO: Clear data properly
	root->data[0] = 0;

	//Now move the right child of left
	//to make space for the new child coming in
	root->left->centMin = root->left->right;
	root->left->right = root->right->left;

	//Now delete the offending child and set it to NULL
	delete root->right;
	root->right = NULL;
	offendingChild = NULL;
}

void Tree::FixRoot2NodeRightOffendingLeft34Node(Node*& root, Node*& offendingChild)
{
	//Transfer case
	root->right->keyes[0] = root->keyes[0];
	root->right->data[0] = root->data[0];
	
	if(root->left->Is3Node())
	{
		root->keyes[0] = root->left->keyes[1];
		root->data[0] = root->left->data[1];
		root->left->keyes[1] = -1;
		//TODO: Clear data properly
		root->left->data[1] = 0;

		root->right->right = root->right->left;
		root->right->left = root->left->right;
		root->left->right = root->left->centMin;
		root->left->centMin = NULL;
	}

	//Left is a 4-node
	else
	{
		root->keyes[0] = root->left->keyes[2];
		root->data[0] = root->left->data[2];
		root->left->keyes[2] = -1;
		//TODO: Clear data properly
		root->left->data[2] = 0;

		root->right->right = root->right->left;
		root->right->left = root->left->right;
		root->right->right = root->right->centMax;
		root->right->centMax = NULL;
	}

	offendingChild = NULL;
}

void Tree::FixRoot3NodeLeftOffendingCentMin2Node(Node*& root, Node*& offendingChild)
{
	//First bring down the data in
	//the root
	//Move the data in the middle child
	//to clear room for the new data
	root->centMin->keyes[1] = root->centMin->keyes[0];
	root->centMin->data[1] = root->centMin->data[0];
	//Now move root's data into the child
	root->centMin->keyes[0] = root->keyes[0];
	root->centMin->data[0] = root->data[0];

	//Now move the child left over in left to the centMin
	//Clear some space for it
	root->centMin->centMin = root->centMin->left;
	root->centMin->left = root->left->left;

	//Don't need to worry about the right child since it will always be NULL

	//Now fix root's out-of-place data
	root->keyes[0] = root->keyes[1];
	root->data[0] = root->data[1];

	//Clear old data
	root->keyes[1] = -1;
	//TODO: Clear data properly
	root->data[1] = 0;

	//Now fix our children
	delete root->left;
	root->left = root->centMin;
	root->centMin = NULL;
	offendingChild = NULL;
}

void Tree::FixRoot3NodeLeftOffendingCentMin34Node(Node*& root, Node*& offendingChild)
{
	//Transfer case
	//Move the data in root into the offending child
	root->left->keyes[0] = root->keyes[0];
	root->left->data[0] = root->data[0];
	//Move the data in centMin into root
	root->keyes[0] = root->centMin->keyes[0];
	root->data[0] = root->centMin->data[0];

	//Now what we do next is dependent on the type of node
	//centMin is
	if(root->centMin->Is3Node())
	{
		root->centMin->keyes[0] = root->centMin->keyes[1];
		root->centMin->data[0] = root->centMin->data[1];
		root->centMin->keyes[1] = -1;
		//TODO: Clear data properly
		root->centMin->data[1] = 0;

		root->left->right = root->centMin->left;
		root->centMin->left = root->centMin->centMin;
		root->centMin->centMin = NULL;
	}

	else
	{
		root->centMin->keyes[0] = root->centMin->keyes[1];
		root->centMin->data[0] = root->centMin->data[1];
		root->centMin->keyes[1] = root->centMin->keyes[2];
		root->centMin->data[1] = root->centMin->data[2];
		root->centMin->keyes[2] = -1;
		//TODO: Clear data properly
		root->centMin->data[2] = 0;

		root->left->right = root->centMin->left;
		root->centMin->left = root->centMin->centMin;
		root->centMin->centMin = root->centMin->centMax;
		root->centMax = NULL;
	}

	offendingChild = NULL;
}

void Tree::FixRoot3NodeCentMinOffendingLeft34Node(Node*& root, Node*& offendingChild)
{
	//Move the data into the offending child
	root->centMin->keyes[0] = root->keyes[0];
	root->centMin->data[0] = root->data[0];

	if(root->left->Is3Node())
	{
		root->keyes[0] = root->left->keyes[1];
		root->data[0] = root->left->data[1];
		root->left->keyes[1] = -1;
		//TODO: Clear data properly
		root->left->data[1] = 0;

		root->centMin->right = root->centMin->left;
		root->centMin->left = root->left->right;
		root->left->right = root->left->centMin;
		root->left->centMin = NULL;
	}

	else
	{
		root->keyes[0] = root->left->keyes[2];
		root->data[0] = root->left->data[2];
		root->left->keyes[2] = -1;
		//TODO: Clear data properly
		root->left->data[2] = 0;

		root->centMin->right = root->centMin->left;
		root->centMin->left = root->left->right;
		root->left->right = root->left->centMax;
		root->left->centMax = NULL;
	}

	offendingChild = NULL;
}

void Tree::FixRoot3NodeCentMinOffendingRight34Node(Node*& root, Node*& offendingChild)
{
	root->centMin->keyes[0] = root->keyes[1];
	root->centMin->data[0] = root->data[1];

	root->keyes[1] = root->right->keyes[0];
	root->data[1] = root->right->data[0];

	if(root->right->Is3Node())
	{
		root->right->keyes[0] = root->right->keyes[1];
		root->right->data[0] = root->right->data[1];
		root->right->keyes[1] = -1;
		//TODO: Clear data properly
		root->right->data[1] = 0;

		//Now Fix the child nodes
		root->centMin->right = root->right->left;
		root->right->left = root->right->centMin;
		root->right->centMin = NULL;
	}

	else
	{				
		root->right->keyes[0] = root->right->keyes[1];
		root->right->data[0] = root->right->data[1];
		root->right->keyes[1] = root->right->keyes[2];
		root->right->data[1] = root->right->data[2];
		root->right->keyes[2] = -1;
		//TODO: Clear data properly
		root->right->data[2] = 0;

		root->centMin->right = root->right->left;
		root->right->left = root->right->centMin;
		root->right->centMin = root->right->centMax;
		root->right->centMax = NULL;
	}

	offendingChild = NULL;
}

void Tree::FixRoot3NodeCentMinOffendingLeftRight2Node(Node*& root, Node*& offendingChild)
{
	//Begin by moving root's right data into the right
	//child
	root->right->keyes[1] = root->right->keyes[0];
	root->right->data[1] = root->right->data[0];
	root->right->keyes[0] = root->keyes[1];
	root->right->data[0] = root->data[1];

	//Now clear the data in root
	root->keyes[1] = -1;
	//TODO: Clear data properly
	root->data[1] = 0;

	//Now merge our children
	root->right->centMin = root->right->left;
	root->right->left = root->centMin->left;

	//Now delete the offending child
	delete root->centMin;
	root->centMin = NULL;
	offendingChild = NULL;
}

void Tree::FixRoot3NodeRightOffendingCentMin2Node(Node*& root, Node*& offendingChild)
{
	//First bring down the data in root
	root->centMin->keyes[1] = root->keyes[1];
	root->centMin->data[1] = root->data[1];
	root->keyes[1] = -1;
	//TODO: Clear data properly
	root->data[1] = 0;

	root->centMin->centMin = root->centMin->right;
	root->centMin->right = root->right->left;

	delete root->right;
	root->right = root->centMin;
	root->centMin = NULL;
	offendingChild = NULL;
}

void Tree::FixRoot3NodeRightOffendingCentMin34Node(Node*& root, Node*& offendingChild)
{
	//Move appropriate data in root to the offending child
	root->right->keyes[0] = root->keyes[1];
	root->right->data[0] = root->data[1];

	if(root->centMin->Is3Node())
	{
		//Move the data in the sibiling to the root
		root->keyes[1] = root->centMin->keyes[1];
		root->data[1] = root->centMin->data[1];
		root->centMin->keyes[1] = -1;
		//TODO: Clear data properly
		root->centMin->data[1] = 0;

		//Now move children accordingly
		root->right->right = root->right->left;
		root->right->left = root->centMin->right;
		root->centMin->right = root->centMin->centMin;
		root->centMin->centMin = NULL;
	}

	else
	{
		//Move the data in the sibiling to the root
		root->keyes[1] = root->centMin->keyes[2];
		root->data[1] = root->centMin->data[2];
		root->centMin->keyes[2] = -1;
		//TODO: Clear data properly
		root->centMin->data[2] = 0;

		//Now move the children accordingly
		root->right->right = root->right->left;
		root->right->left = root->centMin->right;
		root->centMin->right = root->centMin->centMax;
		root->centMin->centMax = NULL;
	}

	offendingChild = NULL;
}

void Tree::FixRoot4NodeLeftOffendingCentMin2Node(Node*& root, Node*& offendingChild)
{
	//Move the data in root into the centMin
	//Clear space for this new data
	root->centMin->keyes[1] = root->centMin->keyes[0];
	root->centMin->data[1] = root->centMin->data[0];
	//Now we may move the data root into centMin
	root->centMin->keyes[0] = root->keyes[0];
	root->centMin->data[0] = root->data[0];
	
	//Before we move on, we need to fix the data in root
	root->keyes[0] = root->keyes[1];
	root->data[0] = root->data[1];
	root->keyes[1] = root->keyes[2];
	root->data[1] = root->data[2];
	root->keyes[2] = -1;
	//TODO: Clear data properly
	root->data[2] = 0;

	//Clear space to take in left's only child
	root->centMin->centMin = root->centMin->left;
	root->centMin->left = root->left->left;

	//With everything moved, we can now get rid of
	//the old left and move everything into place
	delete root->left;
	root->left = root->centMin;
	root->centMin = root->centMax;
	root->centMax = NULL;
	//Don't need to move right
	offendingChild = NULL;
}

void Tree::FixRoot4NodeLeftOffendingCentMin34Node(Node*& root, Node*& offendingChild)
{
	//Transfer case
	//Move data from root into the offending child
	root->left->keyes[0] = root->keyes[0];
	root->left->data[0] = root->data[0];
	//Move the data in the sibiling up to the root
	root->keyes[0] = root->centMin->keyes[0];
	root->data[0] = root->centMin->data[0];

	if(root->centMin->Is3Node())
	{
		//Fix the data in sibiling
		root->centMin->keyes[0] = root->centMin->keyes[1];
		root->centMin->data[0] = root->centMin->data[1];
		root->centMin->keyes[1] = -1;
		//TODO: Clear data properly
		root->centMin->data[1] = 0;

		//Transfer children as needed
		root->left->right = root->centMin->left;
		root->centMin->left = root->centMin->centMin;
		root->centMin->centMin = NULL;
	}

	else
	{
		//Fix the data in sibiling
		root->centMin->keyes[0] = root->centMin->keyes[1];
		root->centMin->data[0] = root->centMin->data[1];
		root->centMin->keyes[1] = root->centMin->keyes[2];
		root->centMin->data[1] = root->centMin->data[2];
		root->centMin->keyes[2] = -1;
		//TODO: Clear data properly
		root->centMin->data[2] = 0;

		//Transfer children
		root->left->right = root->centMin->left;
		root->centMin->left = root->centMin->centMin;
		root->centMin->centMin = root->centMin->centMax;
		root->centMin->centMax = NULL;
	}

	offendingChild = NULL;
}

void Tree::FixRoot4NodeCentMinOffendingLeft34Node(Node*& root, Node*& offendingChild)
{
	//Transfer
	//Move the data in root down into the offending child
	root->centMin->keyes[0] = root->keyes[0];
	root->centMin->data[0] = root->data[0];

	if(root->left->Is3Node())
	{
		//Move the data in the sibiling into the root
		root->keyes[0] = root->left->keyes[1];
		root->data[0] = root->left->data[1];
		//Now get rid of the data in the sibiling
		root->left->keyes[1] = -1;
		//TODO: Clear data properly
		root->left->data[1] = 0;

		//Now transfer children as needed
		root->centMin->right = root->centMin->left;
		root->centMin->left = root->left->right;
		root->left->right = root->left->centMin;
		root->left->centMin = NULL;
	}

	else
	{
		//Move the data in the sibiling into the root
		root->keyes[0] = root->left->keyes[2];
		root->data[0] = root->left->data[2];
		//Now get rid of the data in the sibiling
		root->left->keyes[2] = -1;
		//TODO: Clear data properly
		root->left->data[2] = 0;

		//Now transfer children
		root->centMin->right = root->centMin->left;
		root->centMin->left = root->left->right;
		root->left->right = root->left->centMax;
		root->left->centMax = NULL;
	}

	offendingChild = NULL;
}

void Tree::FixRoot4NodeCentMinOffendingCentMax34Node(Node*& root, Node*& offendingChild)
{
	//Move the data from root into the offending child
	root->centMin->keyes[0] = root->keyes[1];
	root->centMin->data[0] = root->data[1];
	//Move the data from the sibiling into the root
	root->keyes[1] = root->centMax->keyes[0];
	root->data[1] = root->centMax->data[0];

	if(root->centMax->Is3Node())
	{
		//Fix the children in the sibiling
		root->centMax->keyes[0] = root->centMax->keyes[1];
		root->centMax->data[0] = root->centMax->data[1];
		root->centMax->keyes[1] = -1;
		//TODO: Clear data properly
		root->centMax->data[1] = 0;

		//Now transfer children
		root->centMin->right = root->centMax->left;
		root->centMax->left = root->centMax->centMin;
		root->centMax->centMin = NULL;
	}

	else
	{
		//Fix the children in the sibiling
		root->centMax->keyes[0] = root->centMax->keyes[1];
		root->centMax->data[0] = root->centMax->data[1];
		root->centMax->keyes[1] = root->centMax->keyes[2];
		root->centMax->data[1] = root->centMax->data[2];
		root->centMax->keyes[2] = -1;
		//TODO: Clear data properly
		root->centMax->data[2] = 0;

		//Now transfer the children
		root->centMin->right = root->centMax->left;
		root->centMax->left = root->centMax->centMin;
		root->centMax->centMin = root->centMax->centMax;
		root->centMax->centMax = NULL;
	}

	offendingChild = NULL;
}

void Tree::FixRoot4NodeCentMinOffendingSibilingsNode(Node*& root, Node*& offendingChild)
{
	//First we move our data in root into the centMax
	//child
	//Clear space for this new data
	root->centMax->keyes[1] = root->centMax->keyes[0];
	root->centMax->data[1] = root->centMax->data[0];
	//Now move our data
	root->centMax->keyes[0] = root->keyes[1];
	root->centMax->data[0] = root->data[1];
	//Now fix the data in root
	root->keyes[1] = root->keyes[2];
	root->data[1] = root->data[2];
	root->keyes[2] = -1;
	//TODO: Clear data properly
	root->data[2] = 0;

	//Now we can fix our children
	root->centMax->centMin = root->centMax->left;
	root->centMax->left = root->centMin->left;

	delete root->centMin;
	root->centMin = root->centMax;
	root->centMax = NULL;
	offendingChild = NULL;
}

void Tree::FixRoot4NodeCentMaxOffendingCentMin34Node(Node*& root, Node*& offendingChild)
{
	//Transfer case
	//Move the data from root to the offending child
	root->centMax->keyes[0] = root->keyes[1];
	root->centMax->data[0] = root->data[1];

	if(root->centMin->Is3Node())
	{
		//Move the data in the sibiling into the root
		root->keyes[1] = root->centMin->keyes[1];
		root->data[1] = root->centMin->data[1];
		root->centMin->keyes[1] = -1;
		//TODO: Clear data properly
		root->centMin->data[1] = 0;

		//Now transfer the child nodes
		root->centMax->right = root->centMax->left;
		root->centMax->left = root->centMin->right;
		root->centMin->right = root->centMin->centMin;
		root->centMin->centMin = NULL;
	}

	else
	{
		//Move the data in the sibiling into the root
		root->keyes[1] = root->centMin->keyes[2];
		root->data[1] = root->centMin->data[2];
		root->centMin->keyes[2] = -1;
		//TODO: Clear data properly
		root->centMin->data[2] = 0;

		//Now transfer children
		root->centMax->right = root->centMax->left;
		root->centMax->left = root->centMin->right;
		root->centMin->right = root->centMin->centMax;
		root->centMin->centMax = NULL;
	}

	offendingChild = NULL;
}

void Tree::FixRoot4NodeCentMaxOffendingRight34Node(Node*& root, Node*& offendingChild)
{
	//Transfer case
	//Bring the data in root down to the offending child
	root->centMax->keyes[0] = root->keyes[2];
	root->centMax->data[0] = root->data[2];
	//Move the children in the sibiling into the root
	root->keyes[2] = root->right->keyes[0];
	root->data[2] = root->right->data[0];

	if(root->right->Is3Node())
	{
		//Fix the data in sibiling
		root->right->keyes[0] = root->right->keyes[1];
		root->right->data[0] = root->right->data[1];
		root->right->keyes[1] = -1;
		//TODO: Clear data properly
		root->right->data[1] = 0;

		//We now transfer our children
		root->centMax->right = root->right->left;
		root->right->left = root->right->centMin;
		root->right->centMin = NULL;
	}

	else
	{
		//Fix the data in sibiling
		root->right->keyes[0] = root->right->keyes[1];
		root->right->data[0] = root->right->data[1];
		root->right->keyes[1] = root->right->keyes[2];
		root->right->data[1] = root->right->data[2];
		root->right->keyes[2] = -1;
		//TODO: Clear data properly
		root->right->data[2] = 0;

		//We now transfer our children
		root->centMax->right = root->right->left;
		root->right->left = root->right->centMin;
		root->right->centMin = root->right->centMax;
		root->right->centMax = NULL;				
	}

	offendingChild = NULL;
}

void Tree::FixRoot4NodeCentMaxOffendingSibilings2Node(Node*& root, Node*& offendingChild)
{
	//First move middle data in root down into centMin
	//Since centMin can take in data, we don't need to
	//move anything
	root->centMin->keyes[1] = root->keyes[1];
	root->centMin->data[1] = root->data[1];
	//Now fix the data in root
	root->keyes[1] = root->keyes[2];
	root->data[1] = root->data[2];
	root->keyes[2] = -1;
	//TODO: Clear data properly
	root->data[2] = 0;
	//Now we need to fix the children in centMin
	root->centMin->centMin = root->centMin->right;
	root->centMin->right = root->centMax->left;

	delete root->centMax;
	root->centMax = NULL;

	offendingChild = NULL;
}

void Tree::FixRoot4NodeRightOffendingCentMax2Node(Node*& root, Node*& offendingChild)
{
	//Move the data in root into centMax
	root->centMax->keyes[1] = root->keyes[2];
	root->centMax->data[1] = root->data[2];
	root->keyes[2] = -1;
	//TODO: Clear data properly
	root->data[2] = 0;

	//Moving our children
	root->centMax->centMin = root->centMax->right;
	root->centMax->right = root->right->left;

	delete root->right;
	root->right = root->centMax;
	root->centMax = NULL;
	offendingChild = NULL;
}

void Tree::FixRoot4NodeRightOffendingCentMax34Node(Node*& root, Node*& offendingChild)
{
	//Transfer case
	//Move data in root into the offending child
	root->right->keyes[0] = root->keyes[2];
	root->right->data[0] = root->data[2];

	if(root->centMax->Is3Node())
	{
		//Move the data in the sibiling into the root
		root->keyes[2] = root->centMax->keyes[1];
		root->data[2] = root->centMax->data[1];
		root->centMax->keyes[1] = -1;
		//TODO: Clear data properly
		root->centMax->data[1] = 0;

		//Transfer children
		root->right->right = root->right->left;
		root->right->left = root->centMax->right;
		root->centMax->right = root->centMax->centMin;
		root->centMax->centMin = NULL;
	}

	else
	{
		//Move the data in the sibiling into the root
		root->keyes[2] = root->centMax->keyes[2];
		root->data[2] = root->centMax->data[2];
		root->centMax->keyes[2] = -1;
		//TODO: Clear data properly
		root->centMax->data[2] = 0;

		//Transfer children
		root->right->right = root->right->left;
		root->right->left = root->centMax->right;
		root->centMax->right = root->centMax->centMax;
		root->centMax->centMax = NULL;
	}

	offendingChild = NULL;
}

void Tree::Clear()
{
	Clear(root);
}

void Tree::Clear(struct Node*& root)
{
	if(!root)
		return;

	Clear(root->left);
	Clear(root->centMin);
	Clear(root->centMax);
	Clear(root->right);

	delete root;
	root = NULL;
}

void Tree::Display()
{
	Display(root);

	cout << endl;
}

void Tree::Display(Node* root)
{
	if(!root)
		return;

	Display(root->left);
	if(root->keyes[0] != -1)
		cout << root->data[0] << " ";
	Display(root->centMin);
	if(root->keyes[1] != -1)
		cout << root->data[1] << " ";
	Display(root->centMax);
	if(root->keyes[2] != -1)
		cout << root->data[2] << " ";
	Display(root->right);
}

/*NODE IMPLEMENTATIONS*/
Node::Node()
{
	left = NULL;
	centMin = NULL;
	centMax = NULL;
	right = NULL;

	keyes[0] = -1;
	keyes[1] = -1;
	keyes[2] = -1;
}

Node::~Node()
{
	//These nodes are handled elsewhere
	left = NULL;
	centMin = NULL;
	centMax = NULL;
	right = NULL;
}

bool Node::Insert(const int key, const int toInsert)
{
	//TODO: Clean this up!

	//It is not possible to add toInsert into the node
	if(keyes[2] != -1)
		return false;

	//First case: No data in the node
	if(keyes[0] == -1)
	{
		keyes[0] = key;
		data[0] = toInsert;
	}

	//Second case: we've got a piece of data in the node
	//already
	else if(keyes[1] == -1)
	{
		//Data already in node is in correct space
		if(data[0] < toInsert)
		{
			keyes[1] = key;
			data[1] = toInsert;
		}

		//Move data already in node before we have toInsert
		else
		{
			//Move the larger data up
			keyes[1] = keyes[0];
			data[1] = data[0];

			keyes[0] = key;
			data[0] = toInsert;
		}
	}

	//Final case: we've got two pieces of data already in the
	//node
	else if(keyes[2] == -1)
	{
		//If toInsert is the largest
		if(data[1] < toInsert)
		{
			keyes[2] = key;
			data[2] = toInsert;
		}

		//If toInsert is the middle value
		else if(data[0] < toInsert && toInsert < data[1])
		{
			keyes[2] = keyes[1];
			data[2] = data[1];

			keyes[1] = key;
			data[1] = toInsert;
		}

		//toInsert is the smallest value
		else
		{
			keyes[2] = keyes[1];
			data[2] = data[1];

			keyes[1] = keyes[0];
			data[1] = data[0];

			keyes[0] = key;
			data[0] = toInsert;
		}
	}

	return true;
}

int Node::Remove(const int key)
{
	if(!Contains(key))
		return -1;

	//What we will return
	int result;

	if(Is2Node())
	{
		keyes[0] = -1;
		//TODO: Clear data properly
		data[0] = 0;
		result = 0;
	}

	else
	{
		if(Is3Node())
		{
			if(keyes[0] == key)
			{
				//Move data down
				keyes[0] = keyes[1];
				data[0] = data[1];

				//Now update our keyes
				keyes[1] = -1;
				//TODO: Clear data properly
				data[1] = 0;
			}

			else
			{
				keyes[1] = -1;
				//TODO: Clear data properly
				data[1] = 0;
			}
		}

		//We must be a 4-node
		else
		{
			if(keyes[0] == key)
			{
				keyes[0] = keyes[1];
				data[0] = data[1];

				keyes[1] = keyes[2];
				data[1] = data[2];

				keyes[2] = -1;
				//TODO: Clear data properly
				data[2] = 0;
			}

			else if(keyes[1] == key)
			{
				keyes[1] = keyes[2];
				data[1] = data[2];

				keyes[2] = -1;
				//TODO: Clear data properly
				data[2] = 0;
			}

			else
			{
				keyes[2] = -1;
				//TODO: Clear data properly
				data[2] = 0;
			}
		}

		result = 1;
	}

	return result;
}

bool Node::Contains(const int key)
{
	bool result = false;

	for(int i = 0; i < 3; ++i)
	{
		if(keyes[i] == key)
		{
			result = true;
			break;
		}
	}

	return result;
}

void Node::Display() const
{
	for(int i = 0; i < 3; ++i)
	{
		if(keyes[i] != -1)
			cout << data[i] << ' ';
	}

	cout << endl;
}

int Node::FindChildToTraverse(const int key)
{
	//What we will return
	int result;

	if(key < keyes[0])
		result = TRAVERSE_LEFT;
	else if(key < keyes[1])
		result = TRAVERSE_MIN;
	else if(key < keyes[2])
		result = TRAVERSE_MAX;
	else
		result = TRAVERSE_RIGHT;

	return result;
}

void Node::SwapWithInOrderSuccessor(const int key, int& traverse)
{
	if(!Contains(key))
		return;

	//Use this to find in-order successor
	Node* temp;
	//Use this to remember where are are swapping
	int hold;

	if(Is2Node())
	{
		//There's only one possible option here
		temp = right;
		hold = 0;
		traverse = TRAVERSE_RIGHT;
	}

	else if(Is3Node())
	{
		if(keyes[0] == key)
		{
			temp = centMin;
			hold = 0;
			traverse = TRAVERSE_MIN;
		}

		else
		{
			temp = right;
			hold = 1;
			traverse = TRAVERSE_RIGHT;
		}
	}

	else
	{
		if(keyes[0] == key)
		{
			temp = centMin;
			hold = 0;
			traverse = TRAVERSE_MIN;
		}

		else if(keyes[1] == key)
		{
			temp = centMax;
			hold = 1;
			traverse = TRAVERSE_MAX;
		}

		else
		{
			temp = right;
			hold = 2;
			traverse = TRAVERSE_RIGHT;
		}
	}

	while(temp->left)
		temp = temp->left;

	int tempKey, tempData;
	tempKey = keyes[hold];
	tempData = data[hold];

	keyes[hold] = temp->keyes[0];
	data[hold] = temp->data[0];

	temp->keyes[0] = tempKey;
	temp->data[0] = tempData;
}

bool Node::IsLeaf() const
{
	return !left && !centMin && !centMax && !right;
}

bool Node::Is2Node() const
{
	return keyes[0] != -1 && keyes[1] == -1 && keyes[2] == -1;
}

bool Node::Is3Node() const
{
	return keyes[0] != -1 && keyes[1] != -1 && keyes[2] == -1;
}

bool Node::Is4Node() const
{
	return keyes[0] != -1 && keyes[1] != -1 && keyes[2] != -1;
}
