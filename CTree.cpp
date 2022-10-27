#include "CTree.h"
#include <iostream>
using namespace std;

//Constants
const int NO_HEIGHT = 0;//The height of a subtree that has 0 children.
const int SIZE_OF_ONE = 1; //A size of subtree equals 1.



bool CTree::triNode(TreeNode* imbalancedNode) {
	//We are going to perform trinode reconstructering on
	//the imbalancedNode
	//imbalancedNode is Z
	int x = imbalancedNode->getm_key();
	int y;
	int z;

	//If size of one subtree is 4, and the other is 1like in 
	//analysis.txt then this function will work
	int leftSize;
	int rightSize;
	leftSize = imbalancedNode->getm_left()->getm_size();
	rightSize = imbalancedNode->getm_right()->getm_size();

	//Find which subtree is bigger
	//left subtreee is larger
	TreeNode* temp;
	if (leftSize > rightSize) {
		y = imbalancedNode->getm_left()->getm_key();
		temp = getInOrderSuccessorLeft(imbalancedNode->getm_left());
		z = temp->getm_key();

		TreeNode *temp2 = imbalancedNode->getm_left();
		
		//Begin restructure
		temp->setm_right(imbalancedNode);
		temp->setm_parent(NULL);
		temp->setm_left(temp2);
		temp2->setm_right(temp);
	}
	//right subtree is larger
	else {
		y = imbalancedNode->getm_right()->getm_key();
		temp = getInOrderSuccessor(imbalancedNode->getm_right());
		TreeNode* temp2 = imbalancedNode->getm_right();
		z = temp->getm_key();

		imbalancedNode->setm_right(NULL);
		imbalancedNode->setm_parent(temp);
		temp->setm_left(imbalancedNode);
		temp->setm_right(temp2);
		temp2->setm_parent(temp);
	}
	return true;
}




//Copy Constructor
//used to initialize a previously uninitialized 
//object from some other object's data.
CTree::CTree(const CTree& other) {

	



}



CTree::CTree(){
	//Chestnut Tree Constructor
	m_root = NULL;
}

CTree::~CTree() {
	//Destructor for CTree
	recursiveDestructor(m_root);
}

void CTree::recursiveDestructor(TreeNode * tempNode) {

	//perform post order destructor
	if (tempNode != NULL) {

		//Go dwon left subtree
		if (tempNode->getm_left() != NULL) {

			//Recurse down left subtree
			recursiveDestructor(tempNode->getm_left());
		}

		//Move down the right subTree if possible
		if (tempNode->getm_right() != NULL) {

			//Recurse down right subtree
			recursiveDestructor(tempNode->getm_right());
		}

		//Delete 
		delete tempNode;
	}
	
}



bool CTree::locate(const char *position, int& key) {

	TreeNode * tempNode = m_root;
	//Convert to regular string
	const string myString(position);

	//Read one letter at a time from *position

	for (int i = 0; i < myString.length(); ++i){
		//If the character is 'L'
		if(myString[i] == 'L'){

			//if left node exist set to that
			if (tempNode->getm_left() != NULL) {
				tempNode = tempNode->getm_left();
			}

			//If the left node does not exist then return false
			else {
				return false;
			}

		}
		//If the character is 'R'
		else {

			//if right node exist set to that
			if (tempNode->getm_right() != NULL) {
				tempNode = tempNode->getm_right();
			}

			//If the right node does not exist return false.
			else {
				return false;
			}

		}
	}

	//Set Key equal to key of tempNode
	key = tempNode->getm_key();
	return true;
}


bool CTree::remove(int key) {

	TreeNode *tempPointer;
	TreeNode *tempParent;
	TreeNode *childNode;
	bool keyExists;
	//Locate TreeNode that has that key.
	keyExists = find(key);

	//If the key exists remove it.
	if (keyExists == true) {
		findNode(key, m_root);
		return true;
	}

	//If the key does not exist
	else{
		return false;
	}

}

