#ifndef TEST_DATA_HPP
#define TEST_DATA_HPP
#pragma once

#include <unordered_map>
#include <unordered_set>

namespace test {
const std::unordered_set<char> keysAdjList0 { 'a', 'b', 'c', 'd' };
const std::unordered_map<char, std::unordered_set<char>> adjList0 {
    {'a',           { 'b' }},
    {'b',           { 'd' }},
    {'c', { 'a', 'b', 'd' }},
    {'d',           { 'a' }}
};

const std::unordered_set<char> keysAdjList1 { 'a', 'b', 'c', 'd' };
const std::unordered_map<char, std::unordered_set<char>> adjList1 {
    {'a', { 'b', 'c', 'd' }},
    {'b', { 'a', 'c', 'd' }},
    {'c', { 'a', 'b', 'd' }},
    {'d', { 'a', 'b', 'c' }}
};

const std::unordered_set<char> keysAdjList2 { 'e', 'f', 'g', 'h' };
const std::unordered_map<char, std::unordered_set<char>> adjList2 {
    {'e', { 'h', 'f', 'g' }},
    {'f',      { 'h', 'g' }},
    {'g',                {}},
    {'h',           { 'e' }},
};

const std::unordered_set<char> keysAdjList3 { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
const std::unordered_map<char, std::unordered_set<char>> adjList3 {
    {'a', { 'b', 'c', 'd' }},
    {'b', { 'a', 'c', 'd' }},
    {'c', { 'a', 'b', 'd' }},
    {'d', { 'a', 'b', 'c' }},
    {'e', { 'h', 'f', 'g' }},
    {'f',      { 'h', 'g' }},
    {'g',                {}},
    {'h',           { 'e' }},
};

}    // namespace test

#endif    // TEST_DATA_HPP
