#ifndef SYSTEM_H
#define SYSTEM_H

#include "render.h"
#include "shader.h"
#include "input.h"
#include "player.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

class System {
private:
  Player player;
  Render render;
  Input input;

  void ClearScreen();

public:
  System();
  void SystemInit();
  void GameLoop();
  
  void Shutdown();
};
#endif // !SYSTEM_H


