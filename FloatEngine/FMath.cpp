#include "FMath.h"
FVec2::FVec2() :x(0), y(0)
{
}

FVec2::FVec2(float _x, float _y) :x(_x), y(_y)
{
}

FVec2::FVec2(const Vector2& vec) :x(vec.x), y(vec.y)
{
}

FVec2::FVec2(const FVec2& v) :x(v.x), y(v.y)
{
}

bool FVec2::operator==(const FVec2& v)
{
    return x == v.x && y == v.y;
}

FVec2 FVec2::operator+(const FVec2& vec)
{
    return FVec2(x + vec.x, y + vec.y);
}

FVec2 FVec2::operator-(const FVec2& vec)
{
    return FVec2(x - vec.x, y - vec.y);
}

FVec2 FVec2::operator*(const FVec2& vec)
{
    return FVec2(x * vec.x, y * vec.y);
}

FVec2 FVec2::operator*(float f)
{
    return FVec2(x * f, y * f);
}

FVec2 FVec2::operator/(const FVec2& vec)
{
    return FVec2(x / vec.x, y / vec.y);
}

FVec2 FVec2::operator/(float f)
{
    return FVec2(x / f, y / f);
}



FVec2::operator Vector2()
{
    return Vector2(x, y);
}
FVec2::operator const Vector2() const
{
    return Vector2(x, y);
}
F_Shape::F_Shape()
{
    points = {};
    angle = 0;
    rot_origin = FVec2(0, 0);
}
F_Shape::F_Shape(const F_Shape& shape) {
    points = shape.points;
    angle = shape.angle;
    rot_origin = shape.rot_origin;
}

F_Shape::F_Shape(const std::vector<
    FVec2>& p)
{
    points = p;
    angle = 0;
    rot_origin = FVec2(0, 0);
}

void F_Shape::Add_Point(const FVec2& point)
{
    points.push_back(point);
}

void F_Shape::Delete_Point_Back()
{
    points.pop_back();
}
void F_Shape::Delete_Point(const FVec2& point, int number) {
    int index = 0, deled = 0;
    for (auto it = points.begin(); it != points.end(); ++it, ++index) {
        if (deled == number) {
            return;
        }
        if (it->x == point.x && it->y == point.y) {
            it = points.erase(it);
            --it;  // Adjust iterator since erase invalidates it
            deled++;
        }
    }
    if (index == points.size()) {
        std::cout << "错误:F_Shape_Delete_Point: 未找到点" << std::endl;
    }
}

void F_Shape::Delete_Point(int index) {
    if (index >= 0 && index < points.size()) {
        points.erase(points.begin() + index);
    }
}

bool F_Shape::Is_Exists_Point(const FVec2& point)
{
    for (auto p : points) {
        if (p.x == point.x && p.y == point.y) {
            return true;
        }
    }

    return false;
}

void F_Shape::Points_Deduplication() {
    for (auto it = points.begin(); it != points.end(); ++it) {
        for (auto it1 = it + 1; it1 != points.end(); ++it1) {
            if (it->x == it1->x && it->y == it1->y) {
                it1 = points.erase(it1);
                --it1;  // Adjust iterator since erase invalidates it
            }
        }
    }
}

FVec2 F_Shape::Get_Point_FromIndex(int index) const
{
    return points[index];
}

void F_Shape::Set_Angle(float _angle)
{
    angle = _angle;
}

void F_Shape::Set_Origin(FVec2 origin)
{
    rot_origin = origin;
}

int F_Shape::Get_Point_Index(const FVec2& point)
{
    int index = 0;
    for (auto p : points) {
         if (p.x == point.x && p.y == point.y) {
             return index;
         }
        index++;
    }
}

void F_Shape::Shape_Set()
{
}

FVec2 F_Shape::Support(const FVec2& dir) const
{
    float max_dot = -FLT_MAX;
    FVec2 farthest_point = { -1,-1 };

    for (const auto& point : points) {
        float dot_product = Vector2Dot(point, dir);
        if (dot_product > max_dot) {
            max_dot = dot_product;
            farthest_point = point;
        }
    }

    return farthest_point;
}

