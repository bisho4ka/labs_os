#include "../include/filter.h"
#include <iostream>
#include <algorithm>
#include <pthread.h>
#include <memory>
#include <stdexcept>

using namespace std;

int calculateMedian(vector<int> window) {
    if (window.empty())
        throw invalid_argument("Неверные входные параметры");
        
    int size = window.size();
    
    if (size % 2 == 1) {
        nth_element(window.begin(), window.begin() + size / 2, window.end());
        return window[size / 2];
    } 
    else {
        nth_element(window.begin(), window.begin() + size / 2, window.end());
        int a = window[size / 2];
        nth_element(window.begin(), window.begin() + size / 2 - 1, window.end());
        int b = window[size / 2 - 1];
        return (a + b) / 2;
    }
}

struct FilterParams {
    const mtr* matrix;
    mtr* result;
    int startRow;
    int rowsPerThread;
    int windowSize;

    FilterParams(const mtr* input, mtr* output, int startRow, int rowsPerThread, int windowSize)
        : matrix(input), result(output), startRow(startRow), rowsPerThread(rowsPerThread), windowSize(windowSize) {}
};

int getWindowMedian(const mtr& matrix, int row, int col, int windowSize) {
    int halfSize = windowSize / 2;
    int rows = matrix.size();
    int cols = matrix[0].size();
    vector<int> window;

    for (int i = row - halfSize; i <= row + halfSize; i++) {
        for (int j = col - halfSize; j <= col + halfSize; j++) {
            if (i >= 0 && i < rows && j >= 0 && j < cols)
                window.push_back(matrix[i][j]);
        }
    }
    return calculateMedian(window);
}

void* applyFilter(void* args) {
    FilterParams *params = (FilterParams*)(args);
    for (int i = params->startRow; i < (int)params->matrix->size(); i += params->rowsPerThread) {
        for (int j = 0; j < (int)params->matrix->front().size(); j++) {
            (*(params->result))[i][j] = getWindowMedian(*(params->matrix), i, j, params->windowSize);
        }
    }
    delete params;
    pthread_exit(NULL);
}

mtr applyMedianFilterParallel(const mtr& matrix, int windowSize, int threadsCount) {
    if (matrix.empty() || windowSize <= 0 || windowSize % 2 == 0) 
        throw invalid_argument("Неверные входные параметры");

    int rows = matrix.size();
    int cols = matrix[0].size();
    mtr result(rows, vector<int>(cols));
    vector<pthread_t> threads(threadsCount);

    for (int i = 0; i < threadsCount; i++) {
        FilterParams *params = new FilterParams{&matrix, &result, i, threadsCount, windowSize};
        pthread_create(&threads[i], NULL, applyFilter, (void*)params);
    }
 
    for (int i = 0; i < threadsCount; i++) 
        pthread_join(threads[i], NULL);
    
    return result;
}

mtr applyMedianFilter(const mtr& matrix, int windowSize) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    int halfSize = windowSize / 2;

    mtr result(rows, vector<int>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            vector<int> window;

            for (int m = i - halfSize; m <= i + halfSize; m++) {
                for (int n = j - halfSize; n <= j + halfSize; n++) {
                    if (m >= 0 && m < rows && n >= 0 && n < cols) {
                        window.push_back(matrix[m][n]);
                    }
                }
            }

            result[i][j] = calculateMedian(window);
        }
    }

    return result;
}