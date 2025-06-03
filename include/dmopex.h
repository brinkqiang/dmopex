#ifndef __DMOPEX_H_INCLUDE__
#define __DMOPEX_H_INCLUDE__

#include <tuple>
#include <iostream>
#include <utility>
#include <type_traits>

namespace detail {
    template<typename Tuple1, typename Tuple2, typename Op, std::size_t... I>
    constexpr auto tuple_op_impl(const Tuple1& t1, const Tuple2& t2, Op op, std::index_sequence<I...>) {
        return std::make_tuple(op(std::get<I>(t1), std::get<I>(t2))...);
    }

    template<typename Tuple1, typename Tuple2, typename Op>
    constexpr auto tuple_op(const Tuple1& t1, const Tuple2& t2, Op op) {
        constexpr auto size = std::tuple_size_v<std::decay_t<Tuple1>>;
        static_assert(size == std::tuple_size_v<std::decay_t<Tuple2>>, "Tuple sizes must match");
        return detail::tuple_op_impl(t1, t2, op, std::make_index_sequence<size>{});
    }

    template<typename Tuple, std::size_t... I>
    void print_tuple(std::ostream& os, const Tuple& t, std::index_sequence<I...>) {
        os << "(";
        ((os << (I == 0 ? "" : ", ") << std::get<I>(t)), ...);
        os << ")";
    }
} // namespace detail

#define DEFINE_STRUCT_OPERATORS(StructName, ...) \
public: \
    constexpr auto to_tuple() const { \
        return std::make_tuple(__VA_ARGS__); \
    } \
    \
    template<typename TupleType> \
    static constexpr StructName from_tuple(const TupleType& t) { \
        return std::apply([](auto... args) { return StructName{args...}; }, t); \
    } \
    \
    StructName operator+(const StructName& other) const { \
        auto t1 = this->to_tuple(); \
        auto t2 = other.to_tuple(); \
        auto result_tuple = detail::tuple_op(t1, t2, [](auto a, auto b) { return a + b; }); \
        return StructName::from_tuple(result_tuple); \
    } \
    \
    StructName operator-(const StructName& other) const { \
        auto t1 = this->to_tuple(); \
        auto t2 = other.to_tuple(); \
        auto result_tuple = detail::tuple_op(t1, t2, [](auto a, auto b) { return a - b; }); \
        return StructName::from_tuple(result_tuple); \
    } \
    \
    StructName operator*(const StructName& other) const { \
        auto t1 = this->to_tuple(); \
        auto t2 = other.to_tuple(); \
        auto result_tuple = detail::tuple_op(t1, t2, [](auto a, auto b) { return a * b; }); \
        return StructName::from_tuple(result_tuple); \
    } \
    \
    StructName operator/(const StructName& other) const { \
        auto t1 = this->to_tuple(); \
        auto t2 = other.to_tuple(); \
        auto result_tuple = detail::tuple_op(t1, t2, [](auto a, auto b) { return a / b; }); \
        return StructName::from_tuple(result_tuple); \
    } \
    \
    StructName& operator+=(const StructName& other) { \
        *this = *this + other; \
        return *this; \
    } \
    \
    StructName& operator-=(const StructName& other) { \
        *this = *this - other; \
        return *this; \
    } \
    \
    StructName& operator*=(const StructName& other) { \
        *this = *this * other; \
        return *this; \
    } \
    \
    StructName& operator/=(const StructName& other) { \
        *this = *this / other; \
        return *this; \
    } \
    \
    bool operator==(const StructName& other) const { \
        return this->to_tuple() == other.to_tuple(); \
    } \
    \
    bool operator!=(const StructName& other) const { \
        return !(*this == other); \
    } \
    \
    friend std::ostream& operator<<(std::ostream& os, const StructName& obj) { \
        auto t = obj.to_tuple(); \
        constexpr auto size = std::tuple_size_v<std::decay_t<decltype(t)>>; \
        detail::print_tuple(os, t, std::make_index_sequence<size>{}); \
        return os; \
    }

#endif // __DMOPEX_H_INCLUDE__