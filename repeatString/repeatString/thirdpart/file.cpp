#include "file.h"
#include "kmp.h"
#include <assert.h>
#include <direct.h>
#include <windows.h>
#include "string.h"
#include <io.h>


void intsToFile(int *ints,size_t len,const string &fileName)
{
	ofstream file(fileName.c_str());
	if(file.is_open())
	{
		for(size_t i = 0;i < len;++ i)
		{
			file << ints[i] << '\n';
		}
		file.close();
	}
}

void wcharsToFile(wchar_t *chars,size_t len,const string &fileName)
{
	ofstream file(fileName.c_str());
	if(file.is_open())
	{
		for(size_t i = 0;i < len;++ i)
		{
			file << chars[i] << '\n';
		}
		file.close();
	}
}



/************************************************************************/
/* 
ͨ���ı�����ħ���ж��ı�����
0 : ���ܵ�֪
1 : Unicode little endian
2 : Unicode big endian
3 : UTF-8
ANSI������������������	�޸�ʽ���壻 
Unicode(UCS-2) little endian�� ǰ�����ֽ�ΪFFFE�� 
Unicode(UCS-2) big endian����	ǰ���ֽ�ΪFEFF���� 
UTF-8 �� Bom���� ��������ǰ���ֽ�ΪEFBB��
Unicode(UCS-4) little endian�� ǰ�ĸ��ֽ�ΪFFFE0000��
Unicode(UCS-4) big endian����	ǰ�ĸ��ֽ�Ϊ0000FEFF��

��ˣ��ߵ�ַ�д�ŵ�λ���ݣ�С�ˣ��ߵ�ַ��Ÿ�λ���ݡ�
0x1234 
�ڴ��ַ		0x40000		0x4001		
���			0x12		0x34
С��			0x34		0x12
ע�⣬0x1234 �е� 34 �ǵ�λ��12 �Ǹ�λ
*/
/************************************************************************/
char getFileTextEncoding(const char* fileText,size_t bufLen,size_t *realNumLen)
{
	if(bufLen < 2)
		return 0;
	unsigned char first = fileText[0],second = fileText[1];
	if(first == 0xff && second == 0xfe)
		return 1;
	else if(first == 0xfe && second == 0xff)
		return 2;
	else if(first == 0xef && second == 0xbb)
		return 3;
	else
	{
		//��ʱ��֧�� UCS-4
	}
	return 0;
}

//�� Unicode ����д���ļ�, ʹ�� UTF8 ����
void writeFileUtf8(ostream *file,wchar_t *theChs,size_t len)
{
	//��һ��������ʾĿ���ַ����Ĵ���ҳ
	//�������ֻӰ������ļ��ı��룬�������Ϊ 0 ��ʹ��ϵͳĬ�ϵĴ���ҳ���� asscii ���롣���ʹ�� UTF-8����������ļ�����Ϊ UTF-8
	size_t nMBLen = WideCharToMultiByte(CP_UTF8, 0,theChs, len, NULL, 0, NULL, NULL);
	char* pBuffer = new char[nMBLen+1];
	pBuffer[nMBLen] = 0;
	WideCharToMultiByte(CP_UTF8, 0,theChs, len, pBuffer, nMBLen+1, NULL, NULL);
	if(NULL != file)	*file << pBuffer;
	delete[] pBuffer;
}

//·�� path ���һ��б�ܻ�б�ܵ�λ��
template<typename CHAR_TYPE>
int lastSlashPos(const CHAR_TYPE* path)
{
	int lastSlashPos = 0;
	int i = 0;
	CHAR_TYPE c = 0;
	while (0 != (c = *(path + i)))
	{
		if ('/' == c || '\\' == c)
		{
			lastSlashPos = i;
		}
		++ i;
	}
	return lastSlashPos;
}


