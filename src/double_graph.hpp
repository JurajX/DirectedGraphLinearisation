#ifndef DOUBLE_GRAPH_HPP
#define DOUBLE_GRAPH_HPP
#pragma once

#include <undirected_graph.hpp>

#include <list>
#include <ostream>
#include <unordered_map>
#include <unordered_set>
#include <utility>

/**
 * @brief A class representing a simplified directed graph.
 *
 * Consists of two undirected graphs:
 *  - one representing 'double' edges, i.e. a pair of vertices have two edges (one in each direction), and
 *  - one representing 'single' edges, i.e. a pair of vertices have only one edge (in either direction).
 *
 * **Example**
 * \snippet example_double_graph.cpp example_double_graph
 * **Example Output**
 * \snippet example_double_graph.cpp example_double_graph_output
 *
 * @tparam T Type of the graph's vertices.
 */
template<class T>
class DoubleGraph
{
public:
    // ------------- Constructors, Destructors, Assignments

    /// @brief Construct an empty Double Graph.
    DoubleGraph() = default;

    /// @brief Construct a new Double Graph.
    /// @param adjacency_list The adjacency list, does not need to be symmetric.
    DoubleGraph(const std::unordered_map<T, std::unordered_set<T>> &adjacency_list);

    /// @brief Copy constructor from two undirected graphs.
    /// @param single_edge_graph A graph representing single edges.
    /// @param double_edge_graph A graph representing double edges.
    DoubleGraph(const UndirectedGraph<T> &single_edge_graph, const UndirectedGraph<T> &double_edge_graph);

    /// @brief Move constructor from two undirected graphs.
    /// @param single_edge_graph A graph representing single edges.
    /// @param double_edge_graph A graph representing double edges.
    DoubleGraph(UndirectedGraph<T> &&single_edge_graph, UndirectedGraph<T> &&double_edge_graph);

    /// @brief Copy constructor.
    /// @param other The graph to copy.
    DoubleGraph(const DoubleGraph<T> &other);

    /// @brief Move constructor.
    /// @param other The graph to move.
    DoubleGraph(DoubleGraph<T> &&other) noexcept = default;

    /// @brief Destructor.
    virtual ~DoubleGraph() = default;

    /// @brief Copy assignment.
    /// @param other The graph to copy.
    DoubleGraph<T> &operator=(const DoubleGraph<T> &other);

    /// @brief Move assignment.
    /// @param other The graph to move.
    DoubleGraph<T> &operator=(DoubleGraph<T> &&other) noexcept = default;

    // ------------- Setters, Getters

    /// @brief Get the Adjacency List of the undirected graph representing single edges.
    /// @return const std::unordered_map<T, std::unordered_set<T>>& The adjacency list.
    auto getSingleEdgesGraph() const -> const UndirectedGraph<T> &;

    /// @brief Get the Adjacency List of the undirected graph representing double edges.
    /// @return const std::unordered_map<T, std::unordered_set<T>>& The adjacency list.
    auto getDoubleEdgesGraph() const -> const UndirectedGraph<T> &;

    /// @brief Get graph's vertices.
    /// @return std::unordered_set<T> The vertices.
    auto getVertices() const -> std::unordered_set<T>;

    /// @brief Get the vertices of the graph's connected components.
    /// Internally creates an undirected graph by joining the single and double edge graphs and finds the connected components.
    /// @return const std::vector<std::unordered_set<T>> & A constant reference to the vertices of connected components.
    [[maybe_unused]] auto getVerticesOfCcs() -> const std::vector<std::unordered_set<T>> &;

    /// @brief Get the number of graph's connected components.
    /// @return size_t The number of connected components.
    auto getNumCcs() -> size_t;

    /// @brief Get sub-graphs of connected components.
    /// @return std::vector<UndirectedGraph<T>> The sub-graphs.
    auto getCcs() -> std::vector<DoubleGraph<T>>;

    /// @brief Check if graph is connected.
    /// @return bool True if graph is connected; false otherwise.
    auto isConnected() -> bool;

