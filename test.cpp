#include<bits/stdc++.h>
using namespace std;
using LL = long long;

void solve() {
    int n, x;
    cin >> n >> x;
    vector<int> nums(n);
    for(int i = 0; i < n; i++) cin >> nums[i];

    if(n == 0) {
        cout << 0 << endl;
        return;
    }

    vector<vector<LL>> dp(n, vector<LL>(2));
    dp[0][0] = nums[0];  // 不使用修改
    dp[0][1] = x;        // 使用修改（将第一个元素改为x）
    LL max_global = max(dp[0][0], dp[0][1]);

    for(int i = 1; i < n; i++) {
        // 不使用修改的情况
        dp[i][0] = max(LL(nums[i]), dp[i-1][0] + nums[i]);
        
        // 使用修改的情况有两种可能：
        // 1. 在当前元素使用修改权（将nums[i]改为x）
        LL case1 = max(LL(x), dp[i-1][0] + x);
        // 2. 不在当前元素使用修改权（之前已经用过）
        LL case2 = max(LL(nums[i]), dp[i-1][1] + nums[i]);
        dp[i][1] = max(case1, case2);
        
        // 更新全局最大值
        max_global = max({max_global, dp[i][0], dp[i][1]});
    }

    // 确保至少选择一个元素（题目可能允许空子数组和为0）
    max_global = max(max_global, LL(0));
    cout << max_global << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while(t--) {
        solve();
    }
    return 0;
}