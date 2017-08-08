#ifndef TEST_FILE
#define TEST_FILE
#include <string>
using std::string;
using std::wstring;
#include <iostream>
#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ostream;
using std::fstream;
using std::ios_base;
using std::ios;
#include <windows.h>
#include <stdlib.h>
#include <vector>
using std::vector;

#define NEED_DELETE 

//��·���еķ�б���滻Ϊб��. \\ -> /
string formatPath(const string& path);

//��·���е�б���滻Ϊ��б��. / --> \\ 
string formatInWindowsPath(const string& path);

//�����ļ�
int copyFile(const string& source,const string& target);

//�ݹ鴴���ļ���
void createDirRecursion(const string& dir);

//�Ƿ����ļ�
bool isFile(const string& path);

//�Ƿ���Ŀ¼
bool isDirectory(const string& path);

//�����·����þ���·��. ��Ӧ windows
bool fullPath(const string &path,string &full);

//�����·����þ���·��. ��ϵͳ�޹�
bool fullPath_alt(const string &thePath,string &full);

//�����һ��Ŀ¼
bool getParentDir(const string& path,string &dir);


void intsToFile(int *ints,size_t len,const string &fileName);
void wcharsToFile(wchar_t *chars,size_t len,const string &fileName);

/* 
����ı��ֽ����ı���
0 : ���ܵ�֪
1 : Unicode little endian
2 : Unicode big endian
3 : UTF-8
*/
char getFileTextEncoding(const char* fileText,size_t bufLen,size_t *realNumLen);

//�� Unicode ����д���ļ�, ʹ�� UTF8 ����
void writeFileUtf8(ostream *file,wchar_t *theChs,size_t len);

//·�� path ���һ��б�ܻ�б�ܵ�λ��
template<typename CHAR_TYPE>
int lastSlashPos(const CHAR_TYPE* path);

//��õ�ǰִ�г����Ŀ¼. ������һ�����. ���ص��ַ�����Ҫ����
const char* NEED_DELETE getCurrentModulePathWithSlash();

//�ļ��Ƿ����. 0:������, 1:����
int fileExists(const char* filename);

//��ȡ�ļ����ֽ���
const char* NEED_DELETE readFileBytes(const char*fileName,size_t &length);

void listFiles(const string &dir, vector<string>& filePaths );

#endif