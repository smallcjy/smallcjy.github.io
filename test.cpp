#include <bits/stdc++.h>
using namespace std;

vector<int> merge(vector<int>& arr1, vector<int>& arr2) {
    int n = arr1.size(); int m = arr2.size();
    vector<int> ans(n+m);
    int k = 0;
    int i = 0, j = 0;
    while(i < n && j < m) {
        if (arr1[i] < arr2[j]) {
            ans[k++] = arr1[i++];
        } else {
            ans[k++] = arr2[j++];
        }
    }
    while(i < n) ans[k++] = arr1[i++];
    while(j < m) ans[k++] = arr2[j++];
    return ans;
}

vector<int> mergek(vector<vector<int>>& arrays, int left, int right) {
    if(left == right) return arrays[left];
    int mid = (left + right) / 2;
    vector<int> larr = mergek(arrays, left, mid);
    vector<int> rarr = mergek(arrays, mid + 1, right);
    return merge(larr, rarr);
}

int main()
{
    vector<vector<int>> arrays = {{1, 4, 7}, {2, 5, 8}, {3, 6, 9}};
    vector<int> merged = mergek(arrays, 0, arrays.size() - 1);
    for(int x : merged) cout << x << " ";

    cout << endl;

    cout << sizeof(long) << endl;
    cout << sizeof(long long) << endl;
    return 0;
}