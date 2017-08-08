#ifndef TEST_RANDOM
#define TEST_RANDOM

#include <ctime>
#include <stdlib.h>

//���һ������Ϊ strLen �����Сд��ĸ�ַ���
template<typename CHAR_TYPE>
void randomStr(CHAR_TYPE *str,size_t strLen);

//���һ���������Ϊ maxLen �����Сд��ĸ�ظ����ַ���
//����: 1.�ַ������� 2.n:�ظ����� 3.l:ѭ����Ԫ����
template<typename CHAR_TYPE>
size_t randomRepeatStr(CHAR_TYPE *str,size_t maxLen,size_t& l,size_t & n);

//���һ������Ϊ l �Ĵ��ظ� n �ε����Сд��ĸ�ַ���
//����: �ַ�������
template<typename CHAR_TYPE>
size_t randomRepeatStrLN(CHAR_TYPE *str, size_t l, size_t n);


//���һ������Ϊ strLen �����Сд��ĸ�ַ���. str Ӧ������ strLen+1 �� CHAR_TYPE �ռ�
template<typename CHAR_TYPE>
void randomStr(CHAR_TYPE *str,size_t strLen)
{
	srand((unsigned)time(NULL));  /*�������*/
	//97 ~ 122
	for (size_t i = 0;i < strLen;++ i)
	{
		*(str + i) = rand() % 26 + 97;
	}
	*(str + strLen) = char(0);
}


//���һ���������Ϊ maxLen �����Сд��ĸ�ظ����ַ���
// str Ӧ������ strLen+1 �� CHAR_TYPE �ռ�
//����: 1.�ַ������� 2.n:�ظ����� 3.l:ѭ����Ԫ����.
template<typename CHAR_TYPE>
size_t randomRepeatStr(CHAR_TYPE *str,size_t maxLen,size_t& l,size_t & n)
{
	srand((unsigned)time(NULL));  /*�������*/
	n = rand() % (maxLen/RAND_MAX) + 1;		//1 ~ maxLen/RAND_MAX
	l = rand() % RAND_MAX + 1;				//1 ~ RAND_MAX
	//	printf("len: %d, count: %d\r\n", l, n);
	assert(l * n <= maxLen);
	//97 ~ 122
	for (size_t i = 0;i < l;++ i)
	{
		*(str + i) = rand() % 26 + 97;
	}

	for (size_t i = 0;i < n;++ i)
	{
		selfAppend(str + l*i,l);
	}
	*(str + l*n) = char(0);
	return l*n;
}

//���һ������Ϊ l �Ĵ��ظ� n �ε����Сд��ĸ�ַ���
//����: �ַ�������
template<typename CHAR_TYPE>
size_t randomRepeatStrLN(CHAR_TYPE *str, size_t l, size_t n)
{
	srand((unsigned)time(NULL));  /*�������*/

	//97 ~ 122
	for (size_t i = 0;i < l;++ i)
	{
		*(str + i) = rand() % 26 + 97;
	}

	for (size_t i = 0;i < n-1;++ i)
	{
		selfAppend(str + l*i,l);
	}
	*(str + l*n) = CHAR_TYPE(0);
	return l*n;
}

#endif