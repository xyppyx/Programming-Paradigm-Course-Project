/**********************************************
# Project Name : Assignment2-2024 
# File Name    : assignment2_2.cpp
# File Function: Problem Solution
# Author       : YunPeng Xu(徐云鹏)
# Update Date  : 2024/9/29
**********************************************/

#include<iostream>
#include<string>
#include"MyHighPercision.h"

typedef unsigned long long ull;

using std::cin;
using std::cout;
using std::endl;
using std::cerr;

/*
* Function Name   : FastPower
* Function        : calculate the base^power in O(log(power))
* Input Parameters: BigInt - the base
*                   ull - the power
* Return Value    : BigInt - the result of base^power
*/
BigInt FastPower(BigInt base, ull power)
{
    BigInt res = 1;
    while(power > 0)
    {
        if(power & 1)
            res = res * base;
        base = base * base;
        power = power >> 1;
    }
    return res;
}

/*
* Function Name   : Solve
* Input Parameters: ull - the number of digits
*/
void Solve(ull n)
{
    ull num_5 = (n + 1) / 2;
    ull num_4 = (n - num_5);
    BigInt res = FastPower(4, num_4) * FastPower(5, num_5);
    cout<<res;
}


int main()
{
    //NOTE: BigInt is my high percision integer class, which is implemented in "MyHighPercision.h"
    //if ull is not enough, you can use BigInt to calculate after I implement bitwise operations :)
    ull n;
    cin>>n;
    Solve(n);
    return 0;
}