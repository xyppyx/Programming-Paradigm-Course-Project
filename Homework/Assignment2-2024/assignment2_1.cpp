/**********************************************
# Project Name : Assignment2-2024 
# File Name    : assignment2_1.cpp
# File Function: Problem Solution
# Author       : YunPeng Xu(徐云鹏)
# Update Date  : 2024/9/29
**********************************************/

#include<iostream>
#include<string>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::string;

/*
* Function Name   : CheckPalindrome
* Input Parameters: const string& - the string to be checked
* Return Value    : bool - true if the string is a palindrome
*/
bool CheckPalindrome(const string& str)
{
    if(str.empty())
    {
        cerr << "Empty string!" << endl;
        return false;
    }

    auto left = str.begin();
    auto right = str.end() - 1;
    while(left < right)
    {
        if(*left != *right)
            return false;
        ++left;
        --right;
    }
    
    return true;
}

/*
* Function Name   : Solve
* Input Parameters: const string& - the string to be solved
*/
void Solve(const string& str)
{
    if(str.empty())
    {
        cerr << "Invalid string!" << endl;
        return;
    }
    else if(CheckPalindrome(str))
    {
        cout<<"True"<<endl;
        return;
    }

    auto left = str.begin();
    auto right = str.end() - 1;
    //use double pointers to check whether the string is a palindrome after deleting one character in O(n)
    while(left < right)
    {
        if(*left != *right)
        {
            string str1 = str, str2 = str;
            //erase the left character
            str1.erase(left - str.begin(), 1);
            //erase the right character
            str2.erase(right - str.begin(), 1);
            if(CheckPalindrome(str1) || CheckPalindrome(str2))
            {
                cout<<"True"<<endl;
                return;
            }
            else
            {
                cout<<"False"<<endl;
                return;
            }
        }
        ++left;
        --right;
    }
}


int main()
{
    string str;
    cout << "Please input a string composed of uppercase and lowercase letters." << endl;
    cin >> str;
    Solve(str);
    return 0;
}