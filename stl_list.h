#pragma once
#pragma execution_character_set("utf-8")
#include"stl_iterator_type.h"
#include"stl_alloc.h"
#include"stl_construct.h"
namespace stl
{
	//list类前置声明
	template<typename T, typename Alloc>
	class stl_list;
	//list节点
	template <typename T>
	class _list_node
	{
	public:
		_list_node<T>* next;
		_list_node<T>* prev;
		T data;
	};


	//list迭代器
	template <typename T, typename Ref, typename Ptr>
	class _list_iterator
	{
		friend class stl_list < T, alloc > ;
		typedef _list_iterator<T, T&, T*> iterator;
		typedef _list_iterator<T, Ref, Ptr> self;
	public:
		typedef stl_bidirectional_iterator_tag iterator_category;
		typedef T value_type;
		typedef Ptr Pointer;
		typedef const Ptr const_Pointer;
		typedef Ref Reference;
		typedef const Ref const_Reference;
		typedef ptrdiff_t Distance;

		typedef _list_node<T>* link_type;
		typedef size_t size_type;

	protected:
		link_type node;//内部维护一个指向节点的指针

	public:
		_list_iterator(){}
		_list_iterator(link_type lint) :node(lint){}
		_list_iterator(const _list_iterator& rhs) :node(rhs.node){}
		//使用合成的copy assignment
		bool operator==(const self& rhs)const{ return node == rhs.node; }
		bool operator!=(const self& rhs)const{ return !(*this == rhs); }
		//元素取值、成员存取操作
		Reference operator*(){ return (*node).data; }
		Pointer operator->(){ return &(operator*()); }
		//迭代器移动操作
		self& operator++()
		{
			node = node->next;
			return *this;
		}

		self operator++(int)
		{
			self temp = *this;
			++*this;
			return temp;
		}
		self& operator--()
		{
			node = node->prev;
			return *this;
		}
		self operator--(int)
		{
			self temp = *this;
			--*this;
			return temp;
		}
	};


	template <typename T,typename Alloc=alloc>
	class stl_list;

	template <typename T,typename Alloc=alloc>
	bool operator==(const stl_list<T>& lhs, const stl_list<T>& rhs)
	{
		return lhs.node == rhs.node;
	}

	template <typename T,typename Alloc=alloc>
	bool operator!=(const stl_list<T>& lhs, const stl_list<T>& rhs)
	{
		return !(lhs == rhs);
	}

	template <typename T,typename Alloc=alloc>
	bool operator<(const stl_list<T>& lhs, const stl_list<T>& rhs)
	{
		iterator lhs_it = lhs.begin();
		iterator rhs_it = rhs.begin();
		for (; lhs_it != lhs.end() && rhs_it != rhs.end(); ++lhs_it, ++rhs_it)
		{
			if (*lhs_it < *rhs_it)
				return true;
			else if (*lhs_it > *rhs_it)
				return false;
		}
		if (lhs_it != lhs.end())
			return false;
		else if (rhs_it != rhs.end())
			return true;
	}


	//list容器主体
	template <typename T, typename Alloc>
	class stl_list
	{
		friend bool operator==<T, Alloc>(const stl_list<T, Alloc>&, const stl_list<T, Alloc>&);
		friend bool operator!=<T, Alloc>(const stl_list<T, Alloc>&, const stl_list<T, Alloc>&);
		friend bool operator< <T, Alloc>(const stl_list<T, Alloc>&, const stl_list<T, Alloc>&);
	protected:
		typedef _list_node<T> list_node;
		typedef simple_alloc<list_node, Alloc> list_node_allocator;

	public:
		typedef T value_type;
		typedef T* Pointer;
		typedef const T* const_Pointer;
		typedef T& Reference;
		typedef const T& const_Reference;
		typedef list_node* link_type;
		typedef size_t size_type;
		//设立迭代器型别
		typedef _list_iterator<T, T&, T*> iterator;
	public:
		stl_list(){ empty_initialize(); }
		~stl_list();

	protected:
		link_type get_node(){ return list_node_allocator::allocate(); } //在内存中分配一个节点大小的空间
		void put_node(link_type p){ return list_node_allocator::deallocate(p); } //释放节点空间
		link_type create_node(const T& ); //创建节点 get_node+构造节点中元素
		void empty_initialize();//初始化链表
		void destroy_node(link_type );//移除一个节点
	
