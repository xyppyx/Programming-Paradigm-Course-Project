#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_set>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;

void Input(std::vector<int>& numbers) 
{
    cout<<"Please input numbers divided by space without any other characters: "<<endl;
    std::string input;
    getline(cin, input);
    std::istringstream iss(input);
    int number;
    while (iss >> number)
        numbers.push_back(number);
}

void Solve(std::vector<int>& numbers)
{
    std::unordered_set<int> unique_numbers;

    for (const int& num : numbers) {
        if (unique_numbers.find(num) != unique_numbers.end()) 
        {
            cout << "true";
            return;
        }
        unique_numbers.insert(num);
    }
    cout << "false";
}

int main() 
{
    std::vector<int> numbers;
    Input(numbers);
    Solve(numbers);
    return 0;
}