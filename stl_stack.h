#pragma once
#pragma execution_character_set("utf-8")
#include"stl_deque.h"
#ifndef _STL_TEMPLATE_NULL
#define _STL_TEMPLATE_NULL template<>
#endif

namespace stl{
	template <typename T,typename Alloc,class Sequence>
	class stl_stack;


	template <typename T,typename Alloc=alloc,class Sequence=stl_deque<T,Alloc>>
	bool operator==(const stl_stack<T, Alloc, Sequence>& lhs, const stl_stack<T, Alloc, Sequence>& rhs)
	{
		return lhs.c == rhs.c;
	}

	template <typename T,typename Alloc=alloc,class Sequence=stl_deque<T,Alloc>>
	bool operator!=(const stl_stack<T, Alloc, Sequence>& lhs, const stl_stack<T, Alloc, Sequence>& rhs)
	{
		return !(lhs == rhs);
	}

	template <typename T, typename Alloc = alloc, class Sequence = stl_deque<T, Alloc>>
	bool operator<(const stl_stack<T, Alloc, Sequence>& lhs, const stl_stack<T, Alloc, Sequence>& rhs)
	{
		return lhs.c < rhs.c;
	}
	template <typename T,typename Alloc=alloc,class Sequence=stl_deque<T,Alloc>>
	class stl_stack
	{
		friend bool operator==<T,Alloc,Sequence>(const stl_stack<T, Alloc, Sequence>&, const stl_stack<T, Alloc, Sequence>&);
		friend bool operator!=<T,Alloc,Sequence>(const stl_stack<T, Alloc, Sequence>&, const stl_stack<T, Alloc, Sequence>&);
		friend bool operator< <T,Alloc,Sequence>(const stl_stack<T, Alloc, Sequence>&, const stl_stack<T, Alloc, Sequence>&);
	public:
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::Pointer Pointer;
		typedef typename Sequence::const_Pointer const_Pointer;
		typedef typename Sequence::Reference Reference;
		typedef typename Sequence::const_Reference const_Reference;
		typedef typename Sequence::size_type size_type;
	protected:
		Sequence c;
	public:
		bool empty(){ return c.empty(); }
		size_type size(){ return c.size(); }
		void push(const T& value){ c.push_back(value); }
		void pop(){ c.pop_back(value); }
		Reference top(){ return c.back(); }
		const_Reference top()const{ return c.back(); }


	};
}