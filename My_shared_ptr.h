#pragma once
#include<iostream>
#include<set>
#include<map>
#include<string>
#include<vector>
#include<algorithm>
#include<utility>
#include<functional>
using namespace std;
class DebugDelete
{
public:
	DebugDelete(ostream &s = cerr) :os(s){}
	template <typename T>
	void operator()(T *p)const
	{
		cout << "deleting!" << endl;
		delete p;
	}
private:
	ostream &os;
};
template <typename T> class My_shared_ptr;
template <typename T> void swap(My_shared_ptr<T>&, My_shared_ptr<T>&);
template <typename T, typename...Args> My_shared_ptr<T>make_shared1(Args&&...args);
template <typename T>
class My_shared_ptr
{
	friend void swap<T>(My_shared_ptr<T>&, My_shared_ptr<T>&);
public:
	My_shared_ptr() :p(nullptr), use(new size_t(1)), fun(DebugDelete()){};
	My_shared_ptr(My_shared_ptr<T>& q, function<void(T*)>f(DebugDelete())) :p(q.p), use(q.use), fun(f){ ++*use; }
	My_shared_ptr(T *q, function<void(T*)>f(DebugDelete())) :p(q), use(new size_t(1)), fun(f){}
	My_shared_ptr(const My_shared_ptr<T>&);
	My_shared_ptr(T *q) :p(q), use(new size_t(1)){};
	const My_shared_ptr<T>& operator=(My_shared_ptr<T>&);
	T& operator*()const;
	T* operator->()const;
	operator bool()const{
		if (p == nullptr)
			return 0;
		else
			return 1;
	};
	T* get()const;
	void swap(My_shared_ptr<T>&);
	void reset(T *, function<void(T*)>);
	size_t use_count()const
	{
		return *use;
	}
	bool unique()const
	{
		return use_count() == 1 ? true : false;
	}
	~My_shared_ptr()
	{
		if (--*use == 0)
		{
			delete p;
			delete use;
		}
	}
private:
	T *p;
	size_t *use = 0;
	function<void(T*)>fun;
};
template <typename T>
My_shared_ptr <T> ::My_shared_ptr(const My_shared_ptr<T>&rhs)
{
	p = rhs.p;
	use = rhs.use;
	fun = rhs.fun;
	++*use;
}
template <typename T>
void swap(My_shared_ptr<T>&lhs, My_shared_ptr<T>&rhs)
{
	using std::swap;
	swap(lhs.p, rhs.p);
	swap(lhs.use, rhs.use);
	swap(lhs.fun.rhs.fun);
}
template <typename T, typename...Args>
My_shared_ptr<T> make_shared1(Args&&...args)
{
	My_shared_ptr<T> p(new T(std::forward<Args>(args)...));
	return p;
}