/*
 * Copyright (c) 2026 Suyash Bhattarai
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>

#include "cpp_common/identifiers.hpp"

namespace {

template <typename T, std::size_t N>
pgrouting::Identifiers<T> identifiers_from_mask(
        const std::array<T, N>& values, std::size_t mask) {
    pgrouting::Identifiers<T> result;
    for (std::size_t bit = 0; bit < N; ++bit) {
        if (mask & (std::size_t{1} << bit)) result += values[bit];
    }
    return result;
}

template <typename T, std::size_t N>
bool check_equality(const std::array<T, N>& values) {
    const auto subsets = std::size_t{1} << N;
    for (std::size_t left_mask = 0; left_mask < subsets; ++left_mask) {
        const auto left = identifiers_from_mask(values, left_mask);
        for (std::size_t right_mask = 0; right_mask < subsets; ++right_mask) {
            const auto right = identifiers_from_mask(values, right_mask);
            // Each distinct bit mask represents a distinct set of identifiers.
            if ((left == right) != (left_mask == right_mask)) {
                std::cerr << "Incorrect equality for " << left << " and " << right << '\n';
                return false;
            }
        }
    }
    return true;
}

}  // namespace

int main() {
    const std::array<int64_t, 5> signed_values{
        -7, 0, 3, 42, (std::numeric_limits<int64_t>::max)()};
    const std::array<std::size_t, 5> unsigned_values{
        0, 3, 42, 1000, (std::numeric_limits<std::size_t>::max)()};
    if (!check_equality(signed_values) || !check_equality(unsigned_values)) return 1;
    std::cout << "All 2048 identifier-set comparisons passed.\n";
    return 0;
}
