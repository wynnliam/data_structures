/*
 Properties:
	1. Every node is RED or BLACK
	2. NULL node is BLACK
	3. RED nodes have two BLACK children (heh)
	4. Every path from node X down to a leaf
	   Has the same number of BLACK nodes
	5. The root is always BLACK
*/

class Tree
{
	public:
		Tree();
		~Tree();

		//Insert toAdd into the tree
		int Insert(int toAdd);
		//Removes toFind from the tree, if it exists
		int Remove(int toFind);
		//Display the tree. Return 0 if empty
		int Display() const;
		//Clear the tree. Return 0 if empty
		int Clear();

	private:
		int Insert(struct Node*& root, int key, int toAdd);
		int Remove(struct Node*& root, int key);
		int Display(struct Node* root) const;
		int Clear(struct Node*& root);

		/*Traversal utilities*/
		//Given a node's key and some other key, determine which child
		//we want to traverse to
		int FindChildToTraverse(const int key, const int node) const;
		
		/*Insertion Balancing Cases*/
		void InsertionCase1(struct Node* root);
		//Note that root is the grandparent, so parent is
		//a child of root
		void InsertionCase2(struct Node*& root, struct Node*& parent);
		void InsertionCase3(struct Node*& root, struct Node*& parent);
		void InsertionCase4(struct Node*& root, struct Node*& parent);
		void InsertionCase5(struct Node*& root, struct Node*& parent);
		/*Insertion Utilities*/
		//After we insert a node, call this to determine if there are
		//imbalances
		int InsertIsImabalanced(struct Node* root, const int childToTraverse);
		//Manages all of the rotation and recoloring cases
		int RotateAndRecolor(struct Node*& root, const int childToTraverse);
		//As we return from our recursive calls, sometimes the offending
		//node is one of root's children. In this case, we can check if
		//the tree is balanced or not. Otherwise, we must handle imbalances
		//at the grandparent level
		int CheckParent(struct Node* root);
		//Handles all five insertion balacing cases. Returns 1 if the tree
		//is now completely balanced, or 0 if it still needs rebalancing.
		//childToTraverse is the child node we went to when in serting
		int HandleInsertionRebalance(struct Node*& root, const int childToTraverse);
		//Given a node a traversal flag, get the uncle node
		struct Node* GetUncle(struct Node* grandparent, const int traverseFlag);

		/*Removal Balance Cases*/
		//Once we deduce we need to perform one of the removal balancing
		//cases, we use this function to determine which one we do
		int HandleRemovalRebalance(struct Node*& root);
		void RemoveCase1(struct Node*& root);
		void RemoveCase2(struct Node*& root, struct Node*& parent, struct Node*& sibiling);
		void RemoveCase3(struct Node* parent, struct Node* sibiling);
		void RemoveCase4(struct Node* parent, struct Node* sibiling);
		void RemoveCase5(struct Node* parent, struct Node*& sibiling);
		void RemoveCase6(struct Node*& root, struct Node*& parent, struct Node*& sibiling);
		/*Removal Utilities*/
		//Call this when the node we delete is a leaf
		int RemoveIsLeaf(struct Node*& root);
		//Call this when the node we delete has one child
		int RemoveHasOneChild(struct Node*& root);
		//Returns the sibiling of the offending node (assuming the
		//offending node's parent is parent)
		struct Node* GetOffendingNodeSibiling(struct Node* parent);
		//Makes a double black NULL or just regular black
		void FixDoubleBlack(struct Node*& toFix);

		/*Rotations*/
		//Performs a right rotation around the root, assuming it and its
		//right child exist
		bool RightRotation(struct Node*& root);
		//Perform a left rotation around the root, provided it and its
		//left child exist
		bool LeftRotation(struct Node*& root);

		struct Node* root;
		struct Node* offendingNode; //Maintain a pointer to the offending node
		//When we delete a black leaf, we violate property 4. So we replace it
		//with this Node until the tree is full balanced
		struct Node* doubleBlackNull;
};

struct Node
{
	Node();
	~Node();

	//Returns if the node is a leaf
	bool IsLeaf();
	//Returns the key and data of the in-order successor
	bool GetInOrderSuccessor();
	//Given a flag, return the left or right child. If bad flag,
	//then return NULL
	Node* GetChild(int childFlag);
	//Given an insertion flag and a node, attempt to insert a child
	//node. Return 1 on success, and 0 if the child node already exists
	int AttemptInsertChild(int toAdd, int insertFlag);

	int key;
	int data;
	int color; //True = black, False = red

	Node* left;
	Node* right;
};
