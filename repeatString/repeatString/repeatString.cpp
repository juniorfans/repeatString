#include "repeatString.h"
#include "kmpAssist.h"
#include <cassert>
#include "./thirdpart/primers.h"

/************************************************************************/
/* 
	ʹkmp �㷨�жϳ���Ϊ srcLen ���ַ��� str �Ƿ����ظ���
	����� kmp �㷨�� t1 ��������һ��Ԫ��ֵ m ��ʾ������ǰ��׺ƥ�䳤��Ϊ m,
	�۲�����������ƥ��ĳ��� m �Ȳ�ƥ��ĳ��� notC ��������˵������Ȼ��һ���ظ���
	�� m һ���� notC ��������.
	���ǳ�ֱ�Ҫ��. 
		1.���賤��Ϊ len �Ĵ����ظ��ģ���С��Ԫ�� notC���� m(len-notC)
			���� t1 ��������һ��Ԫ��ֵ���� m һ���� notC ����������
		2.���� m �Ǵ� str �� t1 �������Ԫ�ص�ֵ���� m �� notC ��������������������
			m=3*notC��len=4*notC���������� str[0,3*notC) = str[notC,4*notC)���� 
			str[0,3*notC) ��Ϊ�����֣������� str[notC,4*notC) �Ƚϣ���֪
			str[0,notC)=str[notC,2*notC), 
			str[notC,2*notC)=str[2*notC,3*notC), 
			str[2*notC,3*notC)=str[3*notC,4*notC)
			������ str[0,notC)=str[notC,2*notC)=str[2*notC,3*notC)=str[3*notC,4*notC)
			��Ҳ��֤���� str ��һ���ظ����ַ���

			�� m=n*notC ʱ��ͬ��Ҳ���԰� str[0,n*notC) ��Ϊ n ���֣�
			str[0,notC), str[notC,2*notC).... , str[(n-1)*notC, n*notC) ������
			str[notc,(n+1)*notC) �� n ���֣�
			str[notC,2*notC).... , str[(n-1)*notC, n*notC)����Ƚ�����Եó�
			str[0,notC) = str[notC,2*notC) = ...... = str[(n-1)*notC, n*notC) = str[notc,(n+1)*notC)
			��� str ��һ���ظ����ַ���
*/
/************************************************************************/
LoopBody checkRepeatByKmp(char* str, size_t srcLen)
{
	LoopBody ret;
	size_t m = getT1Last(str,srcLen);
	size_t notC = srcLen - m;
	if(1 <= m/notC && 0 == m%notC)
	{
		ret.isLoop = true;
		ret.length = notC;
		ret.count  = 1 + (m / notC);
	}
	else 
	{
		ret.isLoop = false;
	}
	return ret;
}

