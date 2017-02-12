#pragma once
#pragma execution_character_set("utf-8")
#include"stl_alloc.h"
#include"stl_construct.h"
#include"stl_iterator_type.h"
#include"stl_algorithm.h"

#define buf_siz(x,y) _deque_buf_size(x,y)
	size_t _deque_buf_size(size_t n, size_t sz)
	{//�ƶ���������С,nΪ�û�ָ����ģ������Ͳ���Bufsiz��szΪԪ�ش�С
		return n != 0 ? n : ((sz > 512) ? size_t(1) : size_t(512 / sz));
	}
namespace stl{

	template <typename T,typename Alloc,size_t BufSiz>
	class stl_deque;
	template<typename T,typename Ref,typename Ptr,size_t BufSiz>
	class _deque_iterator
	{
		friend class stl_deque < T, alloc, BufSiz > ;
		typedef _deque_iterator<T, T&, T*, BufSiz> iterator;
		typedef _deque_iterator<T, const T&, const T*, BufSiz> const_iterator;
	public:
		typedef stl_random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef Ptr Pointer;
		typedef const Ptr const_Pointer;
		typedef Ref Reference;
		typedef const Ref const_Reference;
		typedef ptrdiff_t Distance;
		//ָ��������ĵ�ָ��,�������Ĵ洢ָ�����������(ͷ��)��ָ��
		typedef T** map_pointer;
		
		typedef _deque_iterator self;
	public:
		//���ݳ�Ա
		T* cur;       //ָ��ǰԪ��
		T* first;     //ָ��ǰ��������ͷ��
		T* last;      //ָ��ǰ��������β��(β��)
		map_pointer node;   //ָ��ǰ���ڵĻ�����

	protected:
		//��������Ծ����(new_nodeΪĿ�Ļ�����)
		//cur����ʹ��set_node����Щ�����е�������
		void set_node(map_pointer new_node)
		{//��������ָ��
			first = *new_node;
			last = *new_node + Distance(buf_siz(BufSiz,sizeof(T)));
			node = new_node;
		}
	public:
		//Ԫ��ȡֵ����Ա��ȡ����
		Reference operator*(){ return *cur; }
		Pointer operator->(){ return &(operator*()); }
		
		//����������������random_access_iterator��Ҫ��
		// this-rhs
		Distance operator-(const self& rhs) const
		{
			Distance d=Distance(buf_siz(BufSiz,sizeof(T))*(node - rhs.node - 1)+
				(cur - first) + (rhs.last - rhs.cur));
			return d;
		}

		self& operator++()
		{
			++cur;
			if (cur == last)
			{
				set_node(node + 1);
				cur = first;
			}
			return *this;
		}
		self operator++(int)
		{
			iterator temp = *this;
			++*this;
			return temp;
		}

		self& operator--()
		{//�������жϳ���֮ǰ�ݼ�cur,��Ϊ��first֮ǰ��û�����ж�cur����Ļ��嵥Ԫ
			if (cur == first)
			{//�ڵݼ�֮ǰ�ѵ���ͷ�����ݼ������
				set_node(node - 1);
				cur = last;
			}
			--cur;
			return *this;
		}
		self operator--(int)
		{
			iterator temp = *this;
			--*this;
			return temp;
		}

		//ʵ�������ȡ,��������Ծn������
		//����nΪ��ֵ�Լ򻯴���
		self& operator+=(Distance n)
		{
			Distance offset = n + (cur - first);//������Ҫ��Ծ��Ԫ����
			if (offset >= 0 && offset < Distance(buf_siz(BufSiz,sizeof(T))))//Ŀ����ͬһ��������
				cur += n;
			else
			{//������㻺����ƫ����
				Distance node_offset =
					offset>0 ? Distance(offset / buf_siz(BufSiz,sizeof(T)))
					: -Distance((offset - 1) / buf_siz(BufSiz,sizeof(T))) - 1;//����offset-1�Ǽ���ǰһ����������last
				set_node(node+node_offset);
				//����cur
				cur = first + (offset - (node_offset*buf_siz(BufSiz,sizeof(T))));
			}
			return *this;
		}