	public:
		iterator begin(){ return (*node).next; }
		iterator end(){ return node; }
		bool empty(){ return node->next == node; }
		size_type size()const{ return mysize; }
		Reference front(){ if (!empty()) return *(iterator)(node->next); }
		const_Reference front()const{ if(!empty()) return *(iterator)(node->next); }
		Reference back(){ if (!empty()) return *(iterator)(node->prev); }
		const_Reference back()const { if (!empty()) return *(iterator)(node->prev); }
		iterator insert(iterator, const T&);
		iterator erase(iterator);
		iterator erase(iterator, iterator);
		void push_back(const T& value){ insert(end(), value); }
		void push_front(const T& value){ insert(begin(), value); }
		void pop_back();
		void pop_front(){ erase(begin()); };
		void clear();
		void remove(const T&);
		void unique();
		void unique(iterator,iterator);
		void splice(iterator, stl_list<T,Alloc>&);
		void splice(iterator, stl_list<T,Alloc>&,iterator, iterator);//list的引用参数用来指明后面区间的归属
		void splice(iterator, iterator);
		void merge(stl_list<T, Alloc>&);
		void reverse();
		void sort();
		void swap(stl_list<T, Alloc>&);
	protected:
		void transfer(iterator, iterator, iterator);
		size_type count(iterator,iterator);

	protected:
		size_type mysize=0;
		link_type node=0;
	};

	template <typename T, typename Alloc>
	typename stl_list<T, Alloc>::link_type stl_list<T, Alloc>::create_node(const T& value)
	{
		link_type p = get_node();
		construct(&p->data, value);
		return p;
	}

	template <typename T, typename Alloc>
	void stl_list<T, Alloc>::destroy_node(link_type p)
	{
		destroy(&(p->data));
		put_node(p);

	}

	template <typename T,typename Alloc>
	void stl_list<T, Alloc>::empty_initialize()
	{
		node = get_node();
		node->next = node;
		node->prev = node;
	}

	template <typename T,typename Alloc>
	typename stl_list<T, Alloc>::iterator stl_list<T, Alloc>::insert(iterator position, const T& value)
	{
		link_type temp = create_node(value);
		temp->next = position.node;
		temp->prev = position.node->prev;
		(link_type(position.node->prev))->next = temp;
		position.node->prev = temp;
		++mysize;
		return temp;

	}

	template<typename T,typename Alloc>
	typename stl_list<T, Alloc>::iterator stl_list<T, Alloc>::erase(iterator position)
	{
		link_type prev_node = position.node->prev;
		link_type next_node = position.node->next;
		prev_node->next = position.node->next;
		next_node->prev = prev_node;
		destroy_node(position.node);
		--mysize;
		return iterator(next_node);//返回被移除元素的下一个元素
	}

	template <typename T,typename Alloc>
	void stl_list<T, Alloc>::pop_back()
	{
		iterator temp=end();
		erase(--temp);
	}
	template <typename T,typename Alloc>
	typename stl_list<T, Alloc>::size_type stl_list<T, Alloc>::count(iterator first, iterator last)
	{
		size_type n;
		for (n = 0; first != last; ++n,++first)
			;
		return n;
	}
	template <typename T,typename Alloc>
	void stl_list<T, Alloc>::clear()
	{
		iterator cur = begin();
		iterator temp;
		while (cur != node)
		{
			temp = cur;
			cur = cur.node->next;
			destroy_node(temp);

		}
		node->next = node;
		node->prev = node;
		mysize = 0;
	}

	template <typename T,typename Alloc>
	void stl_list<T, Alloc>::remove(const T& value)
	{//清除所有相邻且值相同的元素，在对链表排序之后可使用
		iterator first=begin();
		iterator last = end();
		while (first != last)
		{//因为有可能清除first指向的对象，所以保存first->next的信息
			iterator temp = first.node->next;
			if (*first == value)
				erase(first);
			first = temp;
		}

	}

	template <typename T,typename Alloc>
	void stl_list<T, Alloc>::unique()
	{
		iterator first = begin();
		iterator last = end();
		iterator temp = first.node;
		while (++temp != last)
		{
			if (*first == *temp)
				erase(temp);//消除后面的元素
			else
				first = temp;
			/*如果消除temp上的元素，temp重新指向first(在下次循环开始重新指向first->next)
			这样可以消除连续重复的多个元素;如果没有消除temp上的元素，temp跟着first一起向后移动*/
			temp = first;
		}
	}

