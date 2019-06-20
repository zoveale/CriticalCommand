#ifndef SYSTEM_H
#define SYSTEM_H

#include "render.h"
#include "shader.h"



#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
class System {
private:
  Render render;
  

public:
  System();
  void SystemInit();
  void GameLoop();
  void Shutdown();
};
#endif // !SYSTEM_H


