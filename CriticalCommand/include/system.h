#ifndef SYSTEM_H
#define SYSTEM_H

#include "render.h"

class System {
private:
  Render render;

public:
  void SystemInit();
  void GameLoop();
  void Shutdown();
};
#endif // !SYSTEM_H