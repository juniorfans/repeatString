#ifndef TEST_PROCESS_THREAD
#define TEST_PROCESS_THREAD
#include <windows.h>
#include <set>
using std::set;

class ProcessInfo 
{
public:
	ProcessInfo(DWORD pid, DWORD moduleId, DWORD parentPId, TCHAR* path);
	DWORD _pid;
	DWORD _moduleId;
	DWORD _parentPId;
	TCHAR _path [MAX_PATH];

	bool operator < (const ProcessInfo &pi) const
	{
		return this->_pid < pi._pid;
	}
};

//��õ�ǰϵͳ�ڴ�ռ��
SIZE_T getCurrentMemoryUse(SIZE_T *peakMemory);

//���Ե�һ�����г���. ��֮ǰû�������������ҷ��� 0. ��������,����1
int tryFirstRunModule();

//���ݽ��� id kill ����. ���� TRUE ��ʾ kill �ɹ�����ʧ��
BOOL KillProcess(DWORD dwPid, DWORD waitKillMs)  ;

//�������̽���, ���� TRUE �ɹ�����ʧ��
BOOL EnableDebugPrevilige(BOOL   fEnable) ;


/************************************************************************/
/*  
���ݽ����� kill ����.
���� 2 ��ʾû�ҵ�����
���� 1 ��ʾ�رս���ʧ��
���� 0 ��ʾ�رճɹ�
/************************************************************************/
int KillProcessByName(const TCHAR *lpszProcessName) ;

//�������ֲ��ҽ���
BOOL findProcess(const TCHAR *lpszProcessName) ;

//���ݽ�������ý��� id
bool getProcessInfoByName(const TCHAR *lpszProcessName, set<DWORD>& infos );

bool getProcessInfoByNameEx(const TCHAR *lpszProcessName, set<ProcessInfo>& infos);

bool startProcess(const TCHAR *exeFile, const TCHAR *cmdLine, const TCHAR *workDirectory);

bool startProcessSilent(const TCHAR *exeFile, const TCHAR *cmdLine, const TCHAR *workDirectory);

#endif