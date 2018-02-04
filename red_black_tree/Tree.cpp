#include "Tree.h"
#include <iostream>

using namespace std;

#define ROOT 0
#define LEFT 1
#define RIGHT 2

#define RED 0
#define BLACK 1
#define DOUBLE_BLACK 2

/*TREE IMPLEMENTATIONS*/
Tree::Tree()
{
	root = NULL;
	offendingNode = NULL;

	doubleBlackNull = new Node;
	doubleBlackNull->color = DOUBLE_BLACK;
}

Tree::~Tree()
{
	Clear();

	offendingNode = NULL;
	
	if(doubleBlackNull)
	{
		delete doubleBlackNull;
		doubleBlackNull = NULL;
	}
}

//Basically, if the root is NULL, then make a new root.
//Otherwise call the recursive function. If result is 0,
//It doesn't mean insertion failed. All it means is that
//the tree MIGHT still be unbalanced. If it's 0 after
//the recursive function, then it means either A) the root
//is red, or B) we changed the colors of nodes so the
//root MIGHT be red. If it isn't, then all is fine.
int Tree::Insert(int toAdd)
{
	int result; //What we will return

	if(!root)
	{
		root = new Node;
		root->key = toAdd;
		root->data = toAdd;

		result = 0;
	}

	else
		result = Insert(root, toAdd, toAdd);

	if(result == 0)
	{
		if(root->color != BLACK)
			root->color = BLACK;

		result = 1;
	}

	offendingNode = NULL;

	return result;
}

int Tree::Insert(Node*& root, int key, int toAdd)
{
	if(!root)
		return 0; //Something bad happened

	//Basically what we return after insertion/recursive call
	int result;
	//The child we will either insert at
	//or traverse in the next recursive call
	int childToTraverse = FindChildToTraverse(key, root->key);

	//First check if you can actually insert at this node
	if(root->AttemptInsertChild(toAdd, childToTraverse))
		//If we did, then check if there is any actual rebalancing
		//that needs to be done
		result = InsertIsImabalanced(root, childToTraverse);

	else
	{
		if(childToTraverse == LEFT)
			result = Insert(root->left, key, toAdd);
		else
			result = Insert(root->right, key, toAdd);

		if(result == 0)
			result = RotateAndRecolor(root, childToTraverse);
	}

	return result;
}

int Tree::InsertIsImabalanced(Node* root, const int childToTraverse)
{
	if(!root)
		return -1;

	int result; //What we will return

	//root is the tree's root, or is black, so balance preserved
	if((root == this->root) || root->color == BLACK)
		result = 1;

	else
	{
		//root is a red node, and so is the child. Thus an imablance
		offendingNode = root->GetChild(childToTraverse);
		result = 0;
	}

	return result;
}


int Tree::RotateAndRecolor(Node*& root, const int childToTraverse)
{
	int result; //What we will return

	//Check if the offending node is one of root's children. 
	//If so, and the root is black and offendingNode is red,
	//then the tree is balanced. Otherwise, we need to do one 
	//of the rotation/recoloring cases handled by HandleInsertionRebalance
	result = CheckParent(root);

	if(result == -1)
		result = HandleInsertionRebalance(root, childToTraverse);

	return result;
}

int Tree::CheckParent(Node* root)
{
	if(!root || !offendingNode)
		return -1;

	int result; //What we will return

	//We only handle imbalances at grandparent level, so if the offending
	//node is one of my children, then return 0. However, before we return 0,
	//if the offending node is red, and root is black, then return 1.
	if(offendingNode == root->left || offendingNode == root->right)
	{
		if(offendingNode->color == RED && root->color == BLACK)
			result = 1;
		else
			result = 0;
	}

	//Gonna be a grand child so we can actually 
	else
		result = -1;

	return result;
}

