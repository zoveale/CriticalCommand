#ifndef SYSTEM_H
#define SYSTEM_H

#include "render.h"
#include "shader.h"

class System {
private:
  Render render;
  Shaders dShader;

public:
  System();
  void SystemInit();
  void GameLoop();
  void Shutdown();
};
#endif // !SYSTEM_H