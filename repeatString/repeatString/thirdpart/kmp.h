#ifndef KMP_H
#define KMP_H


class KMP
{
private:
	int paternLen;
	const char *paternStr;
	int *t1Length;//A[i] ��ʾ�ַ��� T[0 ~ i-1] ��õ� t1 �ĳ���
	int *t1LastLength;//A[i] ��ʾ�ַ��� T[strlen(T) - i ~ strlen(T)-1] ��õ� t1 �ĳ���
public:
	KMP(const char *pStr);
	~KMP();

	//��������
	int indexOf(const char *sourceStr,int beginPos = 0);

	/************************************************************************/
	/* 
		�Ӻ���ǰ(������)��.beginPos �Ǵ���һ����ʼ������������
	*/
	/************************************************************************/
	int lastIndexOf(const char *sourceStr,int beginPos);




private:
	//��һ���ַ����ķ����ַ�����������
	static void reverseStr(char *str);

	bool hasInitT1Length;
	//��ʼ������� t1LengthArray
	void initT1Length();

	/************************************************************************/
	/* 
	��ʼ������� t1LengthArray
	< ---------------------
	*/
	/************************************************************************/
	bool hasInitReverseT1Length;
	void initReverseT1Length();

	//�ַ��� str ����� t1LengthArray Ӧ���� str �ķ����ַ����� reverseT1LengthArray
	static bool isT1LengthArrayEqual(char *pStr,int aLength);

public:

	//���Է���� t1Length �ǲ�����ȷ��
	static void testT1LengthRigth();

};


#endif