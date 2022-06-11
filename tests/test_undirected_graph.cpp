#include <gtest/gtest.h>

#include <undirected_graph.hpp>

//
// =============================== Main
auto main(int argc, char **argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}