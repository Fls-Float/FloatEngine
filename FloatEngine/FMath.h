/**
 * @file FMath.h
 * @brief 数学工具类
 * @defgroup Math 数学模块
 *
 * 提供各种数学计算和几何形状处理功能：
 * - 向量计算
 * - 矩形、圆形、椭圆形、三角形等几何形状处理
 * - 碰撞检测
 */

#pragma once
#include <math.h>
#include <raylib.hpp>
#include <raymath.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

 /**
  * @struct Size
  * @ingroup Math
  *
  * @brief 尺寸结构体
  */
struct Size {
    float width;  ///< 宽度
    float height; ///< 高度
};

/**
 * @class FVec2
 * @ingroup Math
 *
 * @brief 自定义二维向量类
 *
 * @note 使用示例：
 * @code
 * FVec2 vec1(1.0f, 2.0f);
 * FVec2 vec2(3.0f, 4.0f);
 * FVec2 result = vec1 + vec2;
 * @endcode
 */
class FVec2 {
public:
    float x, y;

    /**
     * @brief 默认构造函数
     */
    FVec2();

    /**
     * @brief 构造函数
     * @param _x x坐标
     * @param _y y坐标
     */
    FVec2(float _x, float _y);

    /**
     * @brief 从Vector2构造
     * @param vec Vector2对象
     */
    FVec2(const Vector2& vec);

    /**
     * @brief 拷贝构造函数
     * @param v FVec2对象
     */
    FVec2(const FVec2& v);

    /**
     * @brief 等于操作符重载
     * @param v FVec2对象
     * @return 如果相等返回true，否则返回false
     */
    bool operator==(const FVec2& v);

    /**
     * @brief 加法操作符重载
     * @param vec FVec2对象
     * @return 返回相加后的FVec2对象
     */
    FVec2 operator+(const FVec2& vec);

    /**
     * @brief 减法操作符重载
     * @param vec FVec2对象
     * @return 返回相减后的FVec2对象
     */
    FVec2 operator-(const FVec2& vec);

    /**
     * @brief 乘法操作符重载
     * @param vec FVec2对象
     * @return 返回相乘后的FVec2对象
     */
    FVec2 operator*(const FVec2& vec);

    /**
     * @brief 乘法操作符重载
     * @param f 浮点数
     * @return 返回相乘后的FVec2对象
     */
    FVec2 operator*(float f);

    /**
     * @brief 除法操作符重载
     * @param vec FVec2对象
     * @return 返回相除后的FVec2对象
     */
    FVec2 operator/(const FVec2& vec);

    /**
     * @brief 除法操作符重载
     * @param f 浮点数
     * @return 返回相除后的FVec2对象
     */
    FVec2 operator/(float f);

    /**
     * @brief 转换为Vector2
     * @return Vector2对象
     */
    operator Vector2();

    /**
     * @brief 从const FVec2转换为Vector2
     * @return Vector2对象
     */
    operator const Vector2() const;
};

/**
 * @class F_Line
 * @ingroup Math
 *
 * @brief 线段类
 */
class F_Line {
public:
    FVec2 p1, p2; ///< 线段的两个端点
};

/**
 * @class F_Shape
 * @ingroup Math
 *
 * @brief 基础形状类
 */
class F_Shape {
protected:
    std::vector<FVec2> points; ///< 形状的顶点
public:
    float angle; ///< 形状的旋转角度
    FVec2 rot_origin; ///< 形状的旋转原点

    /**
     * @brief 默认构造函数
     */
    F_Shape();

    /**
     * @brief 拷贝构造函数
     * @param shape F_Shape对象
     */
    F_Shape(const F_Shape& shape);

    /**
     * @brief 构造函数
     * @param p 顶点数组
     */
    F_Shape(const std::vector<FVec2>& p);

    /**
     * @brief 加入点
     * @param point 顶点
     */
    void Add_Point(const FVec2& point);

    /**
     * @brief 删除最后一个点
     */
    void Delete_Point_Back();

    /**
     * @brief 删除number个值为point的点(-1为删除所有)
     * @param point 顶点
     * @param number 删除的数量
     */
    void Delete_Point(const FVec2& point, int number = -1);

    /**
     * @brief 删除第index个点
     * @param index 索引
     */
    void Delete_Point(int index);

    /**
     * @brief 是否有某个点
     * @param point 顶点
     * @return 如果存在返回true，否则返回false
     */
    bool Is_Exists_Point(const FVec2& point);

    /**
     * @brief 点去重
     */
    void Points_Deduplication();

    /**
     * @brief 根据点的索引来获取点
     * @param index 索引
     * @return 顶点
     */
    FVec2 Get_Point_FromIndex(int index) const;

    /**
     * @brief 获取点的编号
     * @param point 顶点
     * @return 编号
     */
    int Get_Point_Index(const FVec2& point);

