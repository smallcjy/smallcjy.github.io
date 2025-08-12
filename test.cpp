#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

struct Line {
    pair<double, double> begin, end;
    double distance;
};

double countDis(double x, double y) {
    return sqrt(x * x + y * y);
}

int main() {
    int n, s;
    cin >> n;

    vector<pair<double, double>> points(n);
    for (int i = 0; i < n; i++) {
        cin >> points[i].first >> points[i].second;
    }

    cin >> s;

    vector<Line> lines;
    for (int i = 1; i < n; i++) {
        double dis = countDis(points[i].first - points[i - 1].first, points[i].second - points[i - 1].second);
        lines.push_back({
            {points[i - 1].first, points[i - 1].second},
            {points[i].first, points[i].second},
            dis
        });
    }

    double cur_dis = 0;
    cout.precision(5);
    for (int i = 0; i < n - 1; i++) {
        Line cur_line = lines[i];
        double dx = cur_line.end.first - cur_line.begin.first;
        double dy = cur_line.end.second - cur_line.begin.second;
        double distance = cur_line.distance;

        if (cur_dis < distance) {
            double ratio = cur_dis / distance;
            double x = cur_line.begin.first + ratio * dx;
            double y = cur_line.begin.second + ratio * dy;
            cout << fixed << x << " " << y << endl;
        }

        while (cur_dis < distance) {
            cur_dis += s;
            if (cur_dis > distance) break;

            double ratio = cur_dis / distance;
            double x = cur_line.begin.first + ratio * dx;
            double y = cur_line.begin.second + ratio * dy;

            cout << fixed << x << " " << y << endl;
        }
        cur_dis -= distance;
    }

    return 0;
}