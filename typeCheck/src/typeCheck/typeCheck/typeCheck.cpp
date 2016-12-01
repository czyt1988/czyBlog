// typeCheck.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>

#include <iostream>
#include <type_traits>


template<typename TYPE>
void typeCheck(TYPE data);

int _tmain(int argc, _TCHAR* argv[])
{
	int a = 1;
	const int& b = a;
	int& c = a;
	int d[12];

	typeCheck<int>(a);
	typeCheck<const int&>(b);
	typeCheck<int &>(c);
	typeCheck<const int&>(d[7]);
	typeCheck(8);
	system("pause");
	return 0;
}

template<typename TYPE>
void typeCheck(TYPE data)
{
    if(std::is_same<typename std::decay<TYPE>::type,int>::value)
    {
        std::cout<<"int type"<<std::endl;
    }
	else
	{
		std::cout<<"other type"<<std::endl;
	}
}