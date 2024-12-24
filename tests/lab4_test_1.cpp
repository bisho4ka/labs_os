#include <gtest/gtest.h>
#include <cmath>
#include <functions.h>

// Тест для функции PrimeCount (наивный поиск простых чисел)
TEST(PrimeCountTest, Implementation1) {
    int result_1 = PrimeCount(1, 10); // 2, 3, 5, 7
    EXPECT_EQ(result_1, 4); 
    
    int result_2 = PrimeCount(10, 20); // 11, 13, 17, 19
    EXPECT_EQ(result_2, 4);

    int result_3 = PrimeCount(20, 30); // 23, 29
    EXPECT_EQ(result_3, 2);

    int result_4 = PrimeCount(1, 1);
    EXPECT_EQ(result_4, 0);
}

// Тест для функции Pi (ряд Лейбница)
TEST(PiTest, Implementation1) {
    float result_1 = Pi(1);
    EXPECT_NEAR(result_1, 4.0, 1e-5);

    float result_2 = Pi(10);
    EXPECT_NEAR(result_2, 3.04184, 1e-5);

    float result_3 = Pi(100);
    EXPECT_NEAR(result_3, 3.13159, 1e-5);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}