#pragma once
#pragma execution_character_set("utf-8")
#include"stl_alloc.h"
#include"stl_construct.h"
#include"stl_iterator_type.h"
#include"stl_algorithm.h"

#define buf_siz(x,y) _deque_buf_size(x,y)
	size_t _deque_buf_size(size_t n, size_t sz)
	{//制定缓冲区大小,n为用户指定的模板非类型参数Bufsiz，sz为元素大小
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
		//指向控制中心的指针,控制中心存储指向各个缓冲区(头部)的指针
		typedef T** map_pointer;
		
		typedef _deque_iterator self;
	public:
		//数据成员
		T* cur;       //指向当前元素
		T* first;     //指向当前缓冲区的头部
		T* last;      //指向当前缓冲区的尾部(尾后)
		map_pointer node;   //指向当前所在的缓冲区

	protected:
		//缓冲区跳跃操作(new_node为目的缓冲区)
		//cur会在使用set_node的那些操作中单独设置
		void set_node(map_pointer new_node)
		{//调整界限指针
			first = *new_node;
			last = *new_node + Distance(buf_siz(BufSiz,sizeof(T)));
			node = new_node;
		}
	public:
		//元素取值、成员存取操作
		Reference operator*(){ return *cur; }
		Pointer operator->(){ return &(operator*()); }
		
		//迭代器操作，符合random_access_iterator的要求
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
		{//不能在判断出界之前递减cur,因为在first之前并没有能判断cur出界的缓冲单元
			if (cur == first)
			{//在递减之前已到达头部，递减后出界
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

		//实现随机存取,迭代器跳跃n个距离
		//允许n为负值以简化代码
		self& operator+=(Distance n)
		{
			Distance offset = n + (cur - first);//计算需要跳跃的元素数
			if (offset >= 0 && offset < Distance(buf_siz(BufSiz,sizeof(T))))//目标在同一缓冲区内
				cur += n;
			else
			{//下面计算缓冲区偏移量
				Distance node_offset =
					offset>0 ? Distance(offset / buf_siz(BufSiz,sizeof(T)))
					: -Distance((offset - 1) / buf_siz(BufSiz,sizeof(T))) - 1;//这里offset-1是减掉前一个缓冲区的last
				set_node(node+node_offset);
				//设置cur
				cur = first + (offset - (node_offset*buf_siz(BufSiz,sizeof(T))));
			}
			return *this;
		}

		//其余移动操作皆使用+=
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

		//迭代器随机存取
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
		//下面两个迭代器划定缓冲区范围
		iterator start;//第一个缓冲区，start.cur指向第一个元素
		iterator finish; //finish指向最后一个缓冲区,即使里面没有任何元素

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
			//以下函数配置中控器空间以及缓冲区空间，并构造n个值为value的元素
			fill_initialize(n, value); 
		}
		stl_deque(size_type n) :start(), finish(), map(0), map_size(0)
		{
			fill_initialize(n, T());
		}
		stl_deque(iterator, iterator);
	protected:
		//空间配置函数
		void fill_initialize(size_type, const T&);
		void create_map_and_nodes(size_type);
		//在后端配置一个新的缓冲区
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
		create_map_and_nodes(n);	//配置中控器空间以及缓冲区空间
		map_pointer cur;
		try{
			if (n == 0)
				return;
			//以下操作构造元素
			for (cur = start.node; cur < finish.node; ++cur)
				uninitialized_fill(*cur, *cur +buf_siz(BufSiz,sizeof(T)), value);
			//最后一个缓冲区并不填满
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
		//若整除,则额外多配置一个缓冲区
		size_type num_of_node = num_of_elem / buf_siz(BufSiz,sizeof(T)) + 1;
		//最少为8个缓冲区
		map_size = max(num_of_node, 8);
		//配置中控器
		map = map_allocator::allocate(map_size);
		//将起始位置设为所有缓冲区的中间部分，这样两边的可扩展区域对称
		map_pointer node_start = map+(map_size-num_of_node)/2;
		map_pointer node_finish = node_start+num_of_node-1;
		map_pointer cur;
		//配置缓冲区空间
		try
		{//下面配置缓冲区只在node_start与node_finish之间配置,其余作为备用
			for (cur = node_start; cur <= node_finish; ++cur)
				*cur = data_allocator::allocate(buf_siz(BufSiz,sizeof(T)));
		}
		catch (...)
		{
			//commit or rollback
			for (; cur >= node_start; --cur)
				data_allocator::deallocate(*cur);
		}
		//首先设置deque内部两个界限迭代器所在的缓冲区
		start.set_node(node_start);
		finish.set_node(node_finish);
		//其次让设置迭代器当前指向的元素位置
		start.cur = start.first;	//第一个迭代器指向第一个缓冲区的首部
		finish.cur = finish.first + num_of_elem%buf_siz(BufSiz,sizeof(T));
		//如果正好整除，cur指向额外多配的节点的起始处
		//finish.cur指向最后一个元素的后方
	}

	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::push_back(const T& value)
	{
		if (finish.cur != finish.last - 1)
		{//有两个及以上的备用空间(至少有finish.cur与finish.last-1两个空间)
			construct(finish.cur, value);
			++finish.cur; //在之前已经判断过是否出界，这里不使用迭代器的递增操作符，避免函数调用
		}
		else
			push_back_aux(value);
	}

	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::push_back_aux(const T& value)
	{
		reserve_map_at_back();//首先判断map是否有多余空间
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
			//如果缓冲区不足，也就是当finish所在的缓冲区为map中的最后一个缓冲区
			reallocate_map(node_to_add,false);//重新配置map

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
		if (node_to_add + 1 > map_size - (finish.node - map))//计算剩余空间是否满足需求
			reallocate_map(node_to_add, true);
	}

	template <typename T,typename Alloc,size_t BufSiz>
	void stl_deque<T, Alloc, BufSiz>::reallocate_map(size_type node_to_add, bool add_to_front)
	{
		size_type old_num_node = finish.node - start.node + 1;
		size_type new_num_node = old_num_node + node_to_add;
		map_pointer new_start;
		if (map_size > 2 * new_num_node)
		{/*如果还有很多备用空间存在，说明由不平衡的前端(后端)插入造成的
		 此时不需要重新分配map，只需调整一下位置*/
			new_start = map + (map_size - new_num_node) / 2 +
				(add_to_front ? node_to_add : 0); //是否由前端插入造成的空间不足？是就多为前端分配空间
												  //使插入以后前后两端的剩余空间一致
			copy(start.node, finish.node+1, new_start);
		}
		else
		{//空间不足，需要重新配置一个更大的map
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
				pop_back_aux();	//最后一个缓冲区没有任何元素
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
	{	/*只留一个首缓冲区，析构所有元素并释放其余缓冲区
		只有一个缓冲区为deque的初始状态*/
		for (map_pointer node = start.node + 1; node <= finish.node; ++node)
		{//析构并释放除首尾两个缓冲区外的所有缓冲区
			destroy(*node, *node + buf_siz(BufSiz,sizeof(T)));
			data_allocator::deallocate(*node, buf_siz(BufSiz,sizeof(T)));
		}
		if (start.node != finish.node)//如果还有两个缓冲区
		{
			destroy(start.cur, start.last);
			destroy(finish.first, finish.cur);
			data_allocator::deallocate(finish.first, buf_siz(BufSiz,sizeof(T)));
		}
		else
			destroy(start.cur, finish.cur);
		finish = start;  //使用默认的copy assignment,将所有数据成员拷贝
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
			pop_front();  //pop_front可以处理头部的空缓冲区问题
		}
		else
		{
			copy(next, finish, position);
			pop_back();   //pop_back可以处理尾部的空缓冲区问题
		}
		return start + d;	//返回清除元素后的下一个元素
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
			return start + n;	//返回清除区间后的下一个元素
		}

	}

	template <typename T,typename Alloc,size_t BufSiz>
	typename stl_deque<T,Alloc,BufSiz>::iterator
		stl_deque<T, Alloc, BufSiz>::insert(iterator position, const T& value)
	{//此insert为接口函数，处理在头部或尾部插入的特殊情形，一般情形调用insert_aux
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
			return temp;	//insert返回插入点迭代器
		}
		else
			return insert_aux(position, value);
	}

	template <typename T,typename Alloc,size_t BufSiz>
	typename stl_deque<T,Alloc,BufSiz>::iterator
		stl_deque<T, Alloc, BufSiz>::insert_aux(iterator position, const T& value)
	{
		Distance dis = position - start;
		if (dis < buf_siz(BufSiz,sizeof(T)) / 2)				//如果插入点前方元素较少
		{
			iterator old_start = start;
			push_front(front());			//在最前端加入与第一元素同值的元素
			iterator temp = old_start;	//现在start、old_start上的值一样
			++temp;						//所以拷贝操作从old_start-1开始，目标区间以old_start为起点
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
