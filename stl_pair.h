#pragma once
#pragma execution_character_set("utf-8")
#include"stl_algorithm.h"
namespace stl
{
	template <typename T1,typename T2>
	struct pair
	{
		typedef T1 first_type;
		typedef T2 second_type;
		typedef pair<T1, T2> self_type;

		T1 first;
		T2 second;

		pair() :first(), second(){}
		pair(const T1& value1, const T2& value2) :first(value1), second(value2){}
		
		template <typename Other1,typename Other2>
		pair(const Other1& value1, const Other2& value2) : first(value1), second(value2){}

		self_type& operator=(const self_type& rhs)
		{
			if (this != &rhs)
			{
				first = rhs.first;
				second = rhs.first;
			}
			return *this;
		}

		template <typename Other1,typename Other2>
		self_type& operator=(const pair<Other1, Other2>& rhs)
		{
			first = rhs.first;
			second = rhs.second;
			return *this;
		}
		
	};

	template <typename T1,typename T2>
	void swap(pair<T1, T2>& lhs, pair<T1, T2>& rhs)
	{
		if (lhs != rhs)
		{
			swap_aux(lhs.first, rhs.first);
			swap_aux(lhs.second, rhs.second);
		}
	}

	template <typename T1,typename T2>
	bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return lhs == rhs;
	}

	template <typename T1,typename T2>
	bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(lhs == rhs);
	}

	template <typename T1,typename T2>
	bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return (lhs.first<rhs.first) || (!(lhs.first>rhs.first) && (lhs.second < rhs.second));
	}

	template <typename T1,typename T2>
	bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return rhs < lhs;
	}

	template < typename T1, typename T2>
	pair<T1, T2> make_pair(const T1& value1, const T2 value2)
	{
		return pair<T1, T2>(value1, value2);
	}
	

}