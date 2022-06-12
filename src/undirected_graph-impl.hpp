#ifndef UNDIRECTED_GRAPH_IMPL_HPP
#define UNDIRECTED_GRAPH_IMPL_HPP
#pragma once

#include <undirected_graph.hpp>

#include <fmt/format.h>

#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <vector>

//
// =============================== Constructors, Destructors, Assignments

template<class T>
UndirectedGraph<T>::UndirectedGraph(const std::unordered_map<T, std::unordered_set<T>> &adjacency_list)
    : adjacencyList_ { adjacency_list }
{
    symmetrise();
    populateVerticesOfCcs();
}

template<class T>
UndirectedGraph<T>::UndirectedGraph(const UndirectedGraph<T> &other)
    : adjacencyList_ { other.adjacencyList_ }
    , verticesOfCcs_ { other.verticesOfCcs_ }
{ }

template<class T>
UndirectedGraph<T> &UndirectedGraph<T>::operator=(const UndirectedGraph<T> &other)
{
    return *this = UndirectedGraph(other);
}

//
// =============================== Setters, Getters

template<class T>
auto UndirectedGraph<T>::getAdjacencyList() const -> const std::unordered_map<T, std::unordered_set<T>> &
{
    return adjacencyList_;
}

template<class T>
auto UndirectedGraph<T>::getVertices() const -> std::unordered_set<T>
{
    std::unordered_set<T> vertices;
    for (const auto &[vertex, _] : adjacencyList_) {
        vertices.insert(vertex);
    }
    return vertices;
}

template<class T>
auto UndirectedGraph<T>::getNumVertices() const -> size_t
{
    return adjacencyList_.size();
}

template<class T>
auto UndirectedGraph<T>::getVerticesOfCcs() const -> const std::vector<std::unordered_set<T>> &
{
    return verticesOfCcs_;
}

template<class T>
auto UndirectedGraph<T>::getCcs() const -> std::vector<UndirectedGraph<T>>
{
    std::vector<UndirectedGraph<T>> ccs;
    std::transform(verticesOfCcs_.cbegin(), verticesOfCcs_.cend(), std::back_inserter(ccs), [this](const auto &vertices) {
        return makeSubgraph(vertices);
    });
    return ccs;
}

template<class T>
auto UndirectedGraph<T>::getNumCcs() const -> size_t
{
    return verticesOfCcs_.size();
}

template<class T>
auto UndirectedGraph<T>::isConnected() const -> bool
{
    return verticesOfCcs_.size() == 1;
}

//
// =============================== Operators

template<class T>
auto UndirectedGraph<T>::operator==(const UndirectedGraph<T> &other) const -> bool
{
    return adjacencyList_ == other.adjacencyList_;
}

template<class T>
auto UndirectedGraph<T>::operator+(const UndirectedGraph<T> &other) const -> UndirectedGraph<T>
{
    auto adjacencyList { getAdjacencyList() };
    for (const auto &[vertex, adj] : other.getAdjacencyList()) {
        adjacencyList[vertex].insert(adj.cbegin(), adj.cend());
    }
    return UndirectedGraph<T> { adjacencyList };
}

//
// =============================== Misc

template<class T>
auto UndirectedGraph<T>::makeSubgraph(const std::unordered_set<T> &vertices) const -> UndirectedGraph<T>
{
    std::unordered_map<T, std::unordered_set<T>> newAdjacencyList;
    for (const T &vertex : vertices) {
        const auto &adjs { adjacencyList_.at(vertex) };
        std::vector<T> newAdjs;
        std::copy_if(adjs.cbegin(), adjs.cend(), std::back_inserter(newAdjs), [&vertices = std::as_const(vertices)](const T &adj) {
            return vertices.contains(adj);
        });
        newAdjacencyList.try_emplace(vertex, std::move_iterator(newAdjs.begin()), std::move_iterator(newAdjs.end()));
    }
    return { newAdjacencyList };
}

template<class T>
auto UndirectedGraph<T>::contains(const T &vertex) -> bool
{
    return adjacencyList_.contains(vertex);
}

//
// =============================== Helpers

template<class T>
auto UndirectedGraph<T>::symmetrise() -> void
{
    for (const auto &[vertex, adjs] : adjacencyList_) {
        for (const auto &adj : adjs) {
            if (not adjacencyList_.contains(adj)) {
                throw std::invalid_argument("Given adjacency list contains adjacencies that are not among the vertices.");
            }
            adjacencyList_[adj].insert(vertex);
        }
    }
}

template<class T>
auto UndirectedGraph<T>::populateVerticesOfCcs() -> void
{
    std::unordered_set<T> visited;
    for (const auto &verAdjs : adjacencyList_) {
        T vertex { verAdjs.first };
        if (visited.contains(vertex))
            continue;
        std::deque<T> toVisit;
        std::unordered_set<T> connectedComponent;
        toVisit.push_back(vertex);
        connectedComponent.insert(vertex);
        // do breadth-first search
        while (not toVisit.empty()) {
            vertex = toVisit.front();
            toVisit.pop_front();
            visited.insert(vertex);
            const auto &adjs { adjacencyList_.at(vertex) };
            connectedComponent.insert(adjs.cbegin(), adjs.cend());
            std::copy_if(adjs.cbegin(), adjs.cend(), std::back_inserter(toVisit), [&visited = std::as_const(visited)](const T &adj) {
                return not visited.contains(adj);
            });
        }
        verticesOfCcs_.push_back(connectedComponent);
    }
}

//
// =============================== Non-Member Fns

template<class T>
auto operator<<(std::ostream &os, const UndirectedGraph<T> &g) -> std::ostream &
{
    for (const auto &[vortex, adjs] : g.getAdjacencyList()) {
        fmt::print("{:>10}: ", vortex);
        for (const auto &adj : adjs) {
            fmt::print("{}, ", adj);
        }
        std::cout << "\n";
    }
    return os;
}

#endif    // UNDIRECTED_GRAPH_IMPL_HPP