#pragma once
#pragma execution_character_set("utf-8")
#include <new>
#if !defined(_THROW_BAD_ALLOC)
#include<iostream>
#define _THROW_BAD_ALLOC cerr<<"out of memory"<<endl;
#endif
#ifndef _LENGTH_ERROR
#define _LENGTH_ERROR "length_error"
#endif

namespace stl{
	template<int inst>
	class _malloc_alloc_template;
	//将inst非型别参数指定为0(实际上我们并没有用到过这个参数)
	typedef _malloc_alloc_template<0> malloc_alloc;
	typedef malloc_alloc alloc;


	/*
	simple_alloc为底层的内存分配类的外部包装，其成员全部调用_malloc_alloc_template
	的成员
	*/
	template<typename T, class Alloc>
	class simple_alloc
	{
	public:
		static T * allocate(void)
		{
			return (T *)Alloc::allocate(sizeof(T));
		}
		static T * allocate(size_t n)
		{//此allocate接受一个指定对象个数的参数n
			return n == 0 ? nullptr : (T *)Alloc::allocate(sizeof(T)*n);
		}
		static void deallocate(T * p)
		{
			Alloc::deallocate(p, sizeof(T));
		}
		static void deallocate(T * p, size_t n)
		{
			if (n != 0)
				Alloc::deallocate(p, n);
		}
	};



	template<int inst>
	class _malloc_alloc_template
	{
		/* oom_alloc为静态函数成员,用于处理malloc时的内存不足问题
		   oom_realloc为静态函数成员，用于处理realloc时的内存不足问题
		   _malloc_alloc_handler为静态数据成员,为void(*)()类型的函数指针,用于用户自
		   己制定内存分配策略
		   */
		static void * oom_malloc(size_t);
		static void * oom_realloc(void *, size_t);
		static void(*_malloc_alloc_oom_handler)();
	public:
		static void * allocate(size_t n)
		{
			if (n < 0)
				throw(_LENGTH_ERROR);
			void * result = malloc(n);//请求内存
			if (result == nullptr)//如果内存不足
				result = oom_malloc(n);//调用oom_malloc
			return result;
		}
		static void * reallocate(void * p, size_t n)
		{
			if (n < 0)
				throw(_LENGTH_ERROR);
			void *result = realloc(n);
			if (result == nullptr)
				result = oom_realloc(p, n);
			return result;
		}
		static void deallocate(void * p, size_t)
		{
			//使用free函数释放p地址后所分配的内存块
			free(p);
		}

		/*此静态成员函数接受一个void(*)()类型的函数指针作为参数,返回
		void(*)()类型的函数指针。其作用为用用户自己定制的内存调度方法替换
		_malloc_alloc_handler,由此实现类似C++的set_new_handler方法。
		*/

		static void(*set_malloc_handler(void(*f)()))()
		{
			void(*old)() = _malloc_alloc_oom_handler;
			_malloc_alloc_oom_handler = f;
			return old;
		}
	};

	template<int inst>
	void(*_malloc_alloc_template<inst>::_malloc_alloc_oom_handler)() = 0;

	template<int inst>
	void * _malloc_alloc_template<inst>::oom_malloc(size_t n)
	{
		void(*my_oom_handler)();
		void * result;
		//无限循环，直至成功分配内存或用户没有定制内存分配策略
		for (;;)
		{
			my_oom_handler = _malloc_alloc_oom_handler;
			if (my_oom_handler == nullptr)//如果用户没有定制内存分配策略
				exit(1);
			(*my_oom_handler)();//使用用户定制的方法
			result = malloc(n);
			if (result)
				return result;
		}
	}
	template<int inst>
	void * _malloc_alloc_template<inst>::oom_realloc(void * p, size_t n)
	{
		//此函数的设计思路与oom_malloc如出一辙
		void(*my_oom_handler)();
		void * result;
		for (;;)
		{
			my_oom_handler = _malloc_alloc_oom_handler;
			if (my_oom_handler == nullptr)
				exit(1);
			(*my_oom_handler)();
			result = realloc(p, n);
			if (result)
				return result;
		}
	}
}