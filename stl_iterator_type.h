#pragma once
#pragma execution_character_set("utf-8")
#ifndef ptrdiff_t
#define ptrdiff_t __int64 
#endif
namespace stl{
	struct stl_input_iterator_tag{};//���������
	struct stl_output_iterator_tag{};//���������
	struct stl_forward_iterator_tag :public stl_input_iterator_tag{};//�ɶ�д������
	struct stl_bidirectional_iterator_tag :public stl_forward_iterator_tag{};//˫�������
	struct stl_random_access_iterator_tag :public stl_bidirectional_iterator_tag{};//���������


	//�ͱ�װ��,�������Լ̳д���
	template<typename Category, typename T, typename Distance = ptrdiff_t,
		typename Pointer = T*, typename Reference = T&>
	struct iterator
	{
		typedef Category iterator_category;//����������
		typedef T value_type;//��������ָ��������
		typedef Distance difference_type;//������֮���������
		typedef Pointer pointer;//��������ָ�����ָ���ͱ�
		typedef Reference reference;//��������ָ����������ͱ�
	};
	//�ͱ���ȡ��,�����ͱ��ɵ�������������
	template<typename Iterator>//ģ�����Ӧ��Ϊ������
	struct stl_iterator_traits
	{
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::Distance difference_type;
		typedef typename Iterator::Pointer pointer;
		typedef typename Iterator::Reference reference;

	};

	template<typename T>
	typename stl_iterator_traits<T>::iterator_category
		iterator_ceategory(const T&)
	{
		typedef typename stl_iterator_traits<T>::iterator_category category;
		return category();
	}

	//������ԭ��ָ������
	template<typename T>
	struct stl_iterator_traits < T* >
	{
		typedef stl_random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef T* pointer;
		typedef ptrdiff_t difference_type;
		typedef T& reference;
	};

	template<typename T>
	struct stl_iterator_traits < const T* >
	{
		typedef stl_random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef const T* pointer;
		typedef ptrdiff_t difference_type;
		typedef const T& reference;
	};
}