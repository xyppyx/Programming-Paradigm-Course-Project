/**********************************************
# Project Name : Assignment1-2024
# File Name    : Assignment1_4.cpp
# File Function: Problem Solution
# Author       : YunPeng Xu(徐云鹏)
# Update Date  : 2024/9/17
**********************************************/

#include<iostream>
#include<string>

using std::cout;
using std::endl; 

signed main()
{
	//typedef struct
	typedef struct {
		int int_a;
		std::string str_b, str_c;
	}MyType;
	
	//initialize 
	MyType example = {24, "WhatCanISay?", "MambaOut!"};
	
	//output
	cout<<"values of instance:"<<endl<<example.int_a<<" "<<example.str_b<<" "<<example.str_c<<endl;
	
	//get the address of my instance and assign it to a pointer
	MyType* p = &example;
	
	//use pointer to change the value
	p -> int_a = 42;
	p -> str_b = "?yasinactahw";
	p -> str_c = "!tuoabmam";
	
	//output
	cout<<endl<<"changed values:"<<endl<<example.int_a<<" "<<example.str_b<<" "<<example.str_c<<endl;

	return 0;
}