		//�����ƶ�������ʹ��+=
		self& operator-=(Distance n)
		{
			return *this += -n;
		}

		self operator+(Distance n)const
		{
			iterator temp = *this;
			temp += n;
			return temp;
		}
		
		self operator-(Distance n)const
		{
			iterator temp = *this;
			temp -= n;
			return temp;
		}

		//�����������ȡ
		Reference operator[](size_t n){ return *(*this + n); }

		bool operator==(const self& rhs)const{ return cur == rhs.cur; }
		bool operator!=(const self& rhs)const{ return !(*this == rhs); }
		bool operator<(const self& rhs)const
		{
			return node == rhs.node ? (cur < rhs.cur) : (node < rhs.node);
		}
	};



	template <typename T,typename Alloc=alloc,size_t BufSiz=0>
	class stl_deque
	{
	public:
		typedef _deque_iterator<T, T&, T*, BufSiz> iterator;
		typedef _deque_iterator<T,const T&,const T*, BufSiz> const_iterator;
		typedef stl_deque<T, Alloc, BufSiz> self;
	public:
		typedef T value_type;
		typedef T* Pointer;
		typedef const T* const_Pointer;
		typedef const T& const_Reference;
		typedef T& Reference;
		typedef size_t size_type;
		typedef ptrdiff_t Distance;
	protected:
		typedef T** map_pointer;
		typedef simple_alloc<value_type, alloc> data_allocator;
		typedef simple_alloc<Pointer, alloc> map_allocator;
	protected:
		//��������������������������Χ
		iterator start;//��һ����������start.curָ���һ��Ԫ��
		iterator finish; //finishָ�����һ��������,��ʹ����û���κ�Ԫ��

		map_pointer map;
		size_type map_size;
	
	public:
		iterator begin()const{ return start; }
		iterator end()const{ return finish; }
		Reference operator[](size_type n){return start[Distance(n)]; }
		Reference front(){ return *start; }
		const_Reference front()const{ return *start; }
		Reference back(){ return *(finish- 1); }
		const_Reference back()const{ return *(finish - 1); }
		Distance size()
		{ 
			return (finish - start);
		}
		size_type max_size(){ return buf_siz(BufSiz, sizeof(T))*map_size; }
		bool empty(){ return start == finish; }
		void push_back(const T&);
		void push_front(const T&);
		void pop_front();
		void pop_back();
		void clear();
		iterator erase(iterator);
		iterator erase(iterator, iterator);
		iterator insert(iterator, const T&);
		iterator insert_aux(iterator, const T&);
	public:
		bool operator==(const self& rhs)const{ return (start == rhs.start) && (finish == rhs.finish); }
		bool operator!=(const self& rhs)const{ return !(*this == rhs); }
		bool operator<(const self&)const;
	protected:
		void push_back_aux(const T&);
		void push_front_aux(const T&);
		void pop_front_aux();
		void pop_back_aux();
	public:
		stl_deque()
		{
			fill_initialize(0, T());
		}
		stl_deque(size_type n, const T& value) :start(), finish(), map(0), map_size(0)
		{
			//���º��������п����ռ��Լ��������ռ䣬������n��ֵΪvalue��Ԫ��
			fill_initialize(n, value); 
		}
		stl_deque(size_type n) :start(), finish(), map(0), map_size(0)
		{
			fill_initialize(n, T());
		}
		stl_deque(iterator, iterator);
	protected:
		//�ռ����ú���
		void fill_initialize(size_type, const T&);
		void create_map_and_nodes(size_type);
		//�ں������һ���µĻ�����
		void reserve_map_at_back(size_type node_to_add=1);
		void reserve_map_at_front(size_type node_to_add = 1);
		void reallocate_map(size_type, bool);

