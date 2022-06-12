#include <gtest/gtest.h>

#include "resources/test_data.hpp"
#include <undirected_graph.hpp>

class TestUndirectedGraph : public ::testing::Test
{
protected:
    const static inline auto &keysAdjacencyList0 { test::keysAdjList0 };
    const static inline auto &adjacencyList0 { test::adjList0 };
    const static inline auto &keysAdjacencyList1 { test::keysAdjList1 };
    const static inline auto &adjacencyList1 { test::adjList1 };
    const static inline auto &keysAdjacencyList2 { test::keysAdjList2 };
    const static inline auto &adjacencyList2 { test::adjList2 };
    const static inline auto &keysAdjacencyList3 { test::keysAdjList3 };
    const static inline auto &adjacencyList3 { test::adjList3 };
};

TEST_F(TestUndirectedGraph, TestConstructors)
{
    UndirectedGraph<char> g {};
    UndirectedGraph<char> g0 { adjacencyList0 };
    UndirectedGraph<char> g1 { adjacencyList1 };
    EXPECT_EQ(g1, g0);
    UndirectedGraph<char> g2 { g1 };
    EXPECT_EQ(g1, g2);
    UndirectedGraph<char> g3 { UndirectedGraph<char> {} };
    EXPECT_EQ(g, g3);
    g = g1;
    EXPECT_EQ(g, g1);
    g = UndirectedGraph<char> {};
    EXPECT_EQ(g, g3);
}

TEST_F(TestUndirectedGraph, TestInternalAdjacencyLists)
{
    UndirectedGraph<char> g0 { adjacencyList0 };
    EXPECT_EQ(g0.getAdjacencyList(), adjacencyList1);
}

TEST_F(TestUndirectedGraph, TestConnectedness)
{
    UndirectedGraph<char> g1 { adjacencyList1 };
    EXPECT_TRUE(g1.isConnected());
    ASSERT_EQ(g1.getNumCcs(), 1);
    EXPECT_EQ(g1.getVerticesOfCcs().front(), keysAdjacencyList1);
    EXPECT_EQ(g1.getCcs().front(), g1);

    UndirectedGraph<char> g2 { adjacencyList2 };
    EXPECT_TRUE(g2.isConnected());
    ASSERT_EQ(g2.getNumCcs(), 1);
    EXPECT_EQ(g2.getVerticesOfCcs().front(), keysAdjacencyList2);
    EXPECT_EQ(g2.getCcs().front(), g2);

    UndirectedGraph<char> g3 { adjacencyList3 };
    EXPECT_FALSE(g3.isConnected());
    EXPECT_EQ(g3.getNumCcs(), 2);
    for (const auto &vertices : g3.getVerticesOfCcs())
        EXPECT_TRUE((vertices == keysAdjacencyList1) != (vertices == keysAdjacencyList2));    // () XOR ()
    for (const auto &graph : g3.getCcs())
        EXPECT_TRUE((graph == g1) != (graph == g2));    // () XOR ()
}

TEST_F(TestUndirectedGraph, TestVertexExtraction)
{
    UndirectedGraph<char> g1 { adjacencyList1 };
    EXPECT_EQ(g1.getNumVertices(), 4);
    EXPECT_EQ(g1.getVertices(), keysAdjacencyList1);

    UndirectedGraph<char> g2 { adjacencyList2 };
    EXPECT_EQ(g2.getNumVertices(), 4);
    EXPECT_EQ(g2.getVertices(), keysAdjacencyList2);

    UndirectedGraph<char> g3 { adjacencyList3 };
    EXPECT_EQ(g3.getNumVertices(), 8);
    EXPECT_EQ(g3.getVertices(), keysAdjacencyList3);
}

TEST_F(TestUndirectedGraph, TestGraphAddition)
{
    UndirectedGraph<char> g1 { adjacencyList1 };
    UndirectedGraph<char> g2 { adjacencyList2 };
    UndirectedGraph<char> g3 { adjacencyList3 };
    EXPECT_EQ(g1 + g2, g3);
}

//
// =============================== Main
auto main(int argc, char **argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}