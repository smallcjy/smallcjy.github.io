#include <bits/stdc++.h>
using namespace std;

const int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

struct State {
    int x, y;
    int time;
    // 用于 priority_queue 的比较，时间小的优先
    bool operator<(const State& other) const {
        return time > other.time;  // 最小堆
    }
};

int main() {
    int m, n;
    cin >> m >> n;
    vector<vector<int>> grid(m, vector<int>(n));
    int j, k;
    cin >> j >> k;

    // 输入网格
    for (int i = 0; i < m; i++) {
        for (int l = 0; l < n; l++) {
            cin >> grid[i][l];
        }
    }

    // Dijkstra 初始化
    vector<vector<int>> dist(m, vector<int>(n, INT_MAX));
    priority_queue<State> pq;
    dist[j][k] = 0;
    pq.push({j, k, 0});

    while (!pq.empty()) {
        State cur = pq.top();
        pq.pop();

        // 如果当前时间已经大于记录的最短时间，跳过
        if (cur.time > dist[cur.x][cur.y]) {
            continue;
        }

        // 遍历四个方向
        for (auto& dir : dirs) {
            int nx = cur.x + dir[0];
            int ny = cur.y + dir[1];

            // 检查边界和是否为空地
            if (nx < 0 || nx >= m || ny < 0 || ny >= n || grid[nx][ny] == 0) {
                continue;
            }

            // 计算新的传播时间
            int new_time = cur.time + grid[cur.x][cur.y];

            // 如果更优，更新并加入堆
            if (new_time < dist[nx][ny]) {
                dist[nx][ny] = new_time;
                pq.push({nx, ny, new_time});
            }
        }
    }

    // 检查所有信号塔是否可达
    int max_time = 0;
    for (int i = 0; i < m; i++) {
        for (int l = 0; l < n; l++) {
            if (grid[i][l] != 0) {  // 是信号塔
                if (dist[i][l] == INT_MAX) {
                    cout << -1 << endl;
                    return 0;
                }
                max_time = max(max_time, dist[i][l]);
            }
        }
    }

    cout << max_time << endl;
    return 0;
}