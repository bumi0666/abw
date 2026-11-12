#include <abw/abw.hpp>
#include <iostream>

int main() {
    std::vector<std::uint32_t> keys{7, 2, 7, 0, 5, 2};
    abw::sort(keys);
    for (auto key : keys) std::cout << key << ' ';
    std::cout << '\n';
}
