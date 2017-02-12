#pragma once
#pragma execution_character_set("utf-8")
#ifndef _STL_TEMPLATE_NULL
#define _STL_TEMPLATE_NULL template<>
#endif

namespace stl{
	struct _true_type{};
	struct _false_type{};
	template<typename T>
	struct stl_type_traits
	{

		typedef _false_type has_trivial_default_constructor;//Ĭ�Ϲ��캯���Ƿ������壿
		typedef _false_type has_trivial_copy_constructor;//�������캯���Ƿ�������?
		typedef _false_type has_trivial_assignment_operator;//������ֵ�����Ƿ�������?
		typedef _false_type has_trivial_destructor;//���������Ƿ�������?

		/*POD��ָPlain Old Data,Ҳ���Ǳ����ͱ��ͳ��C struct(��ͳ��C structֻ��
		�������ݳ�Ա�����ܰ���������Ա��Ҳ������ν�ľۺ��ࡣPOD�ͱ��Ȼ����������
		��ctor/dtor/copy/assignment������
		*/

		typedef _false_type is_POD_type;//�Ƿ�ΪPlain Old Data?
	};

	_STL_TEMPLATE_NULL
	struct stl_type_traits < char >
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	_STL_TEMPLATE_NULL
	struct stl_type_traits < unsigned char >
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	_STL_TEMPLATE_NULL
	struct stl_type_traits < short >
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	_STL_TEMPLATE_NULL
	struct stl_type_traits < unsigned short >
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	_STL_TEMPLATE_NULL
	struct stl_type_traits < int >
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	_STL_TEMPLATE_NULL
	struct stl_type_traits < unsigned int >
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};


	_STL_TEMPLATE_NULL
	struct stl_type_traits < long >
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	_STL_TEMPLATE_NULL
	struct stl_type_traits < float >
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	_STL_TEMPLATE_NULL
	struct stl_type_traits < double >
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	_STL_TEMPLATE_NULL
	struct stl_type_traits < long double >
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	template<typename T>
	struct stl_type_traits < T* >
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
}