#include "tienich.h"
#include <stdexcept>

// 🔹 Hàm chuyển đổi an toàn từ size_t sang int
int safe_size_t_to_int(const size_t value)
{
    if (value > static_cast<size_t>(std::numeric_limits<int>::max()))
    {
        throw std::overflow_error("Value is too large to fit in an int");
    }
    return static_cast<int>(value);
}

// 🔹 Hàm chuyển đổi an toàn từ int sang size_t
size_t safe_int_to_size_t(const int value)
{
    if (value < 0)
    {
        throw std::underflow_error("Value is negative and cannot be converted to size_t");
    }
    return static_cast<size_t>(value);
}