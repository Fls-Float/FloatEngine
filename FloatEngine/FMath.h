/**
 * @file FMath.h
 * @brief ��ѧ������
 * @defgroup Math ��ѧģ��
 *
 * �ṩ������ѧ����ͼ�����״�����ܣ�
 * - ��������
 * - ���Ρ�Բ�Ρ���Բ�Ρ������εȼ�����״����
 * - ��ײ���
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
  * @brief �ߴ�ṹ��
  */
struct Size {
    float width;  ///< ���
    float height; ///< �߶�
};

/**
 * @class FVec2
 * @ingroup Math
 *
 * @brief �Զ����ά������
 *
 * @note ʹ��ʾ����
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
     * @brief Ĭ�Ϲ��캯��
     */
    FVec2();

    /**
     * @brief ���캯��
     * @param _x x����
     * @param _y y����
     */
    FVec2(float _x, float _y);

    /**
     * @brief ��Vector2����
     * @param vec Vector2����
     */
    FVec2(const Vector2& vec);

    /**
     * @brief �������캯��
     * @param v FVec2����
     */
    FVec2(const FVec2& v);

    /**
     * @brief ���ڲ���������
     * @param v FVec2����
     * @return �����ȷ���true�����򷵻�false
     */
    bool operator==(const FVec2& v);

    /**
     * @brief �ӷ�����������
     * @param vec FVec2����
     * @return ������Ӻ��FVec2����
     */
    FVec2 operator+(const FVec2& vec);

    /**
     * @brief ��������������
     * @param vec FVec2����
     * @return ����������FVec2����
     */
    FVec2 operator-(const FVec2& vec);

    /**
     * @brief �˷�����������
     * @param vec FVec2����
     * @return ������˺��FVec2����
     */
    FVec2 operator*(const FVec2& vec);

    /**
     * @brief �˷�����������
     * @param f ������
     * @return ������˺��FVec2����
     */
    FVec2 operator*(float f);

    /**
     * @brief ��������������
     * @param vec FVec2����
     * @return ����������FVec2����
     */
    FVec2 operator/(const FVec2& vec);

    /**
     * @brief ��������������
     * @param f ������
     * @return ����������FVec2����
     */
    FVec2 operator/(float f);

    /**
     * @brief ת��ΪVector2
     * @return Vector2����
     */
    operator Vector2();

    /**
     * @brief ��const FVec2ת��ΪVector2
     * @return Vector2����
     */
    operator const Vector2() const;
};

/**
 * @class F_Line
 * @ingroup Math
 *
 * @brief �߶���
 */
class F_Line {
public:
    FVec2 p1, p2; ///< �߶ε������˵�
};

/**
 * @class F_Shape
 * @ingroup Math
 *
 * @brief ������״��
 */
class F_Shape {
protected:
    std::vector<FVec2> points; ///< ��״�Ķ���
public:
    float angle; ///< ��״����ת�Ƕ�
    FVec2 rot_origin; ///< ��״����תԭ��

    /**
     * @brief Ĭ�Ϲ��캯��
     */
    F_Shape();

    /**
     * @brief �������캯��
     * @param shape F_Shape����
     */
    F_Shape(const F_Shape& shape);

    /**
     * @brief ���캯��
     * @param p ��������
     */
    F_Shape(const std::vector<FVec2>& p);

    /**
     * @brief �����
     * @param point ����
     */
    void Add_Point(const FVec2& point);

    /**
     * @brief ɾ�����һ����
     */
    void Delete_Point_Back();

    /**
     * @brief ɾ��number��ֵΪpoint�ĵ�(-1Ϊɾ������)
     * @param point ����
     * @param number ɾ��������
     */
    void Delete_Point(const FVec2& point, int number = -1);

    /**
     * @brief ɾ����index����
     * @param index ����
     */
    void Delete_Point(int index);

    /**
     * @brief �Ƿ���ĳ����
     * @param point ����
     * @return ������ڷ���true�����򷵻�false
     */
    bool Is_Exists_Point(const FVec2& point);

    /**
     * @brief ��ȥ��
     */
    void Points_Deduplication();

    /**
     * @brief ���ݵ����������ȡ��
     * @param index ����
     * @return ����
     */
    FVec2 Get_Point_FromIndex(int index) const;

    /**
     * @brief ��ȡ��ı��
     * @param point ����
     * @return ���
     */
    int Get_Point_Index(const FVec2& point);

    /**
     * @brief ���ýǶ�
     * @param _angle �Ƕ�
     */
    void Set_Angle(float _angle);

    /**
     * @brief ������תԭ��(ͬ����ԭ��)
     * @param origin ��תԭ��
     */
    void Set_Rot_Origin(FVec2 origin);

