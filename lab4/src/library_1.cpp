#include <cmath>
#include "functions.h"

extern "C" {
    int PrimeCount(int A, int B) {
        int count = 0;
        for (int num = A; num <= B; num++) {
            if (num < 2)
                continue;

            bool is_prime = true;
            for (int div = 2; div <= (std::sqrt(num) + 1); div++) {
                if (num % div == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                count++;
        }
        return count;
    }

    float Pi(int K) {
        float pi = 0.0;
        for (int i = 0; i < K; i++) 
            pi += (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);

        return pi * 4;
    }
}