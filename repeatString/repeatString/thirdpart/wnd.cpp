#include "wnd.h"
#include <windows.h>
#include <stdio.h>
#include "log.h"
#include <cassert>

HWND findWindow(HWND parentWnd,const char* name)
{
	HWND hWnd = GetWindow(parentWnd, GW_CHILD); 

	while (hWnd != NULL) 
	{    
		char lpString[32] ={0}; 
		GetWindowTextA(hWnd, lpString, sizeof(lpString)); 
		if (strstr(lpString,name)) 
		{ 
			return hWnd; 
		} 

		hWnd = GetWindow(hWnd, GW_HWNDNEXT);
	} 
	return NULL; 
}

/************************************************************************/
/* ��ǰ��Ļ�ǲ��Ǳ������� -- ��ȡ�ö��㴰��ʧ������Ϊ������Ļ��
/************************************************************************/
/*
BOOL hasScreenLocked()
{
	HWND hWnd = GetForegroundWindow();
	if(NULL == hWnd)
	{
		return TRUE;
	}
	else return FALSE;
}
*/

BOOL hasScreenLocked()
{     	// note: we can't call OpenInputDesktop directly because it's not     
	// available on win 9x     
	BOOL bLocked = FALSE;
	HDESK hDesk = ::OpenDesktopW(L"Default", 0, FALSE, DESKTOP_SWITCHDESKTOP);     

	if (hDesk)     
	{     
		bLocked = !::SwitchDesktop(hDesk);     
		// cleanup     
		::CloseDesktop(hDesk);     
	}     
		   
	return bLocked;    
} 


void sendKeysWithoutSetForeground(HWND wnd, BYTE * vks, int klen)
{
	HWND focusWnd = SetFocus(wnd); 
	if(focusWnd != wnd)
	{
		//	klog.write("send focus error. code: ");
		//	klog.write(GetLastError());
		//	klog.addLine();
	}

	INPUT *inputs = new INPUT[klen << 1];
	memset(inputs, 0, (klen<<1) * sizeof(INPUT));

	//������
	int i = 0,j = 0;
	for (;i < klen;++ i, ++ j)
	{
		//���� ���·����
		inputs[j].ki.wVk = *(vks + i);
		inputs[j].type = INPUT_KEYBOARD;
	}

	//�����ɿ�
	for (-- i;i>=0;-- i, ++ j)
	{
		//�ɿ� ���·����
		inputs[j].ki.wVk = *(vks + i);
		inputs[j].type = INPUT_KEYBOARD;
		inputs[j].ki.dwFlags = KEYEVENTF_KEYUP;
	}

	assert(j == (klen << 1));

	//�ú����ϳɼ����¼�������¼�������ģ�������߼��̲������¼����������������߼��̴����������
	SendInput(klen << 1, inputs, sizeof(INPUT));

	delete[] inputs;
}


void sendToForegroundWnd(BYTE* vks, int klen)
{
	HWND hWnd = GetForegroundWindow();
	if(NULL == hWnd)
	{
		LOG_INFO("can't execute.");
		return;
	}

	TCHAR text[256] = {0};
	int tlen = GetWindowText(hWnd,text,255);
	char buffer[1024] = {0};
	sprintf_s(buffer,1023,"current foreground window is : %s",text);
	//	LOG_INFO(buffer);

	sendKeysWithoutSetForeground(hWnd, vks, klen);
}