//If root is NULL, return -1. Otherwise, use the uncle to determine what
//insertion rebalancing case we want to do. Return 1 if the tree is fully
//rebalanced, or 0 if we still have cases to handle
int Tree::HandleInsertionRebalance(Node*& root, const int childToTraverse)
{
	if(!root)
		return -1;

	//What we will return
	int result;
	//Parent is the parent of the offending node,
	//and one of root's children
	Node* parent = NULL;
	//Use uncle to determine the balacing case we do
	Node* uncle = GetUncle(root, childToTraverse);

	//Case 1: If the uncle is red, make it, and the offending node's
	//parent black, and make the grandparent (IE root) red. Make
	//result 0
	if(uncle && uncle->color == RED)
	{
		InsertionCase1(root);
		result = 0; //The tree may still be unbalanced (IE tree's root is now red)
	}

	else
	{
		//The parent is, of course, the child we traversed to to
		//insert
		parent = root->GetChild(childToTraverse);

		//Case 2: parent is root's left, and offending node is parent's left
		if(childToTraverse == LEFT && offendingNode == parent->left)
			InsertionCase2(root, parent);

		//Case 3: parent is root's right, and offending node is parent's right
		else if(childToTraverse == RIGHT && offendingNode == parent->right)
			InsertionCase3(root, parent);

		//Case 4: Parent is root's left, and offending node is parent's right
		else if(childToTraverse == LEFT && offendingNode == parent->right)
			InsertionCase4(root, parent);

		//Case 5: Parent is root's right, and offending node is parent's left
		else
			InsertionCase5(root, parent);

		offendingNode = NULL;

		result = 1;
	}

	return result;
}

void Tree::InsertionCase1(Node* root)
{
	if(!root)
		return;

	if(root->left)
		root->left->color = BLACK;
	if(root->right)
		root->right->color = BLACK;

	root->color = RED;
	offendingNode = root;
}

void Tree::InsertionCase2(Node*& root, Node*& parent)
{
	if(!root || !parent)
		return;
	
	root->color = RED;
	parent->color = BLACK;

	LeftRotation(root);
}

void Tree::InsertionCase3(Node*& root, Node*& parent)
{
	if(!root || !parent)
		return;
	
	root->color = RED;
	parent->color = BLACK;

	RightRotation(root);
}

void Tree::InsertionCase4(Node*& root, Node*& parent)
{
	if(!root || !parent)
		return;
	
	root->color = RED;
	offendingNode->color = BLACK;
	
	RightRotation(parent);
	//When we do the right rotation, the parent node variable changes
	//to the offending node. However, the root is still pointing to
	//the old parent. So doing this we update our tree properly
	root->left = parent;
	LeftRotation(root);
}

void Tree::InsertionCase5(Node*& root, Node*& parent)
{
	if(!root || !parent)
		return;

	root->color = RED;
	offendingNode->color = BLACK;

	LeftRotation(parent);
	//When we do the right rotation, the parent node variable changes
	//to the offending node. However, the root is still pointing to
	//the old parent. So doing this we update our tree properly
	root->right = parent;
	RightRotation(root);
}

//Call the recursive function. If the result is -1, then we
//did not find toFind. As such, return 0. However, if result
//is 0, then the tree remains unbalanced after removal. This
//means that the root is double black, so we simply make it
//black.
int Tree::Remove(int toFind)
{
	//What we get from the recusrive function,
	//and wht we will return
	int result = Remove(root, toFind);

	//We did not find our data
	if(result == -1)
		result = 0;
	//The root is double black
	else if(result == 0)
	{
		RemoveCase1(root);
		result = 1;
	}

	return result;
}

int Tree::Remove(Node*& root, int key)
{
	//We did not find the key
	if(!root)
		return -1;

	int result; //What we will return

	//We found it!
	if(root->key == key)
	{
		//Root has two children, so we need to replace with the
		//in-order successor
		if(root->left && root->right)
		{
			//Replace key and data with in-order successor
			root->GetInOrderSuccessor();
			//Go remove the old in-order successor
			result = Remove(root->right, root->key);
		}

		else if(root->IsLeaf())
			result = RemoveIsLeaf(root);

		//Either the left or the right child exists. In this case,
		//we replace root with that child, but we must also check
		//the colors
		else
			result = RemoveHasOneChild(root);
	}

	else
	{
		if(key < root->key)
			result = Remove(root->left, key);
		else
			result = Remove(root->right, key);
	}

	//The second condition is to keep the balancing cases from
	//happening until we reach the parent of the deleted node
	if(result == 0 && root->color != DOUBLE_BLACK)
		result = HandleRemovalRebalance(root);

	return result;
}

