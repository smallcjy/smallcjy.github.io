// Windows下的毫秒级定时器任务
#include <windows.h>
#include <functional>
#include <stdio.h>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>


const uint64_t SECOND = 1000;

// 定时器任务回调类型
using TimerCallback = std::function<void()>;
HANDLE run_ms_timer_win(int initial_msec, int interval_msec, TimerCallback cb);

struct Task {
    uint64_t tid;
    uint64_t timestamp;
    TimerCallback callback;
};

std::function<bool(const Task&, const Task&)> CMP = [](const Task& a, const Task& b) {
    return a.timestamp > b.timestamp;
};

class TimerQueue {
public:
    TimerQueue() : task_queue(CMP), next_task_id(1) {}
    ~TimerQueue() = default;

    void start() {
        timer_handle = run_ms_timer_win(1, 1, [this](){ update(); });
    }
    void update() {
        auto now = GetTickCount();
        while (!task_queue.empty()) {
            Task task = task_queue.top();
            if (task.timestamp <= now) {
                task.callback();
                task_queue.pop();
            } else {
                break;
            }
        }
    }

    // 添加定时任务 :  毫秒级
    uint64_t add_task(uint64_t delay_ms, TimerCallback cb) {
        uint64_t tid = next_task_id++;
        uint64_t timestamp = GetTickCount() + delay_ms;
        task_queue.push({ tid, timestamp, cb });
        return tid;
    }
private:
    uint64_t next_task_id;
    HANDLE timer_handle;
    std::priority_queue<Task, std::vector<Task>, decltype(CMP)> task_queue;
};

// 首次触发时间，重复间隔，回调函数
HANDLE run_ms_timer_win(int initial_msec, int interval_msec, TimerCallback cb) {
    HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
    if (!hTimer) return NULL;

    LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -initial_msec * 10000LL; // 负值表示相对时间，单位100纳秒

    // 设置定时器
    SetWaitableTimer(hTimer, &liDueTime, interval_msec, NULL, NULL, FALSE);

    while (1) {
        DWORD dwRet = WaitForSingleObject(hTimer, INFINITE);
        if (dwRet == WAIT_OBJECT_0) {
            cb(); // 回调
        }
    }

    return hTimer;
}

TimerQueue GTimer;

void printSize(size_t size);

void print(int, int);

// 示例用法
int main() {
    int ptr = 10;
    int r_ptr = std::move(ptr);

    std::cout<<ptr<<' '<<&ptr<<std::endl;
    std::cout<<r_ptr<<' '<<&r_ptr;
    return 0;
}