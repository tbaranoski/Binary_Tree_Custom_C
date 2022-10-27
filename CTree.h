
#ifndef _CTREE_H_
#define _CTREE_H_

class TreeNode {
public:
	TreeNode(int key);  //used for root
	TreeNode(int key, TreeNode *parent);

	//Regular functions
	bool adjustHeight(TreeNode *tempNode);
	bool adjustSize(TreeNode *tempNode);

	//getters
	TreeNode* getm_left();
	TreeNode* getm_right();
	TreeNode* getm_parent();
	int getm_key();
	int getm_size();
	int getm_height();

	//setters
	void setm_left(TreeNode *node);
	void setm_right(TreeNode *node);
	void setm_parent(TreeNode *node);
	void setm_key(int value);
	void setm_size(int value);
	void setm_height(int value);

private:
	TreeNode *m_left;
	TreeNode *m_right;
	TreeNode *m_parent;
	int m_key;
	int m_size;
	int m_height;

};


class CTree {

public:
	CTree();
	~CTree();
	CTree(const CTree& other);
	const CTree& operator=(const CTree& rhs);
	void insert(int key);
	bool remove(int key);
	bool find(int key);
	void rebalance(TreeNode *&tempNode); //can change
	void inorder(); 
	bool locate(const char *position, int& key);

	bool findRecursivelly(int key, TreeNode* nodePointer);
	TreeNode* findParent(int key, TreeNode* nodePointer);
	bool recursiveInOrder(TreeNode* tempNode);
	void findNode(int key, TreeNode* tempNode);
	TreeNode* getInOrderSuccessor(TreeNode* tempNode);
	TreeNode* getInOrderSuccessorLeft(TreeNode* tempNode);
	void recursiveDestructor(TreeNode * tempNode);
	TreeNode* findImbalance(TreeNode * tempNode, TreeNode * needsRebelancing);
	void populateArray(int *&myArray, TreeNode* tempNode, int &index);
	bool restructureSubTree(int *&myArray, int leftBound, int rightBound, TreeNode*&subTreeRoot);
	bool triNode(TreeNode* imbalancedNode);
	

private:
	TreeNode *m_root;
	int m_numNodes;
};


#endif