#include <gtest/gtest.h>
#include <parent.h>

TEST(Test_Lab1, CorrectDiv_1) {
    const char* fileName = "test.txt";
    
    std::ofstream testInput("input.txt");
    testInput << fileName << "\n16.25 6.5 1";
    testInput.close();

    std::ifstream testFile("input.txt");
    Parent(testFile);
    
    std::ifstream resultFile(fileName);

    std::string result;
    std::getline(resultFile, result);
    EXPECT_EQ(result, "Частное: 2.5");

    std::remove(fileName);
    std::remove("input.txt");
}

TEST(Test_Lab1, CorrectDiv_2) {
    const char* fileName = "test.txt";
    
    std::ofstream testInput("input.txt");
    testInput << fileName << "\n24415192.926509 129.1 1234,213 153.23";
    testInput.close();

    std::ifstream testFile("input.txt");
    Parent(testFile);
    
    std::ifstream resultFile(fileName);

    std::string result;
    std::getline(resultFile, result);
    EXPECT_EQ(result, "Частное: 153.256");

    std::remove(fileName);
    std::remove("input.txt");
}