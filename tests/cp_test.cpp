#include <gtest/gtest.h>
#include "client.h"
#include "server.h"

TEST(ClientTest, GetLogin) {
    Client client(1);
    std::string login = client.getLogin();
    EXPECT_FALSE(login.empty());
}

TEST(ServerTest, ArrangeShips) {
    Server server;
    matrix map(10, std::vector<int>(10, 0));
    matrix ships = {{1, 1, 1, 1, 1}, {2, 2, 2, 2, 3}};
    server.arrangeShips(map, ships);
    EXPECT_EQ(map[0][0], 1);
    EXPECT_EQ(map[1][1], 1);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}