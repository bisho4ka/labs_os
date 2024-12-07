#include <iostream>
#include "include/filter.h"

using namespace std;

int main() {
    int rows, cols, windowSize, threadsCount;

    cout << "Введите количество строк матрицы: ";
    cin >> rows;
    cout << "Введите количество столбцов матрицы: ";
    cin >> cols;

    mtr matrix(rows, vector<int>(cols));
    cout << "Введите элементы матрицы:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cin >> matrix[i][j];
        }
    }

    cout << "Введите размер окна (нечетное число): ";
    cin >> windowSize;

    cout << "Введите максимальное количество потоков: ";
    cin >> threadsCount;

    try {
        mtr result = applyMedianFilterParallel(matrix, windowSize, threadsCount);
        
        cout << "Результирующая матрица после применения медианного фильтра:" << endl;
        for (const auto& row : result) {
            for (const auto& val : row) {
                cout << val << " ";
            }
            cout << endl;
        }
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }

    return 0;
}