//��õ�ǰִ�г����Ŀ¼. ������һ�����. ���ص��ַ�����Ҫ����
const char* NEED_DELETE getCurrentModulePathWithSlash()
{
	char moduleFile[256]  = {0};
	GetModuleFileNameA(NULL,moduleFile,255);
	int length = lastSlashPos(moduleFile) + 1;

	char *ret = new char[length + 1];
	//memset(ret,0,(length + 1) * sizeof(char));
	ret[length] = 0;
	memcpy_s(ret,length,moduleFile,length);
	return ret;
}

//�ļ��Ƿ����. 0:������, 1:����
int fileExists(const char* filename)
{
	fstream file;
	file.open(filename,ios::in);
	if(!file)	//�� file.failed() �ȼ�
	{
		return 0;
	}
	else
	{
		file.close();
		return 1;
	}
}



//�����·����þ���·��. ��ϵͳ�޹�
bool fullPath_alt(const string &thePath,string &full)
{
	string path = formatPath(thePath);

	if(-1 != path.find_first_of(':'))	//�Ѿ��Ǿ���·��
	{
		full = string(path);
	}

	if(0 == KMP("./").indexOf(path.c_str()))	//��ǰĿ¼
	{
		path = path.substr(2,path.length() - 2);
	}

	char pBuf[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH,pBuf);
	string curDir = formatPath(pBuf);
	if ('/' != curDir[curDir.length()-1])
	{
		curDir += "/";
	}
	printf("work dir : %s\r\n",curDir.c_str());
	int upLevel = 0;
	int pos = 0;
	KMP kmp("../");

	while (string::npos != (pos = kmp.indexOf(path.c_str(),pos)))
	{
		pos += 3;
		++ upLevel;
	}
	if(0 == upLevel)	//��ǰĿ¼��
	{
		full = curDir + path;
		return true;
	}
	else	//�� upLevel ��Ŀ¼
	{

		pos = kmp.indexOf(path.c_str());	//pos ָ�� ../ �е� /
		string coreDir;
		if(pos == path.length()-1)
			coreDir = "";
		else
			coreDir = path.substr(pos + 1,path.length() - pos + 1);
		string parentDir(curDir);
		for (int i = 0;i < upLevel;++ i)
		{
			if(!getParentDir(parentDir,parentDir))
			{
				return false;
			}
		}
		if ('/' != parentDir[parentDir.length()-1])
		{
			parentDir += "/";
		}
		full = parentDir + coreDir;
		return true;
	}
}

//�����·����þ���·��. ��Ӧ windows
bool fullPath(const string &thePath,string &full)
{
	char theDir[MAX_PATH] = {0};
	char theFull[MAX_PATH] = {0};
	char *namePos = NULL;
	memcpy_s(theDir,MAX_PATH,thePath.c_str(),thePath.length());
	if(::GetFullPathNameA(theDir,MAX_PATH,theFull,&namePos) == 0)
	{
		int errorCode = GetLastError();
		printf("getLastError : %d",errorCode);
		return false;
	}
	else
	{
		full = formatPath(theFull);
		return true;
	}
}


string formatPath(const string& path)
{
	char tmp[MAX_PATH];

	int len = path.length();

	if (0 == len) {
		return "";
	}


	memset(tmp, '\0', sizeof(tmp));
	memcpy_s(tmp,len,path.c_str(),len);

	for (int i = 0;i < len;++ i)
	{
		if(tmp[i] == '\\')
			tmp[i] = '/';
	}

	return string(tmp);
}

string formatInWindowsPath(const string& path)
{
	char tmp[MAX_PATH];

	int len = path.length();

	if (0 == len) {
		return "";
	}


	memset(tmp, '\0', sizeof(tmp));
	memcpy_s(tmp,len,path.c_str(),len);

	for (int i = 0;i < len;++ i)
	{
		if(tmp[i] == '/')
			tmp[i] = '\\';
	}

	return string(tmp);
}


