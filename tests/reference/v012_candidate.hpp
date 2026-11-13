// SPDX-License-Identifier: MIT
// Copyright (c) 2026 bumi0666
#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <boost/sort/pdqsort/pdqsort.hpp>
#if defined(_MSC_VER)
#include <intrin.h>
#endif

namespace frozen_v012 {
namespace detail {
using value_type = std::uint32_t;
using array_type = std::vector<value_type>;
static_assert(sizeof(std::size_t) >= 8, "ABW currently supports 64-bit targets");

enum class strategy { native, skip, active8, active12 };
struct decision {
    strategy selected = strategy::native;
    unsigned bits = 0;
    unsigned shift = 0;
};

struct rng {
    value_type state;
    explicit rng(value_type seed) : state(seed) {}
    value_type next() {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return state;
    }
};

inline unsigned bit_width(value_type value) {
#if defined(_MSC_VER)
    unsigned long index;
    _BitScanReverse(&index, value);
    return static_cast<unsigned>(index + 1);
#else
    return 32u - static_cast<unsigned>(__builtin_clz(value));
#endif
}

// Called only after the unchanged minimum-size check in choose().
struct probe_result { bool ordered; value_type variation; };
inline probe_result probe(const array_type& a) {
    const auto m = std::min<std::size_t>(256, a.size() - 1);
    rng r(0x12577abc);
    std::size_t up = 0, down = 0;
    value_type variation = 0;
    const value_type reference = a[0];
    for (std::size_t j = 0; j < m; ++j) {
        const auto lo = j * (a.size() - 1) / m;
        const auto hi = (j + 1) * (a.size() - 1) / m;
        const auto i = lo + r.next() % std::max<std::size_t>(1, hi - lo);
        const auto x = a[i], y = a[i + 1];
        up += x < y;
        down += x > y;
        variation |= (x ^ reference) | (y ^ reference);
    }
    return {double(up) / m <= .1 || double(down) / m <= .1, variation};
}

// Frozen E policy. No new thresholds, guards, or sample positions.
inline decision choose(const array_type& a) {
    decision d;
    if (a.size() < 100000) return d;
    const auto order = probe(a);
    // Keep ordered/native exits unless sampled variation is nonzero and within 8 bits.
    if (order.ordered && (order.variation == 0 || order.variation > 255u)) return d;
    const auto m = std::min<std::size_t>(a.size() - 1,
                                        a.size() < 1000000 ? 512 : 2048);
    array_type values(m);
    std::unordered_map<value_type, std::size_t> frequency;
    rng r(0x55ab76c1);
    std::size_t mode = 0;
    for (std::size_t j = 0; j < m; ++j) {
        const auto lo = j * (a.size() - 1) / m;
        const auto hi = (j + 1) * (a.size() - 1) / m;
        const auto value = a[lo + r.next() % std::max<std::size_t>(1, hi - lo)];
        values[j] = value;
        mode = (std::max)(mode, ++frequency[value]);
    }
    const bool duplicate = 1 - double(frequency.size()) / m >= .5 ||
                           double(mode) / m >= .1;
    d.bits = duplicate ? 8 : 12;
    value_type difference = 0;
    for (std::size_t i = 1; i < a.size(); ++i) difference |= a[i] ^ a[0];
    if (!difference) {
        d.selected = strategy::skip;
        return d;
    }
    const auto width = bit_width(difference);
    d.shift = width > d.bits ? width - d.bits : 0;
    if (width <= 8) {
        // Exact whole-array certificate; bypass entropy only for this direct path.
        d.bits = 8;
        d.shift = 0;
        d.selected = strategy::active8;
        return d;
    }
    // A sampled certificate alone cannot authorize partitioning an ordered input.
    if (order.ordered) return d;
    const value_type mask = (1u << d.bits) - 1;
    std::vector<std::size_t> counts(1u << d.bits);
    for (const auto value : values) ++counts[(value >> d.shift) & mask];
    double entropy = 0;
    for (const auto count : counts) {
        if (count) {
            const double p = double(count) / m;
            entropy -= p * std::log2(p);
        }
    }
    if (entropy >= (duplicate ? 1 : 4))
        d.selected = duplicate ? strategy::active8 : strategy::active12;
    return d;
}

// Exact B execution path. Representative zero initialization is retained.
inline bool partition_and_sort(array_type& a, unsigned bits, unsigned shift) {
    // choose() has proved that all bits above mask are common.
    if (shift == 0) {
        const std::size_t B = std::size_t(1) << bits;
        const value_type mask = static_cast<value_type>(B - 1);
        const value_type prefix = a[0] & ~mask;
        std::vector<std::size_t> counts(bits == 8 ? 4 * B : B);
        if (bits == 8) {
            std::size_t i = 0;
            for (; i + 3 < a.size(); i += 4) {
                ++counts[a[i] & mask];
                ++counts[B + (a[i + 1] & mask)];
                ++counts[2 * B + (a[i + 2] & mask)];
                ++counts[3 * B + (a[i + 3] & mask)];
            }
            for (; i < a.size(); ++i) ++counts[a[i] & mask];
            for (std::size_t b = 0; b < B; ++b)
                counts[b] += counts[B + b] + counts[2 * B + b] + counts[3 * B + b];
        } else {
            for (const auto value : a) ++counts[value & mask];
        }
        std::size_t position = 0;
        for (std::size_t b = 0; b < B; ++b) {
            if (counts[b]) {
                std::fill_n(a.begin() + position, counts[b], prefix | static_cast<value_type>(b));
                position += counts[b];
            }
        }
        return true;
    }

    const std::size_t bucket_count = std::size_t(1) << bits;
    const value_type mask = static_cast<value_type>(bucket_count - 1);
    std::vector<std::size_t> counts(bucket_count), starts(bucket_count + 1);
    array_type representative(bucket_count);
    bool single = true;
    std::size_t i = 0;
    for (; i < a.size(); ++i) {
        const auto value = a[i];
        const std::size_t bucket = (value >> shift) & mask;
        const auto old = counts[bucket]++;
        if (!old) representative[bucket] = value;
        else if (representative[bucket] != value) {
            single = false;
            ++i;
            break;
        }
    }
    for (; i < a.size(); ++i) ++counts[(a[i] >> shift) & mask];
    if (single) {
        std::size_t position = 0;
        for (std::size_t b = 0; b < bucket_count; ++b) {
            if (counts[b]) {
                std::fill_n(a.begin() + position, counts[b], representative[b]);
                position += counts[b];
            }
        }
        return true;
    }
    for (std::size_t b = 0; b < bucket_count; ++b)
        starts[b + 1] = starts[b] + counts[b];
    auto cursor = starts;
    array_type temporary(a.size());
    for (const auto value : a) {
        const auto b = (value >> shift) & mask;
        temporary[cursor[b]++] = value;
    }
    for (std::size_t b = 0; b < bucket_count; ++b) {
        if (counts[b] > 1)
            boost::sort::pdqsort(temporary.begin() + starts[b],
                                temporary.begin() + starts[b + 1]);
    }
    std::copy(temporary.begin(), temporary.end(), a.begin());
    return false;
}
} // namespace detail

// Sort uint32_t keys in nondecreasing order. Allocations may throw.
// No stability or attached-record/payload support is provided.
inline void sort(std::vector<std::uint32_t>& values) {
    const auto d = detail::choose(values);
    if (d.selected == detail::strategy::native)
        boost::sort::pdqsort(values.begin(), values.end());
    else if (d.selected != detail::strategy::skip)
        detail::partition_and_sort(values, d.bits, d.shift);
}
} // namespace abw
