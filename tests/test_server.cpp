#include <gtest/gtest.h>
#include "../server/game.h"

TEST(GameManagerTest, CreateGameTest) {
    GameManager manager;
    manager.createGame("Game1", "Player1");
    ASSERT_EQ(manager.getStats("Player1"), 0); // Статистика должна быть 0 при создании игры
}

TEST(GameManagerTest, JoinGameTest) {
    GameManager manager;
    manager.createGame("Game1", "Player1");
    manager.joinGame("Game1", "Player2");
    ASSERT_EQ(manager.getStats("Player2"), 0); // Статистика должна быть 0 при присоединении к игре
}

TEST(GameManagerTest, MakeMoveTest) {
    GameManager manager;
    manager.createGame("Game1", "Player1");
    manager.joinGame("Game1", "Player2");

    // Первый ход игрока 1 (попадание)
    ASSERT_TRUE(manager.makeMove("Game1", "Player1", 0, 0)); // Попадание по кораблю на (0, 0)
    
    // Второй ход игрока 2 (попадание)
    ASSERT_TRUE(manager.makeMove("Game1", "Player2", 1, 1)); // Попадание по кораблю на (1, 1)
    
    // Некорректный ход (повторный выстрел в ту же клетку)
    ASSERT_FALSE(manager.makeMove("Game1", "Player1", 0, 0)); // Некорректный ход
}

TEST(GameManagerTest, GameStatusTest) {
    GameManager manager;
    manager.createGame("Game1", "Player1");
    manager.joinGame("Game1", "Player2");

    // Проверка статуса до начала ходов
    ASSERT_EQ(manager.getGameStatus("Game1", "Player1"), "Current turn: Player1");

    // Игрок 1 делает ход (попадание)
    manager.makeMove("Game1", "Player1", 0, 0);

    // Проверка статуса после хода игрока 1
    ASSERT_EQ(manager.getGameStatus("Game1", "Player1"), "Current turn: Player2");

    // Игрок 2 делает ход (попадание)
    manager.makeMove("Game1", "Player2", 1, 1);

    // Проверка статуса после хода игрока 2
    ASSERT_EQ(manager.getGameStatus("Game1", "Player1"), "Current turn: Player1");
}

TEST(GameManagerTest, WinConditionTest) {
    GameManager manager;

    // Создаем игру и добавляем двух игроков
    manager.createGame("Game1", "Player1");
    manager.joinGame("Game1", "Player2");

    // Проверяем, что игра началась, и ход принадлежит Player1
    ASSERT_EQ(manager.getGameStatus("Game1", "Player1"), "Current turn: Player1");

    // Игрок 1 делает ход и уничтожает корабль на (0, 0)
    ASSERT_TRUE(manager.makeMove("Game1", "Player1", 0, 0)); // Попадание по кораблю на (0, 0)
    ASSERT_EQ(manager.getGameStatus("Game1", "Player1"), "Current turn: Player2");

    // Игрок 2 делает ход и промахивается
    ASSERT_TRUE(manager.makeMove("Game1", "Player2", 3, 3)); // Промах на (3, 3)
    ASSERT_EQ(manager.getGameStatus("Game1", "Player1"), "Current turn: Player1");

    // Игрок 1 делает ход и уничтожает корабль на (1, 1)
    ASSERT_TRUE(manager.makeMove("Game1", "Player1", 1, 1)); // Попадание по кораблю на (1, 1)
    ASSERT_EQ(manager.getGameStatus("Game1", "Player1"), "Current turn: Player2");

    // Игрок 2 делает ход и промахивается
    ASSERT_TRUE(manager.makeMove("Game1", "Player2", 4, 4)); // Промах на (4, 4)
    ASSERT_EQ(manager.getGameStatus("Game1", "Player1"), "Current turn: Player1");

    // Игрок 1 делает ход и уничтожает корабль на (2, 2)
    ASSERT_TRUE(manager.makeMove("Game1", "Player1", 2, 2)); // Попадание по кораблю на (2, 2)

    // Проверяем, что игра завершилась, и Player1 победил
    ASSERT_EQ(manager.getGameStatus("Game1", "Player1"), "Game over. Winner: Player1");

    // Проверяем статистику
    ASSERT_EQ(manager.getStats("Player1"), 1); // Player1 должен иметь 1 победу
    ASSERT_EQ(manager.getStats("Player2"), -1); // Player2 должен иметь 1 поражение
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}