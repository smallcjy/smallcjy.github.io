// 动态规划：dp[i]: i个节点组成的二叉树共有dp[i]种情况
// 思路：因为所有节点的孩子数为0或2，好二叉树的左右子树一定也是好二叉树，所以根据左右子树的好二叉树情况可以组合推断出该节点数的情况。

#include<bits/stdc++.h>
using namespace std;
const int MOD = 1e9+7;

int main()
{
    int n;
    cin>>n;
    vector<int> dp(n+1, 0); // 偶数节点数的树组不成好二叉树
    dp[1] = 1; dp[3] = 1; // 初始化

    for(int i = 5; i <= n; i += 2) {
        for(int j = 1; j < i - 1; j += 2) {
            dp[i] += dp[j] * dp[i - j - 1];
            dp[i] %= MOD;
        }
    }

    cout<<dp[n];
    return 0;
}