	public:
		~stl_deque();
	};

	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::stl_deque(iterator first, iterator last)
	{
		Distance n = last - first;
		create_map_and_nodes(n);
		try{
			if (first!=last)
				copy(first, last, start);
		}
		catch (...)
		{
						//commit or rollback
			for (cur = start.node; cur <= finish.node; ++cur)
				data_allocator::deallocate(*cur, buf_siz(BufSiz, sizeof(T)));
			throw;
		}
	}
	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T,Alloc,BufSiz>::fill_initialize(size_type n,const T& value)
	{
		create_map_and_nodes(n);	//�����п����ռ��Լ��������ռ�
		map_pointer cur;
		try{
			if (n == 0)
				return;
			//���²�������Ԫ��
			for (cur = start.node; cur < finish.node; ++cur)
				uninitialized_fill(*cur, *cur +buf_siz(BufSiz,sizeof(T)), value);
			//���һ����������������
			uninitialized_fill(finish.first, finish.cur, value);
		}
		catch (...)
		{
			//commit or rollback
			for (; cur >= start.node; --cur)
				destroy(*cur, *cur + buf_siz(BufSiz, sizeof(T)));
			for (cur = start.node; cur <= finish.node; ++cur)
				data_allocator::deallocate(*cur, buf_siz(BufSiz, sizeof(T)));
			throw;
		}
	}

	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::create_map_and_nodes(size_type num_of_elem)
	{
		//������,����������һ��������
		size_type num_of_node = num_of_elem / buf_siz(BufSiz,sizeof(T)) + 1;
		//����Ϊ8��������
		map_size = max(num_of_node, 8);
		//�����п���
		map = map_allocator::allocate(map_size);
		//����ʼλ����Ϊ���л��������м䲿�֣��������ߵĿ���չ����Գ�
		map_pointer node_start = map+(map_size-num_of_node)/2;
		map_pointer node_finish = node_start+num_of_node-1;
		map_pointer cur;
		//���û������ռ�
		try
		{//�������û�����ֻ��node_start��node_finish֮������,������Ϊ����
			for (cur = node_start; cur <= node_finish; ++cur)
				*cur = data_allocator::allocate(buf_siz(BufSiz,sizeof(T)));
		}
		catch (...)
		{
			//commit or rollback
			for (; cur >= node_start; --cur)
				data_allocator::deallocate(*cur);
		}
		//��������deque�ڲ��������޵��������ڵĻ�����
		start.set_node(node_start);
		finish.set_node(node_finish);
		//��������õ�������ǰָ���Ԫ��λ��
		start.cur = start.first;	//��һ��������ָ���һ�����������ײ�
		finish.cur = finish.first + num_of_elem%buf_siz(BufSiz,sizeof(T));
		//�������������curָ��������Ľڵ����ʼ��
		//finish.curָ�����һ��Ԫ�صĺ�
	}

	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::push_back(const T& value)
	{
		if (finish.cur != finish.last - 1)
		{//�����������ϵı��ÿռ�(������finish.cur��finish.last-1�����ռ�)
			construct(finish.cur, value);
			++finish.cur; //��֮ǰ�Ѿ��жϹ��Ƿ���磬���ﲻʹ�õ������ĵ��������������⺯������
		}
		else
			push_back_aux(value);
	}

	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::push_back_aux(const T& value)
	{
		reserve_map_at_back();//�����ж�map�Ƿ��ж���ռ�
		*(finish.node + 1) = data_allocator::allocate(buf_siz(BufSiz, sizeof(T)));
		try{
			construct(finish.cur, value);
			finish.node = finish.node + 1;
			finish.cur = finish.first;
		}
		catch (...)
		{
			finish.node = finish.node - 1;
			finish.cur = finish.last;
			data_allocator::deallocate(*(finish.node + 1), buf_siz(BufSiz, sizeof(T)));
			throw;
		}
	}

	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::reserve_map_at_back(size_type node_to_add)
	{
		if (node_to_add + 1 > map_size - (finish.node - map))
			//������������㣬Ҳ���ǵ�finish���ڵĻ�����Ϊmap�е����һ��������
			reallocate_map(node_to_add,false);//��������map

	}

	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::push_front(const T& value)
	{
		if (start.cur != start.first)
		{
			construct(start.cur, value);
			--start.cur;
		}
		else
			push_front_aux(value);
	}

	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::push_front_aux(const T& value)
	{
		reserve_map_at_front();
		*(start.node - 1) = data_allocator::allocate(buf_siz(BufSiz, sizeof(T)));
		try{
			start.node = start.node - 1;
			start.cur = start.last - 1;
			construct(start.cur, value);
		}
		catch (...)
		{
			start.node = start.node - 1;
			start.cur = start.first;
			data_allocator::deallocate((*start.node - 1),buf_siz(BufSiz,sizeof(T)));
		}
	}

	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::reserve_map_at_front(size_type node_to_add = 1)
	{
		if (node_to_add + 1 > map_size - (finish.node - map))//����ʣ��ռ��Ƿ���������
			reallocate_map(node_to_add, true);
	}

	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::reallocate_map(size_type node_to_add, bool add_to_front)
	{
		size_type old_num_node = finish.node - start.node + 1;
		size_type new_num_node = old_num_node + node_to_add;
		map_pointer new_start;
		if (map_size > 2 * new_num_node)
		{/*������кܶ౸�ÿռ���ڣ�˵���ɲ�ƽ���ǰ��(���)������ɵ�
		 ��ʱ����Ҫ���·���map��ֻ�����һ��λ��*/
			new_start = map + (map_size - new_num_node) / 2 +
				(add_to_front ? node_to_add : 0); //�Ƿ���ǰ�˲�����ɵĿռ䲻�㣿�ǾͶ�Ϊǰ�˷���ռ�
												  //ʹ�����Ժ�ǰ�����˵�ʣ��ռ�һ��
			copy(start.node, finish.node+1, new_start);
		}
		else
		{//�ռ䲻�㣬��Ҫ��������һ�������map
			size_type new_map_size = map_size + max(map_size, node_to_add)+2;
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_start = new_map + (new_map_size - new_num_node) / 2 +
				(add_to_front ? node_to_add : 0);
			copy(start.node, finish.node+1, new_start);
			map_allocator::deallocate(map,map_size); 
			map = new_map;
			map_size = new_map_size;
		}
		start.set_node(new_start);
		finish.set_node(new_num_node + new_start - 1);
	}

	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::pop_front()
	{
		if (!empty())
		{
			if (start.cur != start.last - 1)
			{
				destroy(start.cur);
				++start.cur;
			}
			else
				pop_front_aux();
		}

	}

