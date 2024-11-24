#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

class ArrayGenerator {
public:
  ArrayGenerator() : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}

  std::vector<long long> generateRandomArray(int size, int minValue = 0, int maxValue = 6000) {
    std::uniform_int_distribution<long long> dist(minValue, maxValue);
    std::vector<long long> arr(size);
    for (int i = 0; i < size; ++i) {
      arr[i] = dist(rng);
    }
    return arr;
  }

  std::vector<long long> generateReverseSortedArray(int size) {
    std::vector<long long> arr(size);
    for (int i = 0; i < size; ++i) {
      arr[i] = size - i;
    }
    return arr;
  }

  std::vector<long long> generateAlmostSortedArray(int size, int swaps = 10) {
    std::vector<long long> arr(size);
    for (int i = 0; i < size; ++i) {
      arr[i] = i;
    }
    std::uniform_int_distribution<int> dist(0, size - 1);
    for (int i = 0; i < swaps; ++i) {
      int idx1 = dist(rng);
      int idx2 = dist(rng);
      std::swap(arr[idx1], arr[idx2]);
    }
    return arr;
  }

private:
  std::mt19937_64 rng;
};