class Tree
{
	public:
		Tree();
		~Tree();

		int Insert(int toAdd);
		int Remove(int toRemove);
		void Display() const;
		void Clear();

	private:
		int Insert(int key, int toAdd, struct Node*& root);
		int Remove(int key, struct Node*& root);
		void Display(struct Node* root) const;
		void Clear(struct Node*& root);

		/*BALANCING UTILITIES*/
		//If root is unbalanced, we balance it
		void Rebalance(struct Node*& root);
		//Determines if a tree is balanced or not
		bool IsBalanced(struct Node* toCheck);

		/*ROTATION UTILITIES*/
		void LeftRotation(struct Node*& root);
		void RightRotation(struct Node*& root);

		/*UTILITY FUNCTIONS*/
		//Calculate the new height of a subtree
		void RecalculateHeight(struct Node* root);
		//Return the height of a subtree
		int GetSubtreeHeight(struct Node* subtree);
		//Returns the child of subtree wit the longer path
		struct Node* GetChildLongestPath(struct Node* subtree);

		struct Node* root;
};

struct Node
{
	Node();
	~Node();

	bool IsLeaf();
	void SetAsInOrderSuccessor();

	int key;
	int data;

	int height;

	Node* left;
	Node* right;
};
