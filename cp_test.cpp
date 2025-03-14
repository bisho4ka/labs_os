#include <gtest/gtest.h>
#include <fstream>
#include "include/GameManager.h"
#include "include/utils.h"

bool WriteTestConfig(const std::string &filename, const std::string &content) {
    std::ofstream fout(filename);
    if (!fout.is_open()) return false;
    fout << content;
    fout.close();
    return true;
}

// Проверяем корректность создания игрового поля
TEST(GameManagerTest, FieldCreation) {
    std::string fieldFile = "test_field.txt";
    std::string fieldContent = R"(
# # # # # . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
)";

    ASSERT_TRUE(WriteTestConfig(fieldFile, fieldContent));

    GameManager manager(fieldFile.c_str(), "TestPlayer");
    manager.print();

    // Проверяем, что поле корректно загружено
    EXPECT_EQ(manager.check_defeat(), false); // Игрок не проиграл сразу
}

// Проверяем корректность выстрела и обработки попадания
TEST(GameManagerTest, ShotHandling) {
    std::string fieldFile = "test_field.txt";
    std::string fieldContent = R"(
# # # # # . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
)";

    ASSERT_TRUE(WriteTestConfig(fieldFile, fieldContent));

    GameManager manager(fieldFile.c_str(), "TestPlayer");

    // Выстрел в координаты (0, 0) - попадание
    shot_t result = manager.recieve_attack(0, 0);
    EXPECT_EQ(result, SHOT_HIT);

    // Выстрел в координаты (5, 5) - промах
    result = manager.recieve_attack(5, 5);
    EXPECT_EQ(result, SHOT_MISS);
}

// Проверяем завершение игры при уничтожении всех кораблей
TEST(GameManagerTest, GameOver) {
    std::string fieldFile = "test_field.txt";
    std::string fieldContent = R"(
# . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
)";

    ASSERT_TRUE(WriteTestConfig(fieldFile, fieldContent));

    GameManager manager(fieldFile.c_str(), "TestPlayer");

    // Выстрел в координаты (0, 0) - попадание
    shot_t result = manager.recieve_attack(0, 0);
    EXPECT_EQ(result, SHOT_HIT);

    // Проверяем, что игра завершена
    EXPECT_TRUE(manager.check_defeat());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}