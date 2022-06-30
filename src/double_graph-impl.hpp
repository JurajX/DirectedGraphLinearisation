#ifndef DOUBLE_GRAPH_IMPL_HPP
#define DOUBLE_GRAPH_IMPL_HPP
#pragma once

#include <double_graph.hpp>

#include <fmt/format.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>

//
// =============================== Constructors, Destructors, Assignments

template<class T>
DoubleGraph<T>::DoubleGraph(const std::unordered_map<T, std::unordered_set<T>> &adjacency_list)
{
    std::unordered_map<T, std::unordered_set<T>> singles;
    std::unordered_map<T, std::unordered_set<T>> doubles;
    for (const auto &[vertex, adjs] : adjacency_list) {
        singles[vertex];    // make sure each vertex is added
        doubles[vertex];    // make sure each vertex is added
        for (const T &adj : adjs) {
            if (adjacency_list.at(adj).contains(vertex))    // if friendship is symmetric make double edge, else make single edge
                doubles[vertex].insert(adj);
            else
                singles[vertex].insert(adj);
        }
    }
    singleEdgesGraph_ = UndirectedGraph<T> { singles };
    doubleEdgesGraph_ = UndirectedGraph<T> { doubles };
}

template<class T>
DoubleGraph<T>::DoubleGraph(const DoubleGraph<T> &other)
    : singleEdgesGraph_ { other.singleEdgesGraph_ }
    , doubleEdgesGraph_ { other.doubleEdgesGraph_ }
{ }

template<class T>
DoubleGraph<T>::DoubleGraph(const UndirectedGraph<T> &single_edge_graph, const UndirectedGraph<T> &double_edge_graph)
    : singleEdgesGraph_ { single_edge_graph }
    , doubleEdgesGraph_ { double_edge_graph }
{ }

template<class T>
DoubleGraph<T>::DoubleGraph(UndirectedGraph<T> &&single_edge_graph, UndirectedGraph<T> &&double_edge_graph)
    : singleEdgesGraph_ { std::move(single_edge_graph) }
    , doubleEdgesGraph_ { std::move(double_edge_graph) }
{ }

template<class T>
DoubleGraph<T> &DoubleGraph<T>::operator=(const DoubleGraph<T> &other)
{
    return *this = DoubleGraph(other);
}

//
// =============================== Setters, Getters

template<class T>
auto DoubleGraph<T>::getSingleEdgesGraph() const -> const UndirectedGraph<T> &
{
    return singleEdgesGraph_;
}

template<class T>
auto DoubleGraph<T>::getDoubleEdgesGraph() const -> const UndirectedGraph<T> &
{
    return doubleEdgesGraph_;
}

template<class T>
auto DoubleGraph<T>::getVertices() const -> std::unordered_set<T>
{
    return singleEdgesGraph_.getVertices();
}

template<class T>
auto DoubleGraph<T>::getVerticesOfCcs() -> const std::vector<std::unordered_set<T>> &
{
    if (verticesOfCcs_.size() == 0) {
        UndirectedGraph<T> tmp { singleEdgesGraph_ + doubleEdgesGraph_ };
        verticesOfCcs_ = tmp.getVerticesOfCcs();
    }
    return verticesOfCcs_;
}

template<class T>
auto DoubleGraph<T>::getNumCcs() -> size_t
{
    getVerticesOfCcs();
    return verticesOfCcs_.size();
}

template<class T>
auto DoubleGraph<T>::getCcs() -> std::vector<DoubleGraph<T>>
{
    getVerticesOfCcs();
    std::vector<DoubleGraph<T>> ccs;
    std::transform(verticesOfCcs_.cbegin(), verticesOfCcs_.cend(), std::back_inserter(ccs), [this](const auto &vertices) {
        auto g { makeSubgraph(vertices) };
        g.verticesOfCcs_.push_back(g.getVertices());    // now it's connected
        return g;
    });
    return ccs;
}

template<class T>
auto DoubleGraph<T>::isConnected() -> bool
{
    return getNumCcs() == 1;
}

//
// =============================== Operators

template<class T>
auto DoubleGraph<T>::operator==(const DoubleGraph<T> &other) const -> bool
{
    return (singleEdgesGraph_ == other.singleEdgesGraph_) && (doubleEdgesGraph_ == other.doubleEdgesGraph_);
}

//
// =============================== Misc

template<class T>
auto DoubleGraph<T>::makeSubgraph(const std::unordered_set<T> &vertices) const -> DoubleGraph<T>
{
    return DoubleGraph<T> { singleEdgesGraph_.makeSubgraph(vertices), doubleEdgesGraph_.makeSubgraph(vertices) };
}

