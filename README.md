# dmopex

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

## Intro
dmopex
```cpp
#include "dmopex.h"
#include "gtest.h"

class env_dmopex
{
public:
    void init(){}
    void uninit(){}
};

class frame_dmopex : public testing::Test
{
public:
    virtual void SetUp()
    {
        env.init();
    }
    virtual void TearDown()
    {
        env.uninit();
    }
protected:
    env_dmopex env;
};

// 示例1：使用成员变量

class Point2D {
public:
    double x, y;

    Point2D(double x = 0, double y = 0) : x(x), y(y) {}
};

PP_OPERATOR_DEF_V2(Point2D, x, y);

TEST_F(frame_dmopex, init)
{
    Point2D p1(1.0, 2.0);
    Point2D p2(3.0, 4.0);
    
    Point2D p3 = p1 + p2;  // p3.x = 4.0, p3.y = 6.0
    p1 += p2;              // p1.x = 4.0, p1.y = 6.0
}
```
## Contacts

## Thanks
