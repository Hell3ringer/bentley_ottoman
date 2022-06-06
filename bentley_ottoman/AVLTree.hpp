#include "Node.hpp"

template <class T>
class AVLTree
{
	public :
	Node<T> * root;/**< root of subtree*/
	int _size = 0;/**< size of subtree*/
	
	AVLTree()
	{
		root = NULL;
		_size = 0;
	}
	/** balance the nodes in the vector path
	*@param path contains the nodes which might have to be balanced
	*/
	void balance(vector< Node<T> ** > path)
	{
		std::reverse(path.begin(),path.end());
		for (auto node : path)
		{
			
			(*node)->update();
			if((*node)->balanceFactor() >= 2 and (*node)->left->balanceFactor() >= 0)//left-left
				(*node) = (*node)->right_rotate();
			else if((*node)->balanceFactor() <= -2 and (*node)->right->balanceFactor () <= 0)
				(*node) = (*node)->left_rotate();
			else if((*node)->balanceFactor() >= 2)//left-right
			{
				(*node)->left = (*node)->left->left_rotate();
				(*node) = (*node)->right_rotate();
			}
			else if ((*node)->balanceFactor() <= -2)//right-left
			{
				(*node)->right = (*node)->right->right_rotate();
				(*node) = (*node)->left_rotate();
			}

		}
	}
	/** insert a node
	*@param value the value to be inserted
	*/
	void insert(T value)
	{
		std::vector< Node<T>** > path; 
		Node<T> **  node = &root;

		while((*node) != NULL)
		{
			path.push_back(node);
			if((*node)->value > value)
				node = &((*node)->left);
			else
				node = &((*node)->right);
		}	
		*node = new Node<T>(value);

		path.push_back(node);

		balance(path);
		_size++;
	}
	/** deletion a node
	*@param value the value to be deletiond
	*/
	void deletion(T value)
	{
		Node<T> ** node = &root;
		std::vector< Node<T> ** > path;
		while(*node != NULL and (*node)->value != value)
		{
			path.push_back(node);
			if((*node) -> value > value)
				node = & (*node)->left;
			else
				node = & (*node)->right;
		}
		if(*node == NULL)
		{
			cout << "value not found";
			cout << value.idx << endl;
			assert(1 == 2);
			return ;//value not found
		}
		else
			path.push_back(node);		
		
		int pathsize = path.size();
		
		if((*node)->left == NULL and (*node)->right == NULL)
		{
			delete *node;
			*node = NULL;
			path.pop_back();
		}
		else if((*node)->right == NULL)
		{
			Node<T> * toremove = *node;
			*node = (*node)->left;
			delete toremove;
			path.pop_back();
		}
		else
		{
			Node<T> ** predecessor  = & ((*node)->right);
			while((*predecessor)->left != NULL)
			{
				path.push_back(predecessor);
				predecessor = &((*predecessor)->left);
			}
			if(*predecessor == (*node)->right)
			{
				(*predecessor)->left = (*node)->left;
				Node<T> * toremove = *node;
				*node = *predecessor;
				delete toremove;
			}
			else
			{
				Node<T> *temp = *path.back(),*suc = *predecessor;
				//temp is the second last node
				temp->left = (*predecessor)->right;
				suc->left = (*node)->left;
				suc->right = (*node)->right;
				delete *node;
				*node = suc;
				path[pathsize] = & (suc->right);
			}

		}
		balance(path);
		_size--;
	}
	/** finding left neightbour
	*@param value the value whose left neighbour has to be found
	*@return the left neighbour
	*/
	T * predecessor(T value)
	{
		T* ans = NULL;
		Node<T> *cur = root;
		while(cur != NULL)
		{
			
			if(cur->value == value)
			{
				break;
			}

			if(cur-> value < value)
			{
				
				ans = &cur -> value;
				cur = cur -> right;
			}
			else
			{
				cur = cur -> left;
			}
		}
		if(cur != NULL)//find rightmost node in left subtree
		{
			cur = cur -> left;
			while(cur != NULL)
			{
				ans = &cur -> value;
				cur = cur -> right;
			}
		}
		return ans;
	}
	/** finding right neightbour
	*@param value the value whose right neighbour has to be found
	*@return the right neighbour value
	*/
	T* successor(T value)
	{
		T* ans = NULL;
		Node<T> *cur = root;
		std::vector< Node<T> ** > path;
		while(cur != NULL)
		{
			if(cur->value == value)
			{
				break;
			}
			if(cur-> value < value)
			{
				cur = cur -> right;
			}
			else
			{
				ans = &cur -> value;
				cur = cur -> left;
			}
		}
		if(cur != NULL)//find lefttmost node in right subtree
		{
			cur = cur -> right;
			while(cur != NULL)
			{
				ans = &cur -> value;
				cur = cur -> left;
			}
		}
		return ans;
	}
	/** finding left neightbour
	*@param value the value whose lower bound has to be found
	*@return the lower bound of value
	*/
	T* lower_bound(T value)//handle empty tree
	{
		Node<T> *cur = root;
		T* ans = NULL;
		while(cur != NULL)
		{
			if(cur->value < value)
			{
				cur = cur -> right;
				
			}
			else
			{
				ans = &cur->value;
				cur = cur->left;
			}
		}
		return ans;
	}
	/** finding right neightbour
	*@param value the value whose upper bound has to be found
	*@return the upper bound of value
	*/
	T* upper_bound(T value)//handle empty tree
	{
		
		T* ans = NULL;
		
		Node<T> ** node = &root;
		while(*node != NULL)
		{
			if((*node) -> value > value)
			{
				ans = & (*node)->value;
				node = & (*node)->left;
			}
			else
				node = & (*node)->right;
		}
		return ans;
	}
	/** get the minimum value in tree
	*@return the minimum value in tree
	*/
	T* get_min()
	{
		Node<T> *cur = root;
		T* ans = NULL;
		while(cur != NULL)
		{
			ans = &cur->value;
			cur = cur->left;
		}
		return ans;
	}
	/** find the upper bound and delete it
	*@param value the value whose upper bound has to be
	*/
	void upper_bound_and_deletion(T value)
	{
		Node<T> ** node = &root;
		std::vector< Node<T> ** > path;
		int sz ;  
		while(*node != NULL)
		{
			path.push_back(node);
			if((*node) -> value > value)
			{
				sz = path.size();
				node = & (*node)->left;
			}
			else
				node = & (*node)->right;
		}
		
		path.resize(sz);
		node = path[sz - 1];
		if(*node == NULL)
		{	
			
			assert(1 == 2);
			return ;//value not found
		}

		
		int pathsize = path.size();
		if((*node)->left == NULL and (*node)->right == NULL)
		{
			delete *node;
			*node = NULL;
			path.pop_back();
		}
		else if((*node)->right == NULL)
		{
			Node<T> * toremove = *node;
			*node = (*node)->left;
			delete toremove;
			path.pop_back();
		}
		else
		{
			Node<T> ** predecessor  = & ((*node)->right);
			while((*predecessor)->left != NULL)
			{
				path.push_back(predecessor);
				predecessor = &((*predecessor)->left);
			}
			if(*predecessor == (*node)->right)
			{
				(*predecessor)->left = (*node)->left;
				Node<T> * toremove = *node;
				*node = *predecessor;
				delete toremove;
			}
			else
			{
				Node<T> *temp = *path.back(),*suc = *predecessor;
				//temp is the second last node
				temp->left = (*predecessor)->right;
				suc->left = (*node)->left;
				suc->right = (*node)->right;
				delete *node;
				*node = suc;
				path[pathsize] = & (suc->right);
			}

		}
		balance(path);
		_size--;
	}
	/** find if the value exists or not
	*@return 0 or 1
	*/
	bool find(T value)
	{
		Node<T> *cur = root;
		while(cur != NULL)
		{
			if(cur->value == value)
				return 1;
			if(cur->value < value)
			{
				cur = cur -> right;
			}
			else
			{
				cur = cur->left;
			}
		}
		return 0;
	}

};