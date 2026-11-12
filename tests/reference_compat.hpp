#pragma once
// Test-only adapter: retain the archived reference sources byte-for-byte.
#if defined(_MSC_VER)
#include <intrin.h>
inline unsigned abw_reference_clz(unsigned value) {
    unsigned long index;
    return _BitScanReverse(&index, value) ? 31u - unsigned(index) : 32u;
}
#define __builtin_clz abw_reference_clz
#endif
#include "reference/frozen_prefix.hpp"
#if defined(_MSC_VER)
#undef __builtin_clz
#endif
