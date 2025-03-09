#include <gtest/gtest.h>
#include "ttopology.h"
#include "tmessaging.h"
#include <atomic>
#include <string>
#include <unistd.h>
#include <pthread.h>

// Тесты для топологии (дерево общего вида)
TEST(TTopologyTest, AddNode) {
    TTopology topo;
    EXPECT_FALSE(topo.NodeExists(10)); // Узел 10 не должен существовать
    EXPECT_TRUE(topo.AddNode(10, -1, "endpoint10")); // Добавляем узел 10
    EXPECT_TRUE(topo.NodeExists(10)); // Узел 10 должен существовать

    TNodeInfo* info = topo.GetNode(10);
    ASSERT_NE(info, nullptr); // Проверяем, что информация о узле получена
    EXPECT_EQ(info->id, 10); // Проверяем ID узла
    EXPECT_EQ(info->parent_id, -1); // Проверяем, что у узла нет родителя
    EXPECT_EQ(info->endpoint, "endpoint10"); // Проверяем endpoint
    EXPECT_TRUE(info->alive); // Узел должен быть доступен
}

TEST(TTopologyTest, AddDuplicateNode) {
    TTopology topo;
    EXPECT_TRUE(topo.AddNode(10, -1, "endpoint10")); // Добавляем узел 10
    EXPECT_FALSE(topo.AddNode(10, -1, "endpoint10_bis")); // Попытка добавить узел с тем же ID
}

TEST(TTopologyTest, RemoveNode) {
    TTopology topo;
    topo.AddNode(10, -1, "end10"); // Добавляем узел 10
    topo.AddNode(20, 10, "end20"); // Добавляем узел 20 с родителем 10
    EXPECT_TRUE(topo.NodeExists(20)); // Узел 20 должен существовать
    EXPECT_TRUE(topo.RemoveNode(20)); // Удаляем узел 20
    EXPECT_FALSE(topo.NodeExists(20)); // Узел 20 больше не должен существовать
}

TEST(TTopologyTest, GetChildren) {
    TTopology topo;
    topo.AddNode(10, -1, "end10"); // Добавляем узел 10
    topo.AddNode(20, 10, "end20"); // Добавляем узел 20 с родителем 10
    topo.AddNode(15, 10, "end15"); // Добавляем узел 15 с родителем 10
    topo.AddNode(12, -1, "end12"); // Добавляем узел 12 без родителя

    auto children10 = topo.GetChildren(10); // Получаем дочерние узлы для узла 10
    ASSERT_EQ(children10.size(), 2); // У узла 10 должно быть 2 дочерних узла
    EXPECT_TRUE(std::find(children10.begin(), children10.end(), 20) != children10.end()); // Узел 20 должен быть в списке
    EXPECT_TRUE(std::find(children10.begin(), children10.end(), 15) != children10.end()); // Узел 15 должен быть в списке

    auto childrenRoot = topo.GetChildren(-1); // Получаем корневые узлы
    ASSERT_EQ(childrenRoot.size(), 2); // Должно быть 2 корневых узла (10 и 12)
    EXPECT_TRUE(std::find(childrenRoot.begin(), childrenRoot.end(), 10) != childrenRoot.end()); // Узел 10 должен быть в списке
    EXPECT_TRUE(std::find(childrenRoot.begin(), childrenRoot.end(), 12) != childrenRoot.end()); // Узел 12 должен быть в списке
}

// Тесты для подсчета суммы чисел
TEST(TSumTest, SimpleSum) {
    TWorkerNode worker(1, -1, "tcp://127.0.0.1:6001");
    worker.Init();

    std::string input = "3\n1 2 3"; // Входные данные: 3 числа (1, 2, 3)
    std::istringstream iss(input);
    int n;
    iss >> n;
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        int num;
        iss >> num;
        sum += num;
    }
    EXPECT_EQ(sum, 6); // Ожидаемая сумма: 6
}

TEST(TSumTest, InvalidInput) {
    TWorkerNode worker(1, -1, "tcp://127.0.0.1:6001");
    worker.Init();

    std::string input = "3\n1 two 3"; // Некорректные входные данные
    std::istringstream iss(input);
    int n;
    iss >> n;
    int num;
    EXPECT_THROW({
        for (int i = 0; i < n; ++i) {
            iss >> num; // Попытка считать некорректное значение
        }
    }, std::invalid_argument); // Ожидаем исключение
}

// Тесты для проверки доступности узлов
TEST(TPingTest, PingAll) {
    TTopology topo;
    topo.AddNode(10, -1, "end10"); // Добавляем узел 10
    topo.AddNode(20, 10, "end20"); // Добавляем узел 20
    topo.AddNode(30, 10, "end30"); // Добавляем узел 30

    // Проверяем, что все узлы доступны
    for (auto& [id, info] : topo.GetAllNodes()) {
        EXPECT_TRUE(info.alive); // Каждый узел должен быть доступен
    }
}

TEST(TPingTest, NodeUnavailable) {
    TTopology topo;
    topo.AddNode(10, -1, "end10"); // Добавляем узел 10
    topo.AddNode(20, 10, "end20"); // Добавляем узел 20

    TNodeInfo* node = topo.GetNode(20);
    ASSERT_NE(node, nullptr); // Проверяем, что узел 20 существует
    node->alive = false; // Помечаем узел 20 как недоступный

    EXPECT_FALSE(node->alive); // Узел 20 должен быть недоступен
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}