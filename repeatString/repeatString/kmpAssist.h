#ifndef KMP_ASSIST_H
#define KMP_ASSIST_H
#include <memory.h>

/*
��ʼ������� t1 ���飬t1[i] ��ʾ�ַ��� pStr[0 ~ i-1] ��õ�ǰ��׺�ƥ��ĳ���
abcdabc Ϊ 3
ababab  Ϊ 4
*/
template<typename CHAR_TYPE>
inline size_t* initT1Length(const CHAR_TYPE *pStr,size_t totalLength)
{
	const CHAR_TYPE firstChar = *pStr;
	CHAR_TYPE lastChar;	//���һ���ַ�
	CHAR_TYPE beforeLastChar;	//�����ڶ����ַ�
	size_t lastT1Length;	//��һ����� t1 �ĳ���

	size_t *t1Length = new size_t[totalLength + 1];// t1Length[i] ��ʾ�ַ��� pStr[0 ~ i-1] ��õ� t1 �ĳ���
	memset(t1Length,0,sizeof(int) * (1 + totalLength));
	t1Length[0] = t1Length[1] = 0;
	for (size_t i = 2;i <= totalLength;++ i)
	{
		lastT1Length = t1Length[i-1];	//ǰһ���������� i - 1
		if(0 == lastT1Length)	//ǰһ����ĸû�ж�Ӧ
		{
			t1Length[i] = (firstChar == *(pStr + i - 1));
		}
		else
		{
			lastChar = *(pStr + i - 1);
			beforeLastChar = *(pStr + i - 2);
			while(0 != lastT1Length && *(pStr + lastT1Length) != lastChar)	//��Ӧ�ַ��ĺ���һ���ַ���������һ���ַ�������˳�
			{
				lastT1Length = t1Length[lastT1Length];
			}
			if(0 != lastT1Length)	//��Ϊ *(pStr + lastT1Length) == lastChar ������ѭ��
			{
				t1Length[i] = lastT1Length + 1;
			}
			else
			{
				t1Length[i] = (firstChar == *(pStr + i - 1));
			}
		}
	}
	return t1Length;
}


/************************************************************************/
/* 
ȡ t1 �������һ��ֵ
*/
/************************************************************************/
template<typename CHAR_TYPE>
inline size_t getT1Last(const CHAR_TYPE *pStr,size_t totalLength)
{
	size_t *t1Length = initT1Length(pStr,totalLength);
	size_t ret = t1Length[totalLength];
	delete[] t1Length;
	return ret;
}



/************************************************************************/
/* ����һ��ʡ�ռ�ķ���ȥ�� T1 �������һ��Ԫ��*/
/* 
�赱ǰ���[0,x] ��õ��ǰ��׺��ȵĳ���Ϊ t�������¹���
a). �ַ�������ȹ�ϵ: [0,t-1]=[x-t+1,x], [0,2t-x-2]=[x-t+1,t-1], [0,3t-2x-1]=[x-t+1,2t-x]......
b). ���ϸ����� t, 2t-x-1, 3t-2x-2, .... ���������� x-t+1
c). �ַ���ȹ�ϵ: [x]=[x-(x-t+1)]=[x-2(x-t+1)]=......=[x-n(x-t+1)]
1.x=0 ʱ t=0
2.
�� fnd=0
do
--- �� t=0, �� t=([x]==[0]), fnd=1
--- �� t!=0
--- �� [t]==[x] �� t=t+1, fnd=1
--- �� 2t>x-1 �� [2t-x-1]==[x] �� t=2t-x-1, fnd=1
--- �� 3t>2x-2 �� [3t-2x-2]== [x] �� t=3t-2x-2, fnd=1
--- ...
--- �� fnd=1 �� t=([x]==[0]), fnd=1
end
*/
/************************************************************************/
template<typename CHAR_TYPE>
inline size_t alterGetT1Last(const CHAR_TYPE *pStr,size_t totalLength)
{
	if(0 == totalLength)
		return -1;
	else if(1 == totalLength)
		return 0;
	else
	{
		//
	}
	size_t t = 0;


	for (size_t x = 1;x < totalLength;++ x)
	{
		char fnd=0;
		if(0 == t)
		{
			if(pStr[0]==pStr[x])
			{
				t = 1;
				fnd = 1;
			}
		}
		else
		{
			size_t n = 0;

			//t, 2t-x-1, 3t-2x-2
			while ((n+1)*t > n*x+n)
			{
				if(pStr[(n+1)*t-n*x-n]==pStr[x])
				{
					t = (n+1)*t-n*x-n + 1;
					fnd = 1;
					break;
				}
				++ n;
			}
		}

		if(!fnd)
		{
			t = pStr[0]==pStr[x];
		}
	}
	return t;
}




#endif