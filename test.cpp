#include <iostream>
#include <vector>
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
    // 指针的数据类型 是 type*
    // int* char* float* double*
    int a = 0; // 地址00000086E51FFC1C存放0
    printf("Address of a: %p\n", &a);

    int* ptr = &a;

    printf("Address of ptr: %p\n", ptr);
    printf("Address of ptr: %p\n", &ptr);

    printf("Value pointed to by ptr: %d\n", *ptr);

    // 00000086E51FFC10 -> 00000086E51FFC1C
    // 00000086E51FFC1C -> 5

    int* ptr2 = ptr;
    int* ptr3 = ptr;

    int b = 3;
    int c = b;

    return 0;
}
// 64 位输出请用 printf("%lld")