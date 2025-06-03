#include "dmopex_non_intrusive.h" // 使用新的非侵入式头文件
#include "gtest.h"         // 假设 gtest.h 是正确的路径

// 前置声明，如果 dmopex_non_intrusive.h 中没有包含 iostream
// #include <iostream> // 通常 gtest 会包含

class env_dmopex
{
public:
    void init() {}
    void uninit() {}
};

// 结构体定义，不再包含 DEFINE_STRUCT_OPERATORS
struct Point2D {
    double x, y;

    Point2D() = default;
    Point2D(double x_, double y_) : x(x_), y(y_) {}
};
// 在结构体外部为 Point2D 定义操作符
DEFINE_NON_INTRUSIVE_OPERATORS_FOR(Point2D, x, y);

struct Vector3D {
    double x, y, z;

    Vector3D() = default;
    Vector3D(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
};
// 在结构体外部为 Vector3D 定义操作符
DEFINE_NON_INTRUSIVE_OPERATORS_FOR(Vector3D, x, y, z);

struct Color {
    int r, g, b, a;

    Color() = default;
    Color(int r_, int g_, int b_, int a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}
};
// 在结构体外部为 Color 定义操作符
DEFINE_NON_INTRUSIVE_OPERATORS_FOR(Color, r, g, b, a);

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

    // 测试数据保持不变
    Point2D p_default;
    Vector3D v_default;
    Color c_default;

    Point2D p1{ 1.5, 2.5 };
    Point2D p2{ 3.0, 4.0 };
    Point2D p1_copy{ 1.5, 2.5 };

    Vector3D v1_vec{ 1.0, 2.0, 3.0 }; // 重命名以避免与成员变量v1冲突 (如果DmoOpExTest中有v1)
    // 在当前代码中，类成员是 v1, v2，所以不需要重命名，保持原样
    Vector3D v1{ 1.0, 2.0, 3.0 };
    Vector3D v2{ 4.0, 5.0, 6.0 };
    Vector3D v1_copy{ 1.0, 2.0, 3.0 };


    Color c1{ 100, 150, 200 }; // a = 255
    Color c2{ 50, 75, 100, 128 };
    Color c1_copy{ 100, 150, 200, 255 };
    Color c1_different_alpha{ 100, 150, 200, 100 };
};

// Point2D测试 (内容不变)
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