int Tree::HandleRemovalRebalance(Node*& root)
{
	//The parent/root is the parent of the
	//offending node. In some cases, the
	//parent != root, so we use this to
	//reflect that
	Node* parent = root;
	//Rebalancing cases require that we check
	//the sibiling
	Node* sibiling = GetOffendingNodeSibiling(parent);

	//Note: Case 1 is the root of the entire tree 
	//being DOUBLE BLACK. This is handled in the
	//non-recursive Removal function

	//Case 2: The parent is BLACK, the sibiling is RED,
	//and the sibiling's children are BLACK
	if(parent->color == BLACK && 
	   sibiling && sibiling->color == RED &&
	   (!sibiling->left || sibiling->left->color == BLACK) && 
	   (!sibiling->right || sibiling->right->color == BLACK))
	{
		RemoveCase2(root, parent, sibiling);
	}

	//Case 3: Parent and sibiling are BLACK, and sibiling's
	//children are also BLACK. Note that Case 2 never becomes
	//case 3, since the parent becomes red. It goes to case 4
	//instead
	if(parent->color == BLACK && 
	   sibiling && sibiling->color == BLACK &&
	   (!sibiling->left || sibiling->left->color == BLACK) &&
	   (!sibiling->right || sibiling->right->color == BLACK))
	{
		RemoveCase3(parent, sibiling);
		return 0;
	}

	//Case 4: Parent is red, sibiling is black, and the
	//sibiling's children are black
	if(parent->color == RED && 
	   sibiling && sibiling->color == BLACK &&
	   (!sibiling->left || sibiling->left->color == BLACK) &&
	   (!sibiling->right || sibiling->right->color == BLACK))
	{
		RemoveCase4(parent, sibiling);
		return 1;
	}

	//The sibiling is black, and its inner child is red,
	//while its one one is black/NULL. In this case,
	//we rotate around the sibiling. If this does not
	//apply, then we go to case 6. Note that we check if
	//case 5 applies in the function RemoveCase5
	if(sibiling && sibiling->color == BLACK)
		RemoveCase5(parent, sibiling);

	//Case 6: The sibiling is black, while its outter child
	//is red. We rotate around the parent, then do some
	//recoloring.
	if(sibiling && sibiling->color == BLACK)
	{
		RemoveCase6(root, parent, sibiling);
		return 1;
	}

	//This is trivial, but we have it nonetheless
	return 0;
}

void Tree::RemoveCase1(Node*& root)
{
	if(root && root->color == DOUBLE_BLACK)
		FixDoubleBlack(root);
}

void Tree::RemoveCase2(Node*& root, Node*& parent, Node*& sibiling)
{
	//Once we perform a rotation, the offending node's
	//sibiling changes. So we must update that here
	Node* newSibiling;

	parent->color = RED;
	sibiling->color = BLACK;

	//In this case, rotate around the parent. Make the
	//sibiling black, and the parent red.
	//Handle the rotations according to which child is
	//the offending node
	
	//The offending node must be left
	if(sibiling == parent->right)
	{
		//Note that this changes the root of the subtree, but not
		//the parent. So we are safe
		RightRotation(root);
		newSibiling = parent->right;
	}

	//The offending node must be right
	else
	{
		LeftRotation(root);
		newSibiling = parent->left;
	}

	//Update the sibiling node
	sibiling = newSibiling;
}

void Tree::RemoveCase3(Node* parent, Node* sibiling)
{
	//Make the sibiling red, and parent double-black.
	//If the offending node is doubleBlackNull, then make
	//it NULL. Otherwise, make it a regular black. Return
	//0
	parent->color = DOUBLE_BLACK;
	sibiling->color = RED;

	//Offending node is the left
	if(parent->right == sibiling)
		FixDoubleBlack(parent->left);

	//The offending node is the right
	else
		FixDoubleBlack(parent->right);
}

