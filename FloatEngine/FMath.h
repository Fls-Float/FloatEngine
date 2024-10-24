#pragma once
#include <math.h>
#include <raylib.hpp>
#include <raymath.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
struct Size {
	float width;
	float height;
};
//�Զ�������
class FVec2 {
public:
	float x, y;
	FVec2();
	FVec2(float _x, float _y);
   	FVec2(const Vector2& vec);
	FVec2(const FVec2& v);
	bool operator==(const FVec2& v);
    FVec2 operator+(const FVec2& vec);
	FVec2 operator-(const FVec2& vec);
    FVec2 operator*(const FVec2& vec);
	FVec2 operator*(float f);
    FVec2 operator/(const FVec2& vec);
	FVec2 operator/(float f);
	//ת��ΪVector2
	operator Vector2();
	//��const FVec2ת��ΪVector2
   	operator const Vector2() const;
};
class F_Line {
public:
	FVec2 p1, p2;
};
//shapes
class F_Shape {
protected:
	std::vector<FVec2> points;
public:
	float angle;
	FVec2 rot_origin;
	F_Shape();
	F_Shape(const F_Shape& shape);
	F_Shape(const std::vector<FVec2>& p);
	//�����
	void Add_Point(const FVec2& point);
	//ɾ�����һ����
	void Delete_Point_Back();
	//ɾ��number��ֵΪpoint�ĵ�(-1Ϊɾ������)
	void Delete_Point(const FVec2& point,int number=-1);
	//ɾ����index����
	void Delete_Point(int index);
	//�Ƿ���ĳ����
	bool Is_Exists_Point(const FVec2&point);
	//��ȥ��
	void Points_Deduplication();
	//���ݵ����������ȡ��
	FVec2 Get_Point_FromIndex(int index) const;
	//��ȡ��ı��
	int Get_Point_Index(const FVec2& point);
	//���ýǶ�
	void Set_Angle(float _angle);
	//������תԭ��
	void Set_Origin(FVec2 origin);
	//��ȡ�������
	inline int Get_Points_Number() const{
		return static_cast<int>(points.size());
	}
	//��ȡ���е�
	inline std::vector<FVec2> Get_Points() const {
		return points;
	}
	virtual void Shape_Set();
	virtual FVec2 Support(const FVec2& dir) const;
};
class F_Polygon :public F_Shape {
public:
	F_Polygon();
	F_Polygon(const F_Polygon& polygon);
	//�����
	void Add_Edge(const Vector2& point1, const Vector2& point2);
	//ɾ����
   	void Delete_Edge(const Vector2& point1, const Vector2& point2);
	//ɾ�����һ����
   	void Delete_Edge_Back();
	//��ȡ�ߵ�����
	inline int Get_Edge_Number() const {
		return static_cast<int>(points.size() / 2);
	}
	//��ȡ��
    F_Line Get_Edge(int index) const;

};
class F_Rectangle :public F_Shape {
public:
	float x, y, width, height;
	F_Rectangle();
	F_Rectangle(const F_Rectangle& rect);
	F_Rectangle(float _x, float _y, float _width, float _height);
	F_Rectangle(const Rectangle& rect);
	//���þ��εĲ�������ͼ����Ϣ
	void Shape_Set();
	//���þ��β���
	void Set_Rectangle(const Rectangle& _rect);
	//���þ��β���
	void Set_Rectangle(float _x, float _y, float _width, float _height);
	//��������
	void Set_Pos(float _x, float _y);
	//���ô�С
	void Set_Size(float w, float h);
	//��������
	void Set_Pos(const Vector2& pos);
	//���ô�С
	void Set_Size(const Size& size);
	//ת��ΪRectangle(angle����Ч)
	
	Rectangle To_RlRect(int no_origin = false) const;
};
class F_Circle :public F_Shape 
{

