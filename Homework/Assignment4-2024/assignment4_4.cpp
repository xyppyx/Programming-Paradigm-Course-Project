#include <iostream>
#include <ctime>

using std::clock;
using std::cout;
using std::endl;

class Base {
private:
    int m_test = 0;
public:
    virtual void virtualFunction() 
    {
        m_test++;
    }

    void nonVirtualFunction() 
    {
        m_test++;
    }
};

class Derived : public Base 
{
private:
    int m_test = 0;

public:
    void virtualFunction() override 
    {
        m_test++;
    }

    void nonVirtualFunction() 
    {
        m_test++;
    }
};

int main() 
{
    Derived derivedObj;
    Base* basePtr = &derivedObj;

    const int numCalls = 1000000;
    clock_t start, end;

    // 测量虚函数调用时间
    start = clock();
    for (int i = 0; i < numCalls; ++i) 
        basePtr -> virtualFunction();
    end = clock();
    double virtualCallTime = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    cout << "Virtual function call time: " << virtualCallTime << " seconds" << endl;

    // 测量非虚函数调用时间
    start = clock();
    for (int i = 0; i < numCalls; ++i) 
        basePtr -> nonVirtualFunction();
    end = clock();
    double nonVirtualCallTime = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    cout << "Non-virtual function call time: " << nonVirtualCallTime << " seconds" << endl;

    delete basePtr;
    return 0;
}