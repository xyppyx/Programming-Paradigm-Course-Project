#include<iostream>
#include<string>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;

//长方形结构体
struct Rectangle 
{
    int length;
    int width;

    bool operator>=(const Rectangle &r) const 
    {
        return length * width >= r.length * r.width;
    }

    friend std::ostream& operator<<(std::ostream &os, const Rectangle &r) 
    {
        os << r.length << "," << r.width;
        return os;
    }
};

//通用模板
template<typename T>
T Max(const T& t1, const T& t2) 
{
    return t1 >= t2 ? t1 : t2;
}

//string特化
template<>
std::string Max(const std::string& s1, const std::string& s2) 
{
    return s1.length() >= s2.length() ? s1 : s2;
}

//指针特化
template<typename T>
T* Max(T* t1, T* t2) 
{
    return *t1 >= *t2 ? t1 : t2;
}

template<typename T>
T Max(const T& t1, const T& t2, const T& t3) 
{
    return Max(Max(t1, t2), t3);
}

//string特化
template<>
std::string Max(const std::string& s1, const std::string& s2, const std::string& s3) 
{
    return Max(Max(s1, s2), s3);
}

//指针特化
template<typename T>
T* Max(T* t1, T* t2, T* t3) 
{
    return Max(Max(t1, t2), t3);
}


int main() 
{
    cout << Max(1, 2, 3) << endl;
    cout << Max(1.3, 2.4, 4.6) << endl;
    cout << Max('a', 'b', 'd') << endl;
    cout << Max(std::string("csd"), std::string("swe"), std::string("sfdfg")) << endl;

    Rectangle r1 = {1, 2};
    Rectangle r2 = {3, 4};
    Rectangle r3 = {2, 6};
    cout << Max(r1, r2, r3) << endl;

    Rectangle* r4 = new Rectangle{1, 2};
    Rectangle* r5 = new Rectangle{2, 6};
    Rectangle* r6 = new Rectangle{3, 4};
    cout << *Max(r4, r5, r6) << endl;

    delete r4;
    delete r5;
    delete r6;
    return 0;
}