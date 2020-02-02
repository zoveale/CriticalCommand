#ifndef SYSTEM_H
#define SYSTEM_H

#include "render.h"
#include "shader.h"
#include "framebuffer.h"
#include "input.h"
#include "player.h"
#include "camera.h"
#include "model.h"
#include "skybox.h"
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

  Shader simple;
  Shader depthTestShader;
  Shader cubemapDepthShader;
  Shader bombShader;
  Shader normalShader;
  Shader hdrShader;
  Shader lamp;
  Shader skyboxShader;

  std::vector<Model*> bombModels;
  Model testShpere;
  Model default_0;
  Model deadTree0;
  Model magicStoneCircle;
  Model largeRock0;
  Model simpleTorch;
  Model lights;
  Model pointLamp;
  Model spotLamp;
  Model testDefferedRenderSpheres;


  Framebuffer hdrFramebuffer;
  //Framebuffer pointLightOne;
  

  Skybox skyboxOne;

  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;

  const float near_plane = 1.0f, far_plane = 115.0f;
  glm::mat4 lightProjection;
  glm::vec3 pointLightPos;
  glm::mat4 shadowTransforms1[6];

  Framebuffer geometryBuffer;
  Shader multipleRenderTargetShader;
  Shader deferredLightingPass;

public:
  System();
  void SystemInit();
  void GameLoop();
  void Shutdown();

};
#endif // !SYSTEM_H