template<class T>
auto DoubleGraph<T>::lineariseGraph() -> std::list<T>
{
    if (not isConnected())
        throw std::runtime_error("The graph must be connected. Please split the graph to connected components first.");

    auto [vertToDcc, dccsAdjs] { splitOnDoubleEdges() };
    auto [orderedDccsVerts, orderedVertToDcc] = orderDccs(vertToDcc, dccsAdjs);

    std::list<T> linearVertices;
    for (size_t idx = 0; idx < orderedDccsVerts.size(); idx += 1) {
        std::list<T> orderedDcc { orderDcc(orderedDccsVerts[idx], orderedVertToDcc) };
        linearVertices.splice(linearVertices.end(), orderedDcc);
    }
    return linearVertices;
}

//
// =============================== Helpers

template<class T>
auto DoubleGraph<T>::splitOnDoubleEdges() const -> std::tuple<std::unordered_map<T, size_t>, std::vector<std::unordered_map<size_t, size_t>>>
{
    std::unordered_map<T, size_t> vertToDcc { buildVertexToDcc() };
    std::vector<std::unordered_map<size_t, size_t>> dccsAdjs { buildDccsAdjs(vertToDcc) };

    return { vertToDcc, dccsAdjs };
}

template<class T>
auto DoubleGraph<T>::buildVertexToDcc() const -> std::unordered_map<T, size_t>
{
    const auto &dccsVerts { doubleEdgesGraph_.getVerticesOfCcs() };
    std::unordered_map<T, size_t> vertToDcc;
    for (const auto &group : dccsVerts) {
        for (const auto &[vertex, _] : singleEdgesGraph_.getAdjacencyList()) {
            if (group.contains(vertex))
                vertToDcc[vertex] = static_cast<size_t>(&group - &dccsVerts.front());
        }
    }
    return vertToDcc;
}

template<class T>
auto DoubleGraph<T>::buildDccsAdjs(const std::unordered_map<T, size_t> &vert_to_dcc) const -> std::vector<std::unordered_map<size_t, size_t>>
{
    std::vector<std::unordered_map<size_t, size_t>> dccsAdjs { doubleEdgesGraph_.getVerticesOfCcs().size() };
    for (const auto &[vertex, adjs] : singleEdgesGraph_.getAdjacencyList()) {
        for (const auto &adj : adjs) {
            auto dccIdxOfAdj { vert_to_dcc.at(adj) };
            auto dccIdxOfVert { vert_to_dcc.at(vertex) };
            if (dccIdxOfAdj == dccIdxOfVert)
                continue;
            auto &map { dccsAdjs[dccIdxOfVert] };
            map.contains(dccIdxOfAdj) ? map[dccIdxOfAdj] += 1 : map[dccIdxOfAdj] = 1;
        }
    }
    return dccsAdjs;
}

template<class T>
auto DoubleGraph<T>::orderDccs(const std::unordered_map<T, size_t> &vert_to_dcc, std::vector<std::unordered_map<size_t, size_t>> &dccs_adjs) const
  -> std::tuple<std::vector<std::unordered_set<T>>, std::unordered_map<T, size_t>>
{
    auto dccIdxOrder { computeDccIdxOrder(dccs_adjs) };
    auto orderedVertToDcc { orderVertToDcc(dccIdxOrder, vert_to_dcc) };

    const auto &dccsVerts { doubleEdgesGraph_.getVerticesOfCcs() };
    std::vector<std::unordered_set<T>> orderedDccxVerts;
    std::transform(dccIdxOrder.cbegin(), dccIdxOrder.cend(), std::back_inserter(orderedDccxVerts), [&dccsVerts](const auto &idx) {
        return dccsVerts[idx];
    });
    return { orderedDccxVerts, orderedVertToDcc };
}

