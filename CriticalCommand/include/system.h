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
#include "utility.h"

//TODO:: finish all component classes
#include "objects.h"
#include "../include/Components/Graphics/graphicsComponent.h"
#include "../include/Components/Physics/physicsComponent.h"
#include "../include/Components/Input/inputComponent.h"

#include "gtx/matrix_decompose.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"


const unsigned int SHADOW_CASTING_POINT_LIGHTS = 13;
const unsigned int SHADOW_CASTING_SPOT_LIGHTS = 1;

class System {
private:
  Player player;
  Render render;
  Input input;

  Shader skyBox;
  Skybox skyBoxOne;

  Shader lamp;

  Model shadowLights;
  Model pointLamp;
  Model spotLight;

  
  Framebuffer pointShadowCastersBuffer[SHADOW_CASTING_POINT_LIGHTS];
  std::vector<glm::mat4> pointShadowMatrix;
  Shader depthShadowCubeShader;

  Shader pbrShader;
  Model scene[9];
  Model sceneP;

  Model icoSphereModel;
  IcoSphereGraphicsComponent gComp;
  IcoSpherePhysicsComponent pComp;
  GameObject icoSphereObject;

  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;

  Framebuffer gFrameBuffer;
  Shader multipleRenderTargetShader;

  GameObject playerObject;
  Model playerModel;
  PlayerGraphicsComponent playerGraphicsComp;
  PlayerPhysicsComponent playerPhysicsComp;
  PlayerInputComponent playerInputComp;

  Camera* cameraState;
  //const float near_plane = 1.0f, far_plane = 115.0f;
  //glm::mat4 lightProjection;
  //glm::vec3 pointLightPos;
  //glm::mat4 shadowTransforms1[6];

/*
  Framebuffer geometryBuffer;
  Shader multipleRenderTargetShader;
  Shader deferredLightingPass;*/

public:

  System();
  void SystemInit();
  void GameLoop();
  void Shutdown();

};
#endif // !SYSTEM_H