    /**
     * @brief ��ȡ�������
     * @return �������
     */
    inline int Get_Points_Number() const {
        return static_cast<int>(points.size());
    }

    /**
     * @brief ��ȡ���е�
     * @return ��������
     */
    inline std::vector<FVec2> Get_Points() const {
        return points;
    }

    /**
     * @brief ������״
     */
    virtual void Shape_Set();

    /**
     * @brief ֧�ֺ���
     * @param dir ����
     * @return ֧�ֵ�
     */
    virtual FVec2 Support(const FVec2& dir) const;
};

/**
 * @class F_Polygon
 * @ingroup Math
 *
 * @brief �������
 */
class F_Polygon : public F_Shape {
public:
    /**
     * @brief Ĭ�Ϲ��캯��
     */
    F_Polygon();

    /**
     * @brief �������캯��
     * @param polygon F_Polygon����
     */
    F_Polygon(const F_Polygon& polygon);

    /**
     * @brief �����
     * @param point1 �ߵ����
     * @param point2 �ߵ��յ�
     */
    void Add_Edge(const Vector2& point1, const Vector2& point2);

    /**
     * @brief ɾ����
     * @param point1 �ߵ����
     * @param point2 �ߵ��յ�
     */
    void Delete_Edge(const Vector2& point1, const Vector2& point2);

    /**
     * @brief ɾ�����һ����
     */
    void Delete_Edge_Back();

    /**
     * @brief ��ȡ�ߵ�����
     * @return �ߵ�����
     */
    inline int Get_Edge_Number() const {
        return static_cast<int>(points.size() / 2);
    }

    /**
     * @brief ��ȡ��
     * @param index ����
     * @return ��
     */
    F_Line Get_Edge(int index) const;
};

/**
 * @class F_Rectangle
 * @ingroup Math
 *
 * @brief ������
 */
class F_Rectangle : public F_Shape {
public:
    float x, y, width, height;

    /**
     * @brief Ĭ�Ϲ��캯��
     */
    F_Rectangle();

    /**
     * @brief �������캯��
     * @param rect F_Rectangle����
     */
    F_Rectangle(const F_Rectangle& rect);

    /**
     * @brief ���캯��
     * @param _x x����
     * @param _y y����
     * @param _width ���
     * @param _height �߶�
     */
    F_Rectangle(float _x, float _y, float _width, float _height);

    /**
     * @brief ��Rectangle����
     * @param rect Rectangle����
     */
    F_Rectangle(const Rectangle& rect);

    /**
     * @brief ���þ��εĲ�������ͼ����Ϣ
     */
    void Shape_Set();

    /**
     * @brief ���þ��β���
     * @param _rect Rectangle����
     */
    void Set_Rectangle(const Rectangle& _rect);

    /**
     * @brief ���þ��β���
     * @param _x x����
     * @param _y y����
     * @param _width ���
     * @param _height �߶�
     */
    void Set_Rectangle(float _x, float _y, float _width, float _height);

    /**
     * @brief ��������
     * @param _x x����
     * @param _y y����
     */
    void Set_Pos(float _x, float _y);

    /**
     * @brief ���ô�С
     * @param w ���
     * @param h �߶�
     */
    void Set_Size(float w, float h);

    /**
     * @brief ��������
     * @param pos ����
     */
    void Set_Pos(const Vector2& pos);

    /**
     * @brief ���ô�С
     * @param size �ߴ�
     */
    void Set_Size(const Size& size);

    /**
     * @brief ת��ΪRectangle(angle����Ч)
     * @param no_origin �Ƿ����ԭ��
     * @return Rectangle����
     */
    Rectangle To_RlRect(int no_origin = false) const;
};

/**
 * @class F_Circle
 * @ingroup Math
 *
 * @brief Բ����
 */
class F_Circle : public F_Shape {
    int numPoints;
public:
    float x, y, radius;

    /**
     * @brief Ĭ�Ϲ��캯��
     */
    F_Circle();

    /**
     * @brief ���캯��
     * @param _x x����
     * @param _y y����
     * @param _radius �뾶
     */
    F_Circle(float _x, float _y, float _radius);

    /**
     * @brief �������캯��
     * @param circle F_Circle����
     */
    F_Circle(const F_Circle& circle);

    /**
     * @brief ��������
     */
    ~F_Circle();

    /**
     * @brief ��������
     * @param pos ����
     */
    void Set_Pos(const Vector2& pos);

    /**
     * @brief ��������
     * @param _x x����
     * @param _y y����
     */
    void Set_Pos(float _x, float _y);

    /**
     * @brief ���ð뾶
     * @param _radius �뾶
     */
    void Set_Radius(float _radius);

    /**
     * @brief ����Բ
     * @param pos ����
     * @param _radius �뾶
     */
    void Set_Circle(const Vector2& pos, float _radius);