	int numPoints;
public:
	float x, y, radius;
	F_Circle();
	F_Circle(float _x, float _y, float _radius);
	F_Circle(const F_Circle& circle);
	~F_Circle();
	//��������
	void Set_Pos(const Vector2& pos);
	//��������
	void Set_Pos(float _x, float _y);
	//���ð뾶
	void Set_Radius(float _radius);
	//����Բ
	void Set_Circle(const Vector2& pos, float _radius);
	//����Բ
	void Set_Circle(float _x, float _y, float _radius);
	//���þ���
	void Set_NumPoints(int n);
	//����Բ�Ĳ�������ͼ����Ϣ
	void Shape_Set();
	//Support����
    Vector2 Support(const Vector2& dir) const;
};

class F_Ellipse : public F_Shape {
private:
	int numPoints;
public:
	float x, y;  // Center of the ellipse
	float radiusX;   // Horizontal radius
	float radiusY;   // Vertical radius

	F_Ellipse();
	F_Ellipse(const Vector2& c, float rx, float ry);
	F_Ellipse(const F_Ellipse& ellipse);
	~F_Ellipse();
	void Set_Ellipse(const Vector2& c, float rx, float ry);
	void Set_Center(float _x, float _y);
	void Set_Radius(float rx, float ry);
	void Set_NumPoints(int n);
	void Shape_Set();
    Vector2 Support(const Vector2& dir) const;
};
//������
class F_Triangle :public F_Shape {
public:
	FVec2 a, b, c;
	F_Triangle();
	F_Triangle(const F_Triangle& triangle);
	F_Triangle(FVec2 _a, FVec2 _b, FVec2 _c);
	void Shape_Set();
	void Set_Triangle(FVec2 _a, FVec2 _b, FVec2 _c);
	void Set_Angle(float _angle);
	void Set_A(FVec2 _a);
	void Set_B(FVec2 _b);
	void Set_C(FVec2 _c);
	void Set_Origin(FVec2 origin);
};
//������������
bool Sign(float _x);
//�ж����ھ�����
bool Point_In_Rectangle(FVec2 p,F_Rectangle r);
//FVec2Unit
FVec2 FVec2Unit(FVec2 v);
//�ж����߶��Ƿ��ཻ
bool Line_Segment_Collision(FVec2 a, FVec2 b, FVec2 c, FVec2 d);
//��ת��
FVec2 Rotate_Point(FVec2 point, FVec2 origin, float rot);
//ʹ��AABB�ж�(��F_Shape::angle��Ч)
bool Rectangle_Collision_AABB(const F_Rectangle& a, const F_Rectangle& b);
F_Rectangle Get_Rectangle_AABB(const F_Rectangle& a, const F_Rectangle& b);
//�ж�Բ��ײ
bool Circle_Collision(const F_Circle& a,const F_Circle& b);
//�ж�Բ�;�����ײ(��F_Shape::angle��Ч)
bool Circle_Collision_Rectangle(const F_Circle& a,const F_Rectangle& b);
//�ж�Բ�;�����ײ(���Ƕ�)
bool Circle_Collision_RectangleEx(const F_Circle& a,const F_Rectangle& b);
//GJK�㷨
FVec2 TripleProduct(const FVec2& a, const FVec2& b, const FVec2& c);
float Vector2Dot(const FVec2& a, const FVec2& b);
FVec2 Vector2Perpendicular(const FVec2& v);
FVec2 MinkowskiSupport(const F_Shape& shape1, const F_Shape& shape2, const FVec2& direction);
bool ContainsOrigin(std::vector<FVec2>& simplex, FVec2& direction);
bool GJK_Collision(const F_Shape& shape1, const F_Shape& shape2);
float PolygonArea(const std::vector<FVec2>& points);
bool PointInPolygon(const FVec2& p, const std::vector<FVec2>& points);
bool IsConvexPolygon(const std::vector<FVec2>& points);
// �������ֽⰼ�ζ����Ϊ������
std::vector<F_Shape> DecomposeConcavePolygon(const F_Shape& concavePolygon);
bool GJK_Collision_Plus(const F_Shape& shape1, const F_Shape& shape2);