F_Polygon::F_Polygon()
{
}

F_Polygon::F_Polygon(const F_Polygon& polygon)
{
    points = polygon.points;
}

void F_Polygon::Add_Edge(const Vector2& point1, const Vector2& point2)
{
    points.push_back(point1);
}

void F_Polygon::Delete_Edge(const Vector2& point1, const Vector2& point2)
{
    for (auto it = points.begin(); it != points.end(); ++it) {
        if (it->x == point1.x && it->y == point1.y) {
            it = points.erase(it);
            --it;  // Adjust iterator since erase invalidates it
        }
    }
    for (auto it = points.begin(); it != points.end(); ++it) {
        if (it->x == point2.x && it->y == point2.y) {
            it = points.erase(it);
            --it;  // Adjust iterator since erase invalidates it
        }
    }
}

void F_Polygon::Delete_Edge_Back()
{
    points.pop_back();
}

F_Line F_Polygon::Get_Edge(int index) const
{
    return F_Line(points[index*2], points[index*2+1]);  
}

F_Rectangle::F_Rectangle()
{
    x = y = width = height = 0;
    points.resize(4);
}

F_Rectangle::F_Rectangle(const F_Rectangle& rect)
{
    x = rect.x;
    y = rect.y;
    width = rect.width;
    height = rect.height;
    points = rect.points;
}

F_Rectangle::F_Rectangle(float _x, float _y, float _width, float _height)
{
    x = _x;
    y = _y;
    width = _width;
    height = _height;
}

F_Rectangle::F_Rectangle(const Rectangle& rect)
{
    x = rect.x;
    y = rect.y;
    width = rect.width;
    height = rect.height;
}

void F_Rectangle::Shape_Set()
{
    points[0] = Rotate_Point({ x,y }, 
        Vector2Add({ x,y }, { rot_origin.x* width, rot_origin.y* height }), angle);
    points[1] = Rotate_Point({ x+width,y },
        Vector2Add({ x,y }, { rot_origin.x * width, rot_origin.y * height }), angle);
    points[2] = Rotate_Point({ x+width,y+height },
        Vector2Add({ x,y }, { rot_origin.x * width, rot_origin.y * height }), angle);
    points[3] = Rotate_Point({ x,y+height },
        Vector2Add({ x,y }, { rot_origin.x * width, rot_origin.y * height }), angle);

}

void F_Rectangle::Set_Rectangle(const Rectangle& _rect)
{
    x = _rect.x;
    y = _rect.y;
    width = _rect.width;
    height = _rect.height;
}

void F_Rectangle::Set_Rectangle(float _x, float _y, float _width, float _height)
{
    x = _x;
    y = _y;
    width = _width;
    height = _height;
}

void F_Rectangle::Set_Pos(float _x, float _y)
{
    x = _x;
    y = _y;
}
void F_Rectangle::Set_Size(float w, float h)
{
    width = w;
    height = h;
}
void F_Rectangle::Set_Pos(const Vector2& pos) {
    x = pos.x;
    y = pos.y;
}

void F_Rectangle::Set_Size(const Size& size)
{
    width = size.width;
    height = size.height;
}


Rectangle F_Rectangle::To_RlRect(int no_origin) const
{
    if (!no_origin)
        return { x + rot_origin.x * width,y + rot_origin.y * height,width,height };
    else return { x,y,width,height };
}


F_Circle::F_Circle()
{
    x = y = radius = 0;
    numPoints = 360;
}

F_Circle::F_Circle(float _x, float _y, float _radius)
    :x(_x),y(_y),radius(_radius),numPoints(360) 
{
}

F_Circle::F_Circle(const F_Circle& circle)
{
    x = circle.x;
    y = circle.y;
    radius = circle.radius;
    points = circle.points;
    numPoints = circle.numPoints;
}

F_Circle::~F_Circle()
{
    points.clear();
}

void F_Circle::Set_Pos(const Vector2& pos)
{
    x = pos.x;
    y = pos.y;

}

