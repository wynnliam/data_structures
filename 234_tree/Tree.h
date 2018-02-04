class Tree
{
	public:
		Tree();
		~Tree();

		//Insert data into tree
		void Insert(const int data);
		//Remove data from the tree
		bool Remove(const int key);
		//Clears the entire tree
		void Clear();

		//Displays the data in the tree
		void Display();

	private:
		//Insert recursively. Note that splitChild simply stores the
		//child node of the previous recursive root, if splitting occured.
		//If splitChild has no parent, then he must be the root of the
		//entire tree. splitChild exists for the purpose of handling merging
		//after returning from a recursive call
		void Insert(const int key, const int data,
					struct Node*& root, struct Node*& splitChild);
		bool Remove(const int key, struct Node*& root, struct Node*& splitChild);

		/*INSERT UTILITY PROCEDURES*/
		//Handles the logic of splitting root
		void SplitRoot(struct Node* root, struct Node*& splitChild);
		//Handles integration logic
		void Integrate(struct Node* root, struct Node*& toIntegrate);
		//Two sub-procudures of Integrate. One handles a root that
		//is a 2-node, and one for root being a 3-node
		void IntegrateRoo2Node(struct Node* root, struct Node*& toIntegrate);
		void IntegrateRoo3Node(struct Node* root, struct Node*& toIntegrate);

		/*REMOVE UTILITY PROCEDURES*/
		//Attempts to fix the offending child. If a fusion case must happen
		//we return false (meaning the tree, after fixing, we still have violations)
		bool AttemptFix(struct Node*& root, struct Node*& offendingChild);
		//Root is a 2-node, offending child is left, and both children are
		//2-nodes.
		void FixRoot2NodeLeftOffendingRight2Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 2-node, offending child is left, and right is either a 3 or 4 node.
		void FixRoot2NodeLeftOffendingRight34Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 2-node, offending child is right, and left is a 2-node.
		void FixRoot2NodeRightOffendingLeft2Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 2-node, offending child is right, and left is either a 3 or 4 node.
		void FixRoot2NodeRightOffendingLeft34Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 3-node, offending child is left, and centMin is a 2-node.
		void FixRoot3NodeLeftOffendingCentMin2Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 3-node, offending child is left, and centMin is either a 3 or 4 node.
		void FixRoot3NodeLeftOffendingCentMin34Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 3-node, offending is centMin, and left is either a 3 or 4 node.
		void FixRoot3NodeCentMinOffendingLeft34Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 3-node, offending is centMin, and right is either a 3 or 4 node.
		void FixRoot3NodeCentMinOffendingRight34Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 3-node, offending is centMin, and left and right are 2-nodes.
		void FixRoot3NodeCentMinOffendingLeftRight2Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 3-node, offending is right, and centMin is a 2-node.
		void FixRoot3NodeRightOffendingCentMin2Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 3-node, offending is right, and centMin is either a 3 or 4 node.
		void FixRoot3NodeRightOffendingCentMin34Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 4-node, offending is left, and centMin is a 2-node.
		void FixRoot4NodeLeftOffendingCentMin2Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 4-node, offending is left, and centMin is either a 3 or 4 node.
		void FixRoot4NodeLeftOffendingCentMin34Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 4-node, offending is centMin, and left is either a 3 or 4 node.
		void FixRoot4NodeCentMinOffendingLeft34Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 4-node, offending is centMin, and centMax is either a 3 or 4 node.
		void FixRoot4NodeCentMinOffendingCentMax34Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 4-node, offending is centMin, and sibilings are 2-nodes.
		void FixRoot4NodeCentMinOffendingSibilingsNode(struct Node*& root, struct Node*& offendingChild);
		//Root is a 4-node, offending is centMax, and centMin is either a 3 or 4 node.
		void FixRoot4NodeCentMaxOffendingCentMin34Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 4-node, offending is centMax, and right is either a 3 or 4 node.
		void FixRoot4NodeCentMaxOffendingRight34Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 4-node, offending is centMax, and sibilings are 2 nodes.
		void FixRoot4NodeCentMaxOffendingSibilings2Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 4-node, offending is right, and centMax is a 2-node.
		void FixRoot4NodeRightOffendingCentMax2Node(struct Node*& root, struct Node*& offendingChild);
		//Root is a 4-node, offending is right, and centMax is either a 3 or 4 node.
		void FixRoot4NodeRightOffendingCentMax34Node(struct Node*& root, struct Node*& offendingChild);

		//Recursively clears the entire tree
		void Clear(struct Node*& root);
		//Recursively display the data
		void Display(struct Node* root);

		Node* root;
};

struct Node
{
	Node();
	~Node();

	//Inserts data if possible. Returns if it successfully
	//added toInsert into the node
	bool Insert(const int key, const int toInsert);
	//Removes the first instance of key from the node.
	//If it did not find it, it returns -1. If it found it
	//and removed it and was able to fix the node, it returns
	//1. Otherwise it will return 0.
	int Remove(const int key);
	//Returns true if this node has the key
	bool Contains(const int key);
	//A debug function to display all data in the node
	void Display() const;

	//A simple function that, given a key, will determine
	//which child to traverse to
	int FindChildToTraverse(const int key);
	//Swaps data at key with the in-order successor
	void SwapWithInOrderSuccessor(const int key, int& traverse);

	//Returns true if this is a leaf
	bool IsLeaf() const;
	//Returns true if this is a 2-node
	bool Is2Node() const;
	//Returns true if this is a 3-node
	bool Is3Node() const;
	//Returns true if this is a 4-node
	bool Is4Node() const;

	//Stores the data in this node
	int data[3];
	//Stores the associated keyes for each data
	int keyes[3];
	
	//The child nodes
	Node* left;
	Node* centMin;
	Node* centMax;
	Node* right;
};