    /**
     * @brief 设置角度
     * @param _angle 角度
     */
    void Set_Angle(float _angle);

    /**
     * @brief 设置旋转原点(同绘制原点)
     * @param origin 旋转原点
     */
    void Set_Rot_Origin(FVec2 origin);

    /**
     * @brief 获取点的数量
     * @return 点的数量
     */
    inline int Get_Points_Number() const {
        return static_cast<int>(points.size());
    }

    /**
     * @brief 获取所有点
     * @return 顶点数组
     */
    inline std::vector<FVec2> Get_Points() const {
        return points;
    }

    /**
     * @brief 设置形状
     */
    virtual void Shape_Set();

    /**
     * @brief 支持函数
     * @param dir 方向
     * @return 支持点
     */
    virtual FVec2 Support(const FVec2& dir) const;
};

/**
 * @class F_Polygon
 * @ingroup Math
 *
 * @brief 多边形类
 */
class F_Polygon : public F_Shape {
public:
    /**
     * @brief 默认构造函数
     */
    F_Polygon();

    /**
     * @brief 拷贝构造函数
     * @param polygon F_Polygon对象
     */
    F_Polygon(const F_Polygon& polygon);

    /**
     * @brief 加入边
     * @param point1 边的起点
     * @param point2 边的终点
     */
    void Add_Edge(const Vector2& point1, const Vector2& point2);

    /**
     * @brief 删除边
     * @param point1 边的起点
     * @param point2 边的终点
     */
    void Delete_Edge(const Vector2& point1, const Vector2& point2);

    /**
     * @brief 删除最后一条边
     */
    void Delete_Edge_Back();

    /**
     * @brief 获取边的数量
     * @return 边的数量
     */
    inline int Get_Edge_Number() const {
        return static_cast<int>(points.size() / 2);
    }

    /**
     * @brief 获取边
     * @param index 索引
     * @return 边
     */
    F_Line Get_Edge(int index) const;
};

/**
 * @class F_Rectangle
 * @ingroup Math
 *
 * @brief 矩形类
 */
class F_Rectangle : public F_Shape {
public:
    float x, y, width, height;

    /**
     * @brief 默认构造函数
     */
    F_Rectangle();

    /**
     * @brief 拷贝构造函数
     * @param rect F_Rectangle对象
     */
    F_Rectangle(const F_Rectangle& rect);

    /**
     * @brief 构造函数
     * @param _x x坐标
     * @param _y y坐标
     * @param _width 宽度
     * @param _height 高度
     */
    F_Rectangle(float _x, float _y, float _width, float _height);

    /**
     * @brief 从Rectangle构造
     * @param rect Rectangle对象
     */
    F_Rectangle(const Rectangle& rect);

    /**
     * @brief 利用矩形的参数设置图形信息
     */
    void Shape_Set();

    /**
     * @brief 设置矩形参数
     * @param _rect Rectangle对象
     */
    void Set_Rectangle(const Rectangle& _rect);

    /**
     * @brief 设置矩形参数
     * @param _x x坐标
     * @param _y y坐标
     * @param _width 宽度
     * @param _height 高度
     */
    void Set_Rectangle(float _x, float _y, float _width, float _height);

    /**
     * @brief 设置坐标
     * @param _x x坐标
     * @param _y y坐标
     */
    void Set_Pos(float _x, float _y);

    /**
     * @brief 设置大小
     * @param w 宽度
     * @param h 高度
     */
    void Set_Size(float w, float h);

    /**
     * @brief 设置坐标
     * @param pos 坐标
     */
    void Set_Pos(const Vector2& pos);

    /**
     * @brief 设置大小
     * @param size 尺寸
     */
    void Set_Size(const Size& size);

    /**
     * @brief 转换为Rectangle(angle将无效)
     * @param no_origin 是否忽略原点
     * @return Rectangle对象
     */
    Rectangle To_RlRect(int no_origin = false) const;
};

/**
 * @class F_Circle
 * @ingroup Math
 *
 * @brief 圆形类
 */
class F_Circle : public F_Shape {
    int numPoints;
public:
    float x, y, radius;

    /**
     * @brief 默认构造函数
     */
    F_Circle();

    /**
     * @brief 构造函数
     * @param _x x坐标
     * @param _y y坐标
     * @param _radius 半径
     */
    F_Circle(float _x, float _y, float _radius);

    /**
     * @brief 拷贝构造函数
     * @param circle F_Circle对象
     */
    F_Circle(const F_Circle& circle);

    /**
     * @brief 析构函数
     */
    ~F_Circle();

    /**
     * @brief 设置坐标
     * @param pos 坐标
     */
    void Set_Pos(const Vector2& pos);

    /**
     * @brief 设置坐标
     * @param _x x坐标
     * @param _y y坐标
     */
    void Set_Pos(float _x, float _y);

