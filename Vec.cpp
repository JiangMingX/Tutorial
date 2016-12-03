#include "Vec.h"
template <typename T>
pair<T*, T*> Vec<T>::alloc_n_copy(const T*, const T*)
{
	auto data = alloc.allocate(end - begin); 
	return{ data, uninitializer_copy(begin,end,data) };
}
template <typename T>
Vec<T>::Vec(initializer_list<T> il)
{
	auto newdata = alloc_n_copy(il.begin(), il.end());
	elemnts = newdata.first;
	first_free = cap = newdata.second;
}
template <typename T>
Vec<T>::Vec(const Vec<T> &v)
{
	auto newdata = alloc_n_copy(v.begin(), v.end());
	elements = newdata.first;
	first_free = cap = newdata.second;
}
template <typename T>
Vec<T>::Vec(Vec<T> &&v)throw() :elements(v.elements), first_free(v.first_free), cap(v.cap)
{
	v.elements = v.first_free = v.cap = nullptr;
}
