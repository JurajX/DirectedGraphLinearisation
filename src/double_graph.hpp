#ifndef DOUBLE_GRAPH_HPP
#define DOUBLE_GRAPH_HPP
#pragma once

#include <ostream>

template<class T>
class DoubleGraph
{
public:
    // ------------- Constructors, Destructors, Assignments
    DoubleGraph() = default;
    DoubleGraph(const DoubleGraph<T> &other);
    DoubleGraph(DoubleGraph<T> &&other) noexcept = default;

    virtual ~DoubleGraph() = default;

    DoubleGraph<T> &operator=(const DoubleGraph<T> &other);
    DoubleGraph<T> &operator=(DoubleGraph<T> &&other) noexcept = default;

    // ------------- Setters, Getters

    // ------------- Operators

    // ------------- Misc

private:
    // ------------- Helpers
};

template<class T>
auto operator<<(std::ostream &os, const DoubleGraph<T> &g) -> std::ostream &;

#include <double_graph-impl.hpp>

#endif    // DOUBLE_GRAPH_HPP