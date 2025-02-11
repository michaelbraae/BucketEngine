#pragma once

#include <functional>

namespace bucketengine
{
    template <typename T, typename... Rest>
    void hashCombine(std::size_t& seed, const T& v, const Rest&... rest)
    {
        seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 21);
        (hashCombine(seed, rest), ...);
    }
}
