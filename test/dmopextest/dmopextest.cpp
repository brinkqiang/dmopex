
#include "dmopex.h"
#include "gtest.h"

class env_dmopex
{
public:
    void init(){}
    void uninit(){}
};

struct Point2D : public ITupleOperators<Point2D> {
    double x, y;

    Point2D() = default;
    Point2D(double x_, double y_) : x(x_), y(y_) {}

    DEFINE_TUPLE_STRUCT(Point2D, x, y)
};

struct Vector3D : public ITupleOperators<Vector3D> {
    double x, y, z;

    Vector3D() = default;
    Vector3D(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

    DEFINE_TUPLE_STRUCT(Vector3D, x, y, z)
};

struct Color : public ITupleOperators<Color> {
    int r, g, b, a;

    Color() = default;
    Color(int r_, int g_, int b_, int a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}

    DEFINE_TUPLE_STRUCT(Color, r, g, b, a)
};

class DmOpExTest : public testing::Test
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

    Point2D p_default;
    Vector3D v_default;
    Color c_default;

    Point2D p1{ 1.5, 2.5 };
    Point2D p2{ 3.0, 4.0 };
    Point2D p1_copy{ 1.5, 2.5 };

    Vector3D v1{ 1.0, 2.0, 3.0 };
    Vector3D v2{ 4.0, 5.0, 6.0 };
    Vector3D v1_copy{ 1.0, 2.0, 3.0 };

    Color c1{ 100, 150, 200 }; // a = 255
    Color c2{ 50, 75, 100, 128 };
    Color c1_copy{ 100, 150, 200, 255 };
    Color c1_different_alpha{ 100, 150, 200, 100 };
};


// 测试 Point2D 的操作
TEST_F(DmOpExTest, Point2DOperations) {
    Point2D p_sum = p1 + p2;
    EXPECT_EQ(p_sum.x, 4.5);
    EXPECT_EQ(p_sum.y, 6.5);
    // 或者直接比较对象，因为我们重载了 operator==
    EXPECT_EQ(p_sum, Point2D(4.5, 6.5));

    Point2D p_diff = p2 - p1;
    EXPECT_EQ(p_diff, Point2D(1.5, 1.5));

    EXPECT_EQ(p1, p1_copy);
    EXPECT_NE(p1, p2);

    Point2D p_temp = p1;
    p_temp += p2;
    EXPECT_EQ(p_temp, p_sum);

    p_temp = p2;
    p_temp -= p1;
    EXPECT_EQ(p_temp, p_diff);

    // 测试流输出 (gtest 不直接捕获 cout, 但我们可以检查其编译和基本行为)
    // std::cout << "Test Point2D output: " << p1 << std::endl; // 手动验证或用于调试
}

// 测试 Vector3D 的操作
TEST_F(DmOpExTest, Vector3DOperations) {
    Vector3D v_sum = v1 + v2;
    EXPECT_EQ(v_sum, Vector3D(5.0, 7.0, 9.0));

    Vector3D v_diff = v1 - v2;
    EXPECT_EQ(v_diff, Vector3D(-3.0, -3.0, -3.0));

    EXPECT_EQ(v1, v1_copy);
    EXPECT_NE(v1, v2);

    Vector3D v_temp = v1;
    v_temp += v2;
    EXPECT_EQ(v_temp, v_sum);

    v_temp = v1; // Reset
    v_temp -= v2;
    EXPECT_EQ(v_temp, v_diff);
}

// 测试 Color 的操作
TEST_F(DmOpExTest, ColorOperations) {
    Color c_sum = c1 + c2;
    // 注意：这里的加法是逐元素相加，alpha 也会相加
    // 255 + 128 = 383. 如果颜色分量有上限 (如255), 则需要额外逻辑处理。
    // ITupleOperators 不处理这种领域特定的约束。
    EXPECT_EQ(c_sum, Color(150, 225, 300, 383));

    Color c_diff = c1 - c2;
    EXPECT_EQ(c_diff, Color(50, 75, 100, 255 - 128));

    EXPECT_EQ(c1, c1_copy);
    EXPECT_NE(c1, c2);
    EXPECT_NE(c1, c1_different_alpha);

    Color c_temp = c1;
    c_temp += c2;
    EXPECT_EQ(c_temp, c_sum);

    c_temp = c1; // Reset
    c_temp -= c2;
    EXPECT_EQ(c_temp, c_diff);
}