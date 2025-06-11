/**********************************************
# Project Name : Assignment2-2024 
# File Name    : assignment2_5.cpp
# File Function: Problem Solution
# Author       : YunPeng Xu(徐云鹏)
# Update Date  : 2024/9/29
**********************************************/

#include<iostream>
#include"MyHighPercision.h"

using std::cin;
using std::cout;
using std::endl;
using std::cerr;

int main()
{
    //NOTE: BigInt is my high percision integer class, which is implemented in "MyHighPercision.h"
    BigInt a, b;
    int m, n;
    cout<<"Please input the length of a: ";
    cin>>m;
    cout<<"Please input a: ";
    cin>>a;
    cout<<"Please input the length of b: ";
    cin>>n;
    cout<<"Please input b: ";
    cin>>b;
    cout<<"a + b = "<<a + b<<endl;
    return 0;
}