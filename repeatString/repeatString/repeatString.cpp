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
	最近遇到一个问题，需要判定一个字符串是否是重复的. 初一看这个问题比较简单，但实际上并不如此。复杂就复杂在不知道重复单元的长度。
	举例说明重复字符串的定义
	abcabcabc 以 abc 为重复单元重复 3 次
	abcdabc    不重复
	abababababab 以 ab 为单元重复 6 次，或以 abab 为单元重复 3 次，或者以 ababab 为单元重复 2 次
	不需要想的方法是切字符串，一个一个试，但这个方法时间复杂度有点高，估计可以优化的地方是充分利用上一次判断的信息来指导当次判断 --- 这也是很多算法优化的思路。
	无法忍受这个方法。但有没有更高效的解决办法呢？
	刚好这段时间闲来无事，翻了几页《初等数论及其应用》这本书，看到了有关质数的话题，于是比较敏感的就想到了，如果一个字符串是重复的，那么它必然可以分解为两个数字，设 len = L*N, len 为字符串长度，L 为重复串的重复单元，N 为重复次数.
	更进一步地，如果 L 本身是重复的，则可以拆解 L ，使 L 更短，N 更大，这又是一种重复局面。实际上，只需要 L 满足存在前后缀相等则 L 就可拆解。
	下面给出一个判断长 len 的字符串是否是重复的判定算法，并求 L 和 N。

算法思路：通过质数方法检验一个长度为 len 的字符串 str 是否是重复的
	
	如果 str 是重复的，设最小重复单元长度为 L ，重复 N 次

	len = L*N
	str[0, len) 可以写为 str[0,L), str[L,2*L), str[2*L,3*L).... str[(N-1)*L, N*L)
	由式子，若 L*N 是一个正确的重复表达，则可以扩张 L ，收缩 N 寻找到更大重复单元的表示。
	len = L*N = (L*N1)*N2 = (l*N1*N3)*N4 = ...
	也即是当 N 为合数时，可以将它的因子乘到 L 上，使 L 扩张.
	当 N 收缩到最小时必然是一个质数(或1, 此时每个字符都相等)，此时我们得到了一个最大重复单元的表示:
	len = L*P
	因此，我们寻找这个表示的算法可以根据输入的 len，从小到大扫描质数表 pi，假定为 P 值，
	len/P 为 L，然后判断一下输入 str 是不是以 L 为单元重复 N 次. 如果是则找到了.这个方法把寻找重复表示的问题转化为了判断重复表示。

	上面已经指出，由 len = L*P 可以得到一族重复表示: 将闭开区间字符串 [0,L) 单独拿出，迭代此算法，如果它仍然
	是一个重复的，设为 L=LL*LP，则 len = LL*(LP*P), 对 [0,LL) 迭代，若还能展开，则继续寻找
	直到无法展开，这样下去，可以得到一族表示。这一族可以称为以 P 为基的表示.

	注意，当 L 是前后缀相等的，则此时 L 要减去前后缀相等的长度又是一个新的重复单元，它是以另外一个质数为基的表示，并不是以 P 为基。

证明：若 len=L1*P1 是一个重复表示，不能断言必然不存在其它的表示: len = L2*P2, P2 为质数且 P1≠P2 

	------------------------------------------------------------------------------------------------
	先证明如下引理:
	若 p1, p2 是重复周期，即 len = p1*x = p2*y，必存在 p1=n*p2+k (这个式子就相当于求出 n 和 k : n=p1/p2, k=p1%p2), k<p1, k<p2，则 k 也是重复周期.
	证明: 
	由 p1=n*p2+k 及 p1, p2 均为周期，可知
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

	------------------------------------------------------------------------------------------------
	假设 P1 < P2 且都是质数, len=L1*P1 = L2*P2, 都是重复表示.L1>L2, 必然存在一个整数C使得: L1=C*P2, L2=C*P1, len=C*P1*P2.
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

	实例 Str = abababababab
	则确实可以将 Str 看待为长为6的串重复两次，也可以认为长为 4 的串重复三次.

	所以也就证明了 “无法断言L2*P2 必定不存在”这个命题。

	下面给出求 L2*P2 的两种方法.
	1.  L2 < L1 实际上已经暗含了求 L 的方式: L1 包含 L2, 充分必要条件是 L1 前后缀
	匹配, 到这儿解决办法其实已经很明显了: 与 kmp 算法求重复单元一样.
	2.  继续在质数表中将质数赋值给 N，往后面寻找，



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