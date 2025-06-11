#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;

class Traveler 
{
private:
    std::string name;

public:
    // 构造函数
    Traveler(const std::string& n) : name(n) {}

    // 拷贝构造函数
    Traveler(const Traveler& other) : name(other.name) {}

    // 赋值运算符
    Traveler& operator=(const Traveler& other) 
    {
        if (this != &other) {
            name = other.name;
        }
        return *this;
    }

    // 获取名称
    std::string GetName() const 
    {
        return name;
    }
};

class Pager {
private:
    std::string number;

public:
    // 构造函数
    Pager(const std::string& num) : number(num) {}

    // 拷贝构造函数
    Pager(const Pager& other) : number(other.number) {}

    // 赋值运算符
    Pager& operator=(const Pager& other) 
    {
        if (this != &other) {
            number = other.number;
        }
        return *this;
    }

    // 获取号码
    std::string GetNumber() const 
    {
        return number;
    }
};

class BusinessTraveler : public Traveler 
{
private:
    Pager pager;

public:
    // 默认构造函数
    BusinessTraveler() : Traveler(""), pager("") {}

    // 构造函数
    BusinessTraveler(const std::string& n) : Traveler(n), pager(n) {}

    // 拷贝构造函数
    BusinessTraveler(const BusinessTraveler& other) : Traveler(other), pager(other.pager) {}

    // 赋值运算符
    BusinessTraveler& operator=(const BusinessTraveler& other) 
    {
        if (this != &other) {
            Traveler::operator=(other);
            pager = other.pager;
        }
        return *this;
    }

    // 获取 Pager
    Pager GetPager() const 
    {
        return pager;
    }
};

int main() 
{
    // 测试 Traveler 类
    Traveler t1("John Doe");
    Traveler t2 = t1;
    Traveler t3("Jane Doe");
    t3 = t1;
    cout << "Traveler t1: " << t1.GetName() << endl;
    cout << "Traveler t2: " << t2.GetName() << endl;
    cout << "Traveler t3: " << t3.GetName() << endl;

    // 测试 Pager 类
    Pager p1("123-456-7890");
    Pager p2 = p1;
    Pager p3("098-765-4321");
    p3 = p1;
    cout << "Pager p1: " << p1.GetNumber() << endl;
    cout << "Pager p2: " << p2.GetNumber() << endl;
    cout << "Pager p3: " << p3.GetNumber() << endl;

    // 测试 BusinessTraveler 类
    BusinessTraveler bt1("Alice");
    BusinessTraveler bt2 = bt1;
    BusinessTraveler bt3("Bob");
    bt3 = bt1;
    cout << "BusinessTraveler bt1: " << bt1.GetName() << ", Pager: " << bt1.GetPager().GetNumber() << endl;
    cout << "BusinessTraveler bt2: " << bt2.GetName() << ", Pager: " << bt2.GetPager().GetNumber() << endl;
    cout << "BusinessTraveler bt3: " << bt3.GetName() << ", Pager: " << bt3.GetPager().GetNumber() << endl;
    return 0;
}