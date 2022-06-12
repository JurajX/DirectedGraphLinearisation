#include <double_graph.hpp>

#include <fmt/format.h>

#include <iostream>
#include <iterator>
#include <unordered_map>
#include <unordered_set>

template<class T>
auto lineariseDirectedGraph(const std::unordered_map<T, std::unordered_set<T>> &adjacency_list) -> std::list<T>
{
    std::list<T> result;
    DoubleGraph<T> fullGraph { adjacency_list };
    auto conComps { fullGraph.getCcs() };
    for (auto &cc : conComps) {
        auto partialResult { cc.lineariseGraph() };
        result.splice(result.end(), partialResult);
    }
    return result;
}

auto main(int /*argc*/, char ** /*argv*/) -> int    // NOLINT (bugprone-exception-escape)
{
    fmt::print("--- Solution Start\n");

    std::unordered_map<char, std::unordered_set<char>> adjacencyList {
        {'a',      { 'c', 'i' }},
        {'b',           { 'c' }},
        {'c', { 'a', 'b', 'd' }},
        {'d', { 'c', 'e', 'i' }},
        {'e', { 'h', 'f', 'g' }},
        {'f',      { 'h', 'g' }},
        {'g',                {}},
        {'h',           { 'e' }},
        {'i',      { 'j', 'm' }},
        {'j', { 'i', 'k', 'l' }},
        {'k', { 'j', 'l', 'm' }},
        {'l',                {}},
        {'m',           { 'k' }},
        {'n',      { 'm', 'l' }}
    };

    auto result { lineariseDirectedGraph(adjacencyList) };

    std::cout << "printing solution:\n";
    for (const auto &elem : result) {
        std::cout << elem << ", ";
    }
    std::cout << "\n";

    fmt::print("--- Solution End\n");

    return 0;
}