void CTree::findNode(int key, TreeNode *tempNode) {

	TreeNode *tempPointer;
	TreeNode *tempParent;
	TreeNode *childNode;
	bool exit = false;

	//Base case: We have found the key.
	if (tempNode->getm_key() == key) {

		tempPointer = tempNode;


		//1.Removing root where only the root exists.
		if (tempPointer == m_root && tempPointer->getm_size() == SIZE_OF_ONE) {
			delete m_root;
			m_root = NULL;
			exit = true;
		}

		//2.Removing root that has atleast one child.
		if (exit == false) {
			if (tempPointer == m_root && tempPointer->getm_size() > SIZE_OF_ONE) {
				bool leftExists;
				bool rightExists;

				//Determine if the root has one child or two
				if (tempPointer->getm_left() == NULL) {
					leftExists = false;
				}
				else {
					leftExists = true;
				}

				if (tempPointer->getm_right() == NULL) {
					rightExists = false;
				}
				else {
					rightExists = true;
				}

				//1.If only one node exists. Assign that node to m_root.
				if (leftExists == true && rightExists == false) {

					childNode = tempPointer->getm_left();
					childNode->setm_parent(NULL);
					delete m_root;
					m_root = childNode;
				}
				if (leftExists == false && rightExists == true) {
					childNode = tempPointer->getm_right();
					childNode->setm_parent(NULL);
					delete m_root;
					m_root = childNode;

					//Rebalance Procedure
					TreeNode* imbalancedNode = NULL;
					imbalancedNode = findImbalance(childNode, imbalancedNode);

					//Determine if there is an imbalance or not
					//There is an imbalance
					if (imbalancedNode != NULL) {

						//Call rebalance on that Node
						rebalance(imbalancedNode);
					}

				}

				//2.If tempPointer has two children find the smallest key on
				//right subtree of m_root

				if (leftExists == true && rightExists == true) {

					//Find the inorder successor
					TreeNode * inOrderSuccessor;
					TreeNode * childNode;
					inOrderSuccessor = getInOrderSuccessor(tempPointer->getm_right());
					int tempKey = inOrderSuccessor->getm_key();

					//Two Cases:
					//1.InOrderSuccessor's parent is m_root
					//2.InOrderSuccessor's parent is not m_root

					//1.InOrderSuccessor's parent is m_root
					if (inOrderSuccessor->getm_parent() == m_root) {

						//Make m_roots key equal to inOrderSuccessors key.
						//Then set roots m_right = NULL.
						tempKey = inOrderSuccessor->getm_key();
						m_root->setm_key(tempKey);

						//1.Determine if inOrderSuccessor has a right child
						//If inorder Successor has a right child
						if (inOrderSuccessor->getm_right() != NULL) {
							childNode = inOrderSuccessor->getm_right();

							//Set m_roots right child to point to
							//childNode. Set childNodes parent to tempPointer
							tempPointer->setm_right(childNode);
							childNode->setm_parent(tempPointer);

							//Delete inorderSuccessor
							delete inOrderSuccessor;
							m_root->adjustHeight(childNode);
							m_root->adjustSize(childNode);

							//Rebalance Procedure
							TreeNode* imbalancedNode = NULL;
							imbalancedNode = findImbalance(childNode, imbalancedNode);

							//Determine if there is an imbalance or not
							//There is an imbalance
							if (imbalancedNode != NULL) {

								//Call rebalance on that Node
								rebalance(imbalancedNode);
							}

						}

						//2.If In Order successor does not have a right child.
						else {

							int inOrderSuccessorKey;
							inOrderSuccessorKey = inOrderSuccessor->getm_key();

							//set tempPointer key to inOrderSuccessor's key.
							//Set temppointers m_right to NULL
							tempPointer->setm_key(inOrderSuccessorKey);
							tempPointer->setm_right(NULL);
							delete inOrderSuccessor;

							//Subtract one from m_roots size
							int tempSize = m_root->getm_size();
							m_root->setm_size(tempSize - 1);

							//Rebalance Procedure
							TreeNode* imbalancedNode = NULL;
							imbalancedNode = findImbalance(m_root, imbalancedNode);

							//Determine if there is an imbalance or not
							//There is an imbalance
							if (imbalancedNode != NULL) {

								//Call rebalance on that Node
								rebalance(imbalancedNode);
							}
						}

						exit = true;
					}


					//2.InOrderSuccessor's parent is not m_root
					if (exit == false) {
						if (inOrderSuccessor->getm_parent() != m_root) {

							int inOrderSuccessorKey;
							inOrderSuccessorKey = inOrderSuccessor->getm_key();
							//There are two cases
							//1.The Inorder Successor has no right child
							//2. The Inorder Successor has a right child

							//1.The Inorder Successor has no right child
							if (inOrderSuccessor->getm_right() == NULL) {

								//move the key from inOrderSuccessor into tempPointer
								//Get m_parent of inOrderSuccessor and set m_left
								//to NULL
								tempPointer->setm_key(inOrderSuccessorKey);
								tempParent = inOrderSuccessor->getm_parent();
								tempParent->setm_left(NULL);
								delete inOrderSuccessor;
								m_root->adjustSize(tempParent);
								m_root->adjustSize(tempParent);

								//Rebalance Procedure
								TreeNode* imbalancedNode = NULL;
								imbalancedNode = findImbalance(tempParent, imbalancedNode);

								//Determine if there is an imbalance or not
								//There is an imbalance
								if (imbalancedNode != NULL) {

									//Call rebalance on that Node
									rebalance(imbalancedNode);
								}

							}

							//2. The Inorder Successor has a right child
							else {

								int childKey;

								//Move key of child to tempNode
								//Recconect right leave of inOrderSuccessor to parent
								childNode = inOrderSuccessor->getm_right();
								tempPointer->setm_key(inOrderSuccessorKey);
								tempParent = inOrderSuccessor->getm_parent();

								tempParent->setm_left(childNode);
								childNode->setm_parent(tempParent);
								delete inOrderSuccessor;
								m_root->adjustHeight(childNode);
								m_root->adjustSize(childNode);

								//Rebalance Procedure
								TreeNode* imbalancedNode = NULL;
								imbalancedNode = findImbalance(childNode, imbalancedNode);

								//Determine if there is an imbalance or not
								//There is an imbalance
								if (imbalancedNode != NULL) {

									//Call rebalance on that Node
									rebalance(imbalancedNode);
								}

							}
						}
					}
				}
				exit = true;
			}

			//3.Removing not a leaf and not a root. (internal)
			if (exit == false) {
				if (tempPointer != m_root && (tempPointer->getm_left()) != NULL || (tempPointer->getm_right() != NULL)) {

					//There are 2 case:
					//1.Node being removed has one child.
					//2.Node being removed has 2 children.

					//1.Node being removed has only one child. 
					//make parent of tempNodes child point to tempNodes child. 
					//Make tempNodes childs m_parent point to tempNodes parent.

					bool leftChild;
					bool rightChild;
					//Assign booleans

					if (tempPointer->getm_left() != NULL) {
						leftChild = true;
					}
					else {
						leftChild = false;
					}

					if (tempPointer->getm_right() != NULL) {
						rightChild = true;
					}
					else {
						rightChild = false;
					}


					//1.Node being removed has only one child. 
					//make parent of tempNodes child point to tempNodes child. 
					//Make tempNodes childs m_parent point to tempNodes parent.
					if ((leftChild == true && rightChild == false) || (leftChild == false && rightChild == true)) {
						tempParent = tempPointer->getm_parent();

						//Determine which child exists
						bool leftChild;
						bool rightChild;

						//if the left child  does not exists
						if (tempPointer->getm_left() == NULL) {
							leftChild = false;
						}
						//If the left child does exist
						else {
							leftChild = true;
						}

						//if the right child does not exist
						if (tempPointer->getm_right() == NULL) {
							rightChild = false;
						}
						//If the left child does exist
						else {
							rightChild = true;
						}

						//If the right child is the only child initialize childNode
						if (rightChild == true) {
							childNode = tempPointer->getm_right();
						}
						else {
							childNode = tempPointer->getm_left();
						}

						//make parent of tempNodes child point to tempNodes child. 
						//Make tempNodes childs m_parent point to tempNodes parent.
						childNode->setm_parent(tempParent);

						//Determine if tempParents right or left node
						//is being deleted
						int parentKey = tempParent->getm_key();

						//Left subtree of parent
						if (key < parentKey) {
							tempParent->setm_left(childNode);
						}
						//Right subtree of parent
						if (key > parentKey) {
							tempParent->setm_right(childNode);
						}
						exit = true;
						delete tempPointer;
						m_root->adjustHeight(childNode);
						m_root->adjustSize(childNode);

						//Rebalance Procedure
						TreeNode* imbalancedNode = NULL;
						imbalancedNode = findImbalance(childNode, imbalancedNode);

						//Determine if there is an imbalance or not
						//There is an imbalance
						if (imbalancedNode != NULL) {

							//Call rebalance on that Node
							rebalance(imbalancedNode);
						}

					}
					//2.Node being removed has 2 children.
					if (exit == false) {
						if (tempPointer->getm_left() != NULL && tempPointer->getm_right() != NULL) {

							//find inorder successor.
							TreeNode* inOrderSuccessor;
							inOrderSuccessor = getInOrderSuccessor(tempPointer->getm_right());
							int inOrderSuccessorKey = inOrderSuccessor->getm_key();

							//Cases
							//1.In order successor's parent is nodePointer 
							//2.In order successors parent is NOT NodePointer
							bool exit2 = false;

							//1.In order successor's parent is nodePointer
							if (inOrderSuccessor->getm_parent() == tempPointer) {

								//Make tempPointer's key = inOrderSuccessors key
								tempPointer->setm_key(inOrderSuccessorKey);

								//Determine if inOrderSuccessor has a right child
								//If inorder Successor has a right child
								if (inOrderSuccessor->getm_right() != NULL) {
									childNode = inOrderSuccessor->getm_right();

									//Set tempPointers right child to point to
									//childNode. Set childNodes parent to tempPointer
									tempPointer->setm_right(childNode);
									childNode->setm_parent(tempPointer);

									//Delete inorderSuccessor
									delete inOrderSuccessor;
									m_root->adjustHeight(childNode);
									m_root->adjustSize(childNode);

									//Rebalance Procedure
									TreeNode* imbalancedNode = NULL;
									imbalancedNode = findImbalance(childNode, imbalancedNode);

									//Determine if there is an imbalance or not
									//There is an imbalance
									if (imbalancedNode != NULL) {

										//Call rebalance on that Node
										rebalance(imbalancedNode);
									}

								}
								//If inorder successor does not have a right child.
								else {

									//set tempPointer key to inOrderSuccessor's key.
									//Set temppointers m_right to NULL
									tempPointer->setm_key(inOrderSuccessorKey);
									tempPointer->setm_right(NULL);
									tempParent = inOrderSuccessor->getm_parent();
									delete inOrderSuccessor;
									m_root->adjustHeight(tempPointer);
									m_root->adjustSize(tempPointer);


									//Rebalance Procedure
									TreeNode* imbalancedNode = NULL;
									imbalancedNode = findImbalance(tempParent, imbalancedNode);

									//Determine if there is an imbalance or not
									//There is an imbalance
									if (imbalancedNode != NULL) {

										//Call rebalance on that Node
										rebalance(imbalancedNode);
									}

								}
								exit2 = true;
							}

							//2.In order successors parent is NOT NodePointer
							if (exit2 == false) {
								if (inOrderSuccessor->getm_parent() != tempPointer) {

									//There are two cases
									//1.The Inorder Successor has no right child
									//2. The Inorder Successor has a right child

									//1.The Inorder Successor has no children
									if (inOrderSuccessor->getm_right() == NULL) {

										//move the key from inOrderSuccessor into tempPointer
										//Get m_parent of inOrderSuccessor and set m_left
										//to NULL
										tempPointer->setm_key(inOrderSuccessorKey);
										tempParent = inOrderSuccessor->getm_parent();
										tempParent->setm_left(NULL);
										delete inOrderSuccessor;

										m_root->adjustHeight(tempParent);
										m_root->adjustSize(tempParent);

										//Rebalance Procedure
										TreeNode* imbalancedNode = NULL;
										imbalancedNode = findImbalance(tempParent, imbalancedNode);

										//Determine if there is an imbalance or not
										//There is an imbalance
										if (imbalancedNode != NULL) {

											//Call rebalance on that Node
											rebalance(imbalancedNode);
										}
									}

									//2. The Inorder Successor has a right child
									else {

										int childKey;

										//Move key of child to tempNode
										//Recconect right leave of inOrderSuccessor to parent
										childNode = inOrderSuccessor->getm_right();
										tempPointer->setm_key(inOrderSuccessorKey);
										tempParent = inOrderSuccessor->getm_parent();

										tempParent->setm_left(childNode);
										childNode->setm_parent(tempParent);
										delete inOrderSuccessor;
										m_root->adjustHeight(childNode);
										m_root->adjustSize(childNode);

										//Rebalance Procedure
										TreeNode* imbalancedNode = NULL;
										imbalancedNode = findImbalance(childNode, imbalancedNode);

										//Determine if there is an imbalance or not
										//There is an imbalance
										if (imbalancedNode != NULL) {

											//Call rebalance on that Node
											rebalance(imbalancedNode);
										}
									}

								}
							}

						}
					}
					exit = true;

				}
			}

			//4.Removing leaf
			if (exit == false) {
				if ((tempPointer->getm_left()) == NULL && (tempPointer->getm_right() == NULL)) {
					if (tempPointer->getm_parent() != NULL) {

						tempParent = tempPointer->getm_parent();
						delete tempPointer;

						//Determine if node deleted was left or right child
						//of parent.
						//Node deleted was left child.
						if (key < (tempParent->getm_key())) {
							tempParent->setm_left(NULL);

						}
						//Node deleted was right child.
						else {
							tempParent->setm_right(NULL);
						}
						m_root->adjustHeight(tempParent);
						m_root->adjustSize(tempParent);

						//Rebalance Procedure
						TreeNode* imbalancedNode = NULL;
						imbalancedNode = findImbalance(tempParent, imbalancedNode);

						//Determine if there is an imbalance or not
						//There is an imbalance
						if (imbalancedNode != NULL) {

							//Call rebalance on that Node
							rebalance(imbalancedNode);
						}
					}
				}
			}
		}
		exit = true;
	}


	if (exit == false) {
		//Recursive
		if (tempNode->getm_key() != key) {
			if (key < tempNode->getm_key()) {
				findNode(key, tempNode->getm_left());
			}
			else {
				findNode(key, tempNode->getm_right());
			}
		}
	}
}