	template <typename T,typename Alloc>
	void stl_list<T, Alloc>::unique(iterator begin,iterator end)
	{
		iterator first = begin;
		iterator last = end;
		iterator temp = first.node;
		while (++temp != last)
		{
			if (*first == *temp)
				erase(temp);
			else
				first = temp;

			temp = first;
		}
	}

	template <typename T,typename Alloc>
	void stl_list<T, Alloc>::transfer(iterator position, iterator first, iterator last)
	{
		if (position != last)
		{
			(first.node->prev)->next = last.node;
			(last.node->prev)->next = position.node;
			(position.node->prev)->next = first.node;
			link_type temp = position.node->prev;
			position.node->prev = last.node->prev;
			last.node->prev = first.node->prev;
			first.node->prev = temp;
			
		}
	}
	template <typename T,typename Alloc>
	void stl_list<T, Alloc>::splice(iterator position, stl_list<T,Alloc>& rhs)
	{//将rhs中的所有元素放置到position之前
		if (!rhs.empty())
			transfer(position, rhs.begin(), rhs.end());
		mysize += rhs.mysize;
		rhs.mysize = 0;
	}
	
	template <typename T,typename Alloc>
	void stl_list<T, Alloc>::splice(iterator position,stl_list<T,Alloc>& rhs,iterator first, iterator last)
	{//将rhs中的first到last之间的元素放到position之前
		if (this != &rhs)
		{
			if (first == rhs.begin() && last == rhs.end())
				mysize += rhs.mysize;
			else
				mysize += count(first, last);

		}
		if (first != last)
			transfer(position, first, last);

	}

	template <typename T,typename Alloc>
	void stl_list<T, Alloc>::splice(iterator position, iterator i)
	{//将i放到position之前
		iterator j = i;
		++j;
		if (position == j || position == i)
			return;
		transfer(position, i, j);
		++mysize;
	}

	template <typename T,typename Alloc>
	void stl_list<T, Alloc>::merge(stl_list<T, Alloc>& rhs)
	{//符合list的归并排序，将rhs中元素接合到lhs中，两个list必须是递增已序的
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = rhs.begin();
		iterator last2 = rhs.end();
		iterator temp;
		while (first1 != last1&&first2!=last2)
		{
			if (*first1 > *first2)
			{
				temp = first2;
				transfer(first1, first2, ++temp);
				first2 = temp;
			}
			else
				++first1;

		}
		if (first2 != last2)
			transfer(first1, first2, last2);
	}

	template <typename T,typename Alloc>
	void stl_list<T, Alloc>::reverse()
	{
		if (node->next == node || node->next->next == node)
			return;
		iterator first = begin().node->next;
		iterator last = end();
		iterator old;
		while (first != last)
		{
			old = first;
			transfer(begin(), old, ++first);
		}
	}

	template <typename T,typename Alloc>
	void stl_list<T, Alloc>::swap(stl_list<T, Alloc>& rhs)
	{
		if (this == &rhs)
			return;
		using std::swap;
		swap(node, node);
		swap(mysize, rhs.mysize);
	}

	template <typename T,typename Alloc>
	void stl_list<T, Alloc>::sort()
	{
		if (node->next == node || node->next->next == node)
			return;
		size_t MAXBIN = 64;
		stl_list<T, Alloc> temp;//中转站
		stl_list<T, Alloc> bins[MAXBIN];
		int fill = 0;//排序总数
		while (!empty())
		{
			temp.splice(temp.begin(), begin());
			int i;//当前进度标记
			for (i = 0; i < fill&&!bins.empty(); ++i)
			{
				bins[i].merge(temp);
				//将排好序的元素放入中转站中
				bins[i].swap(temp);
			}
			temp.swap(bins[i]);//将当前已序元素放入下个箱中
			if (i == fill)++fill;
		}
		//合并箱子
		for (int n = 0; n < fill; ++n)
			bins[n].merge(bins[n - 1]);
		this->swap(bins[fill - 1]);
	}

	template <typename T,typename Alloc>
	stl_list<T, Alloc>::~stl_list()
	{
		clear();
		list_node_allocator::deallocate(node);
	}
}