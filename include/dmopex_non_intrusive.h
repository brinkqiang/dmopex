#ifndef __DMOPEX_NON_INTRUSIVE_H_INCLUDE__
#define __DMOPEX_NON_INTRUSIVE_H_INCLUDE__

#include <tuple>
#include <iostream>
#include <utility>
#include <type_traits>
#include <functional> // For std::apply

// --- detail namespace (similar to the original dmopex.h) ---
namespace dmopex_non_intrusive_detail {
    // Helper to perform element-wise operations on tuples
    template<typename Tuple1, typename Tuple2, typename Op, std::size_t... I>
    constexpr auto tuple_op_impl(const Tuple1& t1, const Tuple2& t2, Op op, std::index_sequence<I...>) {
        return std::make_tuple(op(std::get<I>(t1), std::get<I>(t2))...);
    }

    template<typename Tuple1, typename Tuple2, typename Op>
    constexpr auto tuple_op(const Tuple1& t1, const Tuple2& t2, Op op) {
        constexpr auto size1 = std::tuple_size_v<std::decay_t<Tuple1>>;
        constexpr auto size2 = std::tuple_size_v<std::decay_t<Tuple2>>;
        static_assert(size1 == size2, "Tuple sizes must match for element-wise operations");
        return dmopex_non_intrusive_detail::tuple_op_impl(t1, t2, op, std::make_index_sequence<size1>{});
    }

    // Helper to print a tuple
    template<typename Tuple, std::size_t... I>
    void print_tuple_impl(std::ostream& os, const Tuple& t, std::index_sequence<I...>) {
        os << "(";
        ((os << (I == 0 ? "" : ", ") << std::get<I>(t)), ...);
        os << ")";
    }

    template<typename Tuple>
    void print_tuple(std::ostream& os, const Tuple& t) {
        constexpr auto size = std::tuple_size_v<std::decay_t<Tuple>>;
        dmopex_non_intrusive_detail::print_tuple_impl(os, t, std::make_index_sequence<size>{});
    }
} // namespace dmopex_non_intrusive_detail

// --- Preprocessor helpers for variadic macros ---

// EXPAND macro to force another round of argument expansion if needed.
#define EXPAND(...) __VA_ARGS__

// Corrected PP_NARG to count arguments in __VA_ARGS__ (supports 0 to 64 arguments)
#define PP_NARG_IMPL( \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
    _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
    _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
    _61, _62, _63, _64, N, ...) N