TreeNode* CTree::getInOrderSuccessor(TreeNode *tempNode){

	//Base Case: We have reached the farthest left node on subtree passed in.
	if (tempNode->getm_left() == NULL){
		//Test Print key
		return tempNode;
	}

	//Recursive Case: The left child is not NULL
	if (tempNode->getm_left() != NULL) {
		getInOrderSuccessor(tempNode->getm_left());
	}
}

TreeNode* CTree::getInOrderSuccessorLeft(TreeNode* tempNode) {

	//Base Case: We have reached the farthest  node on subtree passed in.
	if (tempNode->getm_right() == NULL) {
		//Test Print key
		return tempNode;
	}

	//Recursive Case: The left child is not NULL
	if (tempNode->getm_right() != NULL) {
		getInOrderSuccessor(tempNode->getm_left());
	}
}


void CTree::insert(int key){

	bool duplicate;

	//If the root is null then make the
	//root a node.
	if (m_root == NULL){
		m_root = new TreeNode(key);
		m_numNodes++;
	}
	
	else {
	//If the root is not NULL use recursion to 
	//determine where to insert value.
		duplicate = find(key);

		//If there is not a duplicate insert node in
		//tree. If there is duplicate do nothing. 
		if (duplicate == false){

			TreeNode* tempParent;
			TreeNode* imbalancedNode = NULL;

			tempParent = findParent(key, m_root);

			imbalancedNode = findImbalance(tempParent, imbalancedNode);

			//Determine if there is an imbalance or not

			//There is an imbalance
			if (imbalancedNode != NULL) {

				//Call rebalance on that Node
				rebalance(imbalancedNode);
			}
		}
	}

}

