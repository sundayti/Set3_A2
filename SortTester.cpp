#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>

class SortTester {
public:
  SortTester(const std::string& randomFile, const std::string& reversedFile, const std::string& almostSortedFile)
          : randomArray(readArrayFromFile(randomFile)),
            reversedArray(readArrayFromFile(reversedFile)),
            almostSortedArray(readArrayFromFile(almostSortedFile)) {}

  void test(int threshold, const std::vector<int>& Ns) {
    std::ofstream ms_random_out("merge_sort_random.txt");
    std::ofstream ms_reversed_out("merge_sort_reversed.txt");
    std::ofstream ms_almost_out("merge_sort_almost_sorted.txt");

    std::ofstream hs_random_out("hybrid_sort_random.txt");
    std::ofstream hs_reversed_out("hybrid_sort_reversed.txt");
    std::ofstream hs_almost_out("hybrid_sort_almost_sorted.txt");

    for (int N : Ns) {
      testSort(randomArray, N, ms_random_out, mergeSort);
      testSort(reversedArray, N, ms_reversed_out, mergeSort);
      testSort(almostSortedArray, N, ms_almost_out, mergeSort);
    }

    for (int N : Ns) {
      testSort(randomArray, N, hs_random_out, [&](std::vector<long long>& arr, int left, int right) {
        hybridSort(arr, left, right, threshold);
      });
      testSort(reversedArray, N, hs_reversed_out, [&](std::vector<long long>& arr, int left, int right) {
        hybridSort(arr, left, right, threshold);
      });
      testSort(almostSortedArray, N, hs_almost_out, [&](std::vector<long long>& arr, int left, int right) {
        hybridSort(arr, left, right, threshold);
      });
    }

    ms_random_out.close();
    ms_reversed_out.close();
    ms_almost_out.close();

    hs_random_out.close();
    hs_reversed_out.close();
    hs_almost_out.close();

    std::cout << "Тестирование завершено. Результаты сохранены в файлы.\n";
  }

private:
  std::vector<long long> randomArray;
  std::vector<long long> reversedArray;
  std::vector<long long> almostSortedArray;

  static std::vector<long long> readArrayFromFile(const std::string& filename) {
    std::vector<long long> arr;
    std::ifstream infile(filename);
    long long value;
    while (infile >> value) {
      arr.push_back(value);
    }
    return arr;
  }

  static void mergeSort(std::vector<long long>& arr, int left, int right) {
    if (left >= right)
      return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }

  static void hybridSort(std::vector<long long>& arr, int left, int right, int threshold) {
    if (left >= right)
      return;
    if (right - left + 1 <= threshold) {
      insertionSort(arr, left, right);
      return;
    }
    int mid = left + (right - left) / 2;
    hybridSort(arr, left, mid, threshold);
    hybridSort(arr, mid + 1, right, threshold);
    merge(arr, left, mid, right);
  }

  static void insertionSort(std::vector<long long>& arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
      long long key = arr[i];
      int j = i - 1;
      while (j >= left && arr[j] > key)
        arr[j + 1] = arr[j--];
      arr[j + 1] = key;
    }
  }

  static void merge(std::vector<long long>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<long long> L(n1), R(n2);
    for (int i = 0; i < n1; ++i)
      L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
      R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
      arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    while (i < n1)
      arr[k++] = L[i++];
    while (j < n2)
      arr[k++] = R[j++];
  }

  void testSort(const std::vector<long long>& sourceArray, int N, std::ofstream& outFile,
                void (*sortFunction)(std::vector<long long>&, int, int)) {
    std::vector<long long> arr(sourceArray.begin(), sourceArray.begin() + N);

    auto start = std::chrono::high_resolution_clock::now();
    sortFunction(arr, 0, N - 1);
    auto end = std::chrono::high_resolution_clock::now();

    double time_us = std::chrono::duration<double, std::micro>(end - start).count();
    outFile << N << " " << time_us << "\n";
  }

  template <typename SortFunction>
  void testSort(const std::vector<long long>& sourceArray, int N, std::ofstream& outFile, SortFunction sortFunction) {
    std::vector<long long> arr(sourceArray.begin(), sourceArray.begin() + N);

    auto start = std::chrono::high_resolution_clock::now();
    sortFunction(arr, 0, N - 1);
    auto end = std::chrono::high_resolution_clock::now();

    double time_us = std::chrono::duration<double, std::micro>(end - start).count();
    outFile << N << " " << time_us << "\n";
  }
};