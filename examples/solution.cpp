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
        {'a',      std::unordered_set<char> { 'c', 'i' }},
        {'b',           std::unordered_set<char> { 'c' }},
        {'c', std::unordered_set<char> { 'a', 'b', 'd' }},
        {'d', std::unordered_set<char> { 'c', 'e', 'i' }},
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

    auto result { lineariseDirectedGraph(adjacencyList) };

    std::cout << "printing solution:\n";
    for (const auto &elem : result) {
        std::cout << elem << ", ";
    }
    std::cout << "\n";

    fmt::print("--- Solution End\n");

    return 0;
}