	template <typename T, typename Alloc, size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::pop_front_aux()
	{
		destroy(start.cur);
		data_allocator::deallocate(start.first,buf_siz(BufSiz,sizeof(T)));
		start.set_node(start.node + 1);
		start.cur = start.first;
	}

	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::pop_back()
	{
		if (!empty())
		{
			if (finish.cur != finish.first)
			{
				destroy(finish.cur);
				--finish.cur;
			}
			else
				pop_back_aux();	//���һ��������û���κ�Ԫ��
		}
	}

	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::pop_back_aux()
	{
		data_allocator::deallocate(finish.first, buf_siz(BufSiz, sizeof(T)));
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		destroy(finish.cur);
	}

	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::clear()
	{	/*ֻ��һ���׻���������������Ԫ�ز��ͷ����໺����
		ֻ��һ��������Ϊdeque�ĳ�ʼ״̬*/
		for (map_pointer node = start.node + 1; node <= finish.node; ++node)
		{//�������ͷų���β����������������л�����
			destroy(*node, *node + buf_siz(BufSiz,sizeof(T)));
			data_allocator::deallocate(*node, buf_siz(BufSiz,sizeof(T)));
		}
		if (start.node != finish.node)//�����������������
		{
			destroy(start.cur, start.last);
			destroy(finish.first, finish.cur);
			data_allocator::deallocate(finish.first, buf_siz(BufSiz,sizeof(T)));
		}
		else
			destroy(start.cur, finish.cur);
		finish = start;  //ʹ��Ĭ�ϵ�copy assignment,���������ݳ�Ա����
	}

