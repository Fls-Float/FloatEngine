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
//自定义向量
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
	//转换为Vector2
	operator Vector2();
	//从const FVec2转换为Vector2
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
	//加入点
	void Add_Point(const FVec2& point);
	//删除最后一个点
	void Delete_Point_Back();
	//删除number个值为point的点(-1为删除所有)
	void Delete_Point(const FVec2& point,int number=-1);
	//删除第index个点
	void Delete_Point(int index);
	//是否有某个点
	bool Is_Exists_Point(const FVec2&point);
	//点去重
	void Points_Deduplication();
	//根据点的索引来获取点
	FVec2 Get_Point_FromIndex(int index) const;
	//获取点的编号
	int Get_Point_Index(const FVec2& point);
	//设置角度
	void Set_Angle(float _angle);
	//设置旋转原点
	void Set_Origin(FVec2 origin);
	//获取点的数量
	inline int Get_Points_Number() const{
		return static_cast<int>(points.size());
	}
	//获取所有点
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
	//加入边
	void Add_Edge(const Vector2& point1, const Vector2& point2);
	//删除边
   	void Delete_Edge(const Vector2& point1, const Vector2& point2);
	//删除最后一条边
   	void Delete_Edge_Back();
	//获取边的数量
	inline int Get_Edge_Number() const {
		return static_cast<int>(points.size() / 2);
	}
	//获取边
    F_Line Get_Edge(int index) const;

};
class F_Rectangle :public F_Shape {
public:
	float x, y, width, height;
	F_Rectangle();
	F_Rectangle(const F_Rectangle& rect);
	F_Rectangle(float _x, float _y, float _width, float _height);
	F_Rectangle(const Rectangle& rect);
	//利用矩形的参数设置图形信息
	void Shape_Set();
	//设置矩形参数
	void Set_Rectangle(const Rectangle& _rect);
	//设置矩形参数
	void Set_Rectangle(float _x, float _y, float _width, float _height);
	//设置坐标
	void Set_Pos(float _x, float _y);
	//设置大小
	void Set_Size(float w, float h);
	//设置坐标
	void Set_Pos(const Vector2& pos);
	//设置大小
	void Set_Size(const Size& size);
	//转换为Rectangle(angle将无效)
	
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
	//设置坐标
	void Set_Pos(const Vector2& pos);
	//设置坐标
	void Set_Pos(float _x, float _y);
	//设置半径
	void Set_Radius(float _radius);
	//设置圆
	void Set_Circle(const Vector2& pos, float _radius);
	//设置圆
	void Set_Circle(float _x, float _y, float _radius);
	//设置精度
	void Set_NumPoints(int n);
	//利用圆的参数设置图形信息
	void Shape_Set();
	//Support函数
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
//三角形
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
//返回数字正负
bool Sign(float _x);
//判定点在矩形内
bool Point_In_Rectangle(FVec2 p,F_Rectangle r);
//FVec2Unit
FVec2 FVec2Unit(FVec2 v);
//判定两线段是否相交
bool Line_Segment_Collision(FVec2 a, FVec2 b, FVec2 c, FVec2 d);
//旋转点
FVec2 Rotate_Point(FVec2 point, FVec2 origin, float rot);
//使用AABB判定(对F_Shape::angle无效)
bool Rectangle_Collision_AABB(const F_Rectangle& a, const F_Rectangle& b);
F_Rectangle Get_Rectangle_AABB(const F_Rectangle& a, const F_Rectangle& b);
//判定圆碰撞
bool Circle_Collision(const F_Circle& a,const F_Circle& b);
//判定圆和矩形碰撞(对F_Shape::angle无效)
bool Circle_Collision_Rectangle(const F_Circle& a,const F_Rectangle& b);
//判定圆和矩形碰撞(带角度)
bool Circle_Collision_RectangleEx(const F_Circle& a,const F_Rectangle& b);
//GJK算法
FVec2 TripleProduct(const FVec2& a, const FVec2& b, const FVec2& c);
float Vector2Dot(const FVec2& a, const FVec2& b);
FVec2 Vector2Perpendicular(const FVec2& v);
FVec2 MinkowskiSupport(const F_Shape& shape1, const F_Shape& shape2, const FVec2& direction);
bool ContainsOrigin(std::vector<FVec2>& simplex, FVec2& direction);
bool GJK_Collision(const F_Shape& shape1, const F_Shape& shape2);
float PolygonArea(const std::vector<FVec2>& points);
bool PointInPolygon(const FVec2& p, const std::vector<FVec2>& points);
bool IsConvexPolygon(const std::vector<FVec2>& points);
// 函数：分解凹形多边形为三角形
std::vector<F_Shape> DecomposeConcavePolygon(const F_Shape& concavePolygon);
bool GJK_Collision_Plus(const F_Shape& shape1, const F_Shape& shape2);

