#include<iostream>
#include<sstream>
#include<vector>
#include<set>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;

void Input(std::vector<int>& numbers, int& index_diff, int& value_diff) 
{
    cout<<"Please input index difference: " << endl;
    cin >> index_diff;
    cin.ignore(1024, '\n');

    cout<<"Please input value difference: " << endl;
    cin >> value_diff;
    cin.ignore(1024, '\n');

    cout<<"Please input numbers divided by space without any other characters: " << endl;
    std::string input;
    getline(cin, input);
    std::istringstream iss(input);
    int number;
    while (iss >> number)
        numbers.push_back(number);
}

void Solve(std::vector<int>& numbers, int index_diff, int value_diff)
{
    //store every number in the slide window
    std::set<int> window;
    
    for(int i = 0; i < numbers.size(); i++)
    {
        //find the first number that is greater than or equal to (numbers[i] - value_diff)
        auto lower_bound = window.lower_bound(numbers[i] - value_diff);
        //if lower_bound exists and abs(lower_bound - numbers[i]) <= value_diff
        if(lower_bound != window.end() && std::abs(*lower_bound - numbers[i]) <= value_diff)
        {
            cout << "true";
            return;
        }
        //else move the window
        window.insert(numbers[i]);
        //if the window size is greater than index_diff, remove the first number
        if(i >= index_diff)
            window.erase(numbers[i - index_diff]);
    }
    cout << "false";
}

signed main()
{
    std::vector<int> nums;
    int index_diff, value_diff;
    Input(nums, index_diff, value_diff);
    Solve(nums, index_diff, value_diff);
    return 0;
}