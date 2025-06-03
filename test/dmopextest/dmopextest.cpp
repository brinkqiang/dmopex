#include "dmopex.h"
#include "gtest.h" 

class env_dmopex
{
public:
    void init() {}
    void uninit() {}
};

struct Point2D {
    double x, y;

    Point2D() = default;
    Point2D(double x_, double y_) : x(x_), y(y_) {}

    DEFINE_STRUCT_OPERATORS(Point2D, x, y)
};

struct Vector3D {
    double x, y, z;

    Vector3D() = default;
    Vector3D(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

    DEFINE_STRUCT_OPERATORS(Vector3D, x, y, z)
};

struct Color {
    int r, g, b, a;

    Color() = default;
    Color(int r_, int g_, int b_, int a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}

    DEFINE_STRUCT_OPERATORS(Color, r, g, b, a)
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

// Point2D测试
TEST_F(DmOpExTest, Point2DOperations) {
    // 加法测试
    Point2D p_sum = p1 + p2;
    EXPECT_EQ(p_sum.x, 4.5);
    EXPECT_EQ(p_sum.y, 6.5);
    EXPECT_EQ(p_sum, Point2D(4.5, 6.5));

    // 减法测试
    Point2D p_diff = p2 - p1;
    EXPECT_EQ(p_diff, Point2D(1.5, 1.5));

    // 乘法测试
    Point2D p_mul = p1 * p2;
    EXPECT_EQ(p_mul, Point2D(4.5, 10.0));

    // 除法测试
    Point2D p_div = p2 / p1;
    EXPECT_EQ(p_div, Point2D(2.0, 1.6));

    // 比较测试
    EXPECT_EQ(p1, p1_copy);
    EXPECT_NE(p1, p2);

    // 复合赋值测试
    Point2D p_temp = p1;
    p_temp += p2;
    EXPECT_EQ(p_temp, p_sum);

    p_temp = p2;
    p_temp -= p1;
    EXPECT_EQ(p_temp, p_diff);

    p_temp = p1;
    p_temp *= p2;
    EXPECT_EQ(p_temp, p_mul);

    p_temp = p2;
    p_temp /= p1;
    EXPECT_EQ(p_temp, p_div);
}

// Vector3D测试
TEST_F(DmOpExTest, Vector3DOperations) {
    // 加法测试
    Vector3D v_sum = v1 + v2;
    EXPECT_EQ(v_sum, Vector3D(5.0, 7.0, 9.0));

    // 减法测试
    Vector3D v_diff = v1 - v2;
    EXPECT_EQ(v_diff, Vector3D(-3.0, -3.0, -3.0));

    // 乘法测试 (逐元素乘法)
    Vector3D v_mul = v1 * v2;
    EXPECT_EQ(v_mul, Vector3D(4.0, 10.0, 18.0));

    // 除法测试
    Vector3D v_div = v2 / v1;
    EXPECT_EQ(v_div, Vector3D(4.0, 2.5, 2.0));

    // 比较测试
    EXPECT_EQ(v1, v1_copy);
    EXPECT_NE(v1, v2);

    // 复合赋值测试
    Vector3D v_temp = v1;
    v_temp += v2;
    EXPECT_EQ(v_temp, v_sum);

    v_temp = v1;
    v_temp -= v2;
    EXPECT_EQ(v_temp, v_diff);

    v_temp = v1;
    v_temp *= v2;
    EXPECT_EQ(v_temp, v_mul);

    v_temp = v2;
    v_temp /= v1;
    EXPECT_EQ(v_temp, v_div);
}

// Color测试
TEST_F(DmOpExTest, ColorOperations) {
    // 加法测试
    Color c_sum = c1 + c2;
    EXPECT_EQ(c_sum, Color(150, 225, 300, 383));

    // 减法测试
    Color c_diff = c1 - c2;
    EXPECT_EQ(c_diff, Color(50, 75, 100, 127)); // 255 - 128 = 127

    // 乘法测试 (逐元素乘法，可用于颜色混合效果)
    Color c_mul = c1 * Color(2, 1, 1, 1);
    EXPECT_EQ(c_mul, Color(200, 150, 200, 255));

    // 除法测试 (可用于颜色亮度调节)
    Color c_div = Color(200, 100, 50, 200) / Color(2, 2, 2, 2);
    EXPECT_EQ(c_div, Color(100, 50, 25, 100));

    // 比较测试
    EXPECT_EQ(c1, c1_copy);
    EXPECT_NE(c1, c2);
    EXPECT_NE(c1, c1_different_alpha);

    // 复合赋值测试
    Color c_temp = c1;
    c_temp += c2;
    EXPECT_EQ(c_temp, c_sum);

    c_temp = c1;
    c_temp -= c2;
    EXPECT_EQ(c_temp, c_diff);

    c_temp = c1;
    c_temp *= Color(2, 1, 1, 1);
    EXPECT_EQ(c_temp, Color(200, 150, 200, 255));

    c_temp = Color(200, 100, 50, 200);
    c_temp /= Color(2, 2, 2, 2);
    EXPECT_EQ(c_temp, Color(100, 50, 25, 100));
}

TEST_F(DmOpExTest, readme)
{
    Point2D p1_local{ 1.5, 2.5 }; // Renamed to avoid conflict with member p1 if used directly
    Point2D p2_local{ 3.0, 4.0 }; // Renamed to avoid conflict with member p2 if used directly
    Point2D p1_copy_local{ 1.5, 2.5 }; // Renamed

    // 基本算术运算
    Point2D p_sum = p1_local + p2_local;
    EXPECT_EQ(p_sum, Point2D(4.5, 6.5));

    Point2D p_diff = p2_local - p1_local;
    EXPECT_EQ(p_diff, Point2D(1.5, 1.5));

    Point2D p_mul = p1_local * p2_local;
    EXPECT_EQ(p_mul, Point2D(4.5, 10.0));

    Point2D p_div = p2_local / p1_local;
    EXPECT_EQ(p_div, Point2D(2.0, 1.6));

    // 复合赋值运算
    Point2D p_temp = p1_local;
    p_temp += p2_local;
    EXPECT_EQ(p_temp, Point2D(4.5, 6.5));

    p_temp = p1_local; // Reset p_temp
    p_temp *= p2_local;
    EXPECT_EQ(p_temp, Point2D(4.5, 10.0));

    p_temp = p2_local; // Reset p_temp
    p_temp /= p1_local;
    EXPECT_EQ(p_temp, Point2D(2.0, 1.6));

    // 测试比较运算
    EXPECT_EQ(p1_copy_local, Point2D(1.5, 2.5));
    EXPECT_EQ(Point2D(1.5, 2.5), p1_copy_local);
    EXPECT_NE(p1_copy_local, p2_local);

    // 颜色运算示例
    Color red{ 255, 0, 0 };
    Color green{ 0, 255, 0 };
    Color yellow = red + green;
    EXPECT_EQ(yellow, Color(255, 255, 0, 510)); // red.a (255) + green.a (255) = 510

    // 颜色乘法 - 可用于调整亮度
    Color bright_red = red * Color(2, 2, 2, 1);
    EXPECT_EQ(bright_red, Color(510, 0, 0, 255)); // red.a (255) * 1 = 255

    // 颜色除法 - 可用于降低亮度
    Color dim_red = Color(200, 100, 50, 255) / Color(2, 2, 2, 1);
    EXPECT_EQ(dim_red, Color(100, 50, 25, 255)); // 255 / 1 = 255

    // Vector3D运算示例
    Vector3D v1_local{ 2.0, 4.0, 6.0 }; // Renamed
    Vector3D v2_local{ 1.0, 2.0, 3.0 }; // Renamed

    Vector3D v_mul_local = v1_local * v2_local;
    EXPECT_EQ(v_mul_local, Vector3D(2.0, 8.0, 18.0));

    Vector3D v_div_local = v1_local / v2_local;
    EXPECT_EQ(v_div_local, Vector3D(2.0, 2.0, 2.0));

    Color c_test1{ 10,20,30,40 };
    Color c_test2{ 10,20,30,40 };
    Color c_test3{ 1,2,3,4 };
    EXPECT_EQ(c_test1, c_test2);
    EXPECT_NE(c_test1, c_test3);
}