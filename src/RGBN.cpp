#include "RGBN.hpp"

namespace blink1_lib {
    RGBN::RGBN(const std::uint8_t _r, const std::uint8_t _g, const std::uint8_t _b, const std::uint8_t _n) noexcept
        : r(_r), g(_g), b(_b), n(_n) {}

    bool RGBN::operator==(const RGBN& other) const noexcept {
        return r == other.r && g == other.g && b == other.b && n == other.n;
    }

    bool RGBN::operator!=(const RGBN& other) const noexcept {
        return !(*this == other);
    }

    std::ostream& operator<<(std::ostream& os, const RGBN& rgb) {
        os << "RGBN{r=" << static_cast<unsigned>(rgb.r)
            << ", g=" << static_cast<unsigned>(rgb.g)
            << ", b=" << static_cast<unsigned>(rgb.b)
            << ", n=" << static_cast<unsigned>(rgb.n) << "}";
        return os;
    }
}
