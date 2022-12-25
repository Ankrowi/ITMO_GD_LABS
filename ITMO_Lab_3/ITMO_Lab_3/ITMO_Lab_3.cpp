#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <fstream>

#include "../../ITMO_Lab_2/ITMO_Lab_2/Array.h"
#include "Sort.h"
#include "windows.h"
#include "quicksort.h"

constexpr int Iterations = 1000;

int main()
{

    std::default_random_engine generator;
    std::uniform_int_distribution<int> rand(1, 100);
    generator.seed(time(NULL));
    std::ofstream saveTime("time.txt");
    for (int size = 3; size <= 100; size++) {
        int _ = 0;
        auto insertionTime = std::chrono::nanoseconds(0);
        auto quickSortTime = std::chrono::nanoseconds(0);

        while (_ < Iterations) {
            Array<int> insertionArray(size);
            for (int j = size - 1; j >= 0; --j)
                insertionArray.insert(j);
            Array<int> quickSortArray(size);
            for (int j = 0; j < size; j++)
                quickSortArray.insert(rand(generator));
            
            auto start = std::chrono::high_resolution_clock::now();
            insertionSort(insertionArray.begin(), insertionArray.end() - 1, [](int a, int b) {  return a < b; });
            auto end = std::chrono::high_resolution_clock::now();
            insertionTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

            start = std::chrono::high_resolution_clock::now();
            sort(quickSortArray.begin(), quickSortArray.end(), [](int a, int b) {  return a < b; });
            end = std::chrono::high_resolution_clock::now();
            quickSortTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

            _++;
        }

        insertionTime /= 1e3;
        quickSortTime /= 1e3;

        if (saveTime.is_open()) {
            saveTime << size << ";" << insertionTime.count() << ";" << quickSortTime.count() << std::endl;
        }

    }
	
    return 0;
}

