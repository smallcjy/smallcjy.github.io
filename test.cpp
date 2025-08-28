#include <bits/stdc++.h>
using Seed = std::mt19937;

// Returns a random unsigned integer in the range [min, max]
size_t NextRandUInt(size_t seed, size_t min, size_t max) {
    Seed rng(seed);
    std::uniform_int_distribution<size_t> dist(min, max);
    return dist(rng);
}

// Returns a random double in the range [min, max)
double NextRandDouble(size_t seed, double min, double max) {
    Seed rng(seed);
    std::uniform_real_distribution<double> dist(min, max);
    return dist(rng);
}

int main() {
    size_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();

    std::cout<<NextRandUInt(timestamp, 0, 100)<<std::endl;
    std::cout<<NextRandUInt(timestamp, 0, 100)<<std::endl;

    std::cout<<NextRandUInt(timestamp, 1000, 10000)<<std::endl;
    std::cout<<NextRandUInt(timestamp, 1000, 10000)<<std::endl;

    return 0;
}