template<class T>
auto DoubleGraph<T>::computeDccIdxOrder(std::vector<std::unordered_map<size_t, size_t>> &dcc_adjs) const -> std::list<size_t>
{
    auto eraseIdxFromDccAdjs { [&dcc_adjs](size_t idx) {
        std::for_each(dcc_adjs.begin(), dcc_adjs.end(), [&idx](auto &map) { map.erase(idx); });
    } };
    auto LessOnSecond { [](const auto &a, const auto &b) {
        return a.second < b.second;
    } };
    auto findNonEmptyMap { [&dcc_adjs](auto it) -> std::unordered_map<size_t, size_t> {
        std::unordered_map<size_t, size_t> elem {};
        for (; elem.empty(); ++it)
            elem = dcc_adjs.at(*it);
        return elem;
    } };

    size_t idx { findMostConnectedDcc(dcc_adjs) };
    std::list<size_t> dccIdxOrder;
    dccIdxOrder.push_back(idx);
    eraseIdxFromDccAdjs(idx);

    // find a group from group adjacent list that has the most connection to the right/left-most element of the final list
    // place this group into the final list on the correct side and erase it from the group adjacents list
    while (dccIdxOrder.size() < dcc_adjs.size()) {
        auto front { findNonEmptyMap(dccIdxOrder.cbegin()) };
        auto back { findNonEmptyMap(dccIdxOrder.crbegin()) };
        auto frontMax { std::max_element(front.cbegin(), front.cend(), LessOnSecond) };
        auto backMax { std::max_element(back.cbegin(), back.cend(), LessOnSecond) };
        bool trueFront { front == dcc_adjs.at(dccIdxOrder.front()) };
        bool trueBack { back == dcc_adjs.at(dccIdxOrder.back()) };
        if (trueFront == trueBack) {
            if (frontMax->second > backMax->second) {
                idx = frontMax->first;
                dccIdxOrder.push_front(idx);
            } else {
                idx = backMax->first;
                dccIdxOrder.push_back(idx);
            }
        } else if (trueFront) {    // only front
            idx = frontMax->first;
            dccIdxOrder.push_front(idx);
        } else {    // only back
            idx = backMax->first;
            dccIdxOrder.push_back(idx);
        }
        eraseIdxFromDccAdjs(idx);
    }
    return dccIdxOrder;
}

template<class T>
auto DoubleGraph<T>::findMostConnectedDcc(const std::vector<std::unordered_map<size_t, size_t>> &dcc_adjs) const -> size_t
{
    size_t max { 0 };
    size_t idx { 0 };
    for (const auto &map : dcc_adjs) {
        size_t grpCount { std::accumulate(map.cbegin(), map.cend(), size_t(0), [](const auto &a, const auto &b) { return a + b.second; }) };
        if (grpCount > max) {
            max = grpCount;
            idx = static_cast<size_t>(&map - &dcc_adjs.front());
        }
    }
    return idx;
}

template<class T>
auto DoubleGraph<T>::orderVertToDcc(const std::list<size_t> &dcc_idx_order, std::unordered_map<T, size_t> vert_to_dcc) const
  -> std::unordered_map<T, size_t>
{
    std::vector<size_t> dccIdxOrderInv(dcc_idx_order.size());
    for (auto it { dcc_idx_order.cbegin() }; it != dcc_idx_order.cend(); ++it) {
        auto idx { std::distance(dcc_idx_order.cbegin(), it) };
        dccIdxOrderInv[*it] = static_cast<size_t>(idx);
    }
    // update the vertex to DCC group mapping with the new order of DCCs
    std::for_each(vert_to_dcc.begin(), vert_to_dcc.end(), [&dccIdxOrderInv](auto &p) { p.second = dccIdxOrderInv[p.second]; });
    return vert_to_dcc;
}

template<class T>
auto DoubleGraph<T>::orderDcc(const std::unordered_set<T> &vertices, const std::unordered_map<T, size_t> &vert_to_dcc) const -> std::list<T>
{
    if (vertices.size() == 1)
        return { *vertices.cbegin() };
    auto vertBalance { computeVertexBalance(vertices, vert_to_dcc) };
    T vertex { findMaxDconVert(vertices, vertBalance) };
    std::vector<std::list<T>> chains { createChains(vertex) };
    std::list<std::pair<size_t, int>> chainBalances { computeChainBalances(chains, vertBalance) };
    auto indices { assignChainsLeftRight(chains, chainBalances) };
    return zipLeftRightChains(vertex, chains, indices);
}

template<class T>
auto DoubleGraph<T>::computeVertexBalance(const std::unordered_set<T> &vertices, const std::unordered_map<T, size_t> &vert_to_dcc) const
  -> std::unordered_map<T, int>
{
    std::unordered_map<T, int> vertBalance;
    const auto &sAdjList { singleEdgesGraph_.getAdjacencyList() };
    const size_t position { vert_to_dcc.at(*vertices.cbegin()) };
    for (const auto &vertex : vertices) {
        vertBalance[vertex] = 0;
        for (const T &adj : sAdjList.at(vertex)) {
            if (vert_to_dcc.at(adj) > position)
                vertBalance[vertex] += 1;
            else if (vert_to_dcc.at(adj) < position)
                vertBalance[vertex] -= 1;
        }
    }
    return vertBalance;
}

template<class T>
auto DoubleGraph<T>::findMaxDconVert(const std::unordered_set<T> &vertices, const std::unordered_map<T, int> &vert_balance) const -> T
{
    const auto &dAdjList { doubleEdgesGraph_.getAdjacencyList() };
    T vertex { *vertices.cbegin() };
    size_t maxCon { dAdjList.at(vertex).size() };
    for (const auto &v : vertices) {
        bool replace { (dAdjList.at(v).size() > maxCon)
                       || ((dAdjList.at(v).size() == maxCon) && (std::abs(vert_balance.at(v)) < std::abs(vert_balance.at(vertex)))) };
        if (replace) {
            maxCon = dAdjList.at(v).size();
            vertex = v;
        }
    }
    return vertex;
}

