#include <iostream>
#include <typeinfo>

using std::cout;
using std::endl;

// 基类
class Base 
{
public:
    virtual ~Base() {}

    // 虚函数 clone()，返回指向当前对象副本的指针
    virtual Base* clone() const
    {
        return new Base(*this);
    }

    // 用于演示的虚函数
    virtual void print() const
    {
        cout << "Base" << endl;
    }
};

// 派生类 Derived1
class Derived1 : public Base 
{
public:
    Derived1* clone() const override
    {
        return new Derived1(*this);
    }

    void print() const override 
    {
        cout << "Derived1" << endl;
    }
};

// 派生类 Derived2
class Derived2 : public Base 
{
public:
    Derived2* clone() const override 
    {
        return new Derived2(*this);
    }

    void print() const override 
    {
        cout << "Derived2" << endl;
    }
};

int main() 
{
    Base* obj1 = new Derived1();
    Base* obj2 = new Derived2();

    obj1->print(); // 输出 "Derived1"
    obj1->clone()->print(); // 输出 "Derived1"

    obj2->print(); // 输出 "Derived2"
    obj2->clone()->print(); // 输出 "Derived2"
    
    delete obj1;
    delete obj2;
    return 0;
}