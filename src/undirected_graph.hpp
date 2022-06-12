#ifndef UNDIRECTED_GRAPH_HPP
#define UNDIRECTED_GRAPH_HPP
#pragma once

#include <ostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template<class T>
class UndirectedGraph
{
public:
    // ------------- Constructors, Destructors, Assignments
    UndirectedGraph() = default;
    UndirectedGraph(const std::unordered_map<T, std::unordered_set<T>> &adjacency_list);
    UndirectedGraph(const UndirectedGraph<T> &other);
    UndirectedGraph(UndirectedGraph<T> &&other) noexcept = default;

    virtual ~UndirectedGraph() = default;

    UndirectedGraph<T> &operator=(const UndirectedGraph<T> &other);
    UndirectedGraph<T> &operator=(UndirectedGraph<T> &&other) noexcept = default;

    // ------------- Setters, Getters
    auto getAdjacencyList() const -> const std::unordered_map<T, std::unordered_set<T>> &;
    auto getVertices() const -> std::unordered_set<T>;
    auto getNumVertices() const -> size_t;

    auto getVerticesOfCcs() const -> const std::vector<std::unordered_set<T>> &;
    auto getNumCcs() const -> size_t;
    auto getCcs() const -> std::vector<UndirectedGraph<T>>;
    auto isConnected() const -> bool;

    auto makeSubgraph(const std::unordered_set<T> &vertices) const -> UndirectedGraph<T>;
    [[maybe_unused]] auto contains(const T &vertex) -> bool;

    // ------------- Operators
    auto operator==(const UndirectedGraph<T> &other) const -> bool;
    auto operator+(const UndirectedGraph<T> &other) const -> UndirectedGraph<T>;

private:
    // ------------- Helpers
    auto symmetrise() -> void;
    auto populateVerticesOfCcs() -> void;

    std::unordered_map<T, std::unordered_set<T>> adjacencyList_ {};
    std::vector<std::unordered_set<T>> verticesOfCcs_ {};
};

template<class T>
auto operator<<(std::ostream &os, const UndirectedGraph<T> &g) -> std::ostream &;

#include <undirected_graph-impl.hpp>

// ===== ===== ===== ===== Instantiations
/// tell doxygen to skip from here \cond
extern template class UndirectedGraph<int>;
extern template class UndirectedGraph<char>;
/// tell doxygen to skip untill here \endcond

#include <undirected_graph-impl.hpp>

#endif    // UNDIRECTED_GRAPH_HPP