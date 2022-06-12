#include <double_graph.hpp>

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
        {'e', { 'h', 'f', 'g' }},
        {'f',      { 'h', 'g' }},
        {'g',                {}},
        {'h',                {}}
    };

    /// [example_double_graph]
    DoubleGraph<char> g { adjacencyList };
    fmt::print("The graph 'g'\n - is connected: {}.\n - it's vertices and their adjacency lists are:\n", g.isConnected());
    std::cout << g;
    auto ccs { g.getCcs() };
    fmt::print("\n - it has {} connected component(s). The connected components's vertices and adjacency lists are:\n", ccs.size());
    for (const auto &graph : ccs) {
        std::cout << graph << "\n";
    }
    /// [example_double_graph]
    /// [example_double_graph_output]
    /*
    The graph 'g'
     - is connected: false.
     - it's vertices and their adjacency lists are:
       a   -->single edges: 		-->double edges: c,
       b   -->single edges: 		-->double edges: c,
       c   -->single edges: 		-->double edges: a, b, d,
       d   -->single edges: 		-->double edges: c,
       e   -->single edges: h, f, g, 	-->double edges:
       f   -->single edges: e, h, g, 	-->double edges:
       g   -->single edges: f, e, 		-->double edges:
       h   -->single edges: f, e, 		-->double edges:

     - it has 2 connected component(s). The connected components's vertices and adjacency lists are:
       a   -->single edges: 		-->double edges: c,
       c   -->single edges: 		-->double edges: d, b, a,
       d   -->single edges: 		-->double edges: c,
       b   -->single edges: 		-->double edges: c,

       e   -->single edges: g, f, h, 	-->double edges:
       h   -->single edges: e, f, 		-->double edges:
       f   -->single edges: g, h, e, 	-->double edges:
       g   -->single edges: e, f, 		-->double edges:
    */
    /// [example_double_graph_output]

    fmt::print("----- End of the example.\n");

    return 0;
}    // NOLINTEND
