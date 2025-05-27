
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

#include "dmos.h" // dmos.h�Ѿ�����ƽ̨ͷ�ļ�, �Լ���غ궨��
#include "dmfix_win_utf8.h" // ���� winƽ̨utf8����
#include <utility>
#include <type_traits>

namespace Opex {

// �������ͣ�����Ա����������
template<typename T, typename Accessor>
struct accessor_result {
    using type = std::invoke_result_t<Accessor, T&>;
};

template<typename T, typename Accessor>
using accessor_result_t = typename accessor_result<T, Accessor>::type;

// ��Ҫ��Operable��ģ��
template <typename Derived, auto... Accessors>
class Operable {
private:
    static_assert(sizeof...(Accessors) > 0, "������Ҫ�ṩһ����Ա������");
    
    // ����������Ӧ�ö�Ԫ����
    template<typename Op>
    static void apply_operation(Derived& lhs, const Derived& rhs, Op op) {
        ((op(std::invoke(Accessors, lhs), std::invoke(Accessors, rhs))), ...);
    }
    
    // ����������Ӧ�ñȽ�����
    template<typename Op>
    static bool apply_comparison(const Derived& lhs, const Derived& rhs, Op op) {
        return (op(std::invoke(Accessors, lhs), std::invoke(Accessors, rhs)) && ...);
    }

public:
    // ���ϸ�ֵ�����
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
    
    // ��Ԫ���������
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
    
    // �Ƚ������
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

// �����꣺�򻯳�Ա�����������Ķ���
#define OPEX_MEMBER(member) &std::remove_reference_t<decltype(*this)>::member

// �����꣺��getter�����������Ķ���
#define OPEX_GETTER(getter) &std::remove_reference_t<decltype(*this)>::getter

#endif // __DMOPEX_H_INCLUDE__