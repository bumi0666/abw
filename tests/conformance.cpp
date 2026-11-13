#include <abw/abw.hpp>
#include "reference_compat.hpp"
#include "reference/v012_candidate.hpp"
#include <limits>
#include <stdexcept>

static std::size_t checked = 0, direct = 0, partitioned = 0;
const char* label(abw::detail::strategy s) {
    using S = abw::detail::strategy;
    switch (s) {
    case S::native: return "native";
    case S::skip: return "skip";
    case S::active8: return "active8";
    case S::active12: return "active12";
    }
    throw std::runtime_error("strategy");
}
void check(const V& input) {
    const auto old = frozen_v012::detail::choose(input);
    const auto now = abw::detail::choose(input);
    if (int(old.selected) != int(now.selected))
        throw std::runtime_error("dispatch differs from frozen reference");
    // Native exits do not execute bits/shift; B may leave those fields at zero.
    const bool active = old.selected == frozen_v012::detail::strategy::active8 ||
                        old.selected == frozen_v012::detail::strategy::active12;
    if (active && (old.bits != now.bits || old.shift != now.shift))
        throw std::runtime_error("active bit interval differs from frozen reference");
    auto expected = input;
    std::sort(expected.begin(), expected.end());
    auto actual = input;
    abw::sort(actual);
    if (actual != expected) throw std::runtime_error("public sort differs from std::sort");
    if (old.selected == frozen_v012::detail::strategy::active8 || old.selected == frozen_v012::detail::strategy::active12) {
        auto a = input, b = input;
        const bool x = frozen_v012::detail::partition_and_sort(a, old.bits, old.shift);
        const bool y = abw::detail::partition_and_sort(b, now.bits, now.shift);
        if (x != y || a != expected || b != expected)
            throw std::runtime_error("execution path differs from frozen reference");
        direct += x;
        partitioned += !x;
    }
    ++checked;
}
int main(int argc, char** argv) {
    try {
        check({}); check({0}); check({UINT32_MAX});
        check({UINT32_MAX, 0, 0x80000000u, 1, UINT32_MAX, 0});
        check({3, 3, 3, 3}); check({9, 7, 5, 3, 1});
        RNG r(0x183b7d65);
        for (std::size_t n : {2u, 31u, 100u, 99999u, 100000u, 100001u, 999999u, 1000000u}) {
            V a(n);
            for (auto& v : a) v = r.next();
            check(a);
        }
        for (unsigned width = 1; width <= 32; ++width) {
            const U mask = width == 32 ? UINT32_MAX : (U(1) << width) - 1;
            const U base = 0xa5a5a5a5u & ~mask;
            V a(100003);
            for (auto& v : a) v = base | (r.next() & mask);
            check(a);
        }
        for (U cardinality : {2u, 4u, 8u, 13u, 256u, 1024u, 4096u}) {
            V a(100003);
            for (auto& v : a) v = (r.next() % cardinality) * 0x10001u;
            check(a);
        }
        for (unsigned form = 0; form < 4; ++form) {
            V a(100003);
            for (std::size_t i = 0; i < a.size(); ++i) a[i] = U(i);
            if (form == 1) std::reverse(a.begin(), a.end());
            if (form == 2) std::fill(a.begin(), a.end(), UINT32_MAX);
            if (form == 3) for (unsigned j = 0; j < 100; ++j) std::swap(a[r.next()%a.size()], a[r.next()%a.size()]);
            check(a);
        }
        const U ids[] = {0, 36, 73, 109, 146, 182, 219, 255};
        for (int position : {-1, 1, 50001, 100002}) {
            V a(100003);
            for (auto& v : a) v = (ids[r.next()%8] << 24) | 0x1234u;
            a[0] = 0x1234u;
            if (position >= 0) a[std::size_t(position)] = 0x1235u;
            check(a);
        }
        // Explicitly exercise prefix reconstruction, both bucket widths and tails.
        std::size_t prefix_cases = 0;
        for (U base : {0x12345000u, 0xfffff000u}) {
            for (unsigned bits : {8u, 12u}) {
                for (std::size_t n : {100000u, 100001u, 100002u, 100003u}) {
                    V a(n);
                    const U mask = (U(1) << bits) - 1;
                    for (auto& v : a) v = base | (r.next() & mask);
                    a[0] = base;
                    a[1] = base | mask;
                    const auto d = abw::detail::choose(a);
                    const auto want = bits == 8 ? abw::detail::strategy::active8
                                                : abw::detail::strategy::active12;
                    if (d.selected != want || d.bits != bits || d.shift != 0 ||
                        (a[0] & ~mask) == 0)
                        throw std::runtime_error("prefix test did not enter shortcut");
                    check(a);
                    ++prefix_cases;
                }
            }
        }
        if (prefix_cases != 16) throw std::runtime_error("prefix coverage");
        // Reopened ordered/low-entropy inputs, with nonzero shared prefixes.
        for (U base : {0u, 0x12345600u, 0xffffff00u}) {
            for (unsigned form = 0; form < 3; ++form) {
                V a(100003);
                for (std::size_t i = 0; i < a.size(); ++i)
                    a[i] = base | (form == 0 ? U(i >= 65002) : form == 1 ? U(i % 7) : U(0));
                const auto d = abw::detail::choose(a);
                if (form != 2 && (d.selected != abw::detail::strategy::active8 || d.shift != 0))
                    throw std::runtime_error("low-byte direct path not selected");
                check(a);
            }
        }
        // A late high bit missed by samples must not authorize low-byte output.
        for (std::size_t pos : {1u, 50001u, 100002u}) {
            V a(100003);
            for (std::size_t i = 0; i < a.size(); ++i) a[i] = U(i >= 65002);
            a[pos] = 0x80000000u;
            const auto d = abw::detail::choose(a);
            if (d.selected != abw::detail::strategy::native)
                throw std::runtime_error("ordered high-bit counterexample partitioned");
            check(a);
        }
        for (int i = 1; i < argc; ++i) check(load(argv[i]));
        std::cout << "PASS cases=" << checked << " direct=" << direct
                  << " partitioned=" << partitioned << '\n';
    } catch (const std::exception& e) {
        std::cerr << "FAIL: " << e.what() << '\n';
        return 1;
    }
}
