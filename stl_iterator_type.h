#pragma once
#pragma execution_character_set("utf-8")
#ifndef ptrdiff_t
#define ptrdiff_t __int64 
#endif
namespace stl{
	struct stl_input_iterator_tag{};//输入迭代器
	struct stl_output_iterator_tag{};//输出迭代器
	struct stl_forward_iterator_tag :public stl_input_iterator_tag{};//可读写迭代器
	struct stl_bidirectional_iterator_tag :public stl_forward_iterator_tag{};//双向迭代器
	struct stl_random_access_iterator_tag :public stl_bidirectional_iterator_tag{};//随机迭代器


	//型别集装箱,迭代器皆继承此类
	template<typename Category, typename T, typename Distance = ptrdiff_t,
		typename Pointer = T*, typename Reference = T&>
	struct iterator
	{
		typedef Category iterator_category;//迭代器类型
		typedef T value_type;//迭代器所指对象类型
		typedef Distance difference_type;//迭代器之间距离类型
		typedef Pointer pointer;//迭代器所指对象的指针型别
		typedef Reference reference;//迭代器所指对象的引用型别
	};
	//型别萃取器,类型型别由迭代器自主更改
	template<typename Iterator>//模板参数应设为迭代器
	struct stl_iterator_traits
	{
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::Distance difference_type;
		typedef typename Iterator::Pointer pointer;
		typedef typename Iterator::Reference reference;

	};

	template<typename T>
	typename stl_iterator_traits<T>::iterator_category
		iterator_ceategory(const T&)
	{
		typedef typename stl_iterator_traits<T>::iterator_category category;
		return category();
	}

	//特例化原生指针类型
	template<typename T>
	struct stl_iterator_traits < T* >
	{
		typedef stl_random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef T* pointer;
		typedef ptrdiff_t difference_type;
		typedef T& reference;
	};

	template<typename T>
	struct stl_iterator_traits < const T* >
	{
		typedef stl_random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef const T* pointer;
		typedef ptrdiff_t difference_type;
		typedef const T& reference;
	};
}