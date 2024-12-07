#pragma once

#include <vector>

using mtr = std::vector<std::vector<int>>;

mtr applyMedianFilterParallel(const mtr& matrix, int windowSize, int threadsCount);
mtr applyMedianFilter(const mtr& matrix, int windowSize);

