/**********************************************
# Project Name : Assignment1-2024
# File Name    : Assignment1_3.cpp
# File Function: Problem Solution
# Author       : YunPeng Xu(徐云鹏)
# Update Date  : 2024/9/17
**********************************************/

#include<iostream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

/*
* Function Name   : ToUpperCase
* Function        : change the lower letter in a string into uppercase
* Input Parameters: a string consists of letters 
*/
void ToUpperCase(string& str)
{
	for(int i = 0; i < str.length(); i++)
		if('a' <= str[i] && str[i] <= 'z')
			str[i] = str[i] - 'a' + 'A';
}

/*
* Function Name   : ToLowerCase
* Function        : change the upper letter in a string into lowercase
* Input Parameters: a pointer to a string consists of letters 
*/
void ToLowerCase(string* str)
{
	if(str != NULL)
    {
        for(int i = 0; (*str)[i] != '\0'; i++)
			if('A' <= (*str)[i] && (*str)[i] <= 'Z')
                (*str)[i] = (*str)[i] - 'A' + 'a';
    }
} 

signed main()
{
	//create and initialize a string  
	string init_str = "AbCdEfG";
	cout<<init_str<<endl;
	
	ToUpperCase(init_str);
	cout<<init_str<<endl;
	
	ToLowerCase(&init_str);
	cout<<init_str<<endl;

	return 0;
}

