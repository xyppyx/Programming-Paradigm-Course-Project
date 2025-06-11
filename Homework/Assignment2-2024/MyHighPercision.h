/**********************************************
# Project Name : MyHeaderFile
# File Name    : MyHighPercision.h
# File Function: provide a high percision integer class supporting =, +, -, * and ==, >, <, >=, <=
# Author       : YunPeng Xu(徐云鹏)
# Update Date  : 2024.10.4
**********************************************/

#pragma once
#include<iostream>
#include<string>
#include<vector>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::vector;
using std::string;

//the high percision integer class
class BigInt
{
    private:
        vector<int> digits;
        //flase for non-negative
        bool sign;

    public:
        /*
        * Function Name   : constructor
        * Function        : initialize the BigInt with 0
        */
        BigInt()
        {
            digits.push_back(0);
            sign = 0;
        }

        /*
        * Function Name   : constructor
        * Function        : initialize the BigInt with the given string
        * Input Parameters: const string& - the string to initialize the BigInt
        */
        BigInt(const string& str)
        {
            if(str[0] == '-')
            {
                sign = 1;
                for(int i = str.size() - 1; i > 0; i--)
                {
                    digits.push_back(str[i] - '0');
                }
            }
            else
            {
                sign = 0;
                for(int i = str.size() - 1; i >= 0; i--)
                {
                    digits.push_back(str[i] - '0');
                }
            }
        }

        /*
        * Function Name   : constructor
        * Function        : initialize the BigInt with the given long long number
        * Input Parameters: long long - the number to initialize the BigInt
        */
        BigInt(long long num)
        {
            if(num < 0)
            {
                sign = 1;
                num = -num;
            }
            else if(num == 0)
            {
                sign = 0;
                digits.push_back(0);
            }
            else
                sign = 0;

            while(num)
            {
                digits.push_back(num % 10);
                num /= 10;
            }
        }

        /*
        * Function Name   : copy constructor
        * Function        : copy the given BigInt
        * Input Parameters: const BigInt& - the BigInt to copy
        */
        BigInt(const BigInt& other)
        {
            digits = other.digits;
            sign = other.sign;
        }

    public:
        /*
        * Function Name   : operator<<
        * Function        : output the BigInt
        * Input Parameters: std::ostream& - the output stream
        *                   const BigInt& - the BigInt to output
        * Return Value    : std::ostream& - the output stream
        */
        friend std::ostream& operator<<(std::ostream& os, const BigInt& num)
        {
            if(num.sign)
                os << '-';
            for(int i = num.digits.size() - 1; i >= 0; i--)
            {
                os << num.digits[i];
            }
            return os;
        }

        /*
        * Function Name   : operator>>
        * Function        : input the BigInt
        * Input Parameters: std::istream& - the input stream
        *                   BigInt& - the BigInt to input
        * Return Value    : std::istream& - the input stream
        */
        friend std::istream& operator>>(std::istream& is, BigInt& num)
        {
            num.digits.clear();
            string str;
            is >> str;
            if(str[0] == '-')
            {
                num.sign = 1;
                for(int i = str.size() - 1; i > 0; i--)
                {
                    num.digits.push_back(str[i] - '0');
                }
            }
            else
            {
                num.sign = 0;
                for(int i = str.size() - 1; i >= 0; i--)
                {
                    num.digits.push_back(str[i] - '0');
                }
            }
            return is;
        }

        /*
        * Function Name   : operator==
        * Function        : compare if two BigInt are equal
        * Input Parameters: const BigInt& - the BigInt to compare
        * Return Value    : bool - true if two BigInt are equal
        */
        bool operator==(const BigInt& other) const
        {
            if(sign != other.sign)
                return false;
            if(digits.size() != other.digits.size())
                return false;
            for(int i = 0; i < digits.size(); i++)
            {
                if(digits[i] != other.digits[i])
                    return false;
            }
            return true;
        }

        /*
        * Function Name   : operator>
        * Function        : compare if the BigInt is greater than the given BigInt
        * Input Parameters: const BigInt& - the BigInt to compare
        * Return Value    : bool - true if the BigInt is greater than the given BigInt
        */
        bool operator>(const BigInt& other) const
        {
            if(sign == 0 && other.sign == 1)
                return true;
            else if(sign == 1 && other.sign == 0)
                return false;
            else if(sign == 0 && other.sign == 0)
            {
                if(digits.size() > other.digits.size())
                    return true;
                else if(digits.size() < other.digits.size())
                    return false;
                else
                {
                    for(int i = digits.size() - 1; i >= 0; i--)
                    {
                        if(digits[i] > other.digits[i])
                            return true;
                        else if(digits[i] < other.digits[i])
                            return false;
                    }
                    return false;
                }
            }
            else if(sign == 1 && other.sign == 1)
            {
                if(digits.size() > other.digits.size())
                    return false;
                else if(digits.size() < other.digits.size())
                    return true;
                else
                {
                    for(int i = digits.size() - 1; i >= 0; i--)
                    {
                        if(digits[i] > other.digits[i])
                            return false;
                        else if(digits[i] < other.digits[i])
                            return true;
                    }
                    return false;
                }
            }
            return false;
        }

        /*
        * Function Name   : operator>=
        * Function        : compare if the BigInt is greater than or equal to the given BigInt
        * Input Parameters: const BigInt& - the BigInt to compare
        * Return Value    : bool - true if the BigInt is greater than or equal to the given BigInt
        */
        bool operator>=(const BigInt& other) const
        {
            return *this > other || *this == other;
        }

        /*
        * Function Name   : operator<
        * Function        : compare if the BigInt is less than the given BigInt
        * Input Parameters: const BigInt& - the BigInt to compare
        * Return Value    : bool - true if the BigInt is less than the given BigInt
        */
        bool operator<(const BigInt& other) const
        {
            return !(*this >= other);
        }

