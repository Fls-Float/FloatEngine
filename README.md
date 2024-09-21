# FloatEngine v1.3.4
##### 使用raylib开发的一个C++游戏引擎库
##### 教程:（暂时还没有教程）
##### 示例程序:
```c++
#include <FloatEngine.h>
Game game;
int main(){
      game.CreateWindow(640,480,"Test");
      if(game.CanStart()){
            game.Play(60);
      }
      game.Destroy();
      return 0;
}
```
