#include <abw/abw.hpp>
#include <unordered_map>
#include <iostream>
#include <limits>
#include <stdexcept>

using U = std::uint32_t;
using V = std::vector<U>;
static std::size_t cases = 0;
void check(const V& values) {
    if (values.size() > 2048) throw std::runtime_error("sample bound");
    abw::detail::fixed_frequency table;
    std::unordered_map<U, std::size_t> reference;
    std::size_t mode = 0;
    for (U value : values) {
        const auto expected = ++reference[value];
        const auto actual = table.increment(value);
        if (actual != expected || table.distinct != reference.size())
            throw std::runtime_error("frequency mismatch");
        mode = (std::max)(mode, actual);
    }
    std::size_t reference_mode = 0;
    for (const auto& entry : reference) reference_mode = (std::max)(reference_mode, entry.second);
    if (mode != reference_mode || table.distinct > 2048)
        throw std::runtime_error("summary/bound mismatch");
    ++cases;
}
int main() {
    try {
        constexpr auto capacity = abw::detail::fixed_frequency::capacity;
        static_assert(capacity == 4096 && (capacity & (capacity - 1)) == 0, "mask wraps");
        static_assert(capacity >= 2 * 2048, "maximum sample cannot saturate table");
        static_assert((std::numeric_limits<std::uint16_t>::max)() >= 2048, "counts fit");
        check({}); check({0, UINT32_MAX, 0x80000000u, 0, UINT32_MAX});
        abw::detail::rng random(729831);
        for (std::size_t n : {1u, 31u, 512u, 2048u}) {
            check(V(n, 0)); check(V(n, UINT32_MAX));
            V values(n);
            for (std::size_t i = 0; i < n; ++i) values[i] = U(i);
            check(values);
            for (auto& value : values) value = random.next();
            check(values);
            for (auto& value : values) value = random.next() % 8;
            check(values);
        }
        // Maximum supported distinct count, all starting at the last slot:
        // this forces a 2048-key probe chain across the end of the table.
        V collision;
        for (U value = 0; collision.size() < 2048; ++value)
            if ((abw::detail::fixed_frequency::hash(value) & (capacity - 1)) == capacity - 1)
                collision.push_back(value);
        check(collision);
        std::reverse(collision.begin(), collision.end()); check(collision);
        for (unsigned seed = 0; seed < 32; ++seed) {
            V values(2048); for (auto& value : values) value = random.next(); check(values);
        }
        // Exercise the actual helper and deterministic sample positions.
        for (std::size_t n : {100000u, 1000000u}) {
            V input(n); for (auto& value : input) value = random.next();
            const std::size_t m = n < 1000000 ? 512 : 2048;
            V sampled(m), expected(m);
            const auto summary = abw::detail::collect_sample(input, sampled, m);
            abw::detail::rng positions(0x55ab76c1);
            std::unordered_map<U, std::size_t> reference;
            std::size_t mode = 0;
            for (std::size_t j = 0; j < m; ++j) {
                const auto lo = j * (n - 1) / m;
                const auto hi = (j + 1) * (n - 1) / m;
                expected[j] = input[lo + positions.next() % (std::max)(std::size_t(1), hi - lo)];
                mode = (std::max)(mode, ++reference[expected[j]]);
            }
            if (sampled != expected || summary.distinct != reference.size() || summary.mode != mode)
                throw std::runtime_error("sample helper mismatch");
            ++cases;
        }
        std::cout << "PASS frequency_cases=" << cases
                  << " max_samples=2048 collision_chain=2048 wraparound=yes full_u32=yes\n";
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n'; return 1;
    }
}
