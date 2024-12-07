#include <gtest/gtest.h>
#include "../lab3/include/parent.h"

// Функция для создания входного файла
void CreateFile(const string& inputFile, const string& outputFile, const string& file) {
    ofstream inputFileStream(inputFile);
    inputFileStream << outputFile << "\n" << file;
}

// Функция для проверки содержимого выходного файла
void VerifyOutputFile(const string& outputFile, const string& expectedContent) {
    ifstream resultStream(outputFile);
    ASSERT_TRUE(resultStream.is_open()) << "Ошибка при открытии файла";

    string line;
    getline(resultStream, line);
    EXPECT_EQ(line, expectedContent) << "Результат в выходном файле неверный";
}

TEST(ParentTests, CorrectDiv_1) {
    string outputFile = "CorrectDiv_1_result.txt";
    string inputFile = "CorrectDiv_1_file.txt";

    // Создание входного файла с числами
    CreateFile(inputFile, outputFile, "13.65 6.5");

    ifstream inputStream(inputFile);
    ASSERT_TRUE(inputStream.is_open()) << "Ошибка при открытии файла";

    // Запуск родительского процесса
    Parent(inputStream);  // Передаем поток для тестирования

    // Проверка выходного файла
    VerifyOutputFile(outputFile, "Частное: 2.1");

    // Удаление временных файлов
    remove(outputFile.c_str());
    remove(inputFile.c_str());
}

TEST(ParentTests, CorrectDiv_2) {
    string outputFile = "CorrectDiv_2_result.txt";
    string inputFile = "CorrectDiv_2_file.txt";

    // Создание входного файла с числами
    CreateFile(inputFile, outputFile, "12.81 2.1 6.1");

    ifstream inputStream(inputFile);
    ASSERT_TRUE(inputStream.is_open()) << "Ошибка при открытии файла";

    // Запуск родительского процесса
    Parent(inputStream);  // Передаем поток для тестирования

    // Проверка выходного файла
    VerifyOutputFile(outputFile, "Частное: 1");

    // Удаление временных файлов
    remove(outputFile.c_str());
    remove(inputFile.c_str());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
