#ifndef TEST_DATA_HPP
#define TEST_DATA_HPP
#pragma once

#include <list>
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

const std::unordered_map<char, std::unordered_set<char>> alDiscon {
    {'a',           { 'c' }},
    {'b',           { 'c' }},
    {'c', { 'a', 'b', 'd' }},
    {'d',           { 'c' }},
    {'e', { 'h', 'f', 'g' }},
    {'f',      { 'h', 'g' }},
    {'g',                {}},
    {'h',           { 'e' }},
    {'i',      { 'j', 'm' }},
    {'j', { 'i', 'k', 'l' }},
    {'k', { 'j', 'l', 'm' }},
    {'l',                {}},
    {'m',           { 'k' }},
    {'n',      { 'm', 'l' }}
};
const std::unordered_set<char> keysAlDiscon { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n' };
const std::unordered_set<char> keysAlDiscon1 { 'a', 'b', 'c', 'd' };
const std::unordered_set<char> keysAlDiscon2 { 'e', 'f', 'g', 'h' };
const std::unordered_set<char> keysAlDiscon3 { 'i', 'j', 'k', 'l', 'm', 'n' };

const std::unordered_map<char, std::unordered_set<char>> alCon {
    {'a',      { 'c', 'i' }},
    {'b',      { 'c', 'e' }},
    {'c', { 'a', 'b', 'd' }},
    {'d',      { 'c', 'i' }},
    {'e',      { 'h', 'f' }},
    {'f',      { 'h', 'g' }},
    {'g',                {}},
    {'h',           { 'e' }},
    {'i',      { 'j', 'm' }},
    {'j', { 'i', 'k', 'l' }},
    {'k', { 'j', 'l', 'm' }},
    {'l',                {}},
    {'m',           { 'k' }},
    {'n',      { 'm', 'l' }}
};
const std::unordered_set<char> keysAlCon { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n' };
const std::list<char> alConLin { 'n', 'l', 'm', 'k', 'j', 'i', 'a', 'd', 'c', 'b', 'e', 'h', 'f', 'g' };

}    // namespace test

#endif    // TEST_DATA_HPP