void F_Circle::Set_Pos(float _x, float _y)
{
    x = _x;
    y = _y;
}

void F_Circle::Set_Radius(float _radius)
{
    radius = _radius;
}

void F_Circle::Set_Circle(const Vector2& pos, float _radius)
{
    x = pos.x;
    y = pos.y;
    radius = _radius;

}

void F_Circle::Set_Circle(float _x, float _y, float _radius)
{
    x = _x;
    y = _y;
    radius = _radius;
}

void F_Circle::Set_NumPoints(int n)
{
    numPoints = n;
}

void F_Circle::Shape_Set()
{
    for (float i = 0; i <= numPoints; i++) {
        Vector2 p = { cos(i * PI / 180) * radius + x,sin(i * PI / 180) * radius + y };
        Add_Point(p);
    }
    
}

Vector2 F_Circle::Support(const Vector2& dir) const
{
    Vector2 normalizedDir = Vector2Normalize(dir);
    return Vector2Add({ x, y }, Vector2Scale(normalizedDir, radius));
}


F_Ellipse::F_Ellipse() : x(0),y(0), radiusX(0), radiusY(0),numPoints(360) {}

F_Ellipse::F_Ellipse(const Vector2& c, float rx, float ry)
    : F_Shape(), x(c.x),y(c.y), radiusX(rx), radiusY(ry),numPoints(360) {
}

F_Ellipse::F_Ellipse(const F_Ellipse& ellipse)
    : F_Shape(ellipse), x(ellipse.x), y(ellipse.y), radiusX(ellipse.radiusX), radiusY(ellipse.radiusY),numPoints(ellipse.numPoints) {}
void F_Ellipse::Set_Ellipse(const Vector2& c, float rx, float ry) {
    x = c.x;
    y = c.y;
    radiusX = rx;
    radiusY = ry;
    points.clear();
}

void F_Ellipse::Set_Center(float _x, float _y) {
    x = _x;
    y = _y;
}

void F_Ellipse::Set_Radius(float rx, float ry) {
    radiusX = rx;
    radiusY = ry;
}
void F_Ellipse::Set_NumPoints(int n)
{
    numPoints = n;
}
void F_Ellipse::Shape_Set() {
    points.clear(); // 清空之前的点
    // 遍历生成椭圆上的点
    for (int i = 0; i < numPoints; ++i) {
        float a = (2 * PI * i) / numPoints; // 计算当前角度
        float _x = x + radiusX * std::cos(a); // 椭圆的 x 坐标
        float _y = y + radiusY * std::sin(a); // 椭圆的 y 坐标
        Vector2 p = { _x, _y };
        p = Rotate_Point(p, Vector2Add({ x, y }, Vector2Multiply(rot_origin,{ radiusX,radiusY })), angle);
        points.push_back(p); // 添加点到 points 容器中
    }
}
F_Ellipse::~F_Ellipse() {
    points.clear();
}

Vector2 F_Ellipse::Support(const Vector2 & u) const {
    // 旋转方向向量到椭圆的局部坐标系
    Vector2 rotated_u = Vector2Rotate(u,-angle*PI / 180);  // 将方向向量旋转到与椭圆一致的角度

    // 检查边界情况
    if (rotated_u.x == 0 && rotated_u.y > 0) return Vector2Add(Vector2Rotate({ 0, radiusY },-angle * PI / 180),{x,y});  // 上
    if (rotated_u.x == 0 && rotated_u.y < 0) return Vector2Add(Vector2Rotate({ 0, -radiusY },-angle * PI / 180),{x,y}); // 下
    if (rotated_u.y == 0 && rotated_u.x > 0) return Vector2Add(Vector2Rotate({ radiusX, 0 },-angle * PI / 180), { x,y });  // 右
    if (rotated_u.y == 0 && rotated_u.x < 0) return Vector2Add(Vector2Rotate({ -radiusX, 0 },-angle * PI / 180), { x,y }); // 左

    // 计算支持点
    float k = rotated_u.y / rotated_u.x;
    float d = std::sqrt((radiusX * radiusX + radiusY * radiusY * k * k) / (k * k ));

    if (Vector2Dot(rotated_u,Vector2{ 0, d }) < 0) d = -d;

    float x2 = k * d - (radiusY * radiusY *k* k * k * d) / (radiusX * radiusX + radiusY * radiusY * k * k);
    float y2 = (radiusY * radiusY * k * k * d) / (radiusX * radiusX + radiusY * radiusY * k * k);

    // 返回世界坐标系中的支持点
    return Vector2Add(Vector2Rotate({ x2, y2 }, angle*PI/180),{ x,y }); // 旋转回世界坐标系
}


