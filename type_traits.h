#pragma once
#pragma execution_character_set("utf-8")
#ifndef _STL_TEMPLATE_NULL
#define _STL_TEMPLATE_NULL template<>
#endif

struct _true_type{};
struct _false_type{};
template<typename T>
struct type_traits
{
	
	typedef _false_type has_trivial_default_constructor;//Ĭ�Ϲ��캯���Ƿ������壿
	typedef _false_type has_trivial_copy_constructor;//�������캯���Ƿ�������?
	typedef _false_type has_trivial_assgignment_constructor;//������ֵ�����Ƿ�������?
	typedef _false_type has_trivial_destructor;//���������Ƿ�����?
	typedef _false_type is_POD_type;//�Ƿ�ΪPlain Old Data?
};
_STL_TEMPLATE_NULL
struct type_traits <char>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assgignment_constructor;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};

_STL_TEMPLATE_NULL
struct type_traits <unsigned char>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assgignment_constructor;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
_STL_TEMPLATE_NULL
struct type_traits <short>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assgignment_constructor;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
_STL_TEMPLATE_NULL
struct type_traits <unsigned short>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assgignment_constructor;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
_STL_TEMPLATE_NULL
struct type_traits <int>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assgignment_constructor;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
_STL_TEMPLATE_NULL
struct type_traits <unsigned int>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assgignment_constructor;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
_STL_TEMPLATE_NULL
struct type_traits <long>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assgignment_constructor;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
_STL_TEMPLATE_NULL
struct type_traits <float>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assgignment_constructor;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
_STL_TEMPLATE_NULL
struct type_traits <double>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assgignment_constructor;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
_STL_TEMPLATE_NULL
struct type_traits <long double>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assgignment_constructor;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
template<typename T>
struct type_traits <T*>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assgignment_constructor;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};