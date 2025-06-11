/**********************************************
# Project Name : Assignment2-2024 
# File Name    : assignment2_4.cpp
# File Function: Problem Solution
# Author       : YunPeng Xu(徐云鹏)
# Update Date  : 2024/9/29
**********************************************/

#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::string;

/*
* Function Name   : HexToDec
* Function        : convert a hex string to a decimal number
* Input Parameters: const string& - the hex string
* Return Value    : unsigned long long - the decimal number
*/
unsigned long long HexToDec(const string& hex) 
{
    unsigned long long dec = 0;
    for (char digit : hex) 
    {
        dec = dec * 6 + (digit - '0');
    }
    return dec;
}

/*
* Function Name   : DecToBase
* Function        : convert a decimal number to a base number
* Input Parameters: unsigned long long - the decimal number
*                   int - the base
* Return Value    : string - the base number
*/
string DecToBase(unsigned long long dec, int base) 
{
    const char digits[] = "0123456789ABCDEFGHIJ";
    string result;
    while (dec > 0) 
    {
        result.push_back(digits[dec % base]);
        dec /= base;
    }
    reverse(result.begin(), result.end());
    return result.empty() ? "0" : result;
}

int main() 
{
    string hex;
    int base;
    cout << "Please input the base: ";
    cin >> base;
    cout << "Please input the hex number: ";
    cin >> hex;

    unsigned long long dec = HexToDec(hex);
    string result = DecToBase(dec, base);

    cout << "The result is: " << result << endl;
    return 0;
}