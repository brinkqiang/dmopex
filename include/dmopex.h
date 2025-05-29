#ifndef __DMOPEX_H_INCLUDE__
#define __DMOPEX_H_INCLUDE__

#include <tuple>
#include <iostream> // For std::ostream
#include <utility>  // For std::index_sequence and std::make_index_sequence
#include <type_traits> // For std::tuple_size_v

// Forward declaration for the stream operator
template<typename Derived>
class IDMOperators;

template<typename Derived>
std::ostream& operator<<(std::ostream& os, const IDMOperators<Derived>& obj);

namespace detail {

    template<typename Tuple1, typename Tuple2, typename Op, std::size_t... I>
    constexpr auto tuple_op_impl(const Tuple1& t1, const Tuple2& t2, Op op, std::index_sequence<I...>) {
        return std::make_tuple(op(std::get<I>(t1), std::get<I>(t2))...);
    }

    template<typename Tuple1, typename Tuple2, typename Op>
    constexpr auto tuple_op(const Tuple1& t1, const Tuple2& t2, Op op) {
        constexpr auto size = std::tuple_size_v<std::decay_t<Tuple1>>; // Use std::decay_t for robustness
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

template<typename Derived>
class IDMOperators {
public:
    Derived operator+(const Derived& other) const {
        auto t1 = static_cast<const Derived&>(*this).to_tuple();
        auto t2 = other.to_tuple();
        auto result_tuple = detail::tuple_op(t1, t2, [](auto a, auto b) { return a + b; });
        return Derived::from_tuple(result_tuple);
    }

    Derived operator-(const Derived& other) const {
        auto t1 = static_cast<const Derived&>(*this).to_tuple();
        auto t2 = other.to_tuple();
        auto result_tuple = detail::tuple_op(t1, t2, [](auto a, auto b) { return a - b; });
        return Derived::from_tuple(result_tuple);
    }

    Derived& operator+=(const Derived& other) {
        *static_cast<Derived*>(this) = static_cast<const Derived&>(*this) + other;
        return static_cast<Derived&>(*this);
    }

    Derived& operator-=(const Derived& other) {
        *static_cast<Derived*>(this) = static_cast<const Derived&>(*this) - other;
        return static_cast<Derived&>(*this);
    }

    bool operator==(const Derived& other) const {
        return static_cast<const Derived&>(*this).to_tuple() == other.to_tuple();
    }

    bool operator!=(const Derived& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<< <Derived>(std::ostream& os, const IDMOperators<Derived>& obj);
};

template<typename Derived>
std::ostream& operator<<(std::ostream& os, const IDMOperators<Derived>& obj) {
    const auto& derived_obj = static_cast<const Derived&>(obj);
    auto t = derived_obj.to_tuple();
    constexpr auto size = std::tuple_size_v<std::decay_t<decltype(t)>>;
    detail::print_tuple(os, t, std::make_index_sequence<size>{});
    return os;
}

#define DEFINE_STRUCT_OPERATORS(StructName, ...) \
public:  \
    constexpr auto to_tuple() const { \
        return std::make_tuple(__VA_ARGS__); \
    } \
    template<typename TupleType> \
    static constexpr StructName from_tuple(const TupleType& t) { \
        return std::apply([](auto... args) { return StructName{args...}; }, t); \
    }

#endif // __DMOPEX_H_INCLUDE__