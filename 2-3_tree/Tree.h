class Tree
{
	public:
		Tree();
		~Tree();

		//Insert toAdd into the tree
		int Insert(int toAdd);
		//Remove toRemove from the tree
		int Remove(int toRemove);
		//Display all items in the tree. Return item count.
		int Display();
		//Clears the entire 2-3 tree
		void Clear();

	private:
		/*General case recursive functions*/
		//key is used to traverse, root is our current sub-tree, and subTree
		//tells us if we are the root, left, center, or right sub-trees
		int Insert(int toAdd, int key, struct Node*& root, int subTree);
		int Remove(int toRemove, int key, struct Node*& root, int subTree);
		int Display(struct Node* root);
		void Clear(struct Node*& root);

		/*Insertion utility functions*/
		//When root is just a leaf, we handle that here
		int InsertRootIsLeaf(int toAdd, int key);
		//Insert a node, as the left child, into a 2-node root
		int Insert2NodeLeft(int key, int toAdd, struct Node* root);
		//Insert a node, as the right child, into a 2-node root
		int Insert2NodeRight(int key, int toAdd, struct Node* root);
		//Insert a node, as the left child, into a 3-node root
		int Insert3NodeLeft(int key, int toAdd, struct Node* root);
		//Insert a node, as the center child, into a 3-node root
		int Insert3NodeCenter(int key, int toAdd, struct Node* root);
		//Insert a node, as the right child, into a 3-node root
		int Insert3NodeRight(int key, int toAdd, struct Node* root);
		//When rebalancing the tree, and the split node was the left child of a 2-node root
		int InsertBalance2NodeLeft(struct Node* root);
		//When rebalancing the tree, and the split node was the right child of a 2-node root
		int InsertBalance2NodeRight(struct Node* root);
		//When rebalacing the tree, and the split node was the left child of a 3-node root
		int InsertBalance3NodeLeft(struct Node* root);
		//When rebalacing the tree, and the split node was the center child of a 3-node root
		int InsertBalance3NodeCenter(struct Node* root);
		//When rebalacing the tree, and the split node was the right child of a 3-node root
		int InsertBalance3NodeRight(struct Node* root);

		/*Removal utility functions*/
		//A special case when the root is just a key
		int RemoveRootIsLeaf(int toRemove, int key);
		//Determines which of root's children has toRemove, if it's children are leaves
		int RemoveContains(int toRemove, int key, struct Node* root);
		//Removes the given child from root
		int Remove(struct Node* root, int child);
		//If root was a 3-node, and we removed an item, we use this to fix it up
		void RemoveFix3Node(struct Node* root, int removedChild);
		//Our root is a 2-node, and the left child needs to be fixed
		int RemoveBalance2NodeLeft(struct Node* root);
		//Our root is a 2-node, and the right child needs to be fixed
		int RemoveBalance2NodeRight(struct Node* root);
		//Our root is a 3-node, and the left child needs to be fixed
		int RemoveBalance3NodeLeft(struct Node* root);
		//Our root is a 3-node, and the center child needs to be fixed
		int RemoveBalance3NodeCenter(struct Node* root);
		//Our root is a 3-node, and the right child needs to be fixed
		int RemoveBalance3NodeRight(struct Node* root);

		//Finds the smallest key of a given subtree
		int FindSmallestKey(struct Node* root);
		//Determines what child we go to
		int FindChildToTraverse(int key, struct Node* root);

		struct Node* root;
		struct Node* temp;
};

struct Node
{
	Node();
	~Node();

	bool IsLeaf();
	bool Is2Node();

	Node* left;
	Node* center;
	Node* right;

	int key1, key2;

	int data;
};