    /**
     * @brief ����Բ
     * @param _x x����
     * @param _y y����
     * @param _radius �뾶
     */
    void Set_Circle(float _x, float _y, float _radius);

    /**
     * @brief ���þ���
     * @param n ����
     */
    void Set_NumPoints(int n);

    /**
     * @brief ����Բ�Ĳ�������ͼ����Ϣ
     */
    void Shape_Set();

    /**
     * @brief ֧�ֺ���
     * @param dir ����
     * @return ֧�ֵ�
     */
    Vector2 Support(const Vector2& dir) const;
};

/**
 * @class F_Ellipse
 * @ingroup Math
 *
 * @brief ��Բ��
 */
class F_Ellipse : public F_Shape {
private:
    int numPoints;
public:
    float x, y;  ///< ��Բ������
    float radiusX;   ///< ˮƽ�뾶
    float radiusY;   ///< ��ֱ�뾶

    /**
     * @brief Ĭ�Ϲ��캯��
     */
    F_Ellipse();

    /**
     * @brief ���캯��
     * @param c ��������
     * @param rx ˮƽ�뾶
     * @param ry ��ֱ�뾶
     */
    F_Ellipse(const Vector2& c, float rx, float ry);

    /**
     * @brief �������캯��
     * @param ellipse F_Ellipse����
     */
    F_Ellipse(const F_Ellipse& ellipse);

    /**
     * @brief ��������
     */
    ~F_Ellipse();

    /**
     * @brief ������Բ
     * @param c ��������
     * @param rx ˮƽ�뾶
     * @param ry ��ֱ�뾶
     */
    void Set_Ellipse(const Vector2& c, float rx, float ry);

    /**
     * @brief ��������
     * @param _x x����
     * @param _y y����
     */
    void Set_Center(float _x, float _y);

    /**
     * @brief ���ð뾶
     * @param rx ˮƽ�뾶
     * @param ry ��ֱ�뾶
     */
    void Set_Radius(float rx, float ry);

    /**
     * @brief ���þ���
     * @param n ����
     */
    void Set_NumPoints(int n);

    /**
     * @brief ������Բ�Ĳ�������ͼ����Ϣ
     */
    void Shape_Set();

    /**
     * @brief ֧�ֺ���
     * @param dir ����
     * @return ֧�ֵ�
     */
    Vector2 Support(const Vector2& dir) const;
};

/**
 * @class F_Triangle
 * @ingroup Math
 *
 * @brief ��������
 */
class F_Triangle : public F_Shape {
public:
    FVec2 a, b, c;

    /**
     * @brief Ĭ�Ϲ��캯��
     */
    F_Triangle();

    /**
     * @brief �������캯��
     * @param triangle F_Triangle����
     */
    F_Triangle(const F_Triangle& triangle);

    /**
     * @brief ���캯��
     * @param _a ����a
     * @param _b ����b
     * @param _c ����c
     */
    F_Triangle(FVec2 _a, FVec2 _b, FVec2 _c);

    /**
     * @brief ���������εĲ�������ͼ����Ϣ
     */
    void Shape_Set();

    /**
     * @brief ����������
     * @param _a ����a
     * @param _b ����b
     * @param _c ����c
     */
    void Set_Triangle(FVec2 _a, FVec2 _b, FVec2 _c);

    /**
     * @brief ���ýǶ�
     * @param _angle �Ƕ�
     */
    void Set_Angle(float _angle);

    /**
     * @brief ���ö���a
     * @param _a ����a
     */
    void Set_A(FVec2 _a);

    /**
     * @brief ���ö���b
     * @param _b ����b
     */
    void Set_B(FVec2 _b);

    /**
     * @brief ���ö���c
     * @param _c ����c
     */
    void Set_C(FVec2 _c);

    /**
     * @brief ������תԭ��
     * @param origin ��תԭ��
     */
    void Set_Origin(FVec2 origin);
};

namespace floatapi_math {
    /**
     * @brief ������������
     * @param _x ����
     * @return ���Ϊ������true�����򷵻�false
     */
    bool Sign(float _x);

    /**
     * @brief �ж����ھ�����
     * @param p ��
     * @param r ����
     * @return ������ڷ���true�����򷵻�false
     */
    bool Point_In_Rectangle(FVec2 p, F_Rectangle r);

    /**
     * @brief ���ص�λ����
     * @param v ����
     * @return ��λ����
     */
    FVec2 FVec2Unit(FVec2 v);

