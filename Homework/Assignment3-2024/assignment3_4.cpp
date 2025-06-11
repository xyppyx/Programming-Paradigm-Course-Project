
#include <iostream>
#include <array>
#include <optional>
#include <vector>
#include <unordered_map>
#include <optional>
#include <future>
//#include <boost/multiprecision/cpp_int.hpp>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;


//NOTE:please use UTF-8 encoding to read this file
/*
正确性: 正确。该函数能正确返回 Fibonacci 数列的第 n 项。
特点: 递归实现，易于理解，但时间复杂度为 O(2^n)，对于大 n 非常低效。
适用场景: 学习递归概念和 Fibonacci 数列的定义。
C++语法和细节: 递归函数的基础使用。
*/
// 使用简单递归方法计算第 n 个斐波那契数。
// 该实现具有指数时间复杂度 (O(2^n))，在 n 较大时非常低效。
// 
// 参数:
//   n: 要计算的斐波那契数的索引 (n >= 0)。
// 
// 返回值:
//   第 n 个斐波那契数。
int fibonacci_recursive(int n) 
{
    if (n <= 1) 
        return n;
    return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}

/*
正确性: 正确。能有效计算 Fibonacci 数。
特点: 迭代方法，时间复杂度为 O(n)，空间复杂度为 O(1)。
适用场景: 适合需要效率的场景。
C++语法和细节: 使用了循环和变量交换的技巧。
*/
// 使用迭代方法计算第 n 个斐波那契数。
// 该实现具有线性时间复杂度 (O(n)) 和常数空间复杂度 (O(1))。
// 
// 参数:
//   n: 要计算的斐波那契数的索引 (n >= 0)。
// 
// 返回值:
//   第 n 个斐波那契数。

int fibonacci_iterative(int n) 
{
    if (n <= 1) 
        return n;
    int a = 0, b = 1, temp;
    for (int i = 2; i <= n; ++i) 
    {
        temp = a + b;a = b;b = temp;
    }
    return b;
}

/*
正确性: 正确。动态规划实现。
特点: 使用数组存储中间结果，时间复杂度 O(n)，空间复杂度 O(n)。
适用场景: 中等 n 值的计算。
C++语法和细节: std::vector 的使用，动态分配内存。
*/
// 使用动态规划计算第 n 个斐波那契数。
// 该实现具有线性时间复杂度 (O(n)) 和线性空间复杂度 (O(n))。
// 
// 参数:
//   n: 要计算的斐波那契数的索引 (n >= 0)。
// 
// 返回值:
//   第 n 个斐波那契数。
int fibonacci_dp(int n) 
{
    if (n <= 1) 
        return n;
    std::vector<int> dp(n + 1, 0);
    dp[1] = 1;
    for (int i = 2; i <= n; ++i) 
    {
        dp[i] = dp[i -1] + dp[i -2];
    }
    return dp[n];
}

// 乘两个 2x2 矩阵。
// 该函数用于斐波那契数的矩阵指数法计算。
// 
// 参数:
//   a: 第一个 2x2 矩阵。
//   b: 第二个 2x2 矩阵。
// 
// 返回值:
//   矩阵 a 和 b 的乘积。
std::array<std::array<long long, 2>, 2> matrix_multiply(const std::array<std::array<long long, 2>, 2>& a, const std::array<std::array<long long, 2>, 2>& b) 
{
    std::array<std::array<long long, 2>, 2> result = {{0}};
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k)
                result[i][j] += a[i][k] * b[k][j];
    return result;
}

/*
正确性: 正确。利用矩阵快速幂法计算 Fibonacci。
特点: 效率高，时间复杂度 O(log n)，空间复杂度 O(1)。
适用场景: 大 n 的高效计算。
C++语法和细节: 使用位运算进行快速除法
*/
// 使用矩阵指数法计算第 n 个斐波那契数。
// 该实现具有对数时间复杂度 (O(log n)) 和常数空间复杂度 (O(1))。
// 
// 参数:
//   n: 要计算的斐波那契数的索引 (n >= 0)。
// 
// 返回值:
//   第 n 个斐波那契数。
// long long fibonacci_matrix(int n) 
// {   
//     if (n <= 1) 
//         return n;
//     std::array<std::array<long long, 2>, 2> base = {{1, 1}, {1, 0}};
//     std::array<std::array<long long, 2>, 2> result = {{1, 0}, {0, 1}};
//     n--;
//     while (n > 0) 
//     {
//         if (n & 1) 
//             result = matrix_multiply(result, base);
//         base = matrix_multiply(base, base);
//         n >>= 1;
//     }
//     return result[0][0];
// }

/*
正确性: 正确。使用备忘录优化递归。
特点: 时间复杂度 O(n)，空间复杂度 O(n)。
适用场景: 需要避免重复计算的递归场景。
C++语法和细节: std::unordered_map 用于存储计算结果。
*/
// 使用备忘录法计算第 n 个斐波那契数。
// 该实现具有线性时间复杂度 (O(n)) 和线性空间复杂度 (O(n))。
// 
// 参数:
//   n: 要计算的斐波那契数的索引 (n >= 0)。
//   memo: 用于存储计算结果的映射的引用。
// 
// 返回值:
//   第 n 个斐波那契数。
long long fibonacci_memoization(int n, std::unordered_map<int, long long>& memo) 
{
    if (n <= 1) 
        return n;
    if (memo.find(n) != memo.end()) 
        return memo[n];
    memo[n] = fibonacci_memoization(n -1, memo) + fibonacci_memoization(n -2, memo);
    return memo[n];
}

