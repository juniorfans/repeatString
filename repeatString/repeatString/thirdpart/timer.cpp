#include "log.h"
#include "timer.h"
#include "random.h"

class KLog;
extern KLog klog;

void timeRun(TIMER_CALL_BACK_PROC proc, void *lparam, int intervalSecs)
{
	char timerName[17] = {0};
	randomStr(timerName,16);
	HANDLE keepTimer = CreateWaitableTimerExA(NULL,timerName,CREATE_WAITABLE_TIMER_MANUAL_RESET,EVENT_ALL_ACCESS);
	if(NULL == keepTimer)
	{
		LOG_ERROR("create timer error. ", GetLastError());
		return ;
	}
	if(ERROR_ALREADY_EXISTS == GetLastError())
	{
		LOG_INFO("timer already exists.");
	}
	LARGE_INTEGER liDueTime;//�Ӻ�ʱ��ʼ���ź�
	liDueTime.QuadPart = ((LONGLONG)-10000000) * intervalSecs;	//���ʱ�䣬��ʲôʱ��֮�󴥷�. 100 ����Ϊ��
	//�ص�����Ϊ�գ�����ʹ�� waitForSingleObject ȥ�ȴ�����ص������ᱻ����. ���һ������ָʾ���Ƿ�ϵͳ�ӵ�Դ����ģʽ�лָ�����

	while (true)
	{
		if(0 == SetWaitableTimer(keepTimer,&liDueTime,0,NULL,NULL,FALSE))
		{
			LOG_ERROR("set timer error. ", GetLastError());
			return ;
		}

		if(WAIT_OBJECT_0 != WaitForSingleObject(keepTimer,INFINITE))
		{
			LOG_ERROR("wait timer error. ", GetLastError());
			return ;
		}
		try
		{
			proc(lparam);
		}
		catch (... )
		{
			LOG_ERROR("timer proc exception.", 0);
		}

	}
}