    /**
     * @brief �ж����߶��Ƿ��ཻ
     * @param a �߶�a�����
     * @param b �߶�a���յ�
     * @param c �߶�b�����
     * @param d �߶�b���յ�
     * @return ����ཻ����true�����򷵻�false
     */
    bool Line_Segment_Collision(FVec2 a, FVec2 b, FVec2 c, FVec2 d);
    /**
 * @brief ��ת��
 * @param point Ҫ��ת�ĵ�
 * @param origin ��ת��ԭ��
 * @param rot ��ת�Ƕȣ����ȣ�
 * @return ��ת��ĵ�
 */
    FVec2 Rotate_Point(FVec2 point, FVec2 origin, float rot);

    /**
     * @brief ʹ��AABB�ж�������ײ����F_Shape::angle��Ч��
     * @param a ����a
     * @param b ����b
     * @return �����ײ����true�����򷵻�false
     */
    bool Rectangle_Collision_AABB(const F_Rectangle& a, const F_Rectangle& b);

    /**
     * @brief ��ȡ�������ε�AABB
     * @param a ����a
     * @param b ����b
     * @return �����������ε�AABB
     */
    F_Rectangle Get_Rectangle_AABB(const F_Rectangle& a, const F_Rectangle& b);

    /**
     * @brief �ж�����Բ�Ƿ���ײ
     * @param a Բa
     * @param b Բb
     * @return �����ײ����true�����򷵻�false
     */
    bool Circle_Collision(const F_Circle& a, const F_Circle& b);

    /**
     * @brief �ж�Բ�;����Ƿ���ײ����F_Shape::angle��Ч��
     * @param a Բ
     * @param b ����
     * @return �����ײ����true�����򷵻�false
     */
    bool Circle_Collision_Rectangle(const F_Circle& a, const F_Rectangle& b);

    /**
     * @brief �ж�Բ�;����Ƿ���ײ�����Ƕȣ�
     * @param a Բ
     * @param b ����
     * @return �����ײ����true�����򷵻�false
     */
    bool Circle_Collision_RectangleEx(const F_Circle& a, const F_Rectangle& b);

    /**
     * @brief �������ػ�
     * @param a ����a
     * @param b ����b
     * @param c ����c
     * @return ���ػ����
     */
    FVec2 TripleProduct(const FVec2& a, const FVec2& b, const FVec2& c);

    /**
     * @brief �����������
     * @param a ����a
     * @param b ����b
     * @return ������
     */
    float Vector2Dot(const FVec2& a, const FVec2& b);

    /**
     * @brief ���������Ĵ�ֱ����
     * @param v ����
     * @return ��ֱ����
     */
    FVec2 Vector2Perpendicular(const FVec2& v);

    /**
     * @brief ����Minkowski���֧�ֵ�
     * @param shape1 ��״1
     * @param shape2 ��״2
     * @param direction ����
     * @return ֧�ֵ�
     */
    FVec2 MinkowskiSupport(const F_Shape& shape1, const F_Shape& shape2, const FVec2& direction);

    /**
     * @brief �жϵ������Ƿ����ԭ��
     * @param simplex ������
     * @param direction ����
     * @return �������ԭ�㷵��true�����򷵻�false
     */
    bool ContainsOrigin(std::vector<FVec2>& simplex, FVec2& direction);

    /**
     * @brief ʹ��GJK�㷨�ж���״��ײ
     * @param shape1 ��״1
     * @param shape2 ��״2
     * @return �����ײ����true�����򷵻�false
     */
    bool GJK_Collision(const F_Shape& shape1, const F_Shape& shape2);

    /**
     * @brief �����������
     * @param points ����ζ���
     * @return ��������
     */
    float PolygonArea(const std::vector<FVec2>& points);

    /**
     * @brief �жϵ��Ƿ��ڶ������
     * @param p ��
     * @param points ����ζ���
     * @return ������ڷ���true�����򷵻�false
     */
    bool PointInPolygon(const FVec2& p, const std::vector<FVec2>& points);

    /**
     * @brief �ж϶�����Ƿ�Ϊ͹�����
     * @param points ����ζ���
     * @return �����͹����η���true�����򷵻�false
     */
    bool IsConvexPolygon(const std::vector<FVec2>& points);

    /**
     * @brief �ֽⰼ�ζ����Ϊ������
     * @param concavePolygon ���ζ����
     * @return �ֽ�������������
     */
    std::vector<F_Shape> DecomposeConcavePolygon(const F_Shape& concavePolygon);

    /**
     * @brief ʹ�øĽ���GJK�㷨�ж���״��ײ
     * @param shape1 ��״1
     * @param shape2 ��״2
     * @return �����ײ����true�����򷵻�false
     */
    bool GJK_Collision_Plus(const F_Shape& shape1, const F_Shape& shape2);

    /**
     * @brief ����㵽������������������Ǹ���
     * @param rect ����
     * @param point ��
     * @return ����ĵ�
     */
    Vector2 Get_Closest_Point_On_Rectangle(const F_Rectangle& rect, Vector2 point);
}