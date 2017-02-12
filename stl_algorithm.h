#pragma once
#pragma execution_character_set("utf-8")
#include"stl_iterator_type.h"
#include"stl_type_traits.h"

namespace stl{
	template <typename InputIterator, typename OutputIterator>
	OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		_copy_dispatch<InputIterator, OutputIterator> cd;
		return cd(first, last, result);
	}
	//copy算法的内层实现
	template<typename InputIterator, typename OutputIterator>
	struct _copy_dispatch
	{
		OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
		{
			typedef typename stl_iterator_traits<InputIterator>::iterator_category iterator_category;
			return _copy(first, last, result, iterator_category());
		}
	};

	template<typename T>
	struct _copy_dispatch < T*, T* >
	{
		T * operator()(T* first, T* last, T* result)
		{
			typedef typename stl_type_traits<T>::has_trivial_assignment_operator t;
			return _copy_t(first, last, result, t());
		}
	};

	template<typename T>
	struct _copy_dispatch < const T*, T* >
	{
		T * operator()(const T* first, const T* last, T* result)
		{
			typedef typename stl_type_traits<T>::has_trivial_assginment_operator t;
			return _copy_t(first, last, result, t());
		}
	};

	template<typename InputIterator, typename OutputIterator>
	OutputIterator _copy(InputIterator first, InputIterator last,
		OutputIterator result, stl_input_iterator_tag)
	{
		for (; first != last; ++result, ++first)
			*result = *first;
		return result;
	}

	template<typename RandomAccessIterator, typename OutputIterator>
	OutputIterator _copy(RandomAccessIterator first, RandomAccessIterator last,
		OutputIterator result, stl_random_access_iterator_tag)
	{
		typedef typename stl_iterator_traits<RandomAccessIterator>::difference_type distance_type;
		return _copy_d(first, last, result, distance_type());
	}

	template<typename RandomAccessIterator, typename OutputIterator, typename Distance>
	OutputIterator _copy_d(RandomAccessIterator first, RandomAccessIterator last,
		OutputIterator result, Distance)
	{
		for (Distance n = last - first; n > 0; --n, ++result, ++first)
			*result = *first;
		return result;
	}

	template<typename T>
	T* _copy_t(const T* first, const T* last, T* result, _true_type)
	{
		memmove(result, first, sizeof(T)*(last - first));
		return result + (last - first);
	}

	template<typename T>
	T* _copy_t(const T* first, const T* last, T* result, _false_type)
	{
		typedef typename stl_iterator_traits<const T*>::difference_type distance_type;
		return _copy_d(first, last, result, distance_type());
	}

	//在未初始化空间中构造对象
	template <typename ForwardIterator, typename T, typename Size>
	ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& value)
	{//中转函数，传递迭代器所指对象的类型信息
		typedef typename stl_iterator_traits<ForwardIterator>::value_type value_type;
		return _uninitialized_fill_n(first, n, value, value_type());
	}

	template < typename ForwardIterator, typename T, typename Size, typename T1 >
	ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size n, const T& value, T1)
	{//中转函数，传递迭代器所指对象类型是否为Plain Old Data的信息
		typedef typename stl_type_traits<T1>::is_POD_type is_POD;
		return _uninitialized_fill_n_aux(first, n, value, is_POD());
	}

	template <typename ForwardIterator, typename T, typename Size>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T& value, _true_type)
	{//是Plain Old Data 使用全局函数fill_n为对象赋值
		return fill_n(first, n, value);
	}

	template <typename ForwardIterator, typename T, typename Size>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T& value, _false_type)
	{//非Plain Old Data ，使用构造函数构造对象
		for (; n > 0; --n, ++first)
			construct(&*first, value);
		return first;
	}

	template <typename ForwardIterator, typename T>
	ForwardIterator uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value)
	{
		typedef typename stl_iterator_traits<ForwardIterator>::value_type value_type;
		return _uninitialized_fill(first, last, value, value_type());
	}

	template <typename ForwardIterator, typename T, typename T1>
	ForwardIterator _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value, T1)
	{
		typedef typename stl_type_traits<T1>::is_POD_type is_POD;
		return _uninitialized_fill_aux(first, last, value, is_POD());
	}

	template <typename ForwardIterator, typename T>
	ForwardIterator _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _true_type)
	{
		return fill(first, last, value);
	}

	template <typename ForwardIterator, typename T>
	ForwardIterator _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _false_type)
	{
		for (; first != last; ++first)
			construct(&*first, value);
		return first;

	}

	template <typename InputIterator, typename ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
		ForwardIterator result)
	{
		typedef typename stl_iterator_traits<InputIterator>::value_type value_type;
		return _uninitialized_copy(first, last, result, value_type());
	}

	template <typename InputIterator, typename ForwardIterator, typename T>
	ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last,
		ForwardIterator result, T)
	{
		typedef typename stl_type_traits<T>::is_POD_type is_POD;
		return _uninitialized_copy_aux(first, last, result, is_POD());
	}

	template <typename InputIterator, typename ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _true_type)
	{
		return copy(first, last, result);
	}


	template <typename InputIterator, typename ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _false_type)
	{
		ForwardIterator old_result = result;
		try{
			for (; first != last; ++result, ++first)
				construct((&*first), result);
		}
		catch (...)
		{
			destroy(old_result, result);
			throw;
		}
		return result;
	}

	//特例化版本
	char* uninitialized_copy(const char* first, const char* last, char* result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}

	wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
	{
		memmove(result, first, sizeof(wchar_t)*(last - first));
		return result + (last - first);
	}





	template <typename ForwardIterator, typename T>
	ForwardIterator fill(ForwardIterator first, ForwardIterator last, const T& value)
	{
		for (; first != last; ++first)
			*first = value;
		return first;
	}
	template <typename OutputIterator, typename Size, typename T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value)
	{
		for (; n > 0; --n, ++first)
			*first = value;
		return first;

	}




	template <typename BidirectionalIterator1, typename BidirectionalIterator2>
	BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
		BidirectionalIterator1 last, BidirectionalIterator2 result)
	{
		_copy_backward_dispatch<BidirectionalIterator1, BidirectionalIterator2> cbd;
		return cbd(first, last, result);
	}

	template <typename BidirectionalIterator1, typename BidirectionalIterator2>
	struct _copy_backward_dispatch
	{
		BidirectionalIterator2 operator()(BidirectionalIterator1 first,
			BidirectionalIterator2 last, BidirectionalIterator2 result)
		{
			typedef typename
				stl_iterator_traits<BidirectionalIterator1>::iterator_category iterator_category;
			return _copy_backward(first, last, result, iterator_category());
		}
	};

	template <typename BidirectionalIterator1, typename BidirectionalIterator2>
	BidirectionalIterator2 _copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
		BidirectionalIterator2 result, stl_bidirectional_iterator_tag)
	{
		for (; last != first; --result, --last)
			*result = *last;
		return result;
	}

	template <typename RandomaccessIterator, typename BidirectionalIterator>
	BidirectionalIterator _copy_backward(RandomaccessIterator first,
		RandomaccessIterator last, BidirectionalIterator result, stl_random_access_iterator_tag)
	{
		typedef typename stl_iterator_traits<RandomaccessIterator>::difference_type difference_type;
		return _copy_backward_d(first, last, result, difference_type());
	}

	template <typename RandomaccessIterator, typename BidirectionalIterator, typename Distance>
	BidirectionalIterator _copy_backward_d(RandomaccessIterator first,
		RandomaccessIterator last, BidirectionalIterator result, Distance)
	{
		for (Distance n = last - first; n > 0; --result, --last, --n)
			*result = *last;
		return result;
	}

	template <typename ForwardIterator, typename T>
	ForwardIterator find(ForwardIterator first, ForwardIterator last, const T& value)
	{
		while (first != last && (*first) != value)
			return first;
	}







	template <typename T>
	struct less
	{
		bool operator()(const T& lhs, const T& rhs)const
		{
			return lhs < rhs;
		}
	};

	template <>
	struct less < void >
	{
		template <typename T1, typename T2>
		auto operator()(T1&& lhs, T2&& rhs)const
			->decltype(static_cast<T1&&>(lhs)
			< static_cast<T2&&>(rhs))
		{
			return static_cast<T1&&>(lhs) < static_cast<T2&&>(rhs);
		}
	};

	template <typename RandomAccessIterator, typename T>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last, const T& value)
	{
		_push_heap(first, last, value, (last - first));
	}

	template <typename RandomAccessIterator, typename T, typename Distance>
	void _push_heap(RandomAccessIterator first, RandomAccessIterator last, const T& value, Distance Siz)
	{
		Distance Hole = Siz;
		Distance parent = Siz / 2;
		for (; value > *(first + Hole - 1)&&Hole>0; Hole = parent, parent /= 2)
			*(first + Hole - 1) = *(first + parent - 1);
		*(first + Hole - 1) = value;
	}

	template <typename RandomAccessIterator>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		typedef typename stl_iterator_traits<RandomAccessIterator>::value_type value_type;
		_pop_heap(first, last, value_type(),(last - first));
	}

	template <typename RandomAccessIterator, typename T, typename Distance>
	void _pop_heap(RandomAccessIterator first, RandomAccessIterator last,T, Distance Siz)
	{
		Distance Hole = 1;
		Distance Child;
		T LastElem = *(last - 1);
		T MaxElem = *first;
		--Siz;
		for (; Hole * 2 <= Siz; Hole = Child)
		{
			Child = Hole * 2;
			if (Child != Siz && *(first + Child + 1) > *(first + Child))
				++Child;
			if (LastElem < *(first + Child-1))
				*(first + Hole - 1) = *(first + Child - 1);
			else
				break;
		}
		*(first + Hole - 1) = LastElem;
		*(last - 1) = MaxElem;
	}

	template <typename RandomAccessIterator>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		while (last - first>1)
			pop_heap(first, last--);
	}

	template <typename RandomAccessIterator>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		typedef typename stl_iterator_traits<RandomAccessIterator>::difference_type Distance;
		_make_heap(first, last, Distance());
	}
	template <typename RandomAccessIterator,typename Distance>
	void _make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance)
	{
		Distance len = last - first;
		Distance hole = (last - 1) / 2;
		
		while (1)
		{
			_adjust_heap(first, hole, len);
			if (hole == 0)
				return;
			hole--;
		}
	}

	template <typename RandomAccessIterator,typename Distance>
	void _adjust_heap(RandomAccessIterator first, Distance hole, Distance len)
	{
		Distance Child = hole * 2;
		while (Child <=len)
		{
			if (Child != len&&*(first + Child + 1) > *(first + Child))
				++Child;
			*(first + hole) = *(first + Child);
			hole = Child;
			Child *= 2;
		}
	}

	template <typename T1>
	void swap_aux(T1& lhs, T1& rhs)
	{
		T1 temp = lhs;
		lhs = rhs;;
		rhs = lhs;
	}
}