    /**
     * @brief 设置半径
     * @param _radius 半径
     */
    void Set_Radius(float _radius);

    /**
     * @brief 设置圆
     * @param pos 坐标
     * @param _radius 半径
     */
    void Set_Circle(const Vector2& pos, float _radius);

    /**
     * @brief 设置圆
     * @param _x x坐标
     * @param _y y坐标
     * @param _radius 半径
     */
    void Set_Circle(float _x, float _y, float _radius);

    /**
     * @brief 设置精度
     * @param n 精度
     */
    void Set_NumPoints(int n);

    /**
     * @brief 利用圆的参数设置图形信息
     */
    void Shape_Set();

    /**
     * @brief 支持函数
     * @param dir 方向
     * @return 支持点
     */
    Vector2 Support(const Vector2& dir) const;
};

/**
 * @class F_Ellipse
 * @ingroup Math
 *
 * @brief 椭圆类
 */
class F_Ellipse : public F_Shape {
private:
    int numPoints;
public:
    float x, y;  ///< 椭圆的中心
    float radiusX;   ///< 水平半径
    float radiusY;   ///< 垂直半径

    /**
     * @brief 默认构造函数
     */
    F_Ellipse();

    /**
     * @brief 构造函数
     * @param c 中心坐标
     * @param rx 水平半径
     * @param ry 垂直半径
     */
    F_Ellipse(const Vector2& c, float rx, float ry);

    /**
     * @brief 拷贝构造函数
     * @param ellipse F_Ellipse对象
     */
    F_Ellipse(const F_Ellipse& ellipse);

    /**
     * @brief 析构函数
     */
    ~F_Ellipse();

    /**
     * @brief 设置椭圆
     * @param c 中心坐标
     * @param rx 水平半径
     * @param ry 垂直半径
     */
    void Set_Ellipse(const Vector2& c, float rx, float ry);

    /**
     * @brief 设置中心
     * @param _x x坐标
     * @param _y y坐标
     */
    void Set_Center(float _x, float _y);

    /**
     * @brief 设置半径
     * @param rx 水平半径
     * @param ry 垂直半径
     */
    void Set_Radius(float rx, float ry);

    /**
     * @brief 设置精度
     * @param n 精度
     */
    void Set_NumPoints(int n);

    /**
     * @brief 利用椭圆的参数设置图形信息
     */
    void Shape_Set();

    /**
     * @brief 支持函数
     * @param dir 方向
     * @return 支持点
     */
    Vector2 Support(const Vector2& dir) const;
};

/**
 * @class F_Triangle
 * @ingroup Math
 *
 * @brief 三角形类
 */
class F_Triangle : public F_Shape {
public:
    FVec2 a, b, c;

    /**
     * @brief 默认构造函数
     */
    F_Triangle();

    /**
     * @brief 拷贝构造函数
     * @param triangle F_Triangle对象
     */
    F_Triangle(const F_Triangle& triangle);

    /**
     * @brief 构造函数
     * @param _a 顶点a
     * @param _b 顶点b
     * @param _c 顶点c
     */
    F_Triangle(FVec2 _a, FVec2 _b, FVec2 _c);

    /**
     * @brief 利用三角形的参数设置图形信息
     */
    void Shape_Set();

    /**
     * @brief 设置三角形
     * @param _a 顶点a
     * @param _b 顶点b
     * @param _c 顶点c
     */
    void Set_Triangle(FVec2 _a, FVec2 _b, FVec2 _c);

    /**
     * @brief 设置角度
     * @param _angle 角度
     */
    void Set_Angle(float _angle);

    /**
     * @brief 设置顶点a
     * @param _a 顶点a
     */
    void Set_A(FVec2 _a);

    /**
     * @brief 设置顶点b
     * @param _b 顶点b
     */
    void Set_B(FVec2 _b);

    /**
     * @brief 设置顶点c
     * @param _c 顶点c
     */
    void Set_C(FVec2 _c);

    /**
     * @brief 设置旋转原点
     * @param origin 旋转原点
     */
    void Set_Origin(FVec2 origin);
};

namespace floatapi_math {
    /**
     * @brief 返回数字正负
     * @param _x 数字
     * @return 如果为正返回true，否则返回false
     */
    bool Sign(float _x);

    /**
     * @brief 判定点在矩形内
     * @param p 点
     * @param r 矩形
     * @return 如果在内返回true，否则返回false
     */
    bool Point_In_Rectangle(FVec2 p, F_Rectangle r);

    /**
     * @brief 返回单位向量
     * @param v 向量
     * @return 单位向量
     */
    FVec2 FVec2Unit(FVec2 v);

