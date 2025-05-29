# dmopex: C++元组操作符泛型实现

Copyright (c) 2013-2018 brinkqiang (brink.qiang@gmail.com)

[![dmopex](https://img.shields.io/badge/brinkqiang-dmopex-blue.svg?style=flat-square)](https://github.com/brinkqiang/dmopex)
[![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](https://github.com/brinkqiang/dmopex/blob/master/LICENSE)
[![blog](https://img.shields.io/badge/Author-Blog-7AD6FD.svg)](https://brinkqiang.github.io/)
[![Open Source Love](https://badges.frapsoft.com/os/v3/open-source.png)](https://github.com/brinkqiang)
[![GitHub stars](https://img.shields.io/github/stars/brinkqiang/dmopex.svg?label=Stars)](https://github.com/brinkqiang/dmopex) 
[![GitHub forks](https://img.shields.io/github/forks/brinkqiang/dmopex.svg?label=Fork)](https://github.com/brinkqiang/dmopex)

## Build status
| [Linux][lin-link] | [Mac][mac-link] | [Windows][win-link] |
| :---------------: | :----------------: | :-----------------: |
| ![lin-badge]      | ![mac-badge]       | ![win-badge]        |

[lin-badge]: https://github.com/brinkqiang/dmopex/workflows/linux/badge.svg "linux build status"
[lin-link]:  https://github.com/brinkqiang/dmopex/actions/workflows/linux.yml "linux build status"
[mac-badge]: https://github.com/brinkqiang/dmopex/workflows/mac/badge.svg "mac build status"
[mac-link]:  https://github.com/brinkqiang/dmopex/actions/workflows/mac.yml "mac build status"
[win-badge]: https://github.com/brinkqiang/dmopex/workflows/win/badge.svg "win build status"
[win-link]:  https://github.com/brinkqiang/dmopex/actions/workflows/win.yml "win build status"


[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) `dmopex` 是一个轻量级的 C++ 头文件库，旨在通过 Curiously Recurring Template Pattern (CRTP) 为任何可以转换为 `std::tuple` 并可以从 `std::tuple`构造的自定义结构体，自动提供常用的算术和比较操作符。

## 解决的问题

在 C++ 中，为自定义的结构体（尤其是那些行为类似 POD 或元组的结构体）重载大量的算术操作符（如 `+`, `-`, `+=`, `-=`) 和比较操作符（`==`, `!=`）以及流输出操作符（`<<`）是一项重复且繁琐的任务。`ITupleOperators` 通过泛型编程简化了这一过程，只需用户提供其结构体与 `std::tuple` 之间的转换方法即可。

## 特性

* **代码简洁**：通过继承 `IDMOperators<YourStruct>` 并实现两个转换函数（或使用宏），即可为您的结构体添加操作符。
* **类型安全**：操作在元组的对应元素之间进行，依赖于元素类型的操作符。
* **易于集成**：仅需包含一个头文件 (`dmopex.h`)。
* **支持的操作符**：
    * 加法: `operator+`
    * 减法: `operator-`
    * 复合加法: `operator+=`
    * 复合减法: `operator-=`
    * 相等比较: `operator==`
    * 不等比较: `operator!=`
    * 流输出: `operator<<` (用于 `std::ostream`)
* **辅助宏**：提供 `DEFINE_TUPLE_STRUCT` 宏以快速定义所需的转换函数。

## 要求

* C++17 或更高版本 (主要因为 `detail::print_tuple` 中使用了 C++17 的折叠表达式，以及 `std::tuple_size_v` 等类型特性)。
    * 如果需要兼容旧版 C++ (如 C++11/14)，`detail::print_tuple` 函数需要修改为递归模板实现。

## 如何使用

1.  **包含头文件**:
    ```cpp
    #include "dmopex.h"
    ```

2.  **定义你的结构体**:
    * 让你的结构体 `MyStruct` 公开继承自 `IDMOperators<MyStruct>`。
    * 在你的结构体内部，你需要提供两个核心部分：
        1.  一个 `to_tuple()` const 成员函数，返回一个包含结构体成员的 `std::tuple`。
        2.  一个静态成员函数 `from_tuple(const SomeTupleType& t)`，它接受一个元组并返回 `MyStruct` 的一个实例。
    * 或者，你可以使用 `DEFINE_TUPLE_STRUCT(StructName, ...members)` 宏来自动生成这两个部分。

## 示例

```cpp
#include "dmopex.h"
#include <iostream>
#include <string> // 如果你的结构体成员包含 std::string

// 定义一个二维点结构体
struct Point2D : public IDMOperators<Point2D> {
    double x, y;

    // 默认构造函数和带参构造函数
    Point2D(double x_ = 0.0, double y_ = 0.0) : x(x_), y(y_) {}

    // 使用宏来定义 to_tuple 和 from_tuple
    DEFINE_TUPLE_STRUCT(Point2D, x, y)

    // (可选) 为 Google Test 提供友好的打印函数
    // friend void PrintTo(const Point2D& point, ::std::ostream* os) {
    //     *os << "Point2D(" << point.x << ", " << point.y << ")";
    // }
};

// 定义一个颜色结构体
struct Color : public IDMOperators<Color> {
    int r, g, b, a;

    Color(int r_ = 0, int g_ = 0, int b_ = 0, int a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}

    DEFINE_TUPLE_STRUCT(Color, r, g, b, a)
};


int main() {
    Point2D p1{1.5, 2.5};
    Point2D p2{3.0, 4.0};
    Point2D p1_copy{1.5, 2.5};

    // 测试算术运算
    Point2D p_sum = p1 + p2;
    std::cout << "p1 + p2 = " << p_sum << std::endl; // 预期: (4.5, 6.5)

    Point2D p_diff = p2 - p1;
    std::cout << "p2 - p1 = " << p_diff << std::endl; // 预期: (1.5, 1.5)

    p1 += p2;
    std::cout << "p1 after += p2: " << p1 << std::endl; // 预期: (4.5, 6.5) (p1 自身被修改)

    // 测试比较运算
    std::cout << "p1 == p1_copy: " << (p1_copy == Point2D{1.5, 2.5} ? "true" : "false") << std::endl; // 预期: true (注意 p1 已被修改)
                                                                                               // 应该用未修改的 p1_copy 来比较
    std::cout << "Point2D{1.5, 2.5} == p1_copy: " << (Point2D{1.5, 2.5} == p1_copy ? "true" : "false") << std::endl; // 预期: true
    std::cout << "p1 == p2: " << (p1 == p2 ? "true" : "false") << std::endl; // 预期: false (p1 是 (4.5, 6.5), p2 是 (3.0, 4.0))
    std::cout << "p1_copy != p2: " << (p1_copy != p2 ? "true" : "false") << std::endl; // 预期: true

    Color red{255, 0, 0};
    Color green{0, 255, 0};
    Color yellow = red + green; // 注意：元素级相加，alpha 也会相加
    std::cout << "Yellow (red + green): " << yellow << std::endl; // 预期: (255, 255, 0, 510) (alpha=255+255)

    Color c_test1{10,20,30,40};
    Color c_test2{10,20,30,40};
    Color c_test3{1,2,3,4};
    std::cout << "c_test1 == c_test2: " << (c_test1 == c_test2 ? "true" : "false") << std::endl; // 预期: true
    std::cout << "c_test1 != c_test3: " << (c_test1 != c_test3 ? "true" : "false") << std::endl; // 预期: true

    return 0;
}

```