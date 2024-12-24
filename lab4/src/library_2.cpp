#include <cmath>
#include <vector>
#include "functions.h"

extern "C" {
    int PrimeCount(int A, int B) {
        if (B < 2)
            return 0;

        std::vector<bool> is_prime(B + 1, true);
        is_prime[0] = is_prime[1] = false;

        for (int p = 2; p <= std::sqrt(B); p++) {
            if (is_prime[p]) {
                for (int product  = p * p; product <= B; product += p) 
                    is_prime[product] = false;
            }
        }

        int count = 0;
        for (int i = A; i <= B; i++) {
            if (is_prime[i])
                count++;
        }
        return count;
    }

    float Pi(int K) {
        float pi = 1.0;
        for (int i = 1; i <= K; i++) 
            pi *= ((2.0 * i) / (2.0 * i - 1) * (2.0 * i) / (2.0 * i + 1));
        
        return pi * 2;
    }
}