template<class T>
auto DoubleGraph<T>::createChains(const T &seed) const -> std::vector<std::list<T>>
{
    T vertex { seed };
    const auto &dAdjList { doubleEdgesGraph_.getAdjacencyList() };
    std::unordered_set<T> visited { vertex };
    std::vector<std::deque<T>> toVisit;
    std::vector<std::list<T>> chains;
    for (const auto &adj : dAdjList.at(vertex)) {
        toVisit.push_back(std::deque<T> { adj });
        chains.push_back(std::list<T> { adj });
    }
    while (not std::all_of(toVisit.cbegin(), toVisit.cend(), [](const auto &q) { return q.empty(); })) {
        for (size_t idx = 0; idx < toVisit.size(); idx += 1) {
            if (toVisit[idx].empty())
                continue;
            vertex = toVisit[idx].front();
            toVisit[idx].pop_front();
            visited.insert(vertex);
            const auto &adjs { dAdjList.at(vertex) };
            for (const T &adj : adjs) {
                if (visited.contains(adj))
                    continue;
                chains[idx].push_back(adj);
                toVisit[idx].push_back(adj);
            }
        }
    }
    return chains;
}

template<class T>
auto DoubleGraph<T>::computeChainBalances(const std::vector<std::list<T>> &chains, const std::unordered_map<T, int> &vert_balance) const
  -> std::list<std::pair<size_t, int>>
{
    std::list<std::pair<size_t, int>> chainBalances;
    for (size_t idx = 0; idx < chains.size(); idx += 1) {
        chainBalances.emplace_back(idx, 0);
        for (const auto &v : chains[idx]) {
            chainBalances.back().second += vert_balance.at(v);
        }
    }
    chainBalances.sort([](const auto &a, const auto &b) { return a.second < b.second; });
    return chainBalances;
}

template<class T>
auto DoubleGraph<T>::assignChainsLeftRight(const std::vector<std::list<T>> &chains, std::list<std::pair<size_t, int>> chain_balances) const
  -> std::tuple<std::vector<size_t>, std::vector<size_t>>
{
    std::vector<size_t> indicesL;
    std::vector<size_t> indicesR;
    size_t countL { 0 };
    size_t countR { 0 };
    while (not chain_balances.empty()) {
        bool backBtFront { (std::abs(chain_balances.back().second) > std::abs(chain_balances.front().second)) };
        bool doRight { (countL > countR) || ((countL == countR) && (backBtFront || chain_balances.back().second > 0)) };
        if (doRight) {
            indicesR.push_back(chain_balances.back().first);
            chain_balances.pop_back();
            countR += chains[indicesR.back()].size();
        } else {
            indicesL.push_back(chain_balances.front().first);
            chain_balances.pop_front();
            countL += chains[indicesL.back()].size();
        }
    }
    return { indicesL, indicesR };
}

template<class T>
auto DoubleGraph<T>::zipLeftRightChains(
  const T &seed,
  std::vector<std::list<T>> &chains,
  std::tuple<std::vector<size_t>, std::vector<size_t>> indices) const -> std::list<T>
{
    auto [indicesL, indicesR] { indices };
    std::list<T> orderedDcc { seed };
    while (not std::all_of(chains.cbegin(), chains.cend(), [](const auto &q) { return q.empty(); })) {
        for (auto rit { indicesL.crbegin() }; rit != indicesL.crend(); ++rit) {
            if (not chains[*rit].empty()) {
                orderedDcc.splice(orderedDcc.cbegin(), chains[*rit], chains[*rit].cbegin());
            }
        }
        for (const auto &idx : indicesR) {
            if (not chains[idx].empty()) {
                orderedDcc.splice(orderedDcc.cend(), chains[idx], chains[idx].cbegin());
            }
        }
    }
    return orderedDcc;
}

//
// =============================== Non-Member Fns

template<class T>
auto operator<<(std::ostream &os, const DoubleGraph<T> &g) -> std::ostream &
{
    const auto &doubles { g.getDoubleEdgesGraph().getAdjacencyList() };
    for (const auto &[vortex, adjs] : g.getSingleEdgesGraph().getAdjacencyList()) {
        fmt::print("{:>4}   -->single edges: ", vortex);
        for (const auto &adj : adjs) {
            fmt::print("{}, ", adj);
        }
        if (adjs.size() < 3)
            fmt::print("\t");
        fmt::print("\t-->double edges: ");
        for (const auto &adj : doubles.at(vortex)) {
            fmt::print("{}, ", adj);
        }
        fmt::print("\n");
    }
    return os;
}

#endif    // DOUBLE_GRAPH_IMPL_HPP