void Tree::RemoveCase4(Node* parent, Node* sibiling)
{
	//Make the parent black and the sibiling red. Since
	//we now fixed the number of nodes in each tree,
	//we can then set the offending node to black or NULL
	parent->color = BLACK;
	sibiling->color = RED;

	//Offending node is left
	if(parent->right == sibiling)
		FixDoubleBlack(parent->left);

	//Offending node is the right
	else
		FixDoubleBlack(parent->right);
}

void Tree::RemoveCase5(Node* parent, Node*& sibiling)
{
	if(parent->right == sibiling && 
	  (!sibiling->right || sibiling->right->color == BLACK) &&
		sibiling->left && sibiling->left->color == RED)
	{
		//Recolor
		sibiling->color = RED;
		sibiling->left->color = BLACK;

		//Rotate the node
		LeftRotation(sibiling);

		//Sibiling will be updated, but not the 
		//parent/root child, so we need to update that
		parent->right = sibiling;
	}

	else if(parent->left == sibiling &&
			sibiling->right && sibiling->right->color == RED &&
			(!sibiling->left || sibiling->left->color == BLACK))
	{
		//Recolor
		sibiling->color = RED;
		sibiling->right->color = BLACK;
		
		//Rotate the nodes
		RightRotation(sibiling);

		//Update pointer
		parent->left = sibiling;
	}
}

void Tree::RemoveCase6(Node*& root, Node*& parent, Node*& sibiling)
{
	sibiling->color = parent->color;
	parent->color = BLACK;
	
	if(parent->left == sibiling)
	{
		//Color it black if we can
		if(sibiling->left)
			sibiling->left->color = BLACK;

		//Do the rotation
		LeftRotation(parent);

		//Note, by doing the rotation, the sibiling
		//is now the new parent
		FixDoubleBlack(parent->right->right);
	}

	else
	{
		//Color it black if we can
		if(sibiling->right)
			sibiling->right->color = BLACK;

		//Do the rotation
		RightRotation(parent);

		FixDoubleBlack(parent->left->left);
	}

	//Update the root. parent pointer is now the
	//sibiling, so this will work
	root = parent;
}

int Tree::RemoveIsLeaf(Node*& root)
{
	//This is a trivial check, since when 
	//this function is called, root is never NULL
	if(!root)
		return -1;

	int result; //What we will return

	if(root->color == RED)
	{
		delete root;
		root = NULL;

		result = 1; //Removing red leaves does nothing
	}

	else
	{
		delete root;
		root = doubleBlackNull;

		result = 0; //We removed a black, so imbalance
	}

	return result;
}

int Tree::RemoveHasOneChild(Node*& root)
{
	//If root has one child, we replace him
	//with this upon deletion
	Node* replacementChild;
	//What we will return
	int result;

	if(root->left)
		replacementChild = root->left;
	else
		replacementChild = root->right;
	

	//Either child is RED, or root is. Either way, make new root
	//black and return 1
	if(replacementChild->color == RED || root->color == RED)
	{
		replacementChild->color = BLACK;

		delete root;
		root = replacementChild;

		//We got rid of a red node, so property is violated
		result = 1; 
	}

	//Both nodes are BLACK, so make child a double black and
	//return 0
	else
	{
		replacementChild->color = DOUBLE_BLACK;

		delete root;
		root = replacementChild;

		result = 0;
	}

	return result;
}

Node* Tree::GetOffendingNodeSibiling(Node* parent)
{
	if(!parent)
		return NULL;

	Node* sibiling; //What we will return

	//First determine if the offending node is the left node
	//or the right node. Since only one node at a time is
	//the offending one, if this if-statement doesn't hold,
	//then it must be the right node that is offending.
	if(parent->left && parent->left->color == DOUBLE_BLACK)
		sibiling = parent->right;
	else
		sibiling = parent->left;

	return sibiling;
}

//Precondition: toFix exists and is double black
//Result: toFix becomes a black node
//If toFix is NULL or red, then return. If toFix is
//the double black NULL node, then make him NULL.
//Otherwise, set his color to black
void Tree::FixDoubleBlack(Node*& toFix)
{
	if(!toFix || toFix->color != DOUBLE_BLACK)
		return;

	if(toFix == doubleBlackNull)
		toFix = NULL;
	else
		toFix->color = BLACK;
}

