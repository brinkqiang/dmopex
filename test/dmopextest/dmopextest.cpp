
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
class Point2D : public Opex::Operable<Point2D, &Point2D::x, &Point2D::y> {
public:
    double x, y;
    
    Point2D(double x = 0, double y = 0) : x(x), y(y) {}
};

// 示例2：使用getter函数
class Vector3D : public Opex::Operable<Vector3D, &Vector3D::getX, &Vector3D::getY, &Vector3D::getZ> {
private:
    double m_x, m_y, m_z;
    
public:
    Vector3D(double x = 0, double y = 0, double z = 0) : m_x(x), m_y(y), m_z(z) {}
    
    double& getX() { return m_x; }
    double& getY() { return m_y; }
    double& getZ() { return m_z; }
    
    const double& getX() const { return m_x; }
    const double& getY() const { return m_y; }
    const double& getZ() const { return m_z; }
};

// 示例3：混合使用（成员变量 + lambda）
class Complex : public Opex::Operable<Complex, &Complex::real, &Complex::imag> {
public:
    double real, imag;
    
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
};

// 示例4：使用便利宏
class Rectangle : public Opex::Operable<Rectangle, OPEX_MEMBER(width), OPEX_MEMBER(height)> {
public:
    double width, height;
    Rectangle(double w = 0, double h = 0) : width(w), height(h) {}
};

TEST_F(frame_dmopex, init)
{
    Point2D p1(1.0, 2.0);
    Point2D p2(3.0, 4.0);
    
    Point2D p3 = p1 + p2;  // p3.x = 4.0, p3.y = 6.0
    p1 += p2;              // p1.x = 4.0, p1.y = 6.0
    
    bool equal = (p1 == p3);  // true
    bool less = (p1 < p2);    // false
    
    Vector3D v1(1, 2, 3);
    Vector3D v2(4, 5, 6);
    Vector3D v3 = v1 * v2;  // 各分量相乘
}