F_Triangle::F_Triangle() :a(0, 0), b(0, 0), c(0, 0)
{
}

F_Triangle::F_Triangle(const F_Triangle& triangle) :
    a(triangle.a), b(triangle.b), c(triangle.c)
{
}

F_Triangle::F_Triangle(FVec2 _a, FVec2 _b, FVec2 _c) :
    a(_a), b(_b), c(_c)
{

}

void F_Triangle::Shape_Set()
{
    // 计算 origin 在世界坐标系中的位置
    FVec2 origin_pos = {
        (a.x + b.x + c.x) / (6 * rot_origin.x),
       (a.y + b.y + c.y) / (6 * rot_origin.y)
    };
    points.clear();
    points.push_back(Rotate_Point(a, origin_pos, angle));
    points.push_back(Rotate_Point(b, origin_pos, angle));
    points.push_back(Rotate_Point(c, origin_pos, angle));
}

void F_Triangle::Set_Triangle(FVec2 _a, FVec2 _b, FVec2 _c)
{
    a = _a;
    b = _b;
    c = _c;
}

void F_Triangle::Set_Angle(float _angle)
{
    angle = _angle;
}

void F_Triangle::Set_A(FVec2 _a)
{
    _a = a;
}

void F_Triangle::Set_B(FVec2 _b)
{
    _b = b;
}

void F_Triangle::Set_C(FVec2 _c)
{
    _c = c;
}

void F_Triangle::Set_Origin(FVec2 origin)
{
    rot_origin = origin;
}







//函数
bool Sign(float _x) {
    if (_x == 0) {
        return 0;
    }
    return fabsf(_x) / (_x);
}

bool Point_In_Rectangle(FVec2 p, F_Rectangle r)
{
    if (CheckCollisionPointRec(p, r.To_RlRect(1))) return true;
    return false;
}

FVec2 FVec2Unit(FVec2 v)
{
    if(Vector2Length(v) == 0) return { 0,0 };
    return Vector2Scale(v, 1 / Vector2Length(v));
}

bool Line_Segment_Collision(FVec2 a, FVec2 b, FVec2 c, FVec2 d)
{
    if (CheckCollisionLines(a,b,c,d,NULL)) return true;
    return false;
}


FVec2 Rotate_Point(FVec2 point, FVec2 origin, float rot) {
    return Vector2Add(origin, Vector2Rotate(Vector2Subtract(point,origin), rot));
}

bool Rectangle_Collision_AABB(const F_Rectangle& a, const F_Rectangle& b)
{
    Rectangle a1 = a.To_RlRect(1);
    Rectangle a2 = b.To_RlRect(1);
    if (CheckCollisionRecs(a1, a2)) {
        return true;
    }
    return false;
}

F_Rectangle Get_Rectangle_AABB(const F_Rectangle& a, const F_Rectangle& b)
{
    Rectangle a1 = a.To_RlRect(1);
    Rectangle a2 = b.To_RlRect(1);
    if (CheckCollisionRecs(a1, a2)) {
        return GetCollisionRec(a1, a2);
    }
    return F_Rectangle{ 0, 0, 0, 0 };
}

bool Circle_Collision(const F_Circle& a,const F_Circle& b)
{
    float distance = Vector2Distance({ a.x,a.y }, { b.x,b.y });
    if (distance < a.radius + b.radius) {
        return true;
    }
    return false;
}