void CTree::inorder() {

	bool temp;
	//Call function that recursivelly does in order transversal
	temp = recursiveInOrder(m_root);

}

bool CTree::recursiveInOrder(TreeNode *tempNode) {

	//base Case
	if(tempNode == NULL){
		return true;
	}

	//Recursive Cases

	//Go as far down to the left as possible.
	//then print. After that move to right child.
	cout << "(";
	recursiveInOrder(tempNode->getm_left());
	
	cout << tempNode->getm_key() << ":"
		<< tempNode->getm_height() << ":"
		<< tempNode->getm_size();

	//Go to right child
	recursiveInOrder(tempNode->getm_right());
	cout << ")";
	return true;
}



TreeNode* CTree::findParent(int key, TreeNode* nodePointer) {

	//Base cases. We have reached the bottom of the tree.
	if ((key < nodePointer -> getm_key()) && (nodePointer -> getm_left() == NULL)){
		TreeNode* tempNodePointer = new TreeNode(key, nodePointer);
		TreeNode* imbalancedNode = NULL;
		
		//return dummy to leave recursive
		return tempNodePointer;
	}

	//Base Case. We have reached the bottom of the tree.
	if((key > nodePointer -> getm_key()) && (nodePointer -> getm_right() == NULL)){
		
		TreeNode* tempNodePointer = new TreeNode(key, nodePointer);
		
		//return dummy to leave recursive
		return tempNodePointer;
	}

	//Recursive Cases
	if (key < nodePointer -> getm_key()){
		findParent(key, nodePointer -> getm_left());
	}
	else {
		findParent(key, nodePointer -> getm_right());
	}
}


