#include "math.h"

//2^x <= num���� x �����ֵ
int radix(int num)
{
	if(num < 0)
	{
		num = -num;
	}
	int retDix = 1;
	long long ret = 1;//�������Ҫ������� 32 λ������������� INT_MAX ��˵��ret ��������ֱ��С��0���ٵ�С��0.
	while (ret <= num)
	{
		ret <<= 1;
		++ retDix;
	}
	return --retDix;//ret >> 1;
}