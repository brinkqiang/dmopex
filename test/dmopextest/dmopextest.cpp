
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