//If the root or its right child are NULL, then return
//false. Otherwise, Make the root's right child the left
//child of its former right child. Then make root the
//left child of its former right child. Finally, make
//the original right child the new root
bool Tree::RightRotation(Node*& root)
{
	//Otherwise we cannot do a right rotation
	if(!root || !root->right)
		return false;

	//The child that replaces root upon finishing
	//the rotation
	Node* newRoot = root->right;

	//Perform the rotation itself
	root->right = newRoot->left;
	newRoot->left = root;

	//Update root
	root = newRoot;

	return true;
}

//If the root or its left child are NULL, then return
//false. Otherwise, Make the root's left child the right
//child of its former left child. Then make root the
//right child of its former left child. Finally, make
//the original left child the new root
bool Tree::LeftRotation(Node*& root)
{
	//Otherwise we cannot do a right rotation
	if(!root || !root->left)
		return false;

	//The child that replaces root upon finishing
	//the rotation
	Node* newRoot = root->left;

	//Perform the rotation itself
	root->left = newRoot->right;
	newRoot->right = root;

	//Update root
	root = newRoot;

	return true;
}

//If key is less than node, then return LEFT. Otherwise, return
//RIGHT
int Tree::FindChildToTraverse(const int key, const int node) const
{
	int flag; //What we will return

	if(key < node)
		flag = LEFT;
	else
		flag = RIGHT;

	return flag;
}

//If grandparent is NULL, return NULL. The traversal flag tells us
//which way we traversed to insert/remove. So if the flag is LEFT,
//then return the right node. If it is RIGHT, return the left node.
//If neither, then return
Node* Tree::GetUncle(Node* grandparent, const int traverseFlag)
{
	if(!grandparent || (traverseFlag != LEFT && traverseFlag != RIGHT))
		return NULL;

	if(traverseFlag == LEFT)
		return grandparent->right;
	else
		return grandparent->left; 
}

int Tree::Display() const
{
	int result = Display(root);
	cout << endl;

	return result;
}

//If root is NULL, then return 0. Call the
//function for the left child. Then display
//the data at root. Then call the function
//for the right child.
int Tree::Display(Node* root) const
{
	if(!root)
		return 0;

	//The results of calling the function
	//for the left and right children
	int left, right;

	left = Display(root->left);
	cout << root->data << " ";
	right = Display(root->right);

	return 1 + left + right;
}

int Tree::Clear()
{
	return Clear(root);
}

int Tree::Clear(Node*& root)
{
	if(!root)
		return 0;

	int left, right;

	left = Clear(root->left);
	right = Clear(root->right);

	delete root;
	root = NULL;

	return 1 + left + right;
}

/*NODE IMPLEMENTATIONS*/
Node::Node()
{
	key = 0;
	data = 0;
	color = RED;

	left = NULL;
	right = NULL;
}

Node::~Node()
{
	left = NULL;
	right = NULL;
}

//If the childToTraverse is LEFT, and left is NULL, then make left a 
//new node. If childToTraverse is RIGHT and right is NULL, then make 
//right a new node. Otherwise, return 0
int Node::AttemptInsertChild(int toAdd, int childToTraverse)
{
	int result; //What we will return

	if(childToTraverse == LEFT && !left)
	{
		left = new Node;
		left->key = toAdd;
		left->data = toAdd;

		result = 1;
	}

	else if(childToTraverse == RIGHT && !right)
	{
		right = new Node;
		right->key = toAdd;
		right->data = toAdd;

		result = 1;
	}

	else
		result = 0;

	return result;
}


bool Node::IsLeaf()
{
	return !left && !right;
}

bool Node::GetInOrderSuccessor()
{
	//The in-order successor is the left-most leaf of
	//the right subtree
	Node* temp = right;
	//What we will return
	bool result;

	if(!temp) //If no in-order successor
		result = false;
	else
	{
		while(temp->left)
			temp = temp->left;

		key = temp->key;
		data = temp->data;

		result = true;
	}

	return result;
}

Node* Node::GetChild(int childFlag)
{
	Node* child; //What we will return

	if(childFlag == LEFT)
		child = left;
	else if(childFlag == RIGHT)
		child = right;
	else
		child = NULL;

	return child;
}
