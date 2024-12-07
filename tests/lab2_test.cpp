#include <gtest/gtest.h>
#include "../include/filter.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

TEST(MedianFilterTests, SingleThread) {
    mtr matrix = {
        {2, 3, 10},
        {3, 12, 13},
        {1, 4, -1}
    };
    mtr expectedMatrix = {
        {5, 8, 11},
        {3, 4, 8},
        {3, 3, 8}
    };
    
    mtr result = applyMedianFilter(matrix, 2);

    for(size_t i = 0; i < matrix.size(); i++) {
        for(size_t j = 0; j < matrix[0].size(); j++) {
            EXPECT_EQ(expectedMatrix[i][j], result[i][j]);
        }
    }
}

TEST(MedianFilterTests, MultiThread) {
    mtr matrix(100, vector<int>(100));
    int value = 1;

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            matrix[i][j] = value;
            value += 1;
        }
    }
    
    mtr stResult = applyMedianFilter(matrix, 5);
    mtr mtResult = applyMedianFilterParallel(matrix, 5, 4);

    for(size_t i = 0; i < matrix.size(); i++) {
        for(size_t j = 0; j < matrix[0].size(); j++) {
            EXPECT_EQ(stResult[i][j], mtResult[i][j]);
        }
    }
}

TEST(MedianFilterTests, MultiIsFasterThanSingle) {
    mtr matrix(100, vector<int>(100));
    int value = 1;

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            matrix[i][j] = value;
            value += 1;
        }
    }
    auto start1 = high_resolution_clock::now();
    mtr stResult = applyMedianFilter(matrix, 11);
    auto end1 = high_resolution_clock::now();

    auto durationST = duration_cast<milliseconds>(end1 - start1);

    auto start2 = high_resolution_clock::now();
    mtr mtResult = applyMedianFilterParallel(matrix, 11, 4);
    auto end2 = high_resolution_clock::now();

    auto durationMT = duration_cast<milliseconds>(end2 - start2);   

    EXPECT_GT(durationST.count(), durationMT.count());
}