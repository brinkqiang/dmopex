#ifndef __DMOPEX_H_INCLUDE__
#define __DMOPEX_H_INCLUDE__

#include <tuple>
#include <iostream> // For std::ostream
#include <utility>  // For std::index_sequence and std::make_index_sequence
#include <type_traits> // For std::tuple_size_v

// Forward declaration for the stream operator
template<typename Derived>
class ITupleOperators;

template<typename Derived>
std::ostream& operator<<(std::ostream& os, const ITupleOperators<Derived>& obj);

namespace detail { // Encapsulate helper functions

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

    // Helper for printing tuple elements
    template<typename Tuple, std::size_t... I>
    void print_tuple(std::ostream& os, const Tuple& t, std::index_sequence<I...>) {
        os << "(";
        // Use a fold expression (C++17) for cleaner printing
        ((os << (I == 0 ? "" : ", ") << std::get<I>(t)), ...);
        os << ")";
    }

} // namespace detail

template<typename Derived>
class ITupleOperators {
public:
    // Addition
    Derived operator+(const Derived& other) const {
        auto t1 = static_cast<const Derived&>(*this).to_tuple();
        auto t2 = other.to_tuple();
        auto result_tuple = detail::tuple_op(t1, t2, [](auto a, auto b) { return a + b; });
        return Derived::from_tuple(result_tuple);
    }

    // Subtraction
    Derived operator-(const Derived& other) const {
        auto t1 = static_cast<const Derived&>(*this).to_tuple();
        auto t2 = other.to_tuple();
        auto result_tuple = detail::tuple_op(t1, t2, [](auto a, auto b) { return a - b; });
        return Derived::from_tuple(result_tuple);
    }

    // Compound addition
    Derived& operator+=(const Derived& other) {
        // Re-use operator+ for consistency and to avoid code duplication
        *static_cast<Derived*>(this) = static_cast<const Derived&>(*this) + other;
        return static_cast<Derived&>(*this);
    }

    // Compound subtraction
    Derived& operator-=(const Derived& other) {
        // Re-use operator- for consistency
        *static_cast<Derived*>(this) = static_cast<const Derived&>(*this) - other;
        return static_cast<Derived&>(*this);
    }

    // Equality comparison
    bool operator==(const Derived& other) const {
        // std::tuple already has a well-defined operator==
        return static_cast<const Derived&>(*this).to_tuple() == other.to_tuple();
    }

    // Inequality comparison (optional, but good practice)
    bool operator!=(const Derived& other) const {
        return !(*this == other);
    }

    // Friend declaration for the stream insertion operator
    friend std::ostream& operator<< <Derived>(std::ostream& os, const ITupleOperators<Derived>& obj);
};

// Definition of the stream insertion operator
template<typename Derived>
std::ostream& operator<<(std::ostream& os, const ITupleOperators<Derived>& obj) {
    const auto& derived_obj = static_cast<const Derived&>(obj);
    auto t = derived_obj.to_tuple();
    constexpr auto size = std::tuple_size_v<std::decay_t<decltype(t)>>;
    detail::print_tuple(os, t, std::make_index_sequence<size>{});
    return os;
}


#define DEFINE_TUPLE_STRUCT_BODY(...) \
    constexpr auto to_tuple() const { \
        return std::make_tuple(__VA_ARGS__); \
    } \
    template<typename T> \
    static constexpr auto from_tuple(const T& t) { \
        return std::apply([](auto... args) { return std::decay_t<decltype(*this)>{args...}; }, t); \
    }
// Note: Changed StructName to std::decay_t<decltype(*this)> in from_tuple
// to make it more generic within the macro if the struct name is not passed.
// However, since the original macro took StructName, let's revert to that for consistency
// if DEFINE_TUPLE_STRUCT is preferred.

// Keeping the original macro name for compatibility, but it now just calls the body macro.
// The StructName parameter in the original macro is used by from_tuple.
#define DEFINE_TUPLE_STRUCT(StructName, ...) \
public: /* Ensure members are public if used directly in a struct */ \
    constexpr auto to_tuple() const { \
        return std::make_tuple(__VA_ARGS__); \
    } \
    template<typename TupleType> \
    static constexpr StructName from_tuple(const TupleType& t) { \
        return std::apply([](auto... args) { return StructName{args...}; }, t); \
    }

#endif // __DMOPEX_H_INCLUDE__