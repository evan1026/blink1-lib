#include "RGB.hpp"

namespace blink1_lib {
    RGB::RGB(const std::uint8_t _r, const std::uint8_t _g, const std::uint8_t _b) noexcept : r(_r), g(_g), b(_b) {}

    bool RGB::operator==(const RGB& other) const noexcept {
        return r == other.r && g == other.g && b == other.b;
    }

    bool RGB::operator!=(const RGB& other) const noexcept {
        return !(*this == other);
    }

    std::ostream& operator<<(std::ostream& os, const RGB& rgb) {
        os << "RGB{r=" << static_cast<unsigned>(rgb.r)
            << ", g=" << static_cast<unsigned>(rgb.g)
            << ", b=" << static_cast<unsigned>(rgb.b) << "}";
        return os;
    }
}
