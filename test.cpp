#include<bits/stdc++.h>
using namespace std;

int main()
{
    int n, m;
    cin>>n>>m;
    string str;
    cin>>str;

    for(int i = 0; i < str.size(); i++) {
        if (str[i] == '?') {
            bool flag = false;
            for(char j = 'a'; j < 'a' + m; j++) {
                if ((i==0 || str[i-1] != j) && (i==str.size()-1 || str[i+1] != j)) {
                    flag = true;
                    str[i] = j;
                }

                if (!flag) {
                    str[i] = str[i-1];
                }
            }
        }
    }

    int ans = 0;
    cout<<str<<endl;
    for(int i = 0; i < str.size() - 1; i++) {
        ans += str[i] != str[i+1] ? 1 : 0;
    }

    cout<<ans;
    return 0;
}