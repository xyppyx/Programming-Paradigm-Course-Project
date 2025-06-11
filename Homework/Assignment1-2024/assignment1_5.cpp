/**********************************************
# Project Name : Assignment1-2024 
# File Name    : assignment1_5.cpp
# File Function: Problem Solution
# Author       : YunPeng Xu(徐云鹏)
# Update Date  : 2024/9/17
**********************************************/ 

#include<iostream>
#include<sstream>
#include<vector>
#include<limits>

//define using std namespace
using std::cin;
using std::cout;
using std::endl;

//the max num of day
const int kMaxDayNum = 1e5;

template <typename T>
class MySolution{
	private:
		//store input
		std::vector<T>Prices;
		
	public:
		/*
 		 * Function Name   : Input
 		 * Function        : Get input  
		 */
		void Input()
		{
			cout << "Input:" << endl;
    		cout << "(1 <= prices.length <= 10^5, if input exceeds 10^5, the program will throw an error)" << endl;
    		cout << "(0 <= prices[i] <= 10^4, please separate the data with spaces and press Enter.)" << endl;
			T price = 0;
			std::string all_price;
			std::getline(cin, all_price);
			std::istringstream iss(all_price);
			
			while(iss >> price)
			{
				if(iss.fail() || price < 0 || price > 1e4)
				{
					std::cerr<<"Error: Input data is invalid, please check data type and try again."<<endl;
					exit(0);
				}
				
				if(Prices.size() >= kMaxDayNum)
				{
					std::cerr<<"ERROR:the length of input exceeds 10^5."<<endl;
					exit(0);
				}
				Prices.push_back(price);
			}
		}
		
		/*
 		* Function Name   : Max
 		* Function        : get the max value
 		* Input Parameters: 2 values
 		* Return Value    : max value    
 		*/
		inline T Max(T var1, T var2)
		{
			return var1 >= var2 ? var1 : var2;
		}
		
		/*
 		* Function Name   : Solve
 		* Function        : get the max profit
 		* Return Value    : max profit  
 		*/
		T Solve()
		{
			T max_profit = 0, min_price = std::numeric_limits<T>::max();
			for(int i = 0; i < Prices.size(); i++)
			{
				if(Prices[i] < min_price)
				{
					min_price = Prices[i];
				}
				else if(Prices[i] > min_price)
				{
					max_profit = Max(max_profit, Prices[i] - min_price);
				}
			}
			return max_profit;
		}
		
		/*
 		 * Function Name   : Output
 		 * Function        : Output ans  
		 */
		void Output()
		{
			cout<<"The max profit is: "<<Solve()<<endl; 
		}
};


signed main()
{
	//define solution object
	//if input is of double type, change <int> into <double>
	MySolution<int>solution;
	
	//input data
    solution.Input();
	
	//output ans
	solution.Output();
	
	return 0;
}

