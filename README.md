# Float Engine v1.4.1

##### A C++ game engine built using raylib.

##### Tutorial: 
https://fls-float.github.io/floatengine-doc
##### Example:

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
