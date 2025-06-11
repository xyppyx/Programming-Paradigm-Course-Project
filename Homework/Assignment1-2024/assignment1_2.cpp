/**********************************************
# Project Name : Assignment1-2024
# File Name    : Assignment1_2.cpp
# File Function: Problem Solution
# Author       : YunPeng Xu(徐云鹏)
# Update Date  : 2024/9/17
**********************************************/

#include<iostream>
#include<iomanip>
#include<vector>

using std::cout;
using std::endl;

const int kMaxNum = 25;
const int kPrecision = 5;

signed main()
{
	std::vector<float>numbers(kMaxNum);
	
	for(int i = 0; i < kMaxNum; i++)
		//get random floats and guarantee that 0 <= numbers[i] <= 1000
		numbers[i] = float(rand() / float(RAND_MAX)) * 1000;
	
	//output initial vector
	cout<<"Initial vector consisting of random floats (ranging from 0 to 1000):"<<endl; 
	for(int i = 0; i < kMaxNum; i++)
	{
		cout << std::left << std::fixed << std::setprecision(kPrecision) << std::setw(15) << numbers[i] <<" ";
		if((i + 1) % 5 == 0)
			cout<<endl;
	}
	
	//perform square operation
	//it's guaranteed that numbers[i]^2 will not exceed the limit of float
	for(int i = 0; i < kMaxNum; i++)
		numbers[i] *= numbers[i]; 
		
	//output vector after operation
	cout<<endl<<"Now perform square operation on the vector:"<<endl; 
	for(int i = 0; i < kMaxNum; i++)
	{
		cout << std::left << std::fixed << std::setprecision(kPrecision) << std::setw(15) << numbers[i] << " ";
		if((i + 1) % 5 == 0)
			cout<<endl;
	}
		
	
	return 0;
}

