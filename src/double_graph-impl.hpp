#ifndef DOUBLE_GRAPH_IMPL_HPP
#define DOUBLE_GRAPH_IMPL_HPP
#pragma once

#include <double_graph.hpp>

#include <fmt/format.h>

#include <algorithm>
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
auto DoubleGraph<T>::isConnected() -> bool
{
    return getNumCcs() == 1;
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

//
// =============================== Operators

template<class T>
auto DoubleGraph<T>::operator==(const DoubleGraph<T> &other) -> bool
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
    auto [dccsVerts, vertToDcc, dccsAdjs] { splitOnDoubleEdges() };
    std::list<T> linearVertices;
    auto dccIndexOrder { orderDccs(dccsAdjs) };
    std::vector<size_t> dccIndexOrderInv(dccIndexOrder.size());
    for (auto it { dccIndexOrder.cbegin() }; it != dccIndexOrder.cend(); ++it) {
        auto idx { std::distance(dccIndexOrder.cbegin(), it) };
        dccIndexOrderInv[*it] = static_cast<size_t>(idx);
    }
    std::for_each(vertToDcc.begin(), vertToDcc.end(), [&dccIndexOrderInv](auto &p) { p.second = dccIndexOrderInv[p.second]; });
    for (const auto &idx : dccIndexOrder) {
        std::list<T> orderedDcc { orderDcc(dccIndexOrderInv[idx], dccsVerts[idx], vertToDcc) };
        linearVertices.splice(linearVertices.end(), orderedDcc);
    }
    return linearVertices;
}

template<class T>
auto DoubleGraph<T>::splitOnDoubleEdges() const
  -> std::tuple<std::vector<std::unordered_set<T>>, std::unordered_map<T, size_t>, std::vector<std::unordered_map<size_t, size_t>>>
{
    const std::vector<std::unordered_set<T>> &dccsVerts { doubleEdgesGraph_.getVerticesOfCcs() };
    std::unordered_map<T, size_t> vertToDcc;
    std::vector<std::unordered_map<size_t, size_t>> dccsAdjs { dccsVerts.size() };
    for (const auto &group : dccsVerts) {
        for (const auto &[vertex, _] : singleEdgesGraph_.getAdjacencyList()) {
            if (group.contains(vertex))
                vertToDcc[vertex] = static_cast<size_t>(&group - &dccsVerts.front());
        }
    }
    for (const auto &[vertex, adjs] : singleEdgesGraph_.getAdjacencyList()) {
        for (const auto &adj : adjs) {
            if (vertToDcc[adj] == vertToDcc[vertex])
                continue;
            auto &map { dccsAdjs.at(vertToDcc[vertex]) };
            const auto &key { vertToDcc[adj] };
            map.contains(key) ? map[key] += 1 : map[key] = 1;
        }
    }
    return { dccsVerts, vertToDcc, dccsAdjs };
}

template<class T>
auto DoubleGraph<T>::orderDccs(std::vector<std::unordered_map<size_t, size_t>> &dcc_adjs) const -> std::list<size_t>
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
    std::list<size_t> dccIndexOrder;
    dccIndexOrder.push_back(idx);
    std::for_each(dcc_adjs.begin(), dcc_adjs.end(), [&idx](auto &map) { map.erase(idx); });

    auto LessOnSecond { [](const auto &a, const auto &b) {
        return a.second < b.second;
    } };

    size_t numDccs { dcc_adjs.size() };
    while (dccIndexOrder.size() < numDccs) {
        std::unordered_map<size_t, size_t> front {};
        for (auto it { dccIndexOrder.cbegin() }; front.empty(); ++it) {
            front = dcc_adjs.at(*it);
        }
        std::unordered_map<size_t, size_t> back {};
        for (auto it { dccIndexOrder.crbegin() }; back.empty(); ++it) {
            back = dcc_adjs.at(*it);
        }

        auto fIt { std::max_element(front.cbegin(), front.cend(), LessOnSecond) };
        auto bIt { std::max_element(back.cbegin(), back.cend(), LessOnSecond) };

        if (
          (front == dcc_adjs.at(dccIndexOrder.front()) && back == dcc_adjs.at(dccIndexOrder.back()))
          || (front != dcc_adjs.at(dccIndexOrder.front()) && back != dcc_adjs.at(dccIndexOrder.back()))) {
            if (fIt->second > bIt->second) {
                idx = fIt->first;
                dccIndexOrder.push_front(idx);
            } else {
                idx = bIt->first;
                dccIndexOrder.push_back(idx);
            }
        } else if (front == dcc_adjs.at(dccIndexOrder.front())) {
            idx = fIt->first;
            dccIndexOrder.push_front(idx);
        } else if (back == dcc_adjs.at(dccIndexOrder.back())) {
            idx = bIt->first;
            dccIndexOrder.push_back(idx);
        }
        std::for_each(dcc_adjs.begin(), dcc_adjs.end(), [&idx](auto &map) { map.erase(idx); });
    }
    return dccIndexOrder;
}

template<class T>
auto DoubleGraph<T>::orderDcc(const size_t position, const std::unordered_set<T> &vertices, const std::unordered_map<T, size_t> &vert_to_dcc) const
  -> std::list<T>
{
    if (vertices.size() == 1) {
        return { *vertices.cbegin() };
    }

    const auto &doubleEdgeAdjs { doubleEdgesGraph_.getAdjacencyList() };
    T vertex { *vertices.cbegin() };
    size_t maxCon { doubleEdgeAdjs.at(*vertices.cbegin()).size() };
    for (const auto &v : vertices) {
        if (const auto &adjs { doubleEdgeAdjs.at(v) }; adjs.size() > maxCon) {
            maxCon = adjs.size();
            vertex = v;
        }
    }

    std::list<T> orderedDcc { vertex };
    std::unordered_set<T> visited { vertex };
    std::vector<std::deque<T>> toVisit;
    std::vector<std::list<T>> chains;

    for (const auto &adj : doubleEdgeAdjs.at(vertex)) {
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
            const auto &adjs { doubleEdgeAdjs.at(vertex) };
            for (const T &adj : adjs) {
                if (visited.contains(adj))
                    continue;
                chains[idx].push_back(adj);
                toVisit[idx].push_back(adj);
            }
        }
    }
    visited.clear();

    std::list<std::pair<size_t, int>> balances;
    for (size_t idx = 0; idx < chains.size(); idx += 1) {
        balances.emplace_back(idx, 0);
        for (const auto &v : chains[idx]) {
            for (const auto &adj : singleEdgesGraph_.getAdjacencyList().at(v)) {
                if (vert_to_dcc.at(adj) > position)
                    balances.back().second += 1;
                else if (vert_to_dcc.at(adj) < position)
                    balances.back().second -= 1;
            }
        }
    }
    balances.sort([](const auto &a, const auto &b) { return a.second < b.second; });

    std::vector<size_t> indicesL;
    std::vector<size_t> indicesR;
    size_t countL { 0 };
    size_t countR { 0 };
    while (not balances.empty()) {
        if (countL > countR) {
            indicesR.push_back(balances.back().first);
            balances.pop_back();
            countR += chains[indicesR.back()].size();
        } else {
            indicesL.push_back(balances.front().first);
            balances.pop_front();
            countL += chains[indicesL.back()].size();
        }
    }

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
// =============================== Helpers

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