// Vector3D测试 (内容不变)
TEST_F(DmOpExTest, Vector3DOperations) {
    // 加法测试
    Vector3D v_sum = v1 + v2;
    EXPECT_EQ(v_sum, Vector3D(5.0, 7.0, 9.0));

    // 减法测试
    Vector3D v_diff = v1 - v2; // (1,2,3) - (4,5,6) = (-3,-3,-3)
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

// Color测试 (内容不变)
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
    Point2D p1_local{ 1.5, 2.5 };
    Point2D p2_local{ 3.0, 4.0 };
    Point2D p1_copy_local{ 1.5, 2.5 };

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

    p_temp = p1_local;
    p_temp *= p2_local;
    EXPECT_EQ(p_temp, Point2D(4.5, 10.0));

    p_temp = p2_local;
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
    EXPECT_EQ(yellow, Color(255, 255, 0, 510));

    // 颜色乘法 - 可用于调整亮度
    Color bright_red = red * Color(2, 2, 2, 1);
    EXPECT_EQ(bright_red, Color(510, 0, 0, 255));

    // 颜色除法 - 可用于降低亮度
    Color dim_red = Color(200, 100, 50, 255) / Color(2, 2, 2, 1);
    EXPECT_EQ(dim_red, Color(100, 50, 25, 255));

    // Vector3D运算示例
    Vector3D v1_local{ 2.0, 4.0, 6.0 };
    Vector3D v2_local{ 1.0, 2.0, 3.0 };

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

// Define the struct with 64 members
struct MaxParamsStruct {
    int m1, m2, m3, m4, m5, m6, m7, m8, m9, m10;
    int m11, m12, m13, m14, m15, m16, m17, m18, m19, m20;
    int m21, m22, m23, m24, m25, m26, m27, m28, m29, m30;
    int m31, m32, m33, m34, m35, m36, m37, m38, m39, m40;
    int m41, m42, m43, m44, m45, m46, m47, m48, m49, m50;
    int m51, m52, m53, m54, m55, m56, m57, m58, m59, m60;
    int m61, m62, m63, m64;
};

// Enable non-intrusive operators for MaxParamsStruct
DEFINE_NON_INTRUSIVE_OPERATORS_FOR(MaxParamsStruct,
    m1, m2, m3, m4, m5, m6, m7, m8, m9, m10,
    m11, m12, m13, m14, m15, m16, m17, m18, m19, m20,
    m21, m22, m23, m24, m25, m26, m27, m28, m29, m30,
    m31, m32, m33, m34, m35, m36, m37, m38, m39, m40,
    m41, m42, m43, m44, m45, m46, m47, m48, m49, m50,
    m51, m52, m53, m54, m55, m56, m57, m58, m59, m60,
    m61, m62, m63, m64
)

// Helper function to set all members of MaxParamsStruct to a specific value
void InitializeStruct(MaxParamsStruct& s, int value) {
    s.m1 = value; s.m2 = value; s.m3 = value; s.m4 = value; s.m5 = value;
    s.m6 = value; s.m7 = value; s.m8 = value; s.m9 = value; s.m10 = value;
    s.m11 = value; s.m12 = value; s.m13 = value; s.m14 = value; s.m15 = value;
    s.m16 = value; s.m17 = value; s.m18 = value; s.m19 = value; s.m20 = value;
    s.m21 = value; s.m22 = value; s.m23 = value; s.m24 = value; s.m25 = value;
    s.m26 = value; s.m27 = value; s.m28 = value; s.m29 = value; s.m30 = value;
    s.m31 = value; s.m32 = value; s.m33 = value; s.m34 = value; s.m35 = value;
    s.m36 = value; s.m37 = value; s.m38 = value; s.m39 = value; s.m40 = value;
    s.m41 = value; s.m42 = value; s.m43 = value; s.m44 = value; s.m45 = value;
    s.m46 = value; s.m47 = value; s.m48 = value; s.m49 = value; s.m50 = value;
    s.m51 = value; s.m52 = value; s.m53 = value; s.m54 = value; s.m55 = value;
    s.m56 = value; s.m57 = value; s.m58 = value; s.m59 = value; s.m60 = value;
    s.m61 = value; s.m62 = value; s.m63 = value; s.m64 = value;
}

// Test fixture for MaxParamsStruct tests
class DMOPEX_MaxParamsTest : public ::testing::Test {
protected:
    MaxParamsStruct s1;
    MaxParamsStruct s2;
    MaxParamsStruct expected_s;
};

TEST_F(DMOPEX_MaxParamsTest, AdditionOperator) {
    InitializeStruct(s1, 1); // All members of s1 = 1
    InitializeStruct(s2, 2); // All members of s2 = 2
    InitializeStruct(expected_s, 3); // All members of expected_s = 3

    MaxParamsStruct sum = s1 + s2;
    EXPECT_EQ(sum, expected_s);
}

TEST_F(DMOPEX_MaxParamsTest, EqualityOperators) {
    InitializeStruct(s1, 5);
    InitializeStruct(s2, 5);
    MaxParamsStruct s3;
    InitializeStruct(s3, 10);

    EXPECT_EQ(s1, s2);
    EXPECT_NE(s1, s3);
}

TEST_F(DMOPEX_MaxParamsTest, CompoundAdditionOperator) {
    InitializeStruct(s1, 1);
    InitializeStruct(s2, 2);
    InitializeStruct(expected_s, 3);

    s1 += s2;
    EXPECT_EQ(s1, expected_s);
}


TEST_F(DMOPEX_MaxParamsTest, StreamInsertionOperator) {
    InitializeStruct(s1, 7);

    std::ostringstream oss;
    oss << s1;

    std::string expected_output = "(";
    for (int i = 0; i < 64; ++i) {
        expected_output += std::to_string(7);
        if (i < 63) {
            expected_output += ", ";
        }
    }
    expected_output += ")";

    EXPECT_EQ(oss.str(), expected_output);
}

// You can add more tests for other operators (-, *, /, -=, *=, /=)
// Example for subtraction:
TEST_F(DMOPEX_MaxParamsTest, SubtractionOperator) {
    InitializeStruct(s1, 10);
    InitializeStruct(s2, 3);
    InitializeStruct(expected_s, 7);

    MaxParamsStruct difference = s1 - s2;
    EXPECT_EQ(difference, expected_s);
}
