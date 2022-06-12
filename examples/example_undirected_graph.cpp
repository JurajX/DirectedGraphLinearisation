#include <undirected_graph.hpp>

#include <fmt/format.h>

#include <iostream>
#include <unordered_map>
#include <unordered_set>

auto main(int /*argc*/, char ** /*argv*/) -> int    // NOLINT (bugprone-exception-escape)
{                                                   // NOLINTBEGIN
    fmt::print("----- Start of the example.\n");

    std::unordered_map<char, std::unordered_set<char>> adjacencyList {
        {'a',           { 'c' }},
        {'b',           { 'c' }},
        {'c', { 'a', 'b', 'd' }},
        {'d',           { 'c' }},
        {'e',      { 'h', 'f' }},
        {'f',           { 'h' }},
        {'h',           { 'e' }},
        {'i',           { 'j' }},
        {'j',      { 'i', 'k' }},
        {'k',           { 'j' }}
    };

    /// [example_undirected_graph]
    UndirectedGraph<char> g { adjacencyList };
    fmt::print("The graph 'g'\n - is connected: {}.\n - it's vertices and their adjacency lists are:\n", g.isConnected());
    std::cout << g;
    auto ccs { g.getCcs() };
    fmt::print("\n - it has {} connected component(s). The connected components's vertices and adjacency lists are:\n", ccs.size());
    for (const auto &graph : ccs) {
        std::cout << graph << "\n";
    }
    /// [example_undirected_graph]
    /// [example_undirected_graph_output]
    /*
    The graph 'g'
     - is connected: false.
     - it's vertices and their adjacency lists are:
            k: j,
            j: k, i,
            i: j,
            h: f, e,
            f: e, h,
            e: f, h,
            d: c,
            c: d, b, a,
            b: c,
            a: c,

     - it has 3 connected component(s). The connected components's vertices and adjacency lists are:
            k: j,
            j: i, k,
            i: j,

            h: e, f,
            f: h, e,
            e: h, f,

            d: c,
            c: a, b, d,
            b: c,
            a: c,
    */
    /// [example_undirected_graph_output]

    fmt::print("----- End of the example.\n");

    return 0;
}    // NOLINTEND
