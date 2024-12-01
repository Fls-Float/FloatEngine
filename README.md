# FloatEngine v1.3.4

##### A C++ game engine built using raylib.

##### Tutorial: 
No tutorial now.
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
