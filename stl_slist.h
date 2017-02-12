#pragma once
#pragma execution_character_set("utf-8")

#include"stl_alloc.h"
#include"stl_iterator_type.h"
#include"stl_algorithm.h"
#include"stl_construct.h"

namespace stl{

	template <typename T>
	struct slist_node
	{
		slist_node* next;
		T data;
	};

	template <typename T>
	class slist_iterator;

	template <typename T>
	bool operator==(const slist_iterator<T>& lhs, const slist_iterator<T>& rhs)
	{
		return lhs.node == rhs.node;
	}

	template <typename T>
	bool operator!=(const slist_iterator<T>& lhs, const slist_iterator<T>& rhs)
	{
		return lhs != rhs;
	}

	template <typename T>
	bool operator<(const slist_iterator<T>& lhs, const slist_iterator<T>* rhs)
	{
		return lhs.node < rhs.node;
	}


	template <typename T>
	class slist_iterator
	{
		friend bool operator==<T>(const slist_iterator<T>&, const slist_iterator<T>&);
		friend bool operator!=<T>(const slist_iterator<T>&, const slist_iterator<T>&);
		friend bool operator< <T>(const slist_iterator<T>&, const slist_iterator<T>&);
	public:
		typedef T value_type;
		typedef T* Pointer;
		typedef const T* const_Pointer;
		typedef T& Reference;
		typedef const T& const_Reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef stl_forward_iterator_tag iterator_category;
	protected:
		typedef slist_node<T>* link_type;
		typedef slist_iterator<T> self;
	public:
		link_type node=0;

		slist_iterator(){};
		slist_iterator(link_type lint) :node(lint){}
		slist_iterator(const slist_iterator& rhs) :node(rhs.node){}
		slist_iterator& operator=(const slist_iterator&);

	public:
		Reference operator*(){ return node->data; }
		const_Reference operator*()const{ return node->data; }
		Pointer operator->(){ return &(operator*()); }
		const_Pointer operator->()const{ return &(operator*()); }
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

	};

	template <typename T>
	slist_iterator<T>& slist_iterator<T>::operator=(const slist_iterator& rhs)
	{
		if (this != &rhs)
			node = rhs.node;
		return *this;
	}


	template <typename T,typename Alloc=alloc>
	class stl_slist
	{
	public:
		typedef T value_type;
		typedef T* Pointer;
		typedef const T* const_Pointer;
		typedef T& Reference;
		typedef const T& const_Reference;
		typedef size_t size_type;
		typedef slist_iterator<T> iterator;
	protected:
		typedef slist_node<T> list_node;
		typedef slist_node<T>* link_type;
		typedef simple_alloc<list_node, alloc> node_allocator;

	public:
		link_type node; //header pointer
		size_type mysize = 0;

	public:
		stl_slist() :node(node_allocator::allocate()){ node->next = nullptr; }
		stl_slist(size_type n){ initialize_list(n,T()); }
		stl_slist(size_type n, const T& value){ initialize_list(n, value); }
		stl_slist(const stl_slist& rhs) :node(rhs.node),mysize(rhs.mysize){};
		stl_slist& operator=(const stl_slist& rhs);
		~stl_slist();

	protected:
		void initialize_list(size_type, const T&);

	public:
		iterator begin(){ return node->next; }
		iterator end(){ return iterator(NULL); }
		size_type size(){ return mysize; }
		bool empty(){ return node->next == nullptr; }
		Reference front();
		const_Reference front()const;
		Reference back();
		const_Reference back()const;
		iterator insert_after(iterator, const T&);
		void push_front(const T&);
		void pop_front();
		void clear();
		void unique();
	protected:
		void transfer(iterator, iterator, iterator);
	};

	template <typename T,typename Alloc>
	void stl_slist<T, Alloc>::initialize_list(size_type n, const T& value)
	{
		try{
			node = node_allocator::allocate();
			node->next = nullptr;
			if (n == 0)
				return;
			else
			{
				for (size_type index = n; index > 0; --index)
				{
					link_type temp = node->next;
					link_type node_temp = node_allocator::allocate();
					construct(&node->data, value);
					node_temp->next = temp;
					node->next = node_temp;
				}
			}
		}
		catch (...)
		{
			clear();
			throw;
		}
		mysize = n;
	}

	template <typename T,typename Alloc>
	stl_slist<T, Alloc>& stl_slist<T, Alloc>::operator=(const stl_slist<T, Alloc>& rhs)
	{
		if (this != &rhs)
		{
			node = rhs.node;
			mysize = rhs.mysize;
		}
		return *this;
	}

	template <typename T,typename Alloc>
	void stl_slist<T, Alloc>::clear()
	{
		link_type t = node->next;
		while (t != NULL)
		{
			link_type nodet = t->next;

			node_allocator::deallocate(t);
			t = nodet;
		}
		mysize = 0;
	}

	template <typename T,typename Alloc>
	typename stl_slist<T, Alloc>::Reference stl_slist<T, Alloc>::front()
	{
		if (!empty())
			return *(iterator)(node->next);
		else
		{
			cout << "front on empty slist" << endl;
			throw;
		}
	}

	template <typename T,typename Alloc>
	typename stl_slist<T, Alloc>::const_Reference stl_slist<T, Alloc>::front()const
	{
		if (!empty())
			return *(iterator)(node->next);
		else
		{
			cout << "front on empty slist" << endl;
			throw;
		}
	}

	template <typename T,typename Alloc>
	typename stl_slist<T, Alloc>::Reference stl_slist<T, Alloc>::back()
	{
		if (!empty())
		{
			link_type temp = node;
			while (temp->next != NULL)
				temp=temp->next;
			return temp->data;
		}
		else
		{
			cout << "back on empty slist" << endl;
			throw;
		}
	}

	template <typename T, typename Alloc>
	typename stl_slist<T, Alloc>::const_Reference stl_slist<T, Alloc>::back()const
	{
		if (!empty())
		{
			link_type temp = node;
			while (temp->next != NULL)
				temp=temp->next;
			return temp->data;
		}
		else
		{
			cout << "back on empty slist" << endl;
			throw;
		}
	}

	template <typename T,typename Alloc>
	typename stl_slist<T, Alloc>::iterator stl_slist<T,Alloc>::insert_after(iterator position,const T& value)
	{
		link_type new_node= node_allocator::allocate();
		try{
			construct(&new_node->data, value);
		}
		catch (...)
		{
			destroy(&*new_node);
			node_allocator::deallocate(new_node);
		}
		link_type tmp = position.node->next;
		position.node->next = new_node;
		new_node->next = tmp;
		++mysize;
		return (iterator)new_node;
	}

	template <typename T,typename Alloc>
	void stl_slist<T, Alloc>::push_front(const T& value)
	{
		insert_after(node, value); 
	}
	template <typename T,typename Alloc>
	void stl_slist<T, Alloc>::pop_front()
	{
		link_type next = node->next;
		node->next = next->next;
		destroy(&next->data);
		node_allocator::deallocate(next);
		--mysize;
	}
	template <typename T,typename Alloc>
	void stl_slist<T, Alloc>::unique()
	{
		link_type tmp = node->next;
		while (tmp->next!=NULL)
		{
			if (tmp->data == tmp->next->data)
			{
				link_type next_node = tmp->next->next;
				destroy(&(tmp->next->data));
				node_allocator::deallocate(tmp->next);
				tmp->next = next_node;
				--mysize;
				continue;
			}
			tmp = tmp->next;
		}
	}

	template <typename T,typename Alloc>
	stl_slist<T, Alloc>::~stl_slist()
	{
		clear();
		node_allocator::deallocate(node);
	}
}