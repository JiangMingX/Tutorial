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
	//��inst���ͱ����ָ��Ϊ0(ʵ�������ǲ�û���õ����������)
	typedef _malloc_alloc_template<0> malloc_alloc;
	typedef malloc_alloc alloc;


	/*
	simple_allocΪ�ײ���ڴ��������ⲿ��װ�����Աȫ������_malloc_alloc_template
	�ĳ�Ա
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
		{//��allocate����һ��ָ����������Ĳ���n
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
		/* oom_allocΪ��̬������Ա,���ڴ���mallocʱ���ڴ治������
		   oom_reallocΪ��̬������Ա�����ڴ���reallocʱ���ڴ治������
		   _malloc_alloc_handlerΪ��̬���ݳ�Ա,Ϊvoid(*)()���͵ĺ���ָ��,�����û���
		   ���ƶ��ڴ�������
		   */
		static void * oom_malloc(size_t);
		static void * oom_realloc(void *, size_t);
		static void(*_malloc_alloc_oom_handler)();
	public:
		static void * allocate(size_t n)
		{
			if (n < 0)
				throw(_LENGTH_ERROR);
			void * result = malloc(n);//�����ڴ�
			if (result == nullptr)//����ڴ治��
				result = oom_malloc(n);//����oom_malloc
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
			//ʹ��free�����ͷ�p��ַ����������ڴ��
			free(p);
		}

		/*�˾�̬��Ա��������һ��void(*)()���͵ĺ���ָ����Ϊ����,����
		void(*)()���͵ĺ���ָ�롣������Ϊ���û��Լ����Ƶ��ڴ���ȷ����滻
		_malloc_alloc_handler,�ɴ�ʵ������C++��set_new_handler������
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
		//����ѭ����ֱ���ɹ������ڴ���û�û�ж����ڴ�������
		for (;;)
		{
			my_oom_handler = _malloc_alloc_oom_handler;
			if (my_oom_handler == nullptr)//����û�û�ж����ڴ�������
				exit(1);
			(*my_oom_handler)();//ʹ���û����Ƶķ���
			result = malloc(n);
			if (result)
				return result;
		}
	}
	template<int inst>
	void * _malloc_alloc_template<inst>::oom_realloc(void * p, size_t n)
	{
		//�˺��������˼·��oom_malloc���һ��
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