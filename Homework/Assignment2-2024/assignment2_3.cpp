/**********************************************
# Project Name : Assignment2-2024 
# File Name    : assignment2_3.cpp
# File Function: Problem Solution
# Author       : YunPeng Xu(徐云鹏)
# Update Date  : 2024/9/29
**********************************************/

#include<iostream>
#include<vector>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::vector;

/*
* Function Name   : Check7
* Function        : check if the number is a 7-number(contain 7 or can be divided by 7)
* Input Parameters: int - the number to be checked
* Return Value    : bool - true if the number is a 7-number
*/
bool Check7(int number)
{
    if(number % 7 == 0)
        return true;

    while(number > 0)
    {
        if(number % 10 == 7)
            return true;
        number /= 10;
    }
    return false;
}

/*
* Function Name   : Solve
* Input Parameters: int - the number of elements
*/
void Solve(int n)
{
    vector<bool> del(n + 1, false);
    //all_num: the number of existing elements
    //cur_idx: the current index
    //cur_num: the current number
    int all_num = n, cur_idx = 1, cur_num = 1;

    while(all_num > 1)
    {
        //skip the deleted idx
        while(del[cur_idx])
        {
            cur_idx++;
            if(cur_idx > n)
                cur_idx = 1;
        }
        //if now cur_idx is the 7-number, delete it
        if(Check7(cur_num))
        {
            del[cur_idx] = true;
            all_num--;
        }
        //renew the cur_num and cur_idx
        cur_num++;
        cur_idx++;
        if(cur_idx > n)
            cur_idx = 1;
    }
    //output the answer
    for(int i = 1; i <= n; i++)
    {
        if(!del[i])
            cout << "Ans: " << i << endl;
    }
}

signed main()
{
    int n;
    cout<<"Please input the number of elements: "<<endl;
    cin >> n;
    Solve(n);

    return 0;
}