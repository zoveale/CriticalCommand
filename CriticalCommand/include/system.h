#ifndef SYSTEM_H
#define SYSTEM_H

#include "render.h"
#include "shader.h"
#include "framebuffer.h"
#include "input.h"
#include "player.h"
#include "camera.h"
#include "model.h"
//#include "lightFactory.h"
//#include "physics.h"

//TODO:: finish all component classes
#include "objects.h"
#include "graphics.h"
#include "physicsComponent.h"

#include "gtx/matrix_decompose.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"



class System {
private:
  Player player;
  FirstPerson firstPerson;
  Render render;
  Input input;
  //LightFactory sceneLights;

  //physx::Physics scenePhysics;
  physx::Physics test;
  

public:
  System();
  void SystemInit();
  void GameLoop();
  void Shutdown();

};
#endif // !SYSTEM_H


