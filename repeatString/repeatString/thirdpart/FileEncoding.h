#pragma once


class FileEncoding
{
private:
	inline bool strEqual(const char *l,const char *r)
	{
		if(NULL == l && NULL == r)	return false;
		for (;0 != *l && 0 != *r && *l == * r;++ l,++ r)
		{

		}
		return *l == *r && 0 == *l;
	}
	static const int MAX_CHARSET_NAME = 16;
	char charSet[MAX_CHARSET_NAME];
public:
	FileEncoding(void);
	~FileEncoding(void);
	const char * getCharSet(const char *buf,size_t bufLen);

	int getCharSetPageCode(const char *buf,size_t bufLen);
};

//���ֽ�תΪ���ֽڣ����Ը����ֽ����²� code page. char -> Unicode
wchar_t* multCharSetToWideWithGuessCodePage(const char * buf,size_t bufLen,size_t *realNumLen);

//utf ���ֽ�תΪ���ֽ�. char -> Unicode
wchar_t* utf8MultCharSetToWide(const char * buf,size_t bufLen,size_t *realNumLen);

//default(�������أ����й������� gbk) ����Ķ��ֽ�תΪ���ֽ�. char -> Unicode
wchar_t* defaultMultCharSetToWide(const char * buf,size_t bufLen,size_t *realNumLen);

//���ֽ�תΪ UTF8 ���ֽ�. Unicode -> char
char* wideCharSetToUtf8Mult(const wchar_t * buf,size_t bufLen,size_t *realNumLen);

//���ֽ�תΪ ϵͳĬ�ϵ�(���й������� gbk) ���ֽ�. Unicode -> char
char* wideCharSetToDefaultMult(const wchar_t * buf,size_t bufLen,size_t *realNumLen);

//���ֽ�תΪ���ֽ�. char -> Unicode
wchar_t* multCharSetToWide(const char * buf,size_t bufLen,DWORD codeOfBuf,size_t *realNumLen);

//���ֽ�תΪ���ֽ�. Unicode -> char
char* wideCharSetToMult(const wchar_t * buf,size_t bufLen, DWORD codeOfTarget,size_t *realNumLen);

//���ϵͳ��ǰ codePage
DWORD getSystemCodePage();