void CTree::rebalance(TreeNode *&tempNode) {

	int size;
	//GetSize  of array
	TreeNode * tempParent;
	size = tempNode->getm_size();

	//Make dynamic array of size "size"
	int *myArray = new int [size];

	//Populate Array
	//We do this by in order transversal on node
	//We must first made tempPointer parent NULL so
	//that it appears as root of tree.
	tempParent = tempNode->getm_parent();
	tempNode->setm_parent(NULL);

	int index = 0;
	populateArray(myArray, tempNode, index);

	//Once we have populate array we must perform the reblancing.
	//In this function we will restructure the subtree
	int leftBound = 0;
	int rightBound = size - 1;

	recursiveDestructor(tempNode);
	TreeNode* subTreeRoot = NULL;

	restructureSubTree(myArray, leftBound, rightBound, subTreeRoot);

	//Connect subTreeRoot to tempParent and vice versa
	subTreeRoot->setm_parent(tempParent);

	//Determine if we set left or right child of tempParent
	//to point to the subTreeRoot.

	int subRootKey = subTreeRoot->getm_key();

	//Determine if parent is NULL
	//If parent is not NULL
	if (tempParent != NULL) {

		int parentKey = tempParent->getm_key();

		//Detemrine if left/right child of parent
		if (subRootKey < parentKey) {
			//left child
			tempParent->setm_left(subTreeRoot);
		}
		else {
			//right child
			tempParent->setm_right(subTreeRoot);
		}
	}
	//If parent is NULL
	else {
		m_root = subTreeRoot;

	}

	//Properlly reset height/sizes
	m_root->adjustHeight(subTreeRoot);
	m_root->adjustSize(subTreeRoot);
	delete[] myArray;
}

