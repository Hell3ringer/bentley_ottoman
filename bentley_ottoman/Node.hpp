#include <bits/stdc++.h>
using namespace std;


template<class 	T>
class Node{
	public :
	Node *left;/**< left child of node*/
	Node *right;/**< right child of node*/
	T value;/**< data stored at node*/
	int count;/**< count of number of nodes in subtree*/
	int height;/**< height of subtree*/
	/** constructor
	*@param _value the data to be stored at node
	*/
	Node(T _value)
	{
		value = _value;
		count = 1;
		height  = 1;
		left = NULL;
		right = NULL;

	}
	/** update count and height values
	*/
	void update()
	{
		count = 1 + ((left != NULL)?left->count:0) + ((right!=NULL)?right->count:0);
		height = 1 + max((left != NULL)?left->height:0,(right!=NULL)?right->height:0);

	}
	/** compute balance factor
	*@return difference in height of left and right subtree
	*/
	int balanceFactor()
	{
		return (left != NULL ?left->height : 0) - (right != NULL?right->height : 0);
	}
	/** right rotate 
	*/
	Node * right_rotate()
	{
		Node * L = left;
		left = left->right;
		L->right = this;
		this->update();
		L->update();
		return L;
	}
	/** left rotate 
	*/
	Node * left_rotate()
	{
		Node * R = right;
		right = right -> left;
		R->left = this;
		this->update();
		R->update();
		return R;
	}

};
