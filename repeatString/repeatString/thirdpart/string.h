#ifndef TEST_STRING
#define TEST_STRING
#include <string>
using std::string;
#include "kmp.h"
#include "encoding.h"
#include "math.h"
#include  "pthread.h"

//�ַ�������
template<typename CHAR_TYPE>
size_t tstrLen(const CHAR_TYPE *str);

//���ַ���src��lenλ�ÿ�ʼ׷���Ӵ� [0,src+len-1].
//��������Ҫ���� src �ڴ�ռ�
template<typename CHAR_TYPE>
void selfAppend(CHAR_TYPE *src,size_t len);

//��ӡ����
template<typename E>
void printfArray(E *array, size_t len);

/************************************************************************/
/* 
�����ַ����Ƿ��ǰ׺��ϵ��
0 �� ������
1 �� ��һ���ǵڶ�����ǰ׺
2 �� �ڶ����ǵ�һ����ǰ׺
�������ַ���һ��������Ϊǰһ���Ǻ�һ����ǰ׺
*/
/************************************************************************/
template<typename CHAR_TYPE>
char prefixRelation(const CHAR_TYPE* firstStr,const CHAR_TYPE* secondStr);

//��дתСд
void upToLowCase(char *buf,size_t bufLen);

//�����ֽ�����ǰ�����ֽڲ²��ֽ����ı���
char simpleGetTextEncoding(const char* buf,size_t bufLen,size_t *realNumLen);

//�ַ������
template<typename CHAR_TYPE>
bool strEqual(const CHAR_TYPE *l,const CHAR_TYPE *r);

//һ�����ֽ�תΪ 16 �����ַ���
template<typename WIDE_BYTE_TYPE>
std::string wideByteToHexString(WIDE_BYTE_TYPE multyByte);

template<typename WIDE_BYTE_TYPE>
std::string wideByteToHexStringEx(WIDE_BYTE_TYPE multyByte);




//�ַ�������
template<typename CHAR_TYPE>
size_t tstrLen(const CHAR_TYPE *str)
{
	if(NULL == str)
		return 0;
	size_t len = 0;
	while (*str)
	{
		++ str;
		++ len;
	}
	return len;
}


//���ַ���src��lenλ�ÿ�ʼ׷���Ӵ� [0,src+len-1].
//��������Ҫ���� src �ڴ�ռ�
template<typename CHAR_TYPE>
void selfAppend(CHAR_TYPE *src,size_t len)
{
	memcpy_s(src+len,sizeof(CHAR_TYPE)*len,src,sizeof(CHAR_TYPE)*len);
	src[len << 1] = 0;
}

//��ӡ����
template<typename E>
void printfArray(E *array, size_t len)
{
	for (size_t i = 0;i <= len; ++ i)
	{
		std::cout << array[i] << " ";
	}
	std::cout<< std::endl;
}

/************************************************************************/
/* 
�����ַ����Ƿ��ǰ׺��ϵ��
0 �� ������
1 �� ��һ���ǵڶ�����ǰ׺
2 �� �ڶ����ǵ�һ����ǰ׺
�������ַ���һ��������Ϊǰһ���Ǻ�һ����ǰ׺
*/
/************************************************************************/
template<typename CHAR_TYPE>
char prefixRelation(const CHAR_TYPE* firstStr,const CHAR_TYPE* secondStr)
{
	if(NULL == firstStr || NULL == secondStr)
		return 0;
	for(;;++ firstStr,++secondStr)
	{
		if(0 == *firstStr)	//firstStr �����˱�ʾǰһ���Ǻ�һ����ǰ׺
			return 1;
		if(0 == *secondStr)	//prefix ��û������ secondStr �����ˣ���ʾ��һ����ǰһ����ǰ׺
			return 2;

		if(*firstStr != *secondStr)
			break;
	}
	return 0;
}



template<typename CHAR_TYPE>
bool strEqual(const CHAR_TYPE *l,const CHAR_TYPE *r)
{
	if(NULL == l && NULL == r)	return false;
	for (;0 != *l && 0 != *r && *l == * r;++ l,++ r)
	{

	}
	return *l == *r && 0 == *l;
}





//һ�����ֽ�תΪ 16 �����ַ���
template<typename WIDE_BYTE_TYPE>
std::string wideByteToHexString(WIDE_BYTE_TYPE multyByte)
{
	static std::string hexChars[16] = {"0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f"};
	size_t byteValue = (size_t)multyByte;
	std::string s("");
	for (;;)
	{
		s = hexChars[byteValue % 16] + s;
		byteValue >>= 4;
		if(0 == byteValue)
			break;
	}

	//ǰ�油��
	int width = (sizeof(WIDE_BYTE_TYPE)) * 2;
	int c0 = int(width - s.length());
	for(int i = 0;i < c0;++ i)
		s = "0" + s;
	std::string ret("");
	//ÿ 2 ��16����λ֮�䲹һ���ո�
	ret += s.substr(0,2);
	for(size_t i = 2;i < s.length();i += 2)
	{
		ret += " ";
		ret += s.substr(i,2);
	}
	return ret;
}


template<typename WIDE_BYTE_TYPE>
std::string wideByteToHexStringEx(WIDE_BYTE_TYPE multyByte)
{
	std::stringstream ioss; //�����ַ�����
	std::string s_temp; //���ת�����ַ�
	ioss << std::hex << multyByte; //��ʮ������ʽ���
	ioss >> s_temp; 
	int width = (sizeof(WIDE_BYTE_TYPE)) * 2;
	std::string s(width - s_temp.size(), '0'); //��0
	s += s_temp; //�ϲ�

	//ÿ 2 ��16����λ֮�䲹һ���ո�
	std::string ret("");
	ret += s.substr(0,2);
	for(size_t i = 2;i < s.length();i += 2)
	{
		ret += s.substr(i,2);
		ret += " ";
	}
	return ret;
}

#endif