    /**
     * @brief 判定两线段是否相交
     * @param a 线段a的起点
     * @param b 线段a的终点
     * @param c 线段b的起点
     * @param d 线段b的终点
     * @return 如果相交返回true，否则返回false
     */
    bool Line_Segment_Collision(FVec2 a, FVec2 b, FVec2 c, FVec2 d);
    /**
 * @brief 旋转点
 * @param point 要旋转的点
 * @param origin 旋转的原点
 * @param rot 旋转角度（弧度）
 * @return 旋转后的点
 */
    FVec2 Rotate_Point(FVec2 point, FVec2 origin, float rot);

    /**
     * @brief 使用AABB判定矩形碰撞（对F_Shape::angle无效）
     * @param a 矩形a
     * @param b 矩形b
     * @return 如果碰撞返回true，否则返回false
     */
    bool Rectangle_Collision_AABB(const F_Rectangle& a, const F_Rectangle& b);

    /**
     * @brief 获取两个矩形的AABB
     * @param a 矩形a
     * @param b 矩形b
     * @return 包含两个矩形的AABB
     */
    F_Rectangle Get_Rectangle_AABB(const F_Rectangle& a, const F_Rectangle& b);

    /**
     * @brief 判定两个圆是否碰撞
     * @param a 圆a
     * @param b 圆b
     * @return 如果碰撞返回true，否则返回false
     */
    bool Circle_Collision(const F_Circle& a, const F_Circle& b);

    /**
     * @brief 判定圆和矩形是否碰撞（对F_Shape::angle无效）
     * @param a 圆
     * @param b 矩形
     * @return 如果碰撞返回true，否则返回false
     */
    bool Circle_Collision_Rectangle(const F_Circle& a, const F_Rectangle& b);

    /**
     * @brief 判定圆和矩形是否碰撞（带角度）
     * @param a 圆
     * @param b 矩形
     * @return 如果碰撞返回true，否则返回false
     */
    bool Circle_Collision_RectangleEx(const F_Circle& a, const F_Rectangle& b);

    /**
     * @brief 计算三重积
     * @param a 向量a
     * @param b 向量b
     * @param c 向量c
     * @return 三重积结果
     */
    FVec2 TripleProduct(const FVec2& a, const FVec2& b, const FVec2& c);

    /**
     * @brief 计算向量点积
     * @param a 向量a
     * @param b 向量b
     * @return 点积结果
     */
    float Vector2Dot(const FVec2& a, const FVec2& b);

    /**
     * @brief 计算向量的垂直向量
     * @param v 向量
     * @return 垂直向量
     */
    FVec2 Vector2Perpendicular(const FVec2& v);

    /**
     * @brief 计算Minkowski差集的支持点
     * @param shape1 形状1
     * @param shape2 形状2
     * @param direction 方向
     * @return 支持点
     */
    FVec2 MinkowskiSupport(const F_Shape& shape1, const F_Shape& shape2, const FVec2& direction);

    /**
     * @brief 判断单纯形是否包含原点
     * @param simplex 单纯形
     * @param direction 方向
     * @return 如果包含原点返回true，否则返回false
     */
    bool ContainsOrigin(std::vector<FVec2>& simplex, FVec2& direction);

    /**
     * @brief 使用GJK算法判定形状碰撞
     * @param shape1 形状1
     * @param shape2 形状2
     * @return 如果碰撞返回true，否则返回false
     */
    bool GJK_Collision(const F_Shape& shape1, const F_Shape& shape2);

    /**
     * @brief 计算多边形面积
     * @param points 多边形顶点
     * @return 多边形面积
     */
    float PolygonArea(const std::vector<FVec2>& points);

    /**
     * @brief 判断点是否在多边形内
     * @param p 点
     * @param points 多边形顶点
     * @return 如果在内返回true，否则返回false
     */
    bool PointInPolygon(const FVec2& p, const std::vector<FVec2>& points);

    /**
     * @brief 判断多边形是否为凸多边形
     * @param points 多边形顶点
     * @return 如果是凸多边形返回true，否则返回false
     */
    bool IsConvexPolygon(const std::vector<FVec2>& points);

    /**
     * @brief 分解凹形多边形为三角形
     * @param concavePolygon 凹形多边形
     * @return 分解后的三角形数组
     */
    std::vector<F_Shape> DecomposeConcavePolygon(const F_Shape& concavePolygon);

    /**
     * @brief 使用改进的GJK算法判定形状碰撞
     * @param shape1 形状1
     * @param shape2 形状2
     * @return 如果碰撞返回true，否则返回false
     */
    bool GJK_Collision_Plus(const F_Shape& shape1, const F_Shape& shape2);

    /**
     * @brief 计算点到矩形四条边上最近的那个点
     * @param rect 矩形
     * @param point 点
     * @return 最近的点
     */
    Vector2 Get_Closest_Point_On_Rectangle(const F_Rectangle& rect, Vector2 point);
}