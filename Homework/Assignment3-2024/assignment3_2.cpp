#include<iostream>
#include<sstream>
#include<vector>
#include<map>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;

void Input(std::vector<int>& numbers, int& k) 
{
    cout<<"Please input k: " << endl;
    cin >> k;
    cin.ignore(1024, '\n');

    cout<<"Please input numbers divided by space without any other characters: " << endl;
    std::string input;
    getline(cin, input);
    std::istringstream iss(input);
    int number;
    while (iss >> number)
        numbers.push_back(number);
}

void Solve(std::vector<int>& numbers, int k)
{
    //store the num of every number in a segment
    std::map<int, int> numbers_num;

    for(int i = 0; i < numbers.size() - k; i++)
    {
        //slide the segment after initialization
        if(i >= 1)
        {   //remove the number that is not in the segment
            numbers_num[numbers[i - 1]]--;
            //add a new number
            numbers_num[numbers[i + k]]++;
            //check if the new number is already in the segment
            if(numbers_num[numbers[i + k]] > 1)
            {
                cout << "true";
                return;
            }
        }
        else//initialize the map for [0, k]
        {
            for(int j = 0; j <= k; j++)
            {
                numbers_num[numbers[j]]++;
                if(numbers_num[numbers[j]] > 1)
                {
                    cout << "true";
                    return;
                }
            }
        }
    }

    cout << "false";
}

signed main()
{
    std::vector<int> nums;
    int k;
    Input(nums, k);
    Solve(nums, k);
    return 0;
}