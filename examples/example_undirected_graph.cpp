#include <undirected_graph.hpp>

#include <fmt/format.h>

#include <iostream>
#include <unordered_map>
#include <unordered_set>

auto main(int /*argc*/, char ** /*argv*/) -> int    // NOLINT (bugprone-exception-escape)
{
    fmt::print("----- Start of the example.\n");

    std::unordered_map<char, std::unordered_set<char>> adjacencyList {
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

    UndirectedGraph<char> g { adjacencyList };
    fmt::print("The graph 'g'\n - is connected: {}.\n - it's vertices and their adjacency lists are:\n", g.isConnected());
    std::cout << g;
    auto ccs { g.getCcs() };
    fmt::print("\n - it has {} connected component(s). The connected components's vertices and adjacency lists are:\n", ccs.size());
    for (const auto &graph : ccs) {
        std::cout << graph << "\n";
    }

    fmt::print("----- End of the example.\n");

    return 0;
}
