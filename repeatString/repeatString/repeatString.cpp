#include "repeatString.h"
#include "kmpAssist.h"
#include <cassert>
#include "./thirdpart/primers.h"

/************************************************************************/
/* 
	使kmp 算法判断长度为 srcLen 的字符串 str 是否是重复的
	计算出 kmp 算法的 t1 数组的最后一个元素值 m 表示整个串前后缀匹配长度为 m,
	观察整个串，若匹配的长度 m 比不匹配的长度 notC 更长，则说明它必然是一个重复的
	且 m 一定是 notC 的整数倍.
	这是充分必要的. 
		1.假设长度为 len 的串是重复的，最小单元是 notC，则 m(len-notC)
			即是 t1 数组的最后一个元素值。且 m 一定是 notC 的整数倍。
		2.假设 m 是串 str 的 t1 数组最后元素的值，且 m 是 notC 的整数倍，便于理解设
			m=3*notC，len=4*notC，则首先有 str[0,3*notC) = str[notC,4*notC)，将 
			str[0,3*notC) 分为三部分，依次与 str[notC,4*notC) 比较，可知
			str[0,notC)=str[notC,2*notC), 
			str[notC,2*notC)=str[2*notC,3*notC), 
			str[2*notC,3*notC)=str[3*notC,4*notC)
			于是有 str[0,notC)=str[notC,2*notC)=str[2*notC,3*notC)=str[3*notC,4*notC)
			这也就证明了 str 是一个重复的字符串

			当 m=n*notC 时，同样也可以把 str[0,n*notC) 拆为 n 部分：
			str[0,notC), str[notC,2*notC).... , str[(n-1)*notC, n*notC) 依次与
			str[notc,(n+1)*notC) 的 n 部分：
			str[notC,2*notC).... , str[(n-1)*notC, n*notC)各项比较则可以得出
			str[0,notC) = str[notC,2*notC) = ...... = str[(n-1)*notC, n*notC) = str[notc,(n+1)*notC)
			因此 str 是一个重复的字符串
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
	通过质数方法检验一个长度为 len 的字符串 str 是否是重复的
	
	如果 str 是重复的，设最小重复单元长度为 L ，重复 N 次：则 L 和 N 这两个值
	是确定的.

	len = L*N
	str[0, len) 可以写为 str[0,L), str[L,2*L), str[2*L,3*L).... str[(N-1)*L, N*L)
	由式子，若 L*N 是一个正确的重复表达，则可以扩张 L ，收缩 N 寻找到更大重复单元的
	表示。
	len = L*N = (L*N1)*N2 = (l*N1*N3)*N4 = ...
	也即是当 N 为合数时，可以将它的因子乘到 l 上，使最小重复单元扩张.
	当 N 收缩到最小时必然是一个质数，此时我们得到了一个最大重复单元的表示:
	len = L*P
	因此，我们寻找这个表示的算法可以根据输入的 len，从小到大扫描质数表 pi，假定为 P 值，
	len/P 为 L，然后判断一下输入 str 是不是以 L 为单元重复 N 次. 如果是则找到了.

	由 len = L*P 可以得到一族表示: 将闭开区间字符串 [0,L) 单独拿出，迭代此算法，如果它仍然
	是一个重复的，设为 L=LL*LP，则 len = LL*(LP*P), 对 [0,LL) 迭代，若还能展开，则继续寻找
	直到无法展开，这样下去，可以得到一族表示。这一族可以称为以 P 为基的表示.

	然而，是否还存在其它的表示: len = L2*P2, P2 为质数? 答案是存在.

	------------------------------------------------------------------------------------------------
	先证明如下引理:
	当 p1, p2 是重复周期时，即 len = p1*x = p2*y，必存在 p1=n*p2+k (这个式子就相当于求出 n 和 k : n=p1/p2, k=p1%p2)
	k<p1, k<p2，那么 k 也是重复周期.
	证明: 
	由
	p1=n*p2+k 及 p1, p2 均为周期，可知
	p1		= n*p2 + k
	p1 + 1	= n*p2 + k+1
	p1 + 2	= n*p2 + k+2
	...
	...
	所以有：
	[p1]		= [n*p2 + k]
	[p1 + 1]	= [n*p2 + k+1]
	[p1 + 2]	= [n*p2 + k+2]
	...
	进一步地
	[p1]	 = [0] = [n*p2 + k]   = [k]
	[p1 + 1] = [1] = [n*p2 + k+1] = [k+1]
	[p1 + 2] = [2] = [n*p2 + k+2] = [k+2]
	...
	那么
	[0] = [k]
	[1] = [k+1]
	[2] = [k+2]
	...
	[q] = [k+q]
	这说明，k 也是一个周期. 
	于是就证明了，若 p1=n*p2+k 且 p1 和 p2 是一个周期，则 k 也是周期.

	当 k!= 0 时，使用此结论迭代，最终可证明，1 也是周期.
	因为 k!=0 时，使用 p2 和 k 替换 p1, p2 去迭代，得到的 k 必然是严格递减的.
	它会收敛于什么值，后面再给出.
	------------------------------------------------------------------------------------------------
	P1 < P2 且都是质数, len=L1*P1 = L2*P2, 都是重复表示.L1>L2
	则必然存在一个整数C使得: L1=C*P2, L2=C*P1, len=C*P1*P2.
	由上面的引理，L1, L2 都是周期，则 L1%L2 也是周期，也即
	T1=(C*P2)%(C*P1) = C*(P2%P1)是周期.
	迭代:
	C*(L2%T1) = C*(C*P1%C*(P2%P1)) = C*(P1%(P2%P1)) 是周期

	C*((P2%P1)%(P1%(P2%P1)))
	...
	该结果会收敛于 C.
	这是假设存在另一种表示 L2*P2 得到的结论，它并不与假设矛盾或者与已知的任何结论冲突.
	当 C=1 时，周期为1也就说明了字符串每个字符都一样,此时 L2*P2 必然存在

	举例而言: len = 6*2 = 4*3, P1=2, P2=3, L1=6, L2=4;
	假设 L1*P1 是一个重复表示则 x0=x6, x1=x7, x2=x8, x3=x9, x4=x10, x5=x11
	假设 L2*P2 是一个重复表示则 x0=x4=x8, x1=x5=x9, x2=x6=x10, x3=x7=x11
	若上面两个假设同时成立则:
	x0=x2=x4=x6=x8=10
	x1=x3=x5=x7=x9=x11
	此时周期很明显为 2, 这也符合上面的证明:
	若 L1=6, L2=4 是周期, 由 P1=2, P2=3 求得 C=12/(P1*P2)=2, 
	则 C*(P2%P1)=2 是周期.

	我们可以举个实例 Str = abababababab
	则确实可以将 Str 看待为长为6的串重复两次，也可以认为长为 4 的串重复三次.

	下面给出求 L2*P2 的两种方法.
	1.  L2 < L1 实际上已经暗含了求 L 的方式: L1 包含 L2, 充分必要条件是 L1 前后缀
		匹配, 到这儿解决办法其实已经很明显了: 与 kmp 算法求重复单元一样.
	2.  继续在质数表中往后面寻找 N

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
	使用空间优化后的 kmp 算法判断长度为 srcLen 的字符串 str 是否是重复的
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
长度为 len 的字符串 str 是否以 l 长度为基本单元，重复 n 次
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