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

// Corrected PP_NARG to count arguments in __VA_ARGS__ (supports 0 to 16 arguments)
// _0, _1, ... _16 are placeholders for arguments, N is the count.
#define PP_NARG_IMPL( \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
    _11, _12, _13, _14, _15, _16, N, ...) N
// Sequence of numbers from 16 down to 0, used to "select" N based on how many args consume the sequence.
#define PP_NARG(...) \
    EXPAND(PP_NARG_IMPL(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

// Concatenation helpers: PASTE(a,b) expands to a ## b after 'a' and 'b' are expanded
#define PASTE_IMPL(a, b) a##b
#define PASTE(a, b) PASTE_IMPL(a, b)


// FOR_EACH style macros to apply an operation to each argument
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
// Add more FE_N up to PP_NARG's limit (16) if needed. For example:
#define FE_11(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6), OP(OBJ, M7), OP(OBJ, M8), OP(OBJ, M9), OP(OBJ, M10), OP(OBJ, M11)
#define FE_12(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6), OP(OBJ, M7), OP(OBJ, M8), OP(OBJ, M9), OP(OBJ, M10), OP(OBJ, M11), OP(OBJ, M12)
#define FE_13(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6), OP(OBJ, M7), OP(OBJ, M8), OP(OBJ, M9), OP(OBJ, M10), OP(OBJ, M11), OP(OBJ, M12), OP(OBJ, M13)
#define FE_14(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6), OP(OBJ, M7), OP(OBJ, M8), OP(OBJ, M9), OP(OBJ, M10), OP(OBJ, M11), OP(OBJ, M12), OP(OBJ, M13), OP(OBJ, M14)
#define FE_15(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6), OP(OBJ, M7), OP(OBJ, M8), OP(OBJ, M9), OP(OBJ, M10), OP(OBJ, M11), OP(OBJ, M12), OP(OBJ, M13), OP(OBJ, M14), OP(OBJ, M15)
#define FE_16(OP, OBJ, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15, M16) OP(OBJ, M1), OP(OBJ, M2), OP(OBJ, M3), OP(OBJ, M4), OP(OBJ, M5), OP(OBJ, M6), OP(OBJ, M7), OP(OBJ, M8), OP(OBJ, M9), OP(OBJ, M10), OP(OBJ, M11), OP(OBJ, M12), OP(OBJ, M13), OP(OBJ, M14), OP(OBJ, M15), OP(OBJ, M16)


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
#define DEFINE_NON_INTRUSIVE_OPERATORS_FOR(StructName, ...) \
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