bool CTree::restructureSubTree(int *&myArray, int leftBound, int rightBound, TreeNode*&subTreeRoot) {

	//Fnd the center of array
	int middle = (rightBound + leftBound) / 2;
	
	//Use binary search for left and right branch to place keys 
	//into balanced nodes

	//Base Case: If leftBound becomes larger than right Bound
	if (leftBound > rightBound) {
		return true;
	}

	//Add middle to tree
	//Determine if root exist
	if (subTreeRoot == NULL) {
		subTreeRoot = new TreeNode(myArray[middle]);
	}
	//If root already exists...
	else {

		//insert middle into tree
		findParent(myArray[middle], subTreeRoot);
	}

	//Double recurse for left subtree of node and right subtree
	restructureSubTree(myArray, leftBound, (middle - 1), subTreeRoot);
	restructureSubTree(myArray, (middle + 1), rightBound, subTreeRoot);
}



void CTree::populateArray(int *&myArray, TreeNode* tempNode, int &index) {

	//Go as far down to the left as possible.
	//then print. After that move to right child.
	if (tempNode != NULL) {
		populateArray(myArray, tempNode->getm_left(), index);

		//Store in Array
		int tempKey = tempNode->getm_key();
		myArray[index] = tempKey;
		index = index + 1;


		//Go to right child
		populateArray(myArray, tempNode->getm_right(), index);
	}
}



TreeNode* CTree::findImbalance(TreeNode * tempNode, TreeNode * NeedsRebalancing) {

	int sizeLeft = 0;
	int sizeRight = 0;
	int absoluteValue = 0;

	//Base Case: Stop when we are at root
	if (tempNode->getm_parent() == NULL) {
		
		bool leftExist;
		bool rightExist;

		//Determine if node has left and right children
		//If left child exists
		if (tempNode->getm_left() == NULL) {
			leftExist = false;
		}
		else {
			leftExist = true;
		}

		//If right child exists
		if (tempNode->getm_right() == NULL) {
			rightExist = false;
		}
		else {
			rightExist = true;
		}

		//If the child exists update height
		if (leftExist == true) {
			sizeLeft = tempNode->getm_left()->getm_size();
		}
		//Otherwise initailze to zero.
		else {
			sizeLeft = 0;
		}

		//If the child exists update height
		if (rightExist == true) {
			sizeRight = tempNode->getm_right()->getm_size();
		}
		//Otherwise initailze to zero.
		else {
			sizeRight = 0;
		}

		//Determine if sizeLeft is bigger of sizeRight is bigger
		//Subtract the bigger - smaller value
		if (sizeLeft >= sizeRight) {
			absoluteValue = sizeLeft - sizeRight;
		}
		else {
			absoluteValue = sizeRight - sizeLeft;
		}

		//See if absoluteValue is greater than 2.
		//Node needs to be rebalanced if so.
		if (absoluteValue > 2) {

			//Mark tempNode as pointer that needs to be rebalanced.
			NeedsRebalancing = tempNode;
		}
		return NeedsRebalancing;
	}

	//Recursive Case: Keep Recursing when we are not at root
	if (tempNode ->getm_parent() != NULL) {

		bool leftExist;
		bool rightExist;

		//Determine if node has left and right children
		//If left child exists
		if (tempNode->getm_left() == NULL) {
			leftExist = false;
		}
		else {
			leftExist = true;
		}

		//If right child exists
		if (tempNode->getm_right() == NULL) {
			rightExist = false;
		}
		else {
			rightExist = true;
		}

		//If the child exists update height
		if (leftExist == true) {
			sizeLeft = tempNode->getm_left()->getm_size();
		}
		//Otherwise initailze to zero.
		else {
			sizeLeft = 0;
		}

		//If the child exists update height
		if (rightExist == true) {
			sizeRight = tempNode->getm_right()->getm_size();
		}
		//Otherwise initailze to zero.
		else {
			sizeRight = 0;
		}

		//Determine if sizeLeft is bigger of sizeRight is bigger
		//Subtract the bigger - smaller value
		if (sizeLeft >= sizeRight) {
			absoluteValue = sizeLeft - sizeRight;
		}
		else {
			absoluteValue = sizeRight - sizeLeft;
		}

		//See if absoluteValue is greater than 2.
		//Node needs to be rebalanced.
		if(absoluteValue > 2){
			
			//Mark tempNode as pointer that needs to be rebalanced.
			NeedsRebalancing = tempNode;
		}
		//Recurse
		findImbalance(tempNode->getm_parent(), NeedsRebalancing);
	}

}



