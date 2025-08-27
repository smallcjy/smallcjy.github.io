#include <bits/stdc++.h>
using namespace std;
const int dirs[5] = {1, 0, -1, 0, 1};

bool check(int k, vector<vector<int>> &grid, int power)
{
    int n = grid.size();
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    queue<vector<int>> q;
    q.push({0, 0, 0});
    visited[0][0] = true;
    while (!q.empty())
    {
        int s = q.size();
        auto &cur = q.front();
        q.pop();
        int cur_x = cur[0], cur_y = cur[1], cur_step = cur[2];
        if (cur_x == n - 1 && cur_y == n - 1 && cur_step <= k)
        {
            return true;
        }

        for (int j = 0; j < 4; j++)
        {
            int nx = cur_x + dirs[j];
            int ny = cur_y + dirs[j + 1];
            if (0 <= nx && nx < n && 0 <= ny && ny < n && !visited[nx][ny] && power >= grid[nx][ny])
            {
                q.push({nx, ny, cur_step + 1});
                visited[nx][ny] = true;
            }
        }
    }

    return false;
}

int main()
{
    int n, k;
    cin >> n >> k;
    vector<vector<int>> grid(n, vector<int>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> grid[i][j];
        }
    }

    // 当前辐射值不满足条件时，所有小于当前辐射值的情况都不满足，具备二段性。
    // 二分答案，查找最左值
    int l = 0;
    int r = 10000;
    int ans;
    while (l <= r)
    {
        int mid = (l + r) / 2;
        if (check(k, grid, mid))
        {
            ans = mid;
            r = mid - 1;
        }
        else
            l = mid + 1;
    }

    cout << ans;
    return 0;
}