#include <bits/stdc++.h>
using namespace std;

int main()
{
    vector<vector<int>> dp(10000, vector<int>(2000, INT_MAX));

    // 错误认知示例：只是 vector 对象大小
    cout << "sizeof(dp) = " << sizeof(dp) << endl;

    // 估算数据内存（假设所有行容量相同）
    size_t rows = dp.size();
    size_t cols = dp[0].size();
    size_t perRowCapacity = dp[0].capacity();
    size_t dataBytes = rows * perRowCapacity * sizeof(int);
    size_t innerHeaderBytes = rows * sizeof(vector<int>);
    cout << "approx data bytes = " << dataBytes << endl;
    cout << "inner headers bytes ~= " << innerHeaderBytes << endl;

    // 访问示例
    dp[123][456] = 7;

    // 扁平化替代：更好缓存局部性
    vector<int> flat(rows * cols, INT_MAX);
    size_t i = 123, j = 456;
    flat[i * cols + j] = 7;

    return 0;
}