bool CTree::find(int key) {
	bool duplicate;

	//Call recursive function that will recursivelly search for key.
	duplicate = findRecursivelly(key, m_root);
	return duplicate;
}

bool CTree::findRecursivelly(int value, TreeNode* nodePointer){

	//Base cases

	//If we found the key
	if (value == nodePointer -> getm_key()) {
		return true;
	}

	//If the key does not exist and we reached NULL leaves.
	if (((nodePointer-> getm_left() == NULL)&&(value < nodePointer -> getm_key())) ||
		((nodePointer-> getm_right() == NULL))&&(value > nodePointer -> getm_key())){
		return false;
	}

	//Recursive Cases
	//If the value is greater than the roots node
	//go to right child. If less than, go to left child
	if (value > (nodePointer -> getm_key())) {
		nodePointer = nodePointer -> getm_right();
		findRecursivelly(value, nodePointer);
	}
	else {
		nodePointer = nodePointer -> getm_left();
		findRecursivelly(value, nodePointer);
	}
}


TreeNode::TreeNode(int key) {
	//Constructor for m_root
	m_left = NULL;
	m_right = NULL;
	m_parent = NULL;
	m_key = key;
	m_size = 1;
	m_height = 0;
}

TreeNode::TreeNode(int key, TreeNode *parent) {
	//Constructor if inserting into a tree that exists
	m_left = NULL;
	m_right = NULL;
	m_parent = parent;
	
	//Determine if left of right child of the parent.
	if (key < m_parent->getm_key()){
		m_parent -> setm_left(this);
	}
	else {
		m_parent -> setm_right(this);
	}

	m_key = key;
	m_size = 1;
	m_height = 0;
	adjustSize(this);
	adjustHeight(this);
}

