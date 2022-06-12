#ifndef UNDIRECTED_GRAPH_HPP
#define UNDIRECTED_GRAPH_HPP
#pragma once

#include <ostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * @brief A class representing an undirected graph.
 *
 * **Example**
 * \snippet example_undirected_graph.cpp example_undirected_graph
 * **Example Output**
 * \snippet example_undirected_graph.cpp example_undirected_graph_output
 *
 * @tparam T Type of the graph's vertices.
 */
template<class T>
class UndirectedGraph
{
public:
    // ------------- Constructors, Destructors, Assignments

    /// @brief Construct an empty Undirected Graph.
    UndirectedGraph() = default;

    /// @brief Construct a new Undirected Graph.
    /// @param adjacency_list The adjacency list, does not need to be symmetric.
    UndirectedGraph(const std::unordered_map<T, std::unordered_set<T>> &adjacency_list);

    /// @brief Copy constructor.
    /// @param other The graph to copy.
    UndirectedGraph(const UndirectedGraph<T> &other);

    /// @brief Move constructor.
    /// @param other The graph to move.
    UndirectedGraph(UndirectedGraph<T> &&other) noexcept = default;

    /// @brief Destructor.
    virtual ~UndirectedGraph() = default;

    /// @brief Copy assignment.
    /// @param other The graph to copy.
    UndirectedGraph<T> &operator=(const UndirectedGraph<T> &other);

    /// @brief Move assignment.
    /// @param other The graph to move.
    UndirectedGraph<T> &operator=(UndirectedGraph<T> &&other) noexcept = default;

    // ------------- Setters, Getters

    /// @brief Get the Adjacency List of the graph.
    /// @return const std::unordered_map<T, std::unordered_set<T>>& The adjacency list.
    auto getAdjacencyList() const -> const std::unordered_map<T, std::unordered_set<T>> &;

    /// @brief Get graph's vertices.
    /// @return std::unordered_set<T> The vertices.
    auto getVertices() const -> std::unordered_set<T>;

    /// @brief Get the number of graph's vertices.
    /// @return size_t The number of the vertices.
    auto getNumVertices() const -> size_t;

    /// @brief Get the vertices of the graph's connected components.
    /// @return const std::vector<std::unordered_set<T>> & A constant reference to the vertices of connected components.
    auto getVerticesOfCcs() const -> const std::vector<std::unordered_set<T>> &;

    /// @brief Get the number of graph's connected components.
    /// @return size_t The number of connected components.
    auto getNumCcs() const -> size_t;

    /// @brief Get sub-graphs of connected components.
    /// @return std::vector<UndirectedGraph<T>> The sub-graphs.
    auto getCcs() const -> std::vector<UndirectedGraph<T>>;

    /// @brief Check if graph is connected.
    /// @return bool True if graph is connected; false otherwise.
    auto isConnected() const -> bool;

    // ------------- Operators

    /// @brief Compare two graphs.
    /// @param other The other graph.
    /// @return bool True if the graph's adjacency lists coincide; false otherwise.
    auto operator==(const UndirectedGraph<T> &other) const -> bool;

    /// @brief Add two graphs.
    /// @param other The other graph.
    /// @return UndirectedGraph<T> New graph consisting of merging the other two.
    auto operator+(const UndirectedGraph<T> &other) const -> UndirectedGraph<T>;

    // ------------- Misc

    /// @brief Create a subgraph from selected vertices, all edges not contained in the new sub-graph are prunned.
    /// @param vertices The set of vertices for the sub-graph.
    /// @return UndirectedGraph<T> The sub-graph.
    auto makeSubgraph(const std::unordered_set<T> &vertices) const -> UndirectedGraph<T>;

    /// @brief Check if the graph contains a given vertex.
    /// @param vertex The vertex to check for.
    /// @return bool True if the graph contains the vertex; false otherwise.
    [[maybe_unused]] auto contains(const T &vertex) -> bool;

private:
    // ------------- Helpers
    /// @brief Symmetrises the adjacency list.
    auto symmetrise() -> void;

    /// @brief Determines connected components of the graphs using BFS.
    auto populateVerticesOfCcs() -> void;

    // symmetrised adjacency list
    std::unordered_map<T, std::unordered_set<T>> adjacencyList_ {};

    // vertices of connected components
    std::vector<std::unordered_set<T>> verticesOfCcs_ {};
};

/**
 * @brief Streaming operator.
 *
 * @tparam T Must support the streaming operation.
 * @param os The output stream
 * @param g The graph.
 * @return std::ostream& The output stream.
 */
template<class T>
auto operator<<(std::ostream &os, const UndirectedGraph<T> &g) -> std::ostream &;

#include <undirected_graph-impl.hpp>

// ===== ===== ===== ===== Instantiations for faster compilation of dependencies
/// tell doxygen to skip from here \cond
extern template class UndirectedGraph<char>;
/// tell doxygen to skip untill here \endcond

#include <undirected_graph-impl.hpp>

#endif    // UNDIRECTED_GRAPH_HPP