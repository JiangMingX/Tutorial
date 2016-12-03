#pragma once
#pragma execution_character_set("utf-8")
#include<new>
#include"type_traits.h"

template<typename Iterator>
void destroy(Iterator);

template<typename T>
void destroy_main(T *);

template<typename Iterator,typename T>
void _destroy(Iterator, T);

template <typename Iterator>
void _destroy_aux(Iterator , _true_type);

template<typename Iterator>
void _destroy_aux(Iterator, _false_type);


template<typename T>
void destroy_main(T * pointer)
{
	pointer->~T();
}
template<typename Iterator>
void destroy(Iterator first)
{
	_destroy(first, first);
}
template<typename Iterator,typename T>
void _destroy(Iterator first, T value)
{
	typedef typename type_traits<T>::has_trivial_destructor trivial_destructor;
	_destroy_aux(first, trivial_destructor());
}
template<typename Iterator>
void _destroy_aux(Iterator first, _false_type)
{
	destroy_main(&*first);
}
template<typename Iterator>
void _destroy_aux(Iterator first, _true_type){}
