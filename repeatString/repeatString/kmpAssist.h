#ifndef KMP_ASSIST_H
#define KMP_ASSIST_H
#include <memory.h>

/*
初始化正向的 t1 数组，t1[i] 表示字符串 pStr[0 ~ i-1] 求得的前后缀最长匹配的长度
abcdabc 为 3
ababab  为 4
*/
template<typename CHAR_TYPE>
inline size_t* initT1Length(const CHAR_TYPE *pStr,size_t totalLength)
{
	const CHAR_TYPE firstChar = *pStr;
	CHAR_TYPE lastChar;	//最后一个字符
	CHAR_TYPE beforeLastChar;	//倒数第二个字符
	size_t lastT1Length;	//上一次求的 t1 的长度

	size_t *t1Length = new size_t[totalLength + 1];// t1Length[i] 表示字符串 pStr[0 ~ i-1] 求得的 t1 的长度
	memset(t1Length,0,sizeof(int) * (1 + totalLength));
	t1Length[0] = t1Length[1] = 0;
	for (size_t i = 2;i <= totalLength;++ i)
	{
		lastT1Length = t1Length[i-1];	//前一个，所以是 i - 1
		if(0 == lastT1Length)	//前一个字母没有对应
		{
			t1Length[i] = (firstChar == *(pStr + i - 1));
		}
		else
		{
			lastChar = *(pStr + i - 1);
			beforeLastChar = *(pStr + i - 2);
			while(0 != lastT1Length && *(pStr + lastT1Length) != lastChar)	//对应字符的后面一个字符如果与最后一个字符相等则退出
			{
				lastT1Length = t1Length[lastT1Length];
			}
			if(0 != lastT1Length)	//因为 *(pStr + lastT1Length) == lastChar 跳出的循环
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
取 t1 数组最后一个值
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
/* 另外一种省空间的方法去求 T1 数组最后一个元素*/
/* 
设当前针对[0,x] 求得的最长前后缀相等的长度为 t，有以下规则
a). 字符区间相等关系: [0,t-1]=[x-t+1,x], [0,2t-x-2]=[x-t+1,t-1], [0,3t-2x-1]=[x-t+1,2t-x]......
b). 以上各区间 t, 2t-x-1, 3t-2x-2, .... 项数依次少 x-t+1
c). 字符相等关系: [x]=[x-(x-t+1)]=[x-2(x-t+1)]=......=[x-n(x-t+1)]
1.x=0 时 t=0
2.
置 fnd=0
do
--- 若 t=0, 则 t=([x]==[0]), fnd=1
--- 若 t!=0
--- 若 [t]==[x] 则 t=t+1, fnd=1
--- 若 2t>x-1 且 [2t-x-1]==[x] 则 t=2t-x-1, fnd=1
--- 若 3t>2x-2 且 [3t-2x-2]== [x] 则 t=3t-2x-2, fnd=1
--- ...
--- 若 fnd=1 则 t=([x]==[0]), fnd=1
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