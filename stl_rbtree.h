#pragma once
#pragma execution_character_set("utf-8")
#include"stl_alloc.h"
#include"stl_algorithm.h"
#include"stl_construct.h"
#include "stl_pair.h"
#define RED true
#define BLACK false
namespace stl{

	template <typename Key,typename Value>
	struct Node
	{
		Node(Key k,Value v, bool c, size_t s) :value(v), key(k), color(c), size(s),left(NULL),right(NULL){}
		Value value;
		Key key;
		bool color;
		size_t size;
		Node* parent;
		Node* left;
		Node* right;
	};


	template <typename Key,typename Value>
	class rbtree_iterator;

	template <typename Key, typename Value>
	bool operator==(const rbtree_iterator<Key,Value>& lhs, const rbtree_iterator<Key,Value>& rhs)
	{
		return lhs.node == rhs.node;
	}

	template <typename Key,typename Value>
	bool operator!=(const rbtree_iterator<Key,Value>& lhs, const rbtree_iterator<Key,Value>& rhs)
	{
		return !(lhs == rhs);
	}


	template <typename Key,typename Value>
	class rbtree_iterator
	{
		friend bool operator==<Key,Value>(const rbtree_iterator<Key,Value>&, const rbtree_iterator<Key,Value>&);
		friend bool operator!=<Key,Value>(const rbtree_iterator<Key,Value>&, const rbtree_iterator<Key,Value>&);
	public:
		typedef Value value_type;
		typedef Value* Pointer;
		typedef Value& Reference;
		typedef const Value& const_Reference;
		typedef size_t size_type;

	protected:
		typedef Node<Key,Value>* link_type;
		typedef rbtree_iterator self;
		link_type node;

	public:
		rbtree_iterator() :node(NULL),head(NULL){}
		rbtree_iterator(link_type n) :node(n){}
		rbtree_iterator(const self& rhs) :node(rhs.node){}

		const_Reference operator*(){ return node->value; }
		Pointer operator->(){ return &(operator*()); }

		self& operator++()
		{
			if (node->right!= NULL)
			{
				node= node->right;
				while (node->left != NULL)
					node = node->left;
			}
			else 
			{
				link_type temp = node->parent;
				while (temp->size!=-1&&node == temp->right)
				{
					node = temp;
					temp = temp->parent;
				}
					node = temp;
			}
			return *this;
		}

		self operator++(int)
		{
			self temp = *this;
			++*this;
			ret
				urn temp;
		}

		self& operator--()
		{
			if (node->size == -1)
				node = node->right;
			else if (node->left != NULL)
			{
				node = node->left;
				while (node->right != NULL)
					node = node->right;
			}
			else
			{
				link_type temp = node->parent;
				while (temp->size!=-1&&node == temp->left)
				{
					node = temp;
					temp = temp->parent;
				}
					node = temp;
			}
			return *this;
		}

		self operator--(int)
		{
			self temp = *this;
			--*this;
			return temp;
		}
	};

	template <typename Key, typename Value,class Compare=less<Key>, typename Alloc = alloc>
	class stl_rbtree
	{
	protected:
		typedef void* void_pointer;
		typedef Node<Key,Value> tree_node;
		typedef simple_alloc<tree_node, Alloc> tree_node_allocator;
		typedef bool color_type;
		typedef tree_node* link_type;
	public:
		typedef Key key_type;
		typedef Value value_type;
		typedef Value* pointer;
		typedef const Value* const_pointer;
		typedef Value& reference;
		typedef const Value& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef rbtree_iterator<Key, Value> iterator;
	
	protected:
		link_type head;
		link_type root;
		Compare comp;
	public:
		stl_rbtree() :root(NULL){ initialize(); }
		explicit stl_rbtree(link_type r) :root(r){}
		stl_rbtree(link_type r, Compare c) :root(r),comp(c()){}
		stl_rbtree(const stl_rbtree& rhs) :root(rhs.root){}
		stl_rbtree& operator=(const stl_rbtree&);

	protected:
		void initialize();
		link_type get_node(){ return tree_node_allocator::allocate(); }
		void put_node(link_type n){ tree_node_allocator::deallocate(n); }
		link_type create_node(const Key&,const Value&);
		void destroy_node(link_type);

	public:
		iterator begin(){ return head->left; }
		iterator end(){ return head; }
		size_t Size(link_type);
		bool IsRed(link_type);
		bool insert(const Key&, const Value&);
	protected:
		link_type min_ele();
		link_type max_ele();
		void flip_color(link_type);
		link_type rateleft(link_type);
		link_type rateright(link_type);
		pair<link_type,bool> insert_aux(const Key&, const Value&, link_type);
	};

	template <typename Key, typename Value, class Compare, typename Alloc>
	void stl_rbtree<Key, Value, Compare, Alloc>::initialize()
	{
		head = create_node(Key(), Value());
		head->color = BLACK;
		head->size = -1;
		head->parent = NULL;
		head->left = root;
		head->right = root;
	}

	template <typename Key,typename Value,class Compare,typename Alloc>
	typename stl_rbtree<Key,Value,Compare,Alloc>::link_type
	stl_rbtree<Key, Value,Compare, Alloc>::create_node(const Key& key,const Value& value)
	{
		link_type new_node = get_node();
		//新插入的节点颜色均为红色
		construct(new_node,key,value,RED,1);
		return new_node;
	}

	template <typename Key, typename Value, class Compare, typename Alloc>
	void stl_rbtree<Key, Value, Compare, Alloc>::destroy_node(link_type n)
	{
		destroy(n);
		put_node(n);
	}