#define PP_NARG(...) \
    EXPAND(PP_NARG_IMPL(__VA_ARGS__, \
    64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, \
    48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, \
    32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, \
    16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

// Concatenation helpers: PASTE(a,b) expands to a ## b after 'a' and 'b' are expanded
#define PASTE_IMPL(a, b) a##b
#define PASTE(a, b) PASTE_IMPL(a, b)


// FOR_EACH style macros to apply an operation to each argument (up to 64)
#define FE_1(OP, OBJ, M1) OP(OBJ, M1)
#define FE_2(OP, OBJ, M1, M2) OP(OBJ, M1), OP(OBJ, M2)
#define FE_3(OP, OBJ, M1, M2, M3) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3)
#define FE_4(OP, OBJ, M1, M2, M3, M4) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4)
#define FE_5(OP, OBJ, M1, M2, M3, M4, M5) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5)
#define FE_6(OP, OBJ, M1, M2, M3, M4, M5, M6) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6)
#define FE_7(OP, OBJ, M1, M2, M3, M4, M5, M6, M7) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6), OP(OBJ, M7)
#define FE_8(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6), OP(OBJ, M7), OP(OBJ, M8)
#define FE_9(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6), OP(OBJ, M7), OP(OBJ, M8), OP(OBJ, M9)
#define FE_10(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6), OP(OBJ, M7), OP(OBJ, M8), OP(OBJ, M9), OP(OBJ, M10)
#define FE_11(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6), OP(OBJ, M7), OP(OBJ, M8), OP(OBJ, M9), OP(OBJ, M10), OP(OBJ, M11)
#define FE_12(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6), OP(OBJ, M7), OP(OBJ, M8), OP(OBJ, M9), OP(OBJ, M10), OP(OBJ, M11), OP(OBJ, M12)
#define FE_13(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6), OP(OBJ, M7), OP(OBJ, M8), OP(OBJ, M9), OP(OBJ, M10), OP(OBJ, M11), OP(OBJ, M12), OP(OBJ, M13)
#define FE_14(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6), OP(OBJ, M7), OP(OBJ, M8), OP(OBJ, M9), OP(OBJ, M10), OP(OBJ, M11), OP(OBJ, M12), OP(OBJ, M13), OP(OBJ, M14)
#define FE_15(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6), OP(OBJ, M7), OP(OBJ, M8), OP(OBJ, M9), OP(OBJ, M10), OP(OBJ, M11), OP(OBJ, M12), OP(OBJ, M13), OP(OBJ, M14), OP(OBJ, M15)
#define FE_16(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6), OP(OBJ, M7), OP(OBJ, M8), OP(OBJ, M9), OP(OBJ, M10), OP(OBJ, M11), OP(OBJ, M12), OP(OBJ, M13), OP(OBJ, M14), OP(OBJ, M15), OP(OBJ, M16)
#define FE_17(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17) \
    FE_16(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16), OP(OBJ, M17)
#define FE_18(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18) \
    FE_17(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17), OP(OBJ, M18)
#define FE_19(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19) \
    FE_18(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18), OP(OBJ, M19)
#define FE_20(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20) \
    FE_19(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19), OP(OBJ, M20)
#define FE_21(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21) \
    FE_20(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20), OP(OBJ, M21)
#define FE_22(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22) \
    FE_21(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21), OP(OBJ, M22)
#define FE_23(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23) \
    FE_22(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22), OP(OBJ, M23)
#define FE_24(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24) \
    FE_23(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23), OP(OBJ, M24)
#define FE_25(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25) \
    FE_24(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24), OP(OBJ, M25)
#define FE_26(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26) \
    FE_25(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25), OP(OBJ, M26)
#define FE_27(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27) \
    FE_26(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26), OP(OBJ, M27)
#define FE_28(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28) \
    FE_27(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27), OP(OBJ, M28)
#define FE_29(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29) \
    FE_28(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28), OP(OBJ, M29)
#define FE_30(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30) \
    FE_29(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29), OP(OBJ, M30)
#define FE_31(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31) \
    FE_30(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30), OP(OBJ, M31)
#define FE_32(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32) \
    FE_31(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31), OP(OBJ, M32)
#define FE_33(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33) \
    FE_32(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32), OP(OBJ, M33)
#define FE_34(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34) \
    FE_33(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33), OP(OBJ, M34)
#define FE_35(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35) \
    FE_34(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34), OP(OBJ, M35)
#define FE_36(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36) \
    FE_35(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35), OP(OBJ, M36)
#define FE_37(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37) \
    FE_36(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36), OP(OBJ, M37)
#define FE_38(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38) \
    FE_37(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37), OP(OBJ, M38)
#define FE_39(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39) \
    FE_38(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38), OP(OBJ, M39)
#define FE_40(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40) \
    FE_39(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39), OP(OBJ, M40)
#define FE_41(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41) \
    FE_40(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40), OP(OBJ, M41)
#define FE_42(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42) \
    FE_41(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41), OP(OBJ, M42)
#define FE_43(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43) \
    FE_42(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42), OP(OBJ, M43)
#define FE_44(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44) \
    FE_43(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43), OP(OBJ, M44)
#define FE_45(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45) \
    FE_44(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44), OP(OBJ, M45)
#define FE_46(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46) \
    FE_45(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45), OP(OBJ, M46)
#define FE_47(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47) \
    FE_46(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46), OP(OBJ, M47)
#define FE_48(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48) \
    FE_47(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47), OP(OBJ, M48)
#define FE_49(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49) \
    FE_48(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48), OP(OBJ, M49)
#define FE_50(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50) \
    FE_49(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49), OP(OBJ, M50)
#define FE_51(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51) \
    FE_50(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50), OP(OBJ, M51)
#define FE_52(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52) \
    FE_51(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51), OP(OBJ, M52)
#define FE_53(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53) \
    FE_52(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52), OP(OBJ, M53)
#define FE_54(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54) \
    FE_53(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53), OP(OBJ, M54)
#define FE_55(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55) \
    FE_54(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54), OP(OBJ, M55)
#define FE_56(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56) \
    FE_55(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55), OP(OBJ, M56)
#define FE_57(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56, M57) \
    FE_56(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56), OP(OBJ, M57)
#define FE_58(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56, M57, M58) \
    FE_57(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56, M57), OP(OBJ, M58)
#define FE_59(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56, M57, M58, M59) \
    FE_58(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56, M57, M58), OP(OBJ, M59)
#define FE_60(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56, M57, M58, M59, M60) \
    FE_59(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56, M57, M58, M59), OP(OBJ, M60)
#define FE_61(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56, M57, M58, M59, M60, M61) \
    FE_60(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56, M57, M58, M59, M60), OP(OBJ, M61)
#define FE_62(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56, M57, M58, M59, M60, M61, M62) \
    FE_61(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56, M57, M58, M59, M60, M61), OP(OBJ, M62)
#define FE_63(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56, M57, M58, M59, M60, M61, M62, M63) \
    FE_62(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56, M57, M58, M59, M60, M61, M62), OP(OBJ, M63)
#define FE_64(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56, M57, M58, M59, M60, M61, M62, M63, M64) \
    FE_63(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, M22, M23, M24, M25, M26, M27, M28, M29, M30, M31, M32, M33, M34, M35, M36, M37, M38, M39, M40, M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54, M55, M56, M57, M58, M59, M60, M61, M62, M63), OP(OBJ, M64)


// Multi-level macro expansion for applying operation to each member
// LVL2: The final call to the FE_N macro
#define APPLY_OP_TO_EACH_MEMBER_LVL2(MACRO_NAME_TOKEN, OP_ARG, OBJ_ARG, ...) \
    EXPAND(MACRO_NAME_TOKEN(OP_ARG, OBJ_ARG, __VA_ARGS__))

// LVL1: Constructs the FE_N macro name (e.g., FE_2) using PASTE and the argument count
#define APPLY_OP_TO_EACH_MEMBER_LVL1(NUM_ARGS_VAL, OP_ARG, OBJ_ARG, ...) \
    APPLY_OP_TO_EACH_MEMBER_LVL2(PASTE(FE_, NUM_ARGS_VAL), OP_ARG, OBJ_ARG, __VA_ARGS__)

// Main entry point: Calculates arg count and starts the expansion chain
#define APPLY_OP_TO_EACH_MEMBER(OP_ARG, OBJ_ARG, ...) \
    EXPAND(APPLY_OP_TO_EACH_MEMBER_LVL1(PP_NARG(__VA_ARGS__), OP_ARG, OBJ_ARG, __VA_ARGS__))


// The operation to apply: obj.member
#define OBJ_DOT_MEMBER(obj_name, member_name) obj_name.member_name

// --- struct_access_traits base template (to be specialized) ---
template<typename T>
struct struct_access_traits;

// --- Macro to define the traits specialization for a given struct ---
#define DEFINE_STRUCT_OPERATORS_NON_INTRUSIVE(StructName, ...) \
template<> \
struct struct_access_traits<StructName> { \
    static constexpr auto to_tuple(const StructName& obj) { \
        return std::make_tuple(APPLY_OP_TO_EACH_MEMBER(OBJ_DOT_MEMBER, obj, __VA_ARGS__)); \
    } \
    \
    template<typename TupleType> \
    static constexpr StructName from_tuple(const TupleType& t) { \
        return std::apply([](auto... args) { return StructName{args...}; }, t); \
    } \
};

// --- SFINAE helper to check if struct_access_traits is specialized ---
template<typename T, typename = void>
struct has_struct_access_traits_defined : std::false_type {};

template<typename T>
struct has_struct_access_traits_defined<T, std::void_t<decltype(struct_access_traits<T>::to_tuple(std::declval<const T&>()))>> : std::true_type {};

// --- Generic free function operators ---
template<typename StructName,
    typename = std::enable_if_t<has_struct_access_traits_defined<StructName>::value>>
    StructName operator+(const StructName& lhs, const StructName& rhs) {
    auto t1 = struct_access_traits<StructName>::to_tuple(lhs);
    auto t2 = struct_access_traits<StructName>::to_tuple(rhs);
    auto result_tuple = dmopex_non_intrusive_detail::tuple_op(t1, t2, [](auto a, auto b) { return a + b; });
    return struct_access_traits<StructName>::from_tuple(result_tuple);
}

template<typename StructName,
    typename = std::enable_if_t<has_struct_access_traits_defined<StructName>::value>>
    StructName operator-(const StructName& lhs, const StructName& rhs) {
    auto t1 = struct_access_traits<StructName>::to_tuple(lhs);
    auto t2 = struct_access_traits<StructName>::to_tuple(rhs);
    auto result_tuple = dmopex_non_intrusive_detail::tuple_op(t1, t2, [](auto a, auto b) { return a - b; });
    return struct_access_traits<StructName>::from_tuple(result_tuple);
}

template<typename StructName,
    typename = std::enable_if_t<has_struct_access_traits_defined<StructName>::value>>
    StructName operator*(const StructName& lhs, const StructName& rhs) {
    auto t1 = struct_access_traits<StructName>::to_tuple(lhs);
    auto t2 = struct_access_traits<StructName>::to_tuple(rhs);
    auto result_tuple = dmopex_non_intrusive_detail::tuple_op(t1, t2, [](auto a, auto b) { return a * b; });
    return struct_access_traits<StructName>::from_tuple(result_tuple);
}

template<typename StructName,
    typename = std::enable_if_t<has_struct_access_traits_defined<StructName>::value>>
    StructName operator/(const StructName& lhs, const StructName& rhs) {
    auto t1 = struct_access_traits<StructName>::to_tuple(lhs);
    auto t2 = struct_access_traits<StructName>::to_tuple(rhs);
    auto result_tuple = dmopex_non_intrusive_detail::tuple_op(t1, t2, [](auto a, auto b) { return a / b; });
    return struct_access_traits<StructName>::from_tuple(result_tuple);
}

template<typename StructName,
    typename = std::enable_if_t<has_struct_access_traits_defined<StructName>::value>>
    StructName & operator+=(StructName& lhs, const StructName& rhs) {
    lhs = lhs + rhs;
    return lhs;
}

template<typename StructName,
    typename = std::enable_if_t<has_struct_access_traits_defined<StructName>::value>>
    StructName & operator-=(StructName& lhs, const StructName& rhs) {
    lhs = lhs - rhs;
    return lhs;
}

template<typename StructName,
    typename = std::enable_if_t<has_struct_access_traits_defined<StructName>::value>>
    StructName & operator*=(StructName& lhs, const StructName& rhs) {
    lhs = lhs * rhs;
    return lhs;
}

template<typename StructName,
    typename = std::enable_if_t<has_struct_access_traits_defined<StructName>::value>>
    StructName & operator/=(StructName& lhs, const StructName& rhs) {
    lhs = lhs / rhs;
    return lhs;
}

template<typename StructName,
    typename = std::enable_if_t<has_struct_access_traits_defined<StructName>::value>>
    bool operator==(const StructName& lhs, const StructName& rhs) {
    return struct_access_traits<StructName>::to_tuple(lhs) ==
        struct_access_traits<StructName>::to_tuple(rhs);
}

template<typename StructName,
    typename = std::enable_if_t<has_struct_access_traits_defined<StructName>::value>>
    bool operator!=(const StructName& lhs, const StructName& rhs) {
    return !(lhs == rhs);
}

template<typename StructName,
    typename = std::enable_if_t<has_struct_access_traits_defined<StructName>::value>>
    std::ostream& operator<<(std::ostream& os, const StructName& obj) {
    auto t = struct_access_traits<StructName>::to_tuple(obj);
    dmopex_non_intrusive_detail::print_tuple(os, t);
    return os;
}

#endif // __DMOPEX_NON_INTRUSIVE_H_INCLUDE__