/*
正确性: 正确。返回一个 std::optional 用于处理负输入。
特点: 使用 std::optional 提供安全性。
适用场景: 需要处理非法输入的场合。
C++语法和细节: std::optional 的使用。
*/
// 安全地计算第 n 个斐波那契数，使用 std::optional。
// 该实现对负数索引返回 std::nullopt。
// 
// 参数:
//   n: 要计算的斐波那契数的索引 (可以是负数)。
// 
// 返回值:
//   如果 n >= 0，则返回第 n 个斐波那契数，否则返回 std::nullopt。
// std::optional<long long> fibonacci_safe(int n) 
// {
//     if (n < 0) 
//         return std::nullopt;
//     if (n <= 1) 
//         return n;
//     long long a = 0, b = 1, temp;
//     for (int i = 2; i <= n; ++i) 
//     {
//         temp = a + b;
//         a = b;
//         b = temp;
//     }
//     return b;
// }

/*
正确性: 正确，能够计算第 n 个斐波那契数。
特点: 通过 constexpr 在编译时计算，提高运行时性能。
适用场景: 需要在编译时确定斐波那契数值的情况。
C++语法和细节: constexpr 的使用。
*/
// 使用 constexpr 在编译时计算第 n 个斐波那契数。
// 该实现具有线性时间复杂度 (O(n))。
// 
// 参数:
//   n: 要计算的斐波那契数的索引 (n >= 0)。
// 
// 返回值:
//   第 n 个斐波那契数。
constexpr long long fibonacci_constexpr(int n) 
{
    if (n <= 1) 
        return n;
    long long a = 0, b = 1;
    for (int i = 2; i <= n; ++i) 
    {
        long long temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

/*
正确性: 正确，但递归调用可能导致性能问题。
特点: 使用 std::async 进行异步计算。
适用场景: 处理较大 n 值且支持并行计算的情况。
C++语法和细节: std::async 的使用。
*/
// 使用并行计算计算第 n 个斐波那契数。
// 该实现利用 std::async 来异步计算，可能在某些情况下提升性能。
// 
// 参数:
//   n: 要计算的斐波那契数的索引 (n >= 0)。
// 
// 返回值:
//   第 n 个斐波那契数。
long long fibonacci_parallel(int n) 
{
    if (n <= 1) 
        return n;
    auto future = std::async(std::launch::async, fibonacci_parallel, n -2);
    long long result = fibonacci_parallel(n -1);
    return result + future.get();
}

/*
正确性: 正确，能够处理较大的斐波那契数。
特点: 使用大整数类型避免溢出。
适用场景: 计算非常大的斐波那契数。
C++语法和细节: boost::multiprecision 的使用。
*/
// 使用大整数类型计算第 n 个斐波那契数。
// 该实现适合计算较大的斐波那契数，避免整数溢出。
// 
// 参数:
//   n: 要计算的斐波那契数的索引 (n >= 0)。
// 
// 返回值:
//   第 n 个斐波那契数。
// boost::multiprecision::cpp_int fibonacci_bigint(int n) 
// {
//     if (n <= 1) 
//         return n;
//     boost::multiprecision::cpp_int a = 0, b = 1, temp;
//     for (int i = 2; i <= n; ++i) 
//     {
//         temp = a + b;
//         a = b;
//         b = temp;
//     }
//     return b;
// }
    
/*
正确性: 正确，能够在编译时计算 Fibonacci 数。
特点: 使用模板元编程进行编译时计算。
适用场景: 需要常量表达式的情况。
C++语法和细节: 模板元编程的使用。
*/
// 使用模板元编程计算第 N 个斐波那契数。
// 该实现利用编译期计算，在编译时确定值。
// 
// 参数:
//   N: 要计算的斐波那契数的索引 (N >= 0)。
// 
// 返回值:
//   第 N 个斐波那契数。
template<int N, typename = std::enable_if_t<(N >= 0)>>
struct Fibonacci 
{
    static constexpr long long value = Fibonacci<N-1>::value + Fibonacci<N-2>::value;
};

template<>
struct Fibonacci<0> 
{
    static constexpr long long value = 0;
};

template<>
struct Fibonacci<1> 
{
    static constexpr long long value = 1;
};


//我的代码
namespace my
{
    typedef std::vector<std::vector<unsigned long long>> Matrix;

    // 矩阵乘法
    Matrix multiply(const Matrix &a, const Matrix &b) 
    {
        int n = a.size();
        Matrix c(n, std::vector<unsigned long long>(n, 0));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                for (int k = 0; k < n; ++k) {
                    c[i][j] = (c[i][j] + a[i][k] * b[k][j]);
                }
            }
        }
        return c;
    }

    // 矩阵快速幂
    Matrix matrix_pow(Matrix a, int p) 
    {
        int n = a.size();
        Matrix res(n, std::vector<unsigned long long>(n, 0));
        for (int i = 0; i < n; ++i)
            res[i][i] = 1;

        while (p > 0) 
        {
            if (p % 2 == 1) 
                res = multiply(res, a);
            a = multiply(a, a);
            p >>= 1;
        }
        return res;
    }

    // 使用矩阵快速幂计算斐波那契数
    unsigned long long Fibonacci(int n) 
    {
        if (n == 0) return 0;
        if (n == 1) return 1;
        Matrix F = {{1, 1}, {1, 0}};
        Matrix res = matrix_pow(F, n - 1);
        return res[0][0];
    }
}

int main()
{
    int n;
    cout << "请输入要计算的斐波那契数的索引（0 <= n < 94): ";
    cin >> n;
    cout << "第 " << n << " 个斐波那契数是: " << my::Fibonacci(n) << endl;
    return 0;
}