    // ------------- Operators
    /// @brief Compare two graphs.
    /// @param other The other graph.
    /// @return bool True if the graph's adjacency lists coincide; false otherwise.
    auto operator==(const DoubleGraph<T> &other) const -> bool;

    // ------------- Misc

    /// @brief Create a subgraph from selected vertices, all edges not contained in the new sub-graph are pruned.
    /// @param vertices The set of vertices for the sub-graph.
    /// @return UndirectedGraph<T> The sub-graph.
    auto makeSubgraph(const std::unordered_set<T> &vertices) const -> DoubleGraph<T>;

    /// @brief Order graph's vertices in a linear structure.
    /// The order of vertices tries to minimise the sum of the edge lengths, i.e.
    /// \f$ \sum_{s} \mathrm{dist}(e_{s}) + 2\sum_{d} \mathrm{dist}(e_{d}) \f$,
    /// where \f$ s \f$ and \f$ d \f$ represent single and double edges, respectively. This order is not optimal, but uses heuristic.
    /// @return std::list<T> The ordered list of vertices.
    auto lineariseGraph() -> std::list<T>;

private:
    // ------------- Helpers
    /// @brief Split a connected double graph into double connected components (DCCs).
    /// @return std::tuple<std::vector<std::unordered_set<T>>, std::unordered_map<T, size_t>, std::vector<std::unordered_map<size_t, size_t>>>
    /// Returns a tuple of:
    ///  - a mapping of each vertex into the index of its DCC,
    ///  - an adjacency vector of DCCs containing count of all single-edge connections between the DCCs.
    auto splitOnDoubleEdges() const -> std::tuple<std::unordered_map<T, size_t>, std::vector<std::unordered_map<size_t, size_t>>>;

    /// @brief Builds a map from vertex to its DCC group index.
    /// @return std::unordered_map<T, size_t> The map.
    auto buildVertexToDcc() const -> std::unordered_map<T, size_t>;

    /// @brief Build an DCC adjacency list with counts of single connections between the two DCCs.
    /// @param vert_to_dcc A mapping from vertices to their DCC group indices.
    /// @return std::vector<std::unordered_map<size_t, size_t>> The adjacency list, i.e. for each DCC index there is a map with
    ///  - keys being the other DCC indices, and
    ///  - values being counts of single connections between these two DCCs.
    auto buildDccsAdjs(const std::unordered_map<T, size_t> &vert_to_dcc) const -> std::vector<std::unordered_map<size_t, size_t>>;

    /// @brief Order the DCCs and their mappings from vertices to DCC group indices.
    /// @param vert_to_dcc A mapping from vertices to their DCC group indices.
    /// @param dccs_adjs The DCC adjacency list with counts.
    /// @return std::tuple<std::vector<std::unordered_set<T>>, std::unordered_map<T, size_t>> Tuple of ordered DCCs and mappings.
    auto orderDccs(const std::unordered_map<T, size_t> &vert_to_dcc, std::vector<std::unordered_map<size_t, size_t>> &dccs_adjs) const
      -> std::tuple<std::vector<std::unordered_set<T>>, std::unordered_map<T, size_t>>;

    /// @brief Order of double connected components (DCCs) based on a particular heuristic.
    /// @param dcc_adjs An adjacency vector of DCCs.
    /// @return std::list<size_t> The new order of DCCs.
    auto computeDccIdxOrder(std::vector<std::unordered_map<size_t, size_t>> &dcc_adjs) const -> std::list<size_t>;

    /// @brief Find the DCC with most single connections.
    /// @param dcc_adjs An adjacency vector of DCCs.
    /// @return size_t The index of the most connected DCC.
    auto findMostConnectedDcc(const std::vector<std::unordered_map<size_t, size_t>> &dcc_adjs) const -> size_t;

