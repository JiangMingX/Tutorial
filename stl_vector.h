#pragma once
#pragma execution_character_set("utf-8")
#include"stl_construct.h"
#include"stl_alloc.h"
#include"stl_algorithm.h"

namespace stl{
	template<typename T, class Alloc = alloc>
	class stl_vector
	{
	public:
		typedef T value_type;
		typedef value_type* iterator;
		typedef const value_type* const_iterator;
		typedef value_type* Pointer;
		typedef value_type& Reference;
		typedef const value_type& const_Reference;
		typedef size_t size_type;
		typedef ptrdiff_t Distance;

	protected:
		typedef simple_alloc<value_type, Alloc> data_allocator;

		iterator start;
		iterator finish;
		iterator end_of_storage;

		void insert_aux(iterator, const T&);

		void deallocate();

		void fill_initialize(size_type, const T&);

		iterator allocate_and_fill(size_t, const T&);

	public:
		iterator begin(){ return start; }
		iterator end(){ return finish; }
		size_type size(){ return finish - start; }
		size_type capacity()const{ return size_type(end_of_storage - start); }
		Reference operator[](size_type n){ return *(start + n); }
		Reference front(){ return *start; }
		Reference back(){ return *(finish - 1); }

		void push_back(const T&);
		void pop_back();
		iterator erase(iterator);
		iterator erase(iterator, iterator);
		void resize(size_type, const T&);
		void resize(size_type);
		void insert(iterator, size_type, const T&);

	public:
		const_iterator begin()const{ return start; }
		const_iterator end()const { return finish; }
		const_Reference operator[](size_type n)const{ return *(start + n); }
		bool empty()const{ return start == finish; }
		size_type size()const{ return size_type(finish - start); }
		const_Reference front()const{ return *start; }
		const_Reference back()const{ return *(finish - 1); }
	public:
		stl_vector() :start(nullptr), finish(nullptr), end_of_storage(nullptr){}
		explicit stl_vector(size_type n){ fill_initialize(n, T()); }
		stl_vector(size_type n, const T& value){ fill_initialize(n, value); }
		stl_vector(long n, const T& value){ fill_initialize(n, value); }
		stl_vector(int n, const T& value){ fill_initialize(n, value); }
		stl_vector(iterator, iterator);
		virtual ~stl_vector();


	};
	template <typename T,class Alloc>
	stl_vector<T,Alloc>::stl_vector(iterator first, iterator last)
	{
		size_type n = last - first;
		start=data_allocator::allocate(n);
		finish = start + n;
		end_of_storage = finish;
		copy(first, last, start);
	}
	template <typename T, class Alloc>
	void stl_vector<T, Alloc>::deallocate()
	{
		if (start)
			data_allocator::deallocate(start, end_of_storage - start);
	}

	template<typename T, class Alloc>
	void stl_vector<T, Alloc>::fill_initialize(size_t n, const T& value)
	{
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}
	//配置空间并填满
	template <typename T, class Alloc>
	typename stl_vector<T, Alloc>::iterator stl_vector<T, Alloc>::allocate_and_fill(size_t n, const T& value)
	{
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result, n, value);//全局函数 在未初始化空间中构造对象
		return result;
	}
	template<typename T, class Alloc>
	void stl_vector<T, Alloc>::push_back(const T& x)
	{
		if (finish != end_of_storage)
		{
			construct(finish, x);
			++finish;
		}
		else
			insert_aux(finish, x);
	}
	template<typename T, class Alloc>
	void stl_vector<T, Alloc>::pop_back()
	{
		if (!empty())
		{
			--finish;
			destroy(finish);
		}
		else
			throw("vector empty before pop");
	}
	template<typename T, class Alloc>
	typename stl_vector<T, Alloc>::iterator stl_vector<T, Alloc>::erase(iterator position)
	{
		if (position + 1 != finish)
			copy(position + 1, position, finish);
		--finish;
		destroy(finish);
		return position;
	}

	template <typename T, class Alloc>
	typename stl_vector<T, Alloc>::iterator stl_vector<T, Alloc>::erase(iterator first, iterator last)
	{
		iterator i = copy(last, finish, first);
		destroy(i, finish);
		finish = finish - (last - first);
		return first;
	}

	template <typename T, class Alloc>
	void stl_vector<T, Alloc>::resize(size_type new_size, const T& value)
	{
		if (new_size < size())
			erase(start + new_size, finish);
		else
			insert(finish, new_size - size(), value);
	}
	template <typename T, class Alloc>
	void stl_vector<T, Alloc>::resize(size_type new_size){ resize(new_size, T()); }

	template <typename T, typename Alloc>
	void stl_vector<T, Alloc>::insert_aux(iterator position, const T& value)
	{
		//还有备用空间
		if (finish != end_of_storage)
		{
			construct(finish - 1, *finish);
			++finish;
			copy_backward(position + 1, finish - 2, finish - 1);
			*position = value;
		}
		else
		{
			const size_type old_size = size();
			const size_type len = old_size != 0 ? (old_size * 2) : 1;

			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;

			try{
				new_finish = uninitialized_copy(start, position, new_start);
				construct(&*new_finish, value);
				++new_finish;
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch (...)
			{
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw;
			}
			start = new_start;
			finish = new_finish;
			end_of_storage = start + len;
		}
	}
	//溢出插入操作
	template <typename T, typename Alloc>
	void stl_vector<T, Alloc>::insert(iterator position, size_type n, const T& value)
	{
		if (n > 0)
		{//如果备用空间足够
			if (end_of_storage - finish >= n)
			{
				const size_type elem_after = finish - position;
				iterator old_finish = finish;
				if (elem_after > n)
				{//如果插入点之后元素个数大于插入元素个数
					uninitialized_copy(finish, finish - n, finish);
					finish += n;
					copy_backward(position, old_finish - n, old_finish);
					fill(position, position + n, value);
				}
				else
				{//如果插入点之后元素个数小于插入元素个数
					uninitialized_fill_n(finish, n - elem_after, value);
					finish += n - elem_after;
					uninitialized_copy(position, old_finish, finish);
					finish += elem_after;
					fill(position, old_finish, value);
				}
			}
			else
			{//备用空间不足，需要重新分配空间
				const size_type old_size = size();
				const size_type new_size = old_size + max(old_size, n);
				iterator new_start = data_allocator::allocate(new_size);
				iterator new_finish = new_start;
				try
				{
					/*元素转移分为三步
					1.复制插入点之前的元素到新空间
					2.填充插入元素
					3.复制插入点之后的元素到新空间
					*/
					new_finish = uninitialized_copy(start, position, new_start);
					new_finish = uninitialized_fill_n(new_finish, n, value);
					new_finish = uninitialized_copy(position, finish, new_finish);
				}
				catch (...)
				{
					/*实现"异常安全机制"如果以上三个操作抛出异常，将析构所有新构造的对象，再
					释放新空间,即所谓"commit or rollback”
					*/
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, new_size);
					throw;
				}
				destroy(start, finish);
				data_allocator::deallocate(start, end_of_storage-start);//释放旧空间
				//调整界限，这是插入之前的迭代器全部失效的原因
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + new_size;
			}


		}


	}
	template <typename T,typename Alloc>
	stl_vector<T, Alloc>::~stl_vector()
	{
		destroy(start, finish);
		deallocate();
	}
}