bool TreeNode::adjustHeight(TreeNode *tempNode){
	int highestChild;

	//Base Case:If we are at root node
	if (tempNode -> m_parent == NULL) {

		//If it is only node and root node has no children.
		//Ensure height is 0.
		if (tempNode->m_left == NULL && tempNode->m_right == NULL) {
			m_height = 0;
			return true;
		}
		//If the root node has children find which child is taller
		//and add one to that value.
		else {
			
			//find if right and left child exist. If both exist find 
			//larger m_height of the two.
			bool leftExist;
			bool rightExist;

			//Find if left child exists.
			if (tempNode->m_left != NULL) {
				leftExist = true;
			}
			else {
				leftExist = false;
			}

			//Find if right child exists.
			if (tempNode->m_right != NULL){
				rightExist = true;
			}
			else {
				rightExist = false;
			}

			//If there are two children. Find the one
			//that has larger height.
			if (leftExist == true && rightExist == true) {

				//If the left child has taller height
				if (((tempNode->m_left)->m_height) > ((tempNode->m_right)->m_height)){
					highestChild = (tempNode->m_left)->m_height;
				}
				//If the right child has a taller height.
				else {
					highestChild = (tempNode->m_right)->m_height;
				}
			}
			else {
				//If there only exists one children use that height.
				if (leftExist == true) {
					highestChild = (tempNode->m_left)->m_height;
				}
				else {
					highestChild = (tempNode->m_right)->m_height;
				}
			}
			//Add one to the height value of highestChild and store as new
			//height of tempNode.
			highestChild++;
			tempNode->setm_height(highestChild);
			return true;
		}
	}

	//Recursive Case: If we are at leaf nodes
	//Ensure height is 0.
	if (tempNode->m_left == NULL && tempNode->m_right == NULL){

		if (tempNode->m_parent != NULL) {
			tempNode->setm_height(0);

			//Recursivelly call parent Node.
			adjustHeight(tempNode->m_parent);
		}
	}

	//Recursive Case: If we are not at a leaf node or root
	//node.
	if (tempNode ->m_left != NULL || tempNode -> m_right != NULL){

		if (tempNode->m_parent != NULL) {

			//find if right and left child exist. If both exist find 
			//larger m_height of the two.
			bool leftExist;
			bool rightExist;

			//Find if left child exists.
			if (tempNode->m_left != NULL) {
				leftExist = true;
			}
			else {
				leftExist = false;
			}

			//Find if right child exists.
			if (tempNode->m_right != NULL) {
				rightExist = true;
			}
			else {
				rightExist = false;
			}

			//If there are two children. Find the one
			//that has larger height.
			if (leftExist == true && rightExist == true) {

				//If the left child has taller height
				if (((tempNode->m_left)->m_height) > ((tempNode->m_right)->m_height)){
					highestChild = (tempNode->m_left)->m_height;
				}
				//If the right child has a taller height.
				else {
					highestChild = (tempNode->m_right)->m_height;
				}
			}

			//If there only exists one children use that height.
			else {

				if (leftExist == true) {
					highestChild = (tempNode->m_left)->m_height;
				}
				else {
					highestChild = (tempNode->m_right)->m_height;
				}
			}

			//Add one to the height value of highestChild and store as new
			//height of tempNode.
			highestChild++;
			tempNode->setm_height(highestChild);

			//Recursivelly call on parent node.
			adjustHeight(tempNode->m_parent);
		}
	}
}


bool TreeNode::adjustSize(TreeNode *tempNode) {

	//Base Case: If we have reached the root node.
	if (tempNode->m_parent == NULL) {

		//If the root node has children
		//Get the sum of the size
		if (tempNode->m_left != NULL || tempNode->m_right != NULL) {
			int sum = 0;

			//If the lefet node exists
			if (tempNode->m_left != NULL) {
				sum = tempNode->m_left->m_size;
			}
			//if the right node exists
			if (tempNode->m_right != NULL) {
				sum = sum + (tempNode->m_right->m_size);
			}

			//Add one to the sum
			sum++;
			tempNode->setm_size(sum);
			return true;
		}
		//Root Node does not have children
		//Ensure the sum is 1.
		else {
			tempNode->setm_size(SIZE_OF_ONE);
			return true;
		}
	}

	//Recursive Case: If we are at leaf Node.
	//Assure size = 1.
	if (tempNode->m_right == NULL && tempNode->m_left == NULL) {
		if (tempNode->m_parent != NULL) {
			tempNode->setm_size(SIZE_OF_ONE);

			//Recursivelly call parent node
			adjustSize(tempNode->m_parent);
		}
	}

	//Recursive Case: If we are not at leaf or root node. Add one to sum.
	if (tempNode->m_left != NULL || tempNode->m_right != NULL){
		if (tempNode->m_parent != NULL) {

			int sum = 0;

			//If the lefet node exists
			if (tempNode->m_left != NULL) {
				sum = tempNode->m_left->m_size;
			}
			//if the right node exists
			if (tempNode->m_right != NULL) {
				sum = sum + (tempNode->m_right->m_size);
			}

			//Add one to the sum
			sum++;
			tempNode->setm_size(sum);

			//Recursivelly call parent node
			adjustSize(tempNode->m_parent);
		}
	}
}





//Getters
TreeNode* TreeNode::getm_left(){
	return m_left;
}
TreeNode* TreeNode::getm_right() {
	return m_right;
}
TreeNode* TreeNode::getm_parent() {
	return m_parent;
}
int TreeNode::getm_key() {
	return m_key;
}
int TreeNode::getm_size() {
	return m_size;
}
int TreeNode::getm_height() {
	return m_height;
}



//Setters
void TreeNode::setm_left(TreeNode *node) {
	m_left = node;
}

void TreeNode::setm_right(TreeNode *node){
	m_right = node;
}

void TreeNode::setm_parent(TreeNode *node){
	m_parent = node;
}

void TreeNode::setm_key(int value) {
	m_key = value;
}

void TreeNode::setm_size(int value) {
	m_size = value;
}
void TreeNode::setm_height(int value) {
	m_height = value;
}