        /*
        * Function Name   : operator<=
        * Function        : compare if the BigInt is less than or equal to the given BigInt
        * Input Parameters: const BigInt& - the BigInt to compare
        * Return Value    : bool - true if the BigInt is less than or equal to the given BigInt
        */
        bool operator<=(const BigInt& other) const
        {
            return !(*this > other);
        }

        /*
        * Function Name   : operator=
        * Function        : assign the given BigInt to the BigInt
        * Input Parameters: const BigInt& - the BigInt to assign
        * Return Value    : BigInt& - the BigInt after assignment
        */
        BigInt& operator=(const BigInt& other)
        {
            digits = other.digits;
            sign = other.sign;
            return *this;
        }

        /*
        * Function Name   : operator=
        * Function        : assign the given long long number to the BigInt
        * Input Parameters: long long - the number to assign
        * Return Value    : BigInt& - the BigInt after assignment
        */
        BigInt& operator=(long long num)
        {
            digits.clear();
            if(num < 0)
            {
                sign = 1;
                num = -num;
            }
            else if(num == 0)
            {
                sign = 0;
                digits.push_back(0);
            }
            else
                sign = 0;

            while(num)
            {
                digits.push_back(num % 10);
                num /= 10;
            }
            return *this;
        }

        /*
        * Function Name   : operator+
        * Function        : add the given BigInt to the BigInt
        * Input Parameters: const BigInt& - the BigInt to add
        * Return Value    : BigInt - the BigInt after addition
        */
        BigInt operator+(const BigInt& other) const
        {
            BigInt result;
            result.digits.clear();
            if(sign == other.sign)
            {
                result.sign = sign;
                int carry = 0;
                for(int i = 0; i < digits.size() || i < other.digits.size(); i++)
                {
                    int sum = carry;
                    if(i < digits.size())
                        sum += digits[i];
                    if(i < other.digits.size())
                        sum += other.digits[i];
                    result.digits.push_back(sum % 10);
                    carry = sum / 10;
                }
                if(carry)
                    result.digits.push_back(carry);
            }
            else if(sign != other.sign)
            {
                if(sign == 1 && other.sign == 0)
                {
                    BigInt temp = *this;
                    temp.sign = 0;
                    result = other - temp;
                }
                else if(sign == 0 && other.sign == 1)
                {
                    BigInt temp = other;
                    temp.sign = 0;
                    result = *this - temp;
                }
            }
            return result;
        }

        /*
        * Function Name   : operator-
        * Function        : subtract the given BigInt from the BigInt
        * Input Parameters: const BigInt& - the BigInt to subtract
        * Return Value    : BigInt - the BigInt after subtraction
        */
        BigInt operator-(const BigInt& other) const
        {
            BigInt result;
            result.digits.clear();
            if(sign == other.sign)
            {
                if(*this < other)
                {
                    result = other - *this;
                    result.sign = !sign;
                }
                else
                {
                    result.sign = sign;
                    int borrow = 0;
                    for(int i = 0; i < digits.size(); i++)
                    {
                        int diff = digits[i] - borrow;
                        if(i < other.digits.size())
                            diff -= other.digits[i];
                        if(diff < 0)
                        {
                            diff += 10;
                            borrow = 1;
                        }
                        else
                            borrow = 0;
                        result.digits.push_back(diff);
                    }
                    while(result.digits.size() > 1 && result.digits.back() == 0)
                        result.digits.pop_back();
                }
            }
            else if(sign != other.sign)
            {
                if(sign == 1 && other.sign == 0)
                {
                    BigInt temp = *this;
                    temp.sign = 0;
                    result = temp + other;
                    result.sign = 1;
                }
                else if(sign == 0 && other.sign == 1)
                {
                    BigInt temp = other;
                    temp.sign = 0;
                    result = *this + temp;
                }
            }
            return result;
        }

        /*
        * Function Name   : operator-
        * Function        : get the negative of the BigInt
        * Return Value    : BigInt - the negative of the BigInt
        */
        BigInt operator-() const
        {
            BigInt result = *this;
            result.sign = !sign;
            return result;
        }

        /*
        * Function Name   : operator*
        * Function        : multiply the given BigInt with the BigInt
        * Input Parameters: const BigInt& - the BigInt to multiply
        * Return Value    : BigInt - the BigInt after multiplication
        */
        BigInt operator*(const BigInt& other) const
        {
            BigInt result;
            result.digits.resize(digits.size() + other.digits.size(), 0);
            result.sign = (sign == other.sign) ? 0 : 1;

            //multiply 
            for (size_t i = 0; i < digits.size(); ++i)
            {
                for (size_t j = 0; j < other.digits.size(); ++j)
                {
                    result.digits[i + j] += digits[i] * other.digits[j];
                    result.digits[i + j + 1] += result.digits[i + j] / 10;
                    result.digits[i + j] %= 10;
                }
            }

            //remove the leading zeros
            while (result.digits.size() > 1 && result.digits.back() == 0)
            {
                result.digits.pop_back();
            }

            return result;
        }
    
    public:
        /*
        * Function Name   : size
        * Function        : get the number of digits in the BigInt
        * Return Value    : size_t - the number of digits in the BigInt
        */
        size_t size() const
        {
            return digits.size();
        }

        /*
        * Function Name   : operator[]
        * Function        : get the digit at the index given
        * Input Parameters: size_t - the index(range from 0 to size - 1, from left to right)
        * Return Value    : int - the digit at the index given
        */
        int operator[](size_t index) const
        {
            if(index < 0 || index >= digits.size())
            {
                cerr << "Invalid index!" << endl;
                return -1;
            }
            return digits[digits.size() - 1 - index];
        }
};