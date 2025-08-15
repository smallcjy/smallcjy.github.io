/**
 * Definition for a point.
 * struct Point {
 *     int x;
 *     int y;
 *     Point() : x(0), y(0) {}
 *     Point(int a, int b) : x(a), y(b) {}
 * };
 */

struct Point {
    int x;
    int y;
};

#include<bits/stdc++.h>
using namespace std;

const int dirs_x[8] = {1, 1, 2, 2, -1, -1, -2, -2};
const int dirs_y[8] = {2, -2, 1, -1, 2, -2, 1, -1};

class Solution {
public:
    /**
     * @param grid: a chessboard included 0 (false) and 1 (true)
     * @param source: a point
     * @param destination: a point
     * @return: the shortest path 
     */
    int shortestPath(vector<vector<bool>> &grid, Point &source, Point &destination) {
        int n = grid.size(),m = grid[0].size();
        if (n == 0 || m == 0) {
            return -1;
        }
        
        int dx[] = {1, 1, 2, 2, -1, -1, -2, -2};
        int dy[] = {2, -2, 1, -1, 2, -2, 1, -1};
        queue<Point> q;
        q.push(source);
        grid[source.x][source.y] = true;
        int ans = 0;
        while (!q.empty()) {
            int len_q = q.size();
            for (int k = 0; k < len_q; k++) {
                Point cur = q.front();
                q.pop();
                //到达终点，返回距离
                if (cur.x == destination.x && cur.y == destination.y) {
                    return ans;
                }
                for (int i = 0; i < 8; i++) {
                    Point next;
                    next.x = cur.x + dx[i];
                    next.y = cur.y + dy[i];
                    //判断下一跳可否到达
                    if (is_in_bound(next,n,m) && grid[next.x][next.y] == false) {
                        q.push(next);
                        grid[next.x][next.y] = true;
                    }
                }
            }
            ans++;
        }
        return -1;
    }
    //判断下一跳是否越界
    bool is_in_bound(Point next,int n,int m) {
        return 0 <= next.x && next.x < n && 0 <= next.y && next.y < m;
    }
};