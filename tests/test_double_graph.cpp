#include <gtest/gtest.h>

#include <double_graph.hpp>

class TestDoubleGraph : public ::testing::Test
{
protected:
    const static inline std::unordered_map<char, std::unordered_set<char>> adjacencyList {
        {'a',           std::unordered_set<char> { 'c' }},
        {'b',           std::unordered_set<char> { 'c' }},
        {'c', std::unordered_set<char> { 'a', 'b', 'd' }},
        {'d',           std::unordered_set<char> { 'c' }},
        {'e', std::unordered_set<char> { 'h', 'f', 'g' }},
        {'f',      std::unordered_set<char> { 'h', 'g' }},
        {'g',                std::unordered_set<char> {}},
        {'h',           std::unordered_set<char> { 'e' }},
        {'i',      std::unordered_set<char> { 'j', 'm' }},
        {'j', std::unordered_set<char> { 'i', 'k', 'l' }},
        {'k', std::unordered_set<char> { 'j', 'l', 'm' }},
        {'l',                std::unordered_set<char> {}},
        {'m',           std::unordered_set<char> { 'k' }},
        {'n',      std::unordered_set<char> { 'm', 'l' }}
    };
    const static inline std::unordered_set<char> v1 { 'i', 'j', 'k', 'l', 'm', 'n' };
    const static inline std::unordered_set<char> v2 { 'a', 'b', 'c', 'd' };
    const static inline std::unordered_set<char> v3 { 'e', 'f', 'g', 'h' };
};

TEST_F(TestDoubleGraph, TestConstructors)
{
    DoubleGraph<char> g {};
    DoubleGraph<char> g1 { adjacencyList };
    DoubleGraph<char> g2 { g1 };
    EXPECT_TRUE(g1 == g2);
    DoubleGraph<char> g3 { DoubleGraph<char> {} };
    EXPECT_TRUE(g == g3);
    g = g1;
    EXPECT_TRUE(g == g1);
    g = DoubleGraph<char> {};
    EXPECT_TRUE(g == g3);
}

TEST_F(TestDoubleGraph, TestSetGet)
{
    DoubleGraph<char> g { adjacencyList };
    EXPECT_FALSE(g.isConnected());
    EXPECT_EQ(g.getNumCcs(), 3);
    const auto &vccs { g.getVerticesOfCcs() };
    for (const auto &vs : vccs) {
        EXPECT_TRUE((vs == v1 || vs == v2 || vs == v3));
    }
}

//
// =============================== Main
auto main(int argc, char **argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}