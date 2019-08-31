#ifndef SYSTEM_H
#define SYSTEM_H

#include "render.h"
#include "shader.h"
#include "input.h"
#include "player.h"
#include "camera.h"
#include "model.h"
#include "lightFactory.h"
#include "physics.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"


class System {
private:
  Player player;
  PlayerCamera playerCamera;
  Render render;
  Input input;
  LightFactory sceneLights;

  physx::Physics scenePhysics;

  void ClearScreen();

public:
  System();
  void SystemInit();
  void GameLoop();
  
  void Shutdown();
};
#endif // !SYSTEM_H