bool Circle_Collision_Rectangle(const F_Circle& a,const F_Rectangle& b)
{
    return CheckCollisionCircleRec({ a.x,a.y }, a.radius, b.To_RlRect(1));
}
// Function to check collision between a circle and a rotated rectangle
bool Circle_Collision_RectangleEx(const F_Circle& circle, const F_Rectangle& rect) {
    // Circle center and radius
    float circleX = circle.x;
    float circleY = circle.y;
    float circleRadius = circle.radius;
    // Rectangle parameters
    float rectX = rect.x;
    float rectY = rect.y;
    float rectWidth = rect.width;
    float rectHeight = rect.height;
    float rectAngle = rect.angle;

    // Calculate the rectangle's four corner points
    FVec2 topLeft = { rectX, rectY };
    FVec2 topRight = { rectX + rectWidth, rectY };
    FVec2 bottomLeft = { rectX, rectY + rectHeight };
    FVec2 bottomRight = { rectX + rectWidth, rectY + rectHeight };

    // Rotate rectangle corners around the origin (rect.rot_origin)
    FVec2 origin = rect.rot_origin;
    topLeft = Rotate_Point(topLeft,origin ,rectAngle);
    topRight = Rotate_Point(topRight, origin,rectAngle);
    bottomLeft = Rotate_Point(bottomLeft,origin ,rectAngle);
    bottomRight = Rotate_Point(bottomRight, origin,rectAngle);

    // Get the axis-aligned bounding box of the rotated rectangle
    float minX = std::min({ topLeft.x, topRight.x, bottomLeft.x, bottomRight.x });
    float maxX = std::max({ topLeft.x, topRight.x, bottomLeft.x, bottomRight.x });
    float minY = std::min({ topLeft.y, topRight.y, bottomLeft.y, bottomRight.y });
    float maxY = std::max({ topLeft.y, topRight.y, bottomLeft.y, bottomRight.y });

    // Find the closest point on the AABB to the circle's center
    float closestX = std::max(minX, std::min(circleX, maxX));
    float closestY = std::max(minY, std::min(circleY, maxY));

    // Calculate the distance between the circle's center and this closest point
    float distanceX = circleX - closestX;
    float distanceY = circleY - closestY;
    float distanceSquared = distanceX * distanceX + distanceY * distanceY;

    // Check if the distance is less than or equal to the circle's radius
    return distanceSquared <= (circleRadius * circleRadius);
}



//GJK
FVec2 TripleProduct(const FVec2& a, const FVec2& b, const FVec2& c) {
    float ac = Vector2Dot(a, c);
    float bc = Vector2Dot(b, c);
    return { b.x * ac - a.x * bc, b.y * ac - a.y * bc };
}

float Vector2Dot(const FVec2& a, const FVec2& b) {
    return a.x * b.x + a.y * b.y;
}

FVec2 Vector2Perpendicular(const FVec2& v) {
    return { -v.y, v.x };
}
FVec2 MinkowskiSupport(const F_Shape& shape1, const F_Shape& shape2, const FVec2& direction) {
    FVec2 p1 = shape1.Support( direction);
    FVec2 p2 = shape2.Support( Vector2Negate(direction));
    return Vector2Add(p1, Vector2Negate(p2));
    
}

