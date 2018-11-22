#ifndef __SafeInt_hpp_2018_09_14_14_21
#define __SafeInt_hpp_2018_09_14_14_21
#include <cstdint>
#include <cstddef>
#include <limits>
namespace safe{
    enum class int8_t:std::int8_t{
        MIN = -127,
        MAX = 128,
        ZERO = 0,
        ONE = 1
    };
    enum class uint8_t:std::uint8_t{
        MIN = 0,
        MAX = 255,
        ZERO = 0,
        ONE = 1
    };
    enum class int16_t:std::int16_t{
        MIN = -32768,
        MAX = 32767,
        ZERO = 0,
        ONE = 1
    };
    enum class uint16_t:std::uint16_t{
        MIN = 0,
        MAX = 65536,
        ZERO = 0,
        ONE = 1
    };
    enum class int32_t:std::int32_t{
        MIN = std::numeric_limits<std::int32_t>::min(),
        MAX = std::numeric_limits<std::int32_t>::max(),
        ZERO = 0,
        ONE = 1
    };
    enum class uint32_t:std::uint32_t{
        MIN = 0,
        MAX = std::numeric_limits<std::uint32_t>::max(),
        ZERO = 0,
        ONE = 1
    };
    enum class int64_t:std::int64_t{
        MIN = std::numeric_limits<std::int64_t>::min(),
        MAX = std::numeric_limits<std::int64_t>::max(),
        ZERO = 0,
        ONE = 1
    };
    enum class uint64_t:std::uint64_t{
        MIN = 0,
        MAX = std::numeric_limits<std::uint64_t>::max(),
        ZERO = 0,
        ONE = 1
    };
}

namespace std{
    template<> struct numeric_limits<safe::int8_t>{
    public:
        static constexpr bool is_specialized{true};
        static constexpr bool is_signed{true};
        static constexpr bool is_integer{true};
        static constexpr bool is_exact{true};
        static constexpr bool has_infinity{false};
        static constexpr bool has_quiet_NaN{false};
        static constexpr bool has_signaling_NaN{false};
        static constexpr float_denorm_style has_denorm{denorm_absent};
        static constexpr bool has_denorm_loss{false};
        static constexpr float_round_style round_style{round_toward_zero};
        static constexpr bool is_iec559{false};
        static constexpr bool is_modulo{false};
        static constexpr int digits{7};
        static constexpr int digits10{2};
        static constexpr int max_digits10{3};
        static constexpr int radix{2};
        static constexpr int min_exponent{0};
        static constexpr int min_exponent10{0};
        static constexpr int max_exponent{0}; 
        static constexpr int max_exponent10{0};
        static constexpr bool traps{true};
        static constexpr bool tinyness_before{false};
    };
};

#endif
