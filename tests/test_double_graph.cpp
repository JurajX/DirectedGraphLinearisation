#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "resources/test_data.hpp"
#include <double_graph.hpp>

#include <stdexcept>

class TestDoubleGraph : public ::testing::Test
{
protected:
    const static inline auto &alDiscon { test::alDiscon };
    const static inline auto &keysAlDiscon { test::keysAlDiscon };
    const static inline auto &keysAlDiscon1 { test::keysAlDiscon1 };
    const static inline auto &keysAlDiscon2 { test::keysAlDiscon2 };
    const static inline auto &keysAlDiscon3 { test::keysAlDiscon3 };

    const static inline auto &alCon { test::alCon };
    const static inline auto &keysAlCon { test::keysAlCon };
    const static inline auto &alConLin { test::alConLin };
};

TEST_F(TestDoubleGraph, TestConstructors)
{
    DoubleGraph<char> g {};
    DoubleGraph<char> g1 { alDiscon };
    DoubleGraph<char> g2 { g1 };
    EXPECT_EQ(g1, g2);
    DoubleGraph<char> g3 { DoubleGraph<char> {} };
    EXPECT_EQ(g, g3);
    g = g1;
    EXPECT_EQ(g, g1);
    g = DoubleGraph<char> {};
    EXPECT_EQ(g, g3);
}

TEST_F(TestDoubleGraph, TestConnectedness)
{
    DoubleGraph<char> gC { alCon };
    EXPECT_TRUE(gC.isConnected());
    EXPECT_EQ(gC.getNumCcs(), 1);

    DoubleGraph<char> gD { alDiscon };
    EXPECT_FALSE(gD.isConnected());
    EXPECT_EQ(gD.getNumCcs(), 3);
    for (const auto &vs : gD.getVerticesOfCcs()) {
        EXPECT_TRUE((vs == keysAlDiscon1 || vs == keysAlDiscon2 || vs == keysAlDiscon3));
    }
}

TEST_F(TestDoubleGraph, TestVertexExtraction)
{
    DoubleGraph<char> gC { alCon };
    EXPECT_EQ(gC.getVertices(), keysAlCon);

    DoubleGraph<char> gD { alDiscon };
    EXPECT_EQ(gD.getVertices(), keysAlDiscon);
}

TEST_F(TestDoubleGraph, TestLinearisation)
{
    DoubleGraph<char> gC { alCon };
    auto actual { gC.lineariseGraph() };
    auto alConLinRev { alConLin };
    alConLinRev.reverse();
    EXPECT_THAT(actual, ::testing::AnyOf(alConLin, alConLinRev));

    DoubleGraph<char> gD { alDiscon };
    EXPECT_THROW(gD.lineariseGraph(), std::runtime_error);
}

//
// =============================== Main
auto main(int argc, char **argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}