	template <typename Key, typename Value, class Compare, typename Alloc>
	stl_rbtree<Key, Value, Compare, Alloc>& 
	stl_rbtree<Key, Value, Compare, Alloc>::operator=(const stl_rbtree<Key, Value, Compare, Alloc>& rhs)
	{
		if (this != &rhs)
			root = rhs.root;
		return *this;
	}

	template <typename Key, typename Value, class Compare, typename Alloc>
	typename stl_rbtree<Key, Value, Compare, Alloc>::link_type
		stl_rbtree<Key, Value, Compare, Alloc>::min_ele()
	{
		if (root != NULL)
		{
			link_type temp = root;
			while (temp->left != NULL)
				temp = temp->left;
			return temp;
		}
		else
			return root;
	}

	template <typename Key, typename Value, class Compare, typename Alloc>
	typename stl_rbtree<Key, Value, Compare, Alloc>::link_type
		stl_rbtree<Key, Value, Compare, Alloc>::max_ele()
	{
		if (root != NULL)
		{
			link_type temp = root;
			while (temp->right != NULL)
				temp = temp->right;
			return temp;
		}
		else
			return root;
	}

	template <typename Key, typename Value, class Compare, typename Alloc>
	bool stl_rbtree<Key, Value, Compare, Alloc>::IsRed(link_type n)
	{
		if (n == NULL)
			return BLACK;
		else
			return n->color;
	}

	template <typename Key, typename Value, class Compare, typename Alloc>
	size_t stl_rbtree<Key, Value, Compare, Alloc>::Size(link_type n)
	{
		if (n == NULL)
			return 0;
		else
			return n->size;
	}

	template <typename Key,typename Value,class Compare,typename Alloc>
	bool stl_rbtree<Key, Value, Compare, Alloc>::insert(const Key& key, const Value& value)
	{
	
		pair<link_type,bool> tp = insert_aux(key, value, root);
		bool IsOk = tp.second;
		root = tp.first;
		//根节点颜色固定为黑色
		root->color = BLACK;
		if (root->size==1)
			root->parent = head;
		head->left = min_ele();
		head->right = max_ele();
		return IsOk;
	}

	/*以下为插入操作的实现，此插入操作不允许键值重复，返回的bool变量标识插入
	的成功与否。若想要返回插入点的位置信息，则不能使用递归，需使用循环。
	*/
	template <typename Key, typename Value, class Compare, typename Alloc>
	pair<typename stl_rbtree<Key,Value,Compare,Alloc>::link_type, bool>
		stl_rbtree<Key, Value, Compare, Alloc>::insert_aux(const Key& key, const Value& value, link_type n)
	{
		bool IsOk;
		if (n == NULL)
		{
			link_type new_node=create_node(key, value);
			//插入成功
			return pair<link_type, bool>(new_node, true);
		}
		else if (comp(key, n->key))
		{
			pair<link_type,bool> tp = insert_aux(key, value, n->left);
			n->left = tp.first;
			IsOk=tp.second;
			if ((n->left->left == NULL)&&(n->left->right == NULL))
				n->left->parent = n;
		}
		else if (!comp(key, n->key)&&(key!=n->key))
		{
			pair<link_type, bool> tp = insert_aux(key, value, n->right);
			n->right = tp.first;
			IsOk = tp.second;
			//如果其子节点的左右儿子都为NULL，则一定为新插入结点，需设置其parents
			if ((n->right->left == NULL) && (n->right->right == NULL))
				n->right->parent = n;
		}
		else //与现有键值重复，插入失败
			return pair<link_type, bool>(n, false);
		/*对于每一个非目标节点，都要检查：
		1.是否有红色右子节点且其左儿子不为红色，如果是，则进行左旋转
		2.是否存在连续的红色左节点，如果是，则进行右旋转
		3.是否为3-节点（左儿子右儿子均为红色），如果是，则将中间节点上移
		*/
		if (!IsRed(n->left) && IsRed(n->right))
			n = rateleft(n);
		if (IsRed(n->left) && IsRed(n->left->left))
			n = rateright(n);
		if (IsRed(n->left) && IsRed(n->right))
			flip_color(n);

		n->size = Size(n->left) + Size(n->right) + 1;
		return pair<link_type,bool>(n,IsOk);
	}

	template <typename Key,typename Value,class Compare,typename Alloc>
	typename stl_rbtree<Key,Value,Compare,Alloc>::link_type
		stl_rbtree <Key, Value, Compare, Alloc>::rateleft(link_type n)
	{
		link_type temp = n->right;
		n->right = temp->left;
		temp->left = n;
		temp->parent = n->parent;
		n->parent = temp;
		temp->color = n->color;
		n->color = RED;
		temp->size = n->size;
		n->size = Size(n->left) + Size(n->right) + 1;
		return temp;
	}

	template <typename Key,typename Value,class Compare,typename Alloc>
	typename stl_rbtree<Key, Value, Compare, Alloc>::link_type
		stl_rbtree<Key, Value, Compare, Alloc>::rateright(link_type n)
	{
		link_type temp = n->left;
		n->left = temp->right;
		temp->right = n;
		temp->parent = n->parent;
		n->parent = temp;
		temp->color = n->color;
		n->color = RED;
		temp->size = n->size;
		n->size = Size(n->left) + Size(n->right) + 1;
		return temp;
	}

	template <typename Key,typename Value,class Compare,typename Alloc>
	void stl_rbtree<Key, Value, Compare, Alloc>::flip_color(link_type n)
	{
		n->left->color = BLACK;
		n->right->color = BLACK;
		n->color = RED;
	}

}