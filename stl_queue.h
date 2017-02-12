#pragma once
#pragma execution_character_set("utf-8")
#include"stl_deque.h"

namespace stl{
	template <typename T, typename Alloc=alloc, class Sequence=stl_deque<T,Alloc>>
	class stl_queue;

	template <typename T, typename Alloc=alloc, class Sequence=stl_deque<T,Alloc>>
	bool operator==(const stl_queue<T>& lhs, const stl_queue<T>& rhs)
	{
		return lhs.c == rhs.c;
	}

	template <typename T, typename Alloc = alloc, class Sequence = stl_deque<T, Alloc>>
	bool operator!=(const stl_queue<T>& lhs, const stl_queue<T>& rhs)
	{
		return !(lhs == rhs);
	}
	template <typename T, typename Alloc = alloc, class Sequence = stl_deque<T, Alloc>>
	bool operator<(const stl_queue<T>& lhs, const stl_queue<T>& rhs)
	{
		return lhs.c < rhs.c;
	}
	template <typename T,typename Alloc,class Sequence>
	class stl_queue
	{
		friend bool operator==<T, Alloc, Sequence>(const stl_queue<T,Alloc,Sequence>&, const stl_queue<T,Alloc,Sequence>&);
		friend bool operator!=<T, Alloc, Sequence>(const stl_queue<T, Alloc, Sequence>&, const stl_queue<T, Alloc, Sequence>&);
		friend bool operator< <T, Alloc, Sequence>(const stl_queue<T, Alloc, Sequence>&, const stl_queue<T, Alloc, Sequence>&);
	public:
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::Pointer Pointer;
		typedef typename Sequence::const_Pointer const_Pointer;
		typedef typename Sequence::Reference Reference;
		typedef typename Sequence::const_Reference const_Reference;
		typedef typename Sequence::size_type size_type;
		typedef typename Sequence::Distance Distaance;
	protected:
		Sequence c;
	public:
		void push(const T& value){ return c.push_back(value); }
		void pop(){ return c.pop_front(); }
		bool empty(){ return c.empty(); }
		size_type size(){ return c.size(); }
		Reference front(){ return c.front(); }
		const_Reference front()const{ return c.front(); }
		Reference back(){ return c.back(); }
		const_Reference back()const{ return c.back(); }
	};
}