/************************************************************************/
/* 
	ͨ��������������һ������Ϊ len ���ַ��� str �Ƿ����ظ���
	
	��� str ���ظ��ģ�����С�ظ���Ԫ����Ϊ L ���ظ� N �Σ��� L �� N ������ֵ
	��ȷ����.

	len = L*N
	str[0, len) ����дΪ str[0,L), str[L,2*L), str[2*L,3*L).... str[(N-1)*L, N*L)
	��ʽ�ӣ��� L*N ��һ����ȷ���ظ������������� L ������ N Ѱ�ҵ������ظ���Ԫ��
	��ʾ��
	len = L*N = (L*N1)*N2 = (l*N1*N3)*N4 = ...
	Ҳ���ǵ� N Ϊ����ʱ�����Խ��������ӳ˵� l �ϣ�ʹ��С�ظ���Ԫ����.
	�� N ��������Сʱ��Ȼ��һ����������ʱ���ǵõ���һ������ظ���Ԫ�ı�ʾ:
	len = L*P
	��ˣ�����Ѱ�������ʾ���㷨���Ը�������� len����С����ɨ�������� pi���ٶ�Ϊ P ֵ��
	len/P Ϊ L��Ȼ���ж�һ������ str �ǲ����� L Ϊ��Ԫ�ظ� N ��. ��������ҵ���.

	�� len = L*P ���Եõ�һ���ʾ: ���տ������ַ��� [0,L) �����ó����������㷨���������Ȼ
	��һ���ظ��ģ���Ϊ L=LL*LP���� len = LL*(LP*P), �� [0,LL) ������������չ���������Ѱ��
	ֱ���޷�չ����������ȥ�����Եõ�һ���ʾ����һ����Գ�Ϊ�� P Ϊ���ı�ʾ.

	Ȼ�����Ƿ񻹴��������ı�ʾ: len = L2*P2, P2 Ϊ����? ���Ǵ���.

	------------------------------------------------------------------------------------------------
	��֤����������:
	�� p1, p2 ���ظ�����ʱ���� len = p1*x = p2*y���ش��� p1=n*p2+k (���ʽ�Ӿ��൱����� n �� k : n=p1/p2, k=p1%p2)
	k<p1, k<p2����ô k Ҳ���ظ�����.
	֤��: 
	��
	p1=n*p2+k �� p1, p2 ��Ϊ���ڣ���֪
	p1		= n*p2 + k
	p1 + 1	= n*p2 + k+1
	p1 + 2	= n*p2 + k+2
	...
	...
	�����У�
	[p1]		= [n*p2 + k]
	[p1 + 1]	= [n*p2 + k+1]
	[p1 + 2]	= [n*p2 + k+2]
	...
	��һ����
	[p1]	 = [0] = [n*p2 + k]   = [k]
	[p1 + 1] = [1] = [n*p2 + k+1] = [k+1]
	[p1 + 2] = [2] = [n*p2 + k+2] = [k+2]
	...
	��ô
	[0] = [k]
	[1] = [k+1]
	[2] = [k+2]
	...
	[q] = [k+q]
	��˵����k Ҳ��һ������. 
	���Ǿ�֤���ˣ��� p1=n*p2+k �� p1 �� p2 ��һ�����ڣ��� k Ҳ������.

	�� k!= 0 ʱ��ʹ�ô˽��۵��������տ�֤����1 Ҳ������.
	��Ϊ k!=0 ʱ��ʹ�� p2 �� k �滻 p1, p2 ȥ�������õ��� k ��Ȼ���ϸ�ݼ���.
	����������ʲôֵ�������ٸ���.
	------------------------------------------------------------------------------------------------
	P1 < P2 �Ҷ�������, len=L1*P1 = L2*P2, �����ظ���ʾ.L1>L2
	���Ȼ����һ������Cʹ��: L1=C*P2, L2=C*P1, len=C*P1*P2.
	�������������L1, L2 �������ڣ��� L1%L2 Ҳ�����ڣ�Ҳ��
	T1=(C*P2)%(C*P1) = C*(P2%P1)������.
	����:
	C*(L2%T1) = C*(C*P1%C*(P2%P1)) = C*(P1%(P2%P1)) ������

	C*((P2%P1)%(P1%(P2%P1)))
	...
	�ý���������� C.
	���Ǽ��������һ�ֱ�ʾ L2*P2 �õ��Ľ��ۣ������������ì�ܻ�������֪���κν��۳�ͻ.
	�� C=1 ʱ������Ϊ1Ҳ��˵�����ַ���ÿ���ַ���һ��,��ʱ L2*P2 ��Ȼ����

	��������: len = 6*2 = 4*3, P1=2, P2=3, L1=6, L2=4;
	���� L1*P1 ��һ���ظ���ʾ�� x0=x6, x1=x7, x2=x8, x3=x9, x4=x10, x5=x11
	���� L2*P2 ��һ���ظ���ʾ�� x0=x4=x8, x1=x5=x9, x2=x6=x10, x3=x7=x11
	��������������ͬʱ������:
	x0=x2=x4=x6=x8=10
	x1=x3=x5=x7=x9=x11
	��ʱ���ں�����Ϊ 2, ��Ҳ���������֤��:
	�� L1=6, L2=4 ������, �� P1=2, P2=3 ��� C=12/(P1*P2)=2, 
	�� C*(P2%P1)=2 ������.

	���ǿ��Ծٸ�ʵ�� Str = abababababab
	��ȷʵ���Խ� Str ����Ϊ��Ϊ6�Ĵ��ظ����Σ�Ҳ������Ϊ��Ϊ 4 �Ĵ��ظ�����.

	��������� L2*P2 �����ַ���.
	1.  L2 < L1 ʵ�����Ѿ��������� L �ķ�ʽ: L1 ���� L2, ��ֱ�Ҫ������ L1 ǰ��׺
		ƥ��, ���������취��ʵ�Ѿ���������: �� kmp �㷨���ظ���Ԫһ��.
	2.  ��������������������Ѱ�� N

*/
/************************************************************************/
LoopBody checkRepeatByPrimer(char * str, size_t len)
{
	assert((int)len >= 0);
	LoopBody ret;
	ret.isLoop = false;

	for (size_t i = 0;i < primer_table_size; ++ i)
	{
		if(primer_table[i] < (int)len && 0 == len%primer_table[i])
		{
			size_t l = len/primer_table[i];
			size_t n = primer_table[i];
			if(verifyRepeatOn(str,len,l,n))
			{
				ret.isLoop = true;
				ret.length = l;
				ret.count = n;
				return ret;
			}
		}
	}
	return ret;
}

/************************************************************************/
/* 
	ʹ�ÿռ��Ż���� kmp �㷨�жϳ���Ϊ srcLen ���ַ��� str �Ƿ����ظ���
*/
/************************************************************************/
LoopBody checkRepeatByKmpOpt(char* str, size_t srcLen)
{
	LoopBody ret;
	size_t m = alterGetT1Last(str,srcLen);
	size_t notC = srcLen - m;
	if(1 <= m/notC && 0 == m%notC)
	{
		ret.isLoop = true;
		ret.length = notC;
		ret.count  = 1 + (m / notC);
	}
	else 
	{
		ret.isLoop = false;
	}
	return ret;
}



/************************************************************************/
/* 
����Ϊ len ���ַ��� str �Ƿ��� l ����Ϊ������Ԫ���ظ� n ��
*/
/************************************************************************/
int verifyRepeatOn(char* str, size_t len, size_t l, size_t n)
{
	assert(len == l * n);
	for (size_t i = 0;i < n-1;++ i)
	{
		//compare i and i+1
		for (size_t j = 0;j < l;++ j)
		{
			if(str[j*i] != str[j*i + l])
				return false;
		}
	}
	return true;
}