    /// @brief Order the map from vertex to its DCC group index based on the new order of DCCs.
    /// @param dcc_idx_order The new order of the DCCs.
    /// @param vert_to_dcc The old Vertex to DCC group map.
    /// @return std::unordered_map<T, size_t> The map from vertex to the new ordered DCCs.
    auto orderVertToDcc(const std::list<size_t> &dcc_idx_order, std::unordered_map<T, size_t> vert_to_dcc) const -> std::unordered_map<T, size_t>;

    /// @brief Order of vertices in a double connected component (DCC) based on a particular heuristic.
    /// @param vertices Vertices of DCC to order.
    /// @param vert_to_dcc A mapping of each vertex into the index of its DCC.
    /// @return std::list<size_t> The new order of vertices in DCC.
    auto orderDcc(const std::unordered_set<T> &vertices, const std::unordered_map<T, size_t> &vert_to_dcc) const -> std::list<T>;

    /// @brief Compute vertex balance.
    /// I.e. the sum of single connections of a vertex to each side (connections to the right/left are positive/negative).
    /// @param vertices Vertices of DCC to order.
    /// @param vert_to_dcc A mapping of each vertex into the index of its DCC.
    /// @return std::unordered_map<T, int> The balance of each vertex.
    auto computeVertexBalance(const std::unordered_set<T> &vertices, const std::unordered_map<T, size_t> &vert_to_dcc) const
      -> std::unordered_map<T, int>;

    /// @brief A vertex with maximum double connections and balance closest to zero.
    /// @param vertices Vertices of DCC to order.
    /// @param vert_balance The balance of each vertex.
    /// @return T The desired vertex.
    auto findMaxDconVert(const std::unordered_set<T> &vertices, const std::unordered_map<T, int> &vert_balance) const -> T;

    /// @brief Create chains that start at the seed vertex using BFS.
    /// The number of chains depends on the number of double connections of the seed.
    /// @param seed The seed vertex to start from.
    /// @return std::vector<std::list<T>> Vector of vertex chains.
    auto createChains(const T &seed) const -> std::vector<std::list<T>>;

    /// @brief Aggregate vertex balance for each chain
    /// @param chains Chains of vertices.
    /// @param vert_balance The balance of each vertex.
    /// @return std::list<std::pair<size_t, int>>
    auto computeChainBalances(const std::vector<std::list<T>> &chains, const std::unordered_map<T, int> &vert_balance) const
      -> std::list<std::pair<size_t, int>>;

    /// @brief Assign chains into left and right groups of the seed.
    /// @param chains Chains of vertices.
    /// @param chain_balances The balance of each chain.
    /// @return std::tuple<std::vector<size_t>, std::vector<size_t>> Tuple of chain position indices for the Left/Rigth group.
    auto assignChainsLeftRight(const std::vector<std::list<T>> &chains, std::list<std::pair<size_t, int>> chain_balances) const
      -> std::tuple<std::vector<size_t>, std::vector<size_t>>;

    /// @brief Zip the left and right chain groups around the seed.
    /// @param seed The initial vertex chains are formed around.
    /// @param chains Chains of vertices.
    /// @param indices The indices of the left and right groups of the seed.
    /// @return std::list<T> The ordered DCC.
    auto zipLeftRightChains(const T &seed, std::vector<std::list<T>> &chains, std::tuple<std::vector<size_t>, std::vector<size_t>> indices) const
      -> std::list<T>;

    // Graph representing the single-edged connections.
    UndirectedGraph<T> singleEdgesGraph_;

    // Graph representing the double-edged connections.
    UndirectedGraph<T> doubleEdgesGraph_;

    // vertices of connected components
    std::vector<std::unordered_set<T>> verticesOfCcs_ {};
};

template<class T>
auto operator<<(std::ostream &os, const DoubleGraph<T> &g) -> std::ostream &;

#include <double_graph-impl.hpp>

// ===== ===== ===== ===== Instantiations
/// tell doxygen to skip from here \cond
extern template class DoubleGraph<char>;
/// tell doxygen to skip untill here \endcond

#endif    // DOUBLE_GRAPH_HPP