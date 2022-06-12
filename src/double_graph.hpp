#ifndef DOUBLE_GRAPH_HPP
#define DOUBLE_GRAPH_HPP
#pragma once

#include <undirected_graph.hpp>

#include <list>
#include <ostream>
#include <unordered_map>
#include <unordered_set>
#include <utility>

template<class T>
class DoubleGraph
{
public:
    // ------------- Constructors, Destructors, Assignments
    DoubleGraph() = default;
    DoubleGraph(const std::unordered_map<T, std::unordered_set<T>> &adjacency_list);
    DoubleGraph(const UndirectedGraph<T> &single_edge_graph, const UndirectedGraph<T> &double_edge_graph);
    DoubleGraph(UndirectedGraph<T> &&single_edge_graph, UndirectedGraph<T> &&double_edge_graph);
    DoubleGraph(const DoubleGraph<T> &other);
    DoubleGraph(DoubleGraph<T> &&other) noexcept = default;

    virtual ~DoubleGraph() = default;

    DoubleGraph<T> &operator=(const DoubleGraph<T> &other);
    DoubleGraph<T> &operator=(DoubleGraph<T> &&other) noexcept = default;

    // ------------- Setters, Getters
    auto getSingleEdgesGraph() const -> const UndirectedGraph<T> &;
    auto getDoubleEdgesGraph() const -> const UndirectedGraph<T> &;
    auto getVertices() const -> std::unordered_set<T>;

    [[maybe_unused]] auto getVerticesOfCcs() -> const std::vector<std::unordered_set<T>> &;
    auto getNumCcs() -> size_t;
    auto isConnected() -> bool;
    auto getCcs() -> std::vector<DoubleGraph<T>>;

    // ------------- Operators
    auto operator==(const DoubleGraph<T> &other) const -> bool;

    // ------------- Misc
    auto makeSubgraph(const std::unordered_set<T> &vertices) const -> DoubleGraph<T>;
    auto lineariseGraph() -> std::list<T>;

private:
    // ------------- Helpers
    auto splitOnDoubleEdges() const
      -> std::tuple<std::vector<std::unordered_set<T>>, std::unordered_map<T, size_t>, std::vector<std::unordered_map<size_t, size_t>>>;
    auto orderDccs(std::vector<std::unordered_map<size_t, size_t>> &dcc_adjs) const -> std::list<size_t>;
    auto orderDcc(const size_t position, const std::unordered_set<T> &vertices, const std::unordered_map<T, size_t> &vert_to_dcc) const
      -> std::list<T>;

    UndirectedGraph<T> singleEdgesGraph_;
    UndirectedGraph<T> doubleEdgesGraph_;
    std::vector<std::unordered_set<T>> verticesOfCcs_ {};
};

template<class T>
auto operator<<(std::ostream &os, const DoubleGraph<T> &g) -> std::ostream &;

#include <double_graph-impl.hpp>

// ===== ===== ===== ===== Instantiations
/// tell doxygen to skip from here \cond
extern template class DoubleGraph<int>;
extern template class DoubleGraph<char>;
/// tell doxygen to skip untill here \endcond

#endif    // DOUBLE_GRAPH_HPP