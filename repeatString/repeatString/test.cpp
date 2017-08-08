#include <stdio.h>
#include <stdlib.h>
#include <vector>
using std::vector;
#include <cassert>
#include "./thirdpart/utils.h"
#include "kmpAssist.h"
#include "repeatString.h"

void compareIsLoopTest()
{
	size_t testCases = 10;
	size_t len = 1024*1024*512;
	char *str = new char[len + 1];
	memset(str,0,sizeof(char) * (len+1));
	size_t repeatCounts = 0;
	DWORD st,et;
	DWORD cost1 ,cost2;
	int diff = 0;
	while (0 != --testCases)
	{
		size_t l,n;
		size_t strLen = randomRepeatStr(str,len,l,n);
		st = GetTickCount();
		LoopBody loop = checkRepeatByKmp(str, strLen);
		et = GetTickCount();
		cost1 = et-st;
		if(loop.isLoop)
		{
			++ repeatCounts;
			st = GetTickCount();
			LoopBody ret = checkRepeatByPrimer(str,strLen);
			et = GetTickCount();
			cost2 = et - st;
			if(!ret.isLoop)
			{
				printf("---------------------------------oops, not correct\r\n");
			}
			else{
				diff += (cost1 - cost2);
			}
		}
	}
	printf("total repeat counts : %d\r\n", repeatCounts);
	printf("diff (cost1-cost2): %d\r\n", diff);

	delete[] str;
}


void compareLastTest()
{
	size_t l = 53453, n = 3733;
	size_t len = l * n;
	char *str = new char[len + 1];
	size_t strLen = randomRepeatStrLN(str,l,n);
	//memcpy_s(str,l*n,"lldptlldpt",l*n);
	//memcpy_s(str,strLen,"abscabscabsgabscabscabsc",strLen);
	DWORD cost1, cost2, s, e;
	s = GetTickCount();
	size_t lastT = alterGetT1Last(str,strLen);
	e = GetTickCount();
	cost1 = e-s;
	s = GetTickCount();
	size_t *tArray = initT1Length(str,strLen);
	e = GetTickCount();
	cost2 = e-s;
	//printf("%s -- first: %d, second: %d\r\n",lastT == tArray[strLen] ? "yes":"no" ,lastT, tArray[strLen]);
	if(lastT != tArray[strLen])
	{
		printf("------- oops, lastT: %d, lastT2: %d\r\n", lastT, tArray[strLen]);
		char ch = str[l];
		str[l] = 0;
		printf("%s\r\n", str);
		str[l] = ch;
	}
	else{
		printf("diff : %d\r\n", cost2-cost1);
	}

	delete[] str;
	delete[] tArray;
}



void compareIsLoopTest2()
{
	size_t testCases = 10;
	size_t len = 1024*1024*256;
	char *str = new char[len + 1];
	memset(str,0,sizeof(char) * (len+1));
	size_t repeatCounts = 0;
	DWORD st,et;
	DWORD cost1 ,cost2;
	int diff = 0;
	while (0 != --testCases)
	{
		size_t l,n;
		size_t strLen = randomRepeatStr(str,len,l,n);
		st = GetTickCount();
		LoopBody loop = checkRepeatByKmpOpt(str, strLen);
		et = GetTickCount();
		cost1 = et-st;
		if(loop.isLoop)
		{
			++ repeatCounts;
			st = GetTickCount();
			LoopBody ret = checkRepeatByPrimer(str,strLen);
			et = GetTickCount();
			cost2 = et - st;
			if(!ret.isLoop)
			{
				printf("---------------------------------oops, not correct\r\n");
			}
			else{
				diff += (cost1 - cost2);
			}
		}
	}
	printf("total repeat counts : %d\r\n", repeatCounts);
	printf("diff (cost1-cost2): %d\r\n", diff);

	delete[] str;
}


void performanceTest()
{
	size_t testCases = 10;
	size_t len = 1024*1024*256;
	char *str = new char[len + 1];
	memset(str,0,sizeof(char) * (len+1));
	size_t repeatCounts = 0;
	DWORD st,et;
	DWORD cost1 = 0;
	while (0 != --testCases)
	{
		size_t l,n;
		size_t strLen = randomRepeatStr(str,len,l,n);
		st = GetTickCount();
		LoopBody loop = checkRepeatByKmpOpt(str, strLen);
		et = GetTickCount();
		cost1 += et-st;
		if(loop.isLoop)
		{
			++ repeatCounts;
		}
		else
		{
			printf("oops---------------\r\n");
		}
	}
	printf("total repeat counts : %d\r\n", repeatCounts);
	printf("cost: %d\r\n", cost1);

	delete[] str;
}

void main()
{
	while(true)
	{
		performanceTest();
		system("PAUSE");
	}
}