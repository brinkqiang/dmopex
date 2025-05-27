
// Copyright (c) 2018 brinkqiang (brink.qiang@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __DMOPEX_H_INCLUDE__
#define __DMOPEX_H_INCLUDE__

#include "dmos.h" // dmos.h已经处理平台头文件, 以及相关宏定义
#include "dmfix_win_utf8.h" // 处理 win平台utf8问题
#include <utility>
#include <type_traits>

namespace Opex {

// 帮助类型：检测成员访问器类型
template<typename T, typename Accessor>
struct accessor_result {
    using type = std::invoke_result_t<Accessor, T&>;
};

template<typename T, typename Accessor>
using accessor_result_t = typename accessor_result<T, Accessor>::type;

// 主要的Operable类模板
template <typename Derived, auto... Accessors>
class Operable {
private:
    static_assert(sizeof...(Accessors) > 0, "至少需要提供一个成员访问器");
    
    // 辅助函数：应用二元运算
    template<typename Op>
    static void apply_operation(Derived& lhs, const Derived& rhs, Op op) {
        ((op(std::invoke(Accessors, lhs), std::invoke(Accessors, rhs))), ...);
    }
    
    // 辅助函数：应用比较运算
    template<typename Op>
    static bool apply_comparison(const Derived& lhs, const Derived& rhs, Op op) {
        return (op(std::invoke(Accessors, lhs), std::invoke(Accessors, rhs)) && ...);
    }

public:
    // 复合赋值运算符
    friend Derived& operator+=(Derived& lhs, const Derived& rhs) {
        apply_operation(lhs, rhs, [](auto& l, const auto& r) { l += r; });
        return lhs;
    }
    
    friend Derived& operator-=(Derived& lhs, const Derived& rhs) {
        apply_operation(lhs, rhs, [](auto& l, const auto& r) { l -= r; });
        return lhs;
    }
    
    friend Derived& operator*=(Derived& lhs, const Derived& rhs) {
        apply_operation(lhs, rhs, [](auto& l, const auto& r) { l *= r; });
        return lhs;
    }
    
    friend Derived& operator/=(Derived& lhs, const Derived& rhs) {
        apply_operation(lhs, rhs, [](auto& l, const auto& r) { l /= r; });
        return lhs;
    }
    
    // 二元算术运算符
    friend Derived operator+(Derived lhs, const Derived& rhs) {
        lhs += rhs;
        return lhs;
    }
    
    friend Derived operator-(Derived lhs, const Derived& rhs) {
        lhs -= rhs;
        return lhs;
    }
    
    friend Derived operator*(Derived lhs, const Derived& rhs) {
        lhs *= rhs;
        return lhs;
    }
    
    friend Derived operator/(Derived lhs, const Derived& rhs) {
        lhs /= rhs;
        return lhs;
    }
    
    // 比较运算符
    friend bool operator==(const Derived& lhs, const Derived& rhs) {
        return apply_comparison(lhs, rhs, [](const auto& l, const auto& r) { return l == r; });
    }
    
    friend bool operator!=(const Derived& lhs, const Derived& rhs) {
        return !(lhs == rhs);
    }
    
    friend bool operator<(const Derived& lhs, const Derived& rhs) {
        return apply_comparison(lhs, rhs, [](const auto& l, const auto& r) { return l < r; });
    }
    
    friend bool operator<=(const Derived& lhs, const Derived& rhs) {
        return !(rhs < lhs);
    }
    
    friend bool operator>(const Derived& lhs, const Derived& rhs) {
        return rhs < lhs;
    }
    
    friend bool operator>=(const Derived& lhs, const Derived& rhs) {
        return !(lhs < rhs);
    }
};


} // namespace Opex

// 便利宏：简化成员变量访问器的定义
#define OPEX_MEMBER(member) &std::remove_reference_t<decltype(*this)>::member

// 便利宏：简化getter函数访问器的定义
#define OPEX_GETTER(getter) &std::remove_reference_t<decltype(*this)>::getter

#endif // __DMOPEX_H_INCLUDE__