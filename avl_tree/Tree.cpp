#include "Tree.h"
#include <cmath>
#include <iostream>

using namespace std;

/*TREE IMPLEMENTATIONS*/
Tree::Tree()
{
	root = NULL;
}

Tree::~Tree()
{
	Clear();
}

int Tree::Insert(int toAdd)
{
	return Insert(toAdd, toAdd, root);
}

int Tree::Insert(int key, int toAdd, Node*& root)
{
	if(!root)
	{
		root = new Node;
		root->key = key;
		root->data = toAdd;
		
		return 1;
	}

	int result; //What we will return

	if(key < root->key)
		result = Insert(key, toAdd, root->left);
	else
		result = Insert(key, toAdd, root->right);

	RecalculateHeight(root);
	Rebalance(root);

	return result;
}

int Tree::Remove(int toRemove)
{
	return Remove(toRemove, root);
}

int Tree::Remove(int key, Node*& root)
{
	if(!root)
		return 0;

	int result;

	if(key == root->key)
	{
		if(root->IsLeaf())
		{
			delete root;
			root = NULL;

			result = 1;
		}

		else if(root->left && root->right)
		{
			root->SetAsInOrderSuccessor();
			result = Remove(root->key, root->right);
		}

		else
		{
			Node* temp;

			if(root->left)
				temp = root->left;
			else
				temp = root->right;

			delete root;
			root = temp;

			result = 1;
		}
	}

	else
	{
		if(key < root->key)
			result = Remove(key, root->left);
		else
			result = Remove(key, root->right);
	}

	if(root && result != 0)
	{
		RecalculateHeight(root);
		Rebalance(root);
	}

	return result;
}

void Tree::Display() const
{
	Display(root);
	cout << endl;
}

void Tree::Display(Node* root) const
{
	if(!root)
		return;

	Display(root->left);
	cout << root->data << " ";
	Display(root->right);
}

void Tree::Clear()
{
	Clear(root);
}

void Tree::Clear(Node*& root)
{
	if(!root)
		return;

	Clear(root->left);
	Clear(root->right);

	delete root;
	root = NULL;
}

void Tree::Rebalance(Node*& root)
{
	if(IsBalanced(root))
		return;

	Node* child = GetChildLongestPath(root);
	Node* grandChild = GetChildLongestPath(child);

	//Case 1: Left Left Rotation
	if(root->left == child && child->left == grandChild)
		RightRotation(root);

	//Case 2: Left Right Rotation
	else if(root->left == child && child->right == grandChild)
	{
		LeftRotation(child);
		root->left = child;
		RightRotation(root);
	}

	//Case 3: Right Left Rotation
	else if(root->right == child && child->left == grandChild)
	{
		RightRotation(child);
		root->right = child;
		LeftRotation(root);
	}

	//Case 4: Right Right Rotation
	else
		LeftRotation(root);

	RecalculateHeight(root->left);
	RecalculateHeight(root->right);
	RecalculateHeight(root);
}

bool Tree::IsBalanced(Node* toCheck)
{
	if(!toCheck)
		return false;

	int heightL, heightR;

	heightL = GetSubtreeHeight(toCheck->left);
	heightR = GetSubtreeHeight(toCheck->right);

	return abs(heightL - heightR) <= 1;
}

void Tree::LeftRotation(Node*& root)
{
	if(!root)
		return;

	Node* temp = root->right;

	root->right = temp->left;
	temp->left = root;

	root = temp;
}

void Tree::RightRotation(Node*& root)
{
	if(!root)
		return;

	Node* temp = root->left;

	root->left = temp->right;
	temp->right = root;

	root = temp;
}

void Tree::RecalculateHeight(Node* root)
{
	if(!root)
		return;

	int heightL, heightR;

	heightL = GetSubtreeHeight(root->left);
	heightR = GetSubtreeHeight(root->right);

	if(heightL < heightR)
		root->height = 1 + heightR;
	else
		root->height = 1 + heightL;
}

int Tree::GetSubtreeHeight(Node* subtree)
{
	if(!subtree)
		return 0;
	else
		return subtree->height;
}

Node* Tree::GetChildLongestPath(Node* subtree)
{
	if(!subtree)
		return NULL;

	int heightL, heightR;

	heightL = GetSubtreeHeight(subtree->left);
	heightR = GetSubtreeHeight(subtree->right);

	if(heightR > heightL)
		return subtree->right;
	else
		return subtree->left;
}

/*NODE IMPLEMENTATIONS*/
Node::Node()
{
	key = 0;
	data = 0;

	height = 1;

	left = NULL;
	right = NULL;
}

Node::~Node()
{
	left = NULL;
	right = NULL;
}

bool Node::IsLeaf()
{
	return !left && !right;
}

void Node::SetAsInOrderSuccessor()
{
	if(!right)
		return;

	Node* temp = right;

	while(temp->left)
		temp = temp->left;

	key = temp->key;
	data = temp->data;
}