bool isDirectory(const string& path)
{
	WIN32_FIND_DATAA fd;  
	bool ret = false;  
	HANDLE hFind = FindFirstFileA(path.c_str(), &fd);
	if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))  
	{  
		ret = TRUE;  
	}  
	FindClose(hFind);  
	return ret;
}

bool isFile(const string& path)
{
	WIN32_FIND_DATAA fd;  
	bool ret = false;  
	HANDLE hFind = FindFirstFileA(path.c_str(), &fd);
	if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & 
		(FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM)))
	{  
		ret = TRUE;  
	}  
	FindClose(hFind);  
	return ret;
}

bool DirectoryExist(const string& path)  
{  
	return isDirectory(path);
} 

void mkdirs(const char *dir)
{
	if(DirectoryExist(string(dir)))
		return;
	char tmp[1024];
	char *p;

	if (strlen(dir) == 0 || dir == NULL) {
		printf("strlen(dir) is 0 or dir is NULL.\n");
		return;
	}

	int len = strlen(dir);
	memset(tmp, '\0', sizeof(tmp));
	memcpy_s(tmp,len,dir,len);

	if (tmp[0] == '/') 
		p = strchr(tmp + 1, '/');
	else 
		p = strchr(tmp, '/');

	if (p) 
	{
		*p = '\0';
		_mkdir(tmp);
		_chdir(tmp);
	} else 
	{
		_mkdir(tmp);
		_chdir(tmp);
		return;
	}

	mkdirs(p + 1);
}

int copyFile(const string& source,const string& target)
{
	FILE *sfile = NULL;
	fopen_s(&sfile,source.c_str(),"rb");
	if(NULL == sfile)
	{
		return 1;
	}
	fseek(sfile,0,SEEK_END);
	long sfileSize = ftell(sfile);
	fseek(sfile,0,SEEK_SET);

	FILE* tfile = NULL;
	fopen_s(&tfile,target.c_str(),"w+b");
	if(NULL == tfile)
	{
		return 2;
	}
	char buff[1024];

	size_t total = 0;
	size_t rc = 0;
	while (0 != (rc = fread_s(buff,1024,1,1024,sfile)))
	{
		total += rc;
		size_t wc = fwrite(buff,1,rc,tfile);
		assert(rc == wc);
	}
	fclose(sfile);
	fclose(tfile);
	assert(total == sfileSize);
	return 0;
}

void createDirRecursion(const string& dir)
{
	mkdirs(dir.c_str());
}

bool getParentDir(const string& thePath,string &dir)
{
	string path;
	if(!fullPath(thePath,path))
	{
		return false;
	}

	int pos = path.find_last_of('/');
	if(-1 == pos)
		return false;
	else
	{
		if(pos == path.length()-1)
			pos = path.substr(0,path.length()-2).find_last_of('/');
		if(-1 == pos)
			return false;
		dir = path.substr(0,pos);	//0 ~ pos-1
		return true;
	}
}

//��ȡ�ļ����ֽ���
const char* NEED_DELETE readFileBytes(const char*fileName,size_t &length)
{
	ifstream file(fileName);
	if(!file)
	{
		length = 0;
		return NULL;
	}
	file.seekg(0,std::ios::end);
	length = file.tellg();
	file.seekg(0,std::ios::beg);
	char *ret = new char[length];
	file.read(ret,length);
	file.close();
	return ret;
}


void listFiles(const string &dir, vector<string>& filePaths )
{
	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst((dir+"\\*.*").c_str(), &findData);
	if (handle == -1)        // ����Ƿ�ɹ�
		return;

	do
	{
		if (findData.attrib & _A_SUBDIR)
		{
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;

			listFiles(dir+"\\"+findData.name,filePaths);
		}
		else
		{
			filePaths.push_back(dir + "\\" + findData.name);
		}
	} while (_findnext(handle, &findData) == 0);

	_findclose(handle);    // �ر��������
}