	template <typename T,typename Alloc,size_t BufSiz>
	typename stl_deque<T,Alloc,BufSiz>::iterator 
		stl_deque<T, Alloc, BufSiz>::erase(iterator position)
	{
		iterator next = position+1;
		Distance d = position - start;
		if (d < size() / 2)
		{
			copy_backward(start, position, next);
			pop_front();  //pop_front���Դ���ͷ���Ŀջ���������
		}
		else
		{
			copy(next, finish, position);
			pop_back();   //pop_back���Դ���β���Ŀջ���������
		}
		return start + d;	//�������Ԫ�غ����һ��Ԫ��
	}

	template <typename T,typename Alloc,size_t BufSiz>
	typename stl_deque<T, Alloc, BufSiz>::iterator
		stl_deque<T, Alloc, BufSiz>::erase(iterator first, iterator last)
	{

		if (first == start&&last == finish)
		{
			clear();
			return finish;
		}
		else
		{
			Distance n=last - first;
			if (first - start < (buf_siz(BufSiz,sizeof(T))-n)/2)
			{
				copy_backward(start, first, last);
				iterator new_start = start + n;
				destroy(start, new_start);
				for (map_pointer node = start.node; node != new_start.node; ++node)
					data_allocator::deallocate(*node, buf_siz(BufSiz,sizeof(T)));
				start = new_start;
			}
			else
			{
				copy(last, finish, first);
				iterator new_finish = finish - n;
				destroy(new_finish, finish);
				for (map_pointer node = new_finish.node; node != finish.node; ++node)
					data_allocator::deallocate(*node, buf_siz(BufSiz,sizeof(T)));
				finish = new_finish;
			}
			return start + n;	//���������������һ��Ԫ��
		}

	}

	template <typename T,typename Alloc,size_t BufSiz>
	typename stl_deque<T,Alloc,BufSiz>::iterator
		stl_deque<T, Alloc, BufSiz>::insert(iterator position, const T& value)
	{//��insertΪ�ӿں�����������ͷ����β��������������Σ�һ�����ε���insert_aux
		if (position == start)
		{
			push_front(value);
			return start;
		}
		else if (position == finish)
		{
			push_back(value);
			iterator temp = finish;
			--temp;
			return temp;	//insert���ز���������
		}
		else
			return insert_aux(position, value);
	}

	template <typename T,typename Alloc,size_t BufSiz>
	typename stl_deque<T,Alloc,BufSiz>::iterator
		stl_deque<T, Alloc, BufSiz>::insert_aux(iterator position, const T& value)
	{
		Distance dis = position - start;
		if (dis < buf_siz(BufSiz,sizeof(T)) / 2)				//��������ǰ��Ԫ�ؽ���
		{
			iterator old_start = start;
			push_front(front());			//����ǰ�˼������һԪ��ֵͬ��Ԫ��
			iterator temp = old_start;	//����start��old_start�ϵ�ֵһ��
			++temp;						//���Կ���������old_start-1��ʼ��Ŀ��������old_startΪ���
			position = start + dis;			
			iterator next = position ;
			++next;

			copy(temp, next, old_start);
		}
		else
		{
			iterator old_finish = finish;
			push_back(back());
			iterator temp = old_finish - 1;

			copy_backward(position, temp, old_finish);
		}
		*position = value;
		return position;
	}

	template <typename T,typename Alloc,size_t BufSiz>
	stl_deque<T,Alloc,BufSiz>::~stl_deque()
	{
		for (map_pointer node = start.node; node <= finish.node; ++node)
			data_allocator::deallocate(*node, buf_siz(BufSiz,sizeof(T)));
		map_allocator::deallocate(map, map_size);
	}

	template <typename T,typename Alloc,size_t BufSiz>
	bool stl_deque<T, Alloc, BufSiz>::operator<(const self& rhs)const
	{
		iterator lhs_it=begin();
		iterator rhs_it = rhs.begin();
		for (; lhs_it != end() && rhs_it != rhs.end(); ++lhs_it,++rhs_it)
		{
			if (*lhs_it < *rhs_it)
				return true;
			else if (*lhs_it > *rhs_it)
				return false;
		}
		if (lhs_it != end())
			return false;
		else if (rhs_it != rhs.end())
			return true;
	}
}
