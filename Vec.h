#pragma once
#include<iostream>
#include<set>
#include<memory>
#include<map>
#include<string>
#include<vector>
#include<algorithm>
#include<utility>
using namespace std;


template <typename T> class Vec;
template <typename T> bool operator==(const Vec<T>&, const Vec<T>&);
template <typename T> bool operator!=(const Vec<T>&, const Vec<T>&);
template <typename T> bool operator<(const Vec<T>&, const Vec<T>&);


template <typename T>
class Vec
{
	typedef size_t size_type;
	friend bool operator==(const Vec<T>&, const Vec<T>&);
	friend bool operator!=(const Vec<T>&, const Vec<T>&);
	friend bool operator<(const Vec<T>&, const Vec<T>&);
public:
	Vec() :elements(nullptr), first_free(nullptr), cap(nullptr){}
	Vec(initializer_list<T>);
	Vec(const Vec<T>&);
	Vec(Vec<T> &&)throw();
	Vec<T>& operator=(const Vec<T> &);
	Vec<T>& operator=(Vec<T>&&);
	T& operator[](size_t);
	const T& operator[](size_t)const;
	~Vec();
	void push_back(const T&);
	void pop_back(const T&);
	size_t capacity(){ return cap - elements; }
	size_t size(){ return firsr_free; }
	T* begin(){ return elements; }
	T* end(){ return first_free; }
	void reserve(size_t);
private:
	allocator<T>alloc;
	void chk_n_alloc()
	{
		if (size() == capacity())reallocate();
	}
	T* elements; //元素头部
	T*first_free;  //元素尾部
	T*cap;    //空间尾部
	pair<T*, T*>alloc_n_copy(const T*, const T*);
	void free();
	void reallocate();
};
