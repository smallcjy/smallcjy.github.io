#include <iostream>
#include <cmath>
#include <vector>
#include <atomic>

#define ADD_FUNC_AND_COUT(x, y) (cout << (x) + (y) << endl)

using namespace std;

template <typename T>
concept HasFunc1 = requires(T a) {
    { a.func1(1, 2) } -> same_as<int>;
}

class ClassA {
public :
    int func1(int, int);
};

int main() {
}