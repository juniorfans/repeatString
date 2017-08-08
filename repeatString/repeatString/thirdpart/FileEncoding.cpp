#include <windows.h>
#include "FileEncoding.h"


//�� utf8 ����Ķ��ֽ�תΪ���ֽ�. char -> Unicode
wchar_t* utf8MultCharSetToWide(const char * buf,size_t bufLen,size_t *realNumLen)
{
	//���ֽ��ַ���ת��Ϊ���ַ�������һ��������ʾԴ�ַ����Ĵ���ҳ
	size_t nMBLen = MultiByteToWideChar(CP_UTF8,0,buf,bufLen,NULL,NULL);
	if(0 == nMBLen)
	{
		int ne = GetLastError();
		return NULL;
	}
	wchar_t* szWcsBuffer = (wchar_t*)malloc(sizeof(wchar_t) * (nMBLen + 1));

	memset(szWcsBuffer,0,sizeof(wchar_t)*(nMBLen+1));
	MultiByteToWideChar(CP_UTF8,0,buf,bufLen, szWcsBuffer, nMBLen);
	*realNumLen = nMBLen;
	return szWcsBuffer;
}

wchar_t* defaultMultCharSetToWide(const char * buf,size_t bufLen,size_t *realNumLen)
{
	//setlocale(LC_ALL,"chs"); //ȷ�������ϵ����������� chs. �й�
	size_t nMBLen = MultiByteToWideChar(CP_ACP,0,buf,bufLen,NULL,NULL);
	if(0 == nMBLen)
	{
		int ne = GetLastError();
		return NULL;
	}
	wchar_t* szWcsBuffer = (wchar_t*)malloc(sizeof(wchar_t) * (nMBLen + 1));

	memset(szWcsBuffer,0,sizeof(wchar_t)*(nMBLen+1));
	MultiByteToWideChar(CP_ACP,0,buf,bufLen, szWcsBuffer, nMBLen);
	*realNumLen = nMBLen;
	return szWcsBuffer;
}

//���ֽ�תΪ���ֽ�. Unicode -> char
char* wideCharSetToUtf8Mult(const wchar_t * buf,size_t bufLen,size_t *realNumLen)
{
	//setlocale(LC_ALL,"chs");
	//���ֽ��ַ���ת��Ϊ���ַ�������һ��������ʾĿ���ַ����Ĵ���ҳ
	size_t nMBLen = WideCharToMultiByte(CP_UTF8,0,buf,bufLen,NULL,0,NULL,NULL);
	if(0 == nMBLen)
	{
		int ne = GetLastError();
		return NULL;
	}
	char* charBuffer = (char*)malloc(sizeof(char) * (nMBLen + 1));

	memset(charBuffer,0,sizeof(char)*(nMBLen+1));
	WideCharToMultiByte(CP_UTF8,0,buf,bufLen, charBuffer, nMBLen,NULL,NULL);
	*realNumLen = nMBLen;
	return charBuffer;
}

//���ֽ�תΪ���ֽ�. Unicode -> char
char* wideCharSetToDefaultMult(const wchar_t * buf,size_t bufLen,size_t *realNumLen)
{
	//setlocale(LC_ALL,"chs");
	//���ֽ��ַ���ת��Ϊ���ַ�������һ��������ʾĿ���ַ����Ĵ���ҳ
	size_t nMBLen = WideCharToMultiByte(CP_ACP,0,buf,bufLen,NULL,0,NULL,NULL);
	if(0 == nMBLen)
	{
		int ne = GetLastError();
		return NULL;
	}
	char* charBuffer = (char*)malloc(sizeof(char) * (nMBLen + 1));

	memset(charBuffer,0,sizeof(char)*(nMBLen+1));
	WideCharToMultiByte(CP_ACP,0,buf,bufLen, charBuffer, nMBLen,NULL,NULL);
	*realNumLen = nMBLen;
	return charBuffer;
}

//���ֽ�תΪ���ֽ�. char -> Unicode
wchar_t* multCharSetToWide(const char * buf,size_t bufLen,DWORD codeOfBuf,size_t *realNumLen)
{	
	//���ֽ��ַ���ת��Ϊ���ַ�������һ��������ʾԴ�ַ����Ĵ���ҳ
	size_t nMBLen = MultiByteToWideChar(codeOfBuf,0,buf,bufLen,NULL,NULL);
	if(0 == nMBLen)
	{
		int ne = GetLastError();
		return NULL;
	}
	wchar_t* szWcsBuffer = (wchar_t*)malloc(sizeof(wchar_t) * (nMBLen + 1));

	memset(szWcsBuffer,0,sizeof(wchar_t)*(nMBLen+1));
	MultiByteToWideChar(codeOfBuf,0,buf,bufLen, szWcsBuffer, nMBLen);
	*realNumLen = nMBLen;
	return szWcsBuffer;
}

//���ֽ�תΪ���ֽ�. Unicode -> char
char* wideCharSetToMult(const wchar_t * buf,size_t bufLen, DWORD codeOfTarget,size_t *realNumLen)
{
	//���ֽ��ַ���ת��Ϊ���ַ�������һ��������ʾĿ���ַ����Ĵ���ҳ
	size_t nMBLen = WideCharToMultiByte(codeOfTarget,0,buf,bufLen,NULL,0,NULL,NULL);
	if(0 == nMBLen)
	{
		int ne = GetLastError();
		return NULL;
	}
	char* charBuffer = (char*)malloc(sizeof(char) * (nMBLen + 1));

	memset(charBuffer,0,sizeof(char)*(nMBLen+1));
	WideCharToMultiByte(codeOfTarget,0,buf,bufLen, charBuffer, nMBLen,NULL,NULL);
	*realNumLen = nMBLen;
	return charBuffer;
}



DWORD getSystemCodePage()
{
	WCHAR pwCodePage[16] = {0};  
	GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_IDEFAULTCODEPAGE, pwCodePage, sizeof(pwCodePage));  

	return  _wtoi(pwCodePage);
}