bool ContainsOrigin(std::vector<FVec2>& simplex, FVec2& direction) {
    FVec2 a = simplex.back();
    FVec2 ao = Vector2Negate(a);

    if (simplex.size() == 3) {
        FVec2 b = simplex[1];
        FVec2 c = simplex[0];

        FVec2 ab = Vector2Add(b , Vector2Negate(a));
        FVec2 ac = Vector2Add(c, Vector2Negate(a));

        FVec2 abPerp = TripleProduct(ac, ab, ab);
        FVec2 acPerp = TripleProduct(ab, ac, ac);

        if (Vector2Dot (abPerp, ao) > 0) {
            simplex.erase(simplex.begin());
            direction = abPerp;
        }
        else if (Vector2Dot(acPerp, ao) > 0) {
            simplex.erase(simplex.begin() + 1);
            direction = acPerp;
        }
        else {
            return true;  // 原点在三角形中
        }
    }
    else {
        FVec2 b = simplex[0];
        FVec2 ab = Vector2Add(b, Vector2Negate(a));

        direction = TripleProduct(ab, ao, ab);
        if (Vector2Dot(direction, direction) == 0) {
            direction = Vector2Perpendicular(ab);
        }
    }

    return false;
}
bool GJK_Collision(const F_Shape& shape1, const F_Shape& shape2) {
    FVec2 direction = { 1, 0 };  // 初始方向
    std::vector<FVec2> simplex;
    simplex.push_back(MinkowskiSupport(shape1, shape2, direction));

    direction = Vector2Negate(simplex[0]);

    while (true) {
        FVec2 new_point = MinkowskiSupport(shape1, shape2, direction);
        if (Vector2Dot(new_point, direction) <= 0) {
            return false;  // 如果最远点不在原点前方，图形不相交
        }

        simplex.push_back(new_point);

        if (ContainsOrigin(simplex, direction)) {
            return true;  // 原点在单纯形内，图形相交
        }
    }
}
float PolygonArea(const std::vector<FVec2>& points) {
    float area = 0.0f;
    int n = points.size();
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        area += points[i].x * points[j].y - points[j].x * points[i].y;
    }
    return area / 2.0f;
}

// 函数：检查点是否在多边形内部
bool PointInPolygon(const FVec2& p, const std::vector<FVec2>& points) {
    bool inside = false;
    int n = points.size();
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if ((points[i].y > p.y) != (points[j].y > p.y) &&
            (p.x < (points[j].x - points[i].x) * (p.y - points[i].y) / (points[j].y - points[i].y) + points[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}
bool IsConvexPolygon(const std::vector<FVec2>& points) {
    int n = points.size();
    if (n < 3) return false; // 少于3个点不是多边形

    bool isPositive = false;
    for (int i = 0; i < n; ++i) {
        FVec2 A = points[i];
        FVec2 B = points[(i + 1) % n];
        FVec2 C = points[(i + 2) % n];

        float crossProduct = (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
        if (i == 0) {
            isPositive = crossProduct > 0; // 记录第一个叉积的符号
        }
        else {
            if ((crossProduct > 0) != isPositive) {
                return false; // 叉积符号不一致，说明是凹形
            }
        }
    }
    return true; // 所有叉积符号一致，说明是凸形
}

// 函数：分解凹形多边形为三角形
std::vector<F_Shape> DecomposeConcavePolygon(const F_Shape& concavePolygon) {
    std::vector<F_Shape> triangles;
    if (IsConvexPolygon(concavePolygon.Get_Points())) {
        triangles.push_back(concavePolygon);
        return triangles;
    }
    std::vector<FVec2> points = concavePolygon.Get_Points();
    int n = points.size();

    // 确保有足够的点
    if (n < 3) return triangles;

    // 选择一个点作为参考点
    FVec2 referencePoint = points[0];

    // 将多边形的其他点按角度排序
    std::vector<FVec2> sortedPoints = points;
    std::sort(sortedPoints.begin() + 1, sortedPoints.end(), [&](const FVec2& a, const FVec2& b) {
        float angleA = atan2(a.y - referencePoint.y, a.x - referencePoint.x);
        float angleB = atan2(b.y - referencePoint.y, b.x - referencePoint.x);
        return angleA < angleB;
        });

    // 分解为三角形
    for (int i = 1; i < n - 1; ++i) {
        std::vector<FVec2> triangle = { referencePoint, sortedPoints[i], sortedPoints[i + 1] };
        triangles.push_back(F_Shape(triangle));
    }

    return triangles;
}
bool GJK_Collision_Plus(const F_Shape&shape1, const F_Shape& shape2) {
    auto concaveShapes1 = DecomposeConcavePolygon(shape1);
    auto concaveShapes2 = DecomposeConcavePolygon(shape2);
    for (const auto& s1 : concaveShapes1) {
        for (const auto& s2 : concaveShapes2) {
            if (GJK_Collision(s1, s2)) {
                return true;
            }
        }
    }
    return false;  // 所有形状都没有相交
}
