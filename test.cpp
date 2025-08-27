#include<bits/stdc++.h>
using namespace std;
using LL = long long;

LL ans = 0;

void backtracing(int idx, vector<bool>& blocks, int& cnt,  int n) {
	if (idx > n*n-1) {
		ans = max(ans, LL(cnt));
        return;
    }
    backtracing(idx+1, blocks, cnt, n);
    
    if (idx+1 > n*n-1 || idx +n+1 > n*n-1) return;
    if (blocks[idx] && blocks[idx + 1] && blocks[idx + n] && blocks[idx + n + 1]) {
        cnt++;
        blocks[idx] = false;
        blocks[idx+1] = false;
            blocks[idx+n]= false;
            blocks[idx+n+1] = false;
        backtracing(idx+2, blocks, cnt, n);
        cnt--;
        blocks[idx] = true;
        blocks[idx+1] = true;
        blocks[idx+n]= true;
        blocks[idx+n+1] = true;
    }
}

int main()
{
    int n,k;
    cin>>n>>k;
	vector<bool> blocks(n*n, true);
    for(int c = 0; c < k; c++) {
		int i, j;
        cin>>i>>j;
        blocks[i*n+j] = false;
    }
    
    int cnt = 0;
    backtracing(0, blocks, cnt, n);
    
    cout<<ans;
	return 0;
}