#ifndef UNDIRECTED_GRAPH_HPP
#define UNDIRECTED_GRAPH_HPP
#pragma once

#include <ostream>

template<class T>
class UndirectedGraph
{
public:
    // ------------- Constructors, Destructors, Assignments
    UndirectedGraph() = default;
    UndirectedGraph(const UndirectedGraph<T> &other);
    UndirectedGraph(UndirectedGraph<T> &&other) noexcept = default;

    virtual ~UndirectedGraph() = default;

    UndirectedGraph<T> &operator=(const UndirectedGraph<T> &other);
    UndirectedGraph<T> &operator=(UndirectedGraph<T> &&other) noexcept = default;

    // ------------- Setters, Getters

    // ------------- Operators

    // ------------- Misc

private:
    // ------------- Helpers
};

template<class T>
auto operator<<(std::ostream &os, const UndirectedGraph<T> &g) -> std::ostream &;

#include <undirected_graph-impl.hpp>

#endif    // UNDIRECTED_GRAPH_HPP