#ifndef WND_H
#define WND_H
#include <windows.h>

//���Ҵ���
HWND findWindow(HWND parentWnd,const char* name);

//��Ļ�Ƿ��Ѿ�������
BOOL hasScreenLocked();

//�� wnd ���ͼ�����Ϣ, ���� wnd ����Ϊ��ǰ��Ĵ���
void sendKeysWithoutSetForeground(HWND wnd, BYTE * vks, int klen);

//����ǰ��Ĵ��ڷ��ͼ�����Ϣ
void sendToForegroundWnd(BYTE* vks, int klen);


#endif