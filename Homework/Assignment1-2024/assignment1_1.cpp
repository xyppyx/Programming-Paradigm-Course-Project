/**********************************************
# Project Name : Assignment1-2024 
# File Name    : assignment1_1.cpp
# File Function: Problem Solution
# Author       : YunPeng Xu(徐云鹏) 
# Update Date  : 2024/9/17
**********************************************/ 

#include<iostream>
#include<fstream>
#include<string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

/*
* Function Name   : ToLowerCase
* Function        : change a upper letter into lower letter
* Input Parameters: a upper letter 
* Return Value    : its lower case   
*/
char ToLowerCase(char ch)
{
	if('A' <= ch && ch <= 'Z')
		return char(ch + 'a' - 'A');
	else
		return ch;
}

signed main()
{
	//open file
	std::ifstream input_file("D:\\Coding\\myProject\\Grade2\\ProgrammingParadigm\\week2\\input.txt");
	
	if (!input_file) {
        std::cerr << "Error opening file." << endl;
        return 0;
    }
    
    char ch;
    string current_word = "";
    int that_num = 0;
    
    //count the num of 'that'
    while(input_file >> std::noskipws >> ch)
    {
    	ch = ToLowerCase(ch);
    	
    	if('a' <= ch && ch <= 'z')
    	{
    		current_word += ch;
		}
		else 
		{
			if(current_word == "that")
				++that_num;
			current_word = "";
		}
	}
	if(current_word == "that")
		++that_num;
	
	//close file
	input_file.close();
	
	cout << "The word 'that' appears " << that_num << " times in the file." << endl;
	return 0;
}
