# FloatEngine
##### 使用raylib开发的一个C++游戏引擎库
##### 教程:（暂时还没有教程）
##### 示例程序:
```c++
#include <FloatEngine.h>
Game game;
class Obj_Test:public Object{

};
class Room_Test {
public:
      Object* obj_test;
      Room_Test(){
            obj_test = new Obj_Test();
            Create(obj_test);
      }
};
int main(){
      game.CreateWindow(640,480,"Test");
      if(game.CanStart()){
            auto r1 = new Room_Test;
            Room_Goto(r1);
            game.Play(60);
            delete r1;
      }
      game.Destroy();
      return 0;
}
```
