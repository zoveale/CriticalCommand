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

  Shader skyBoxShader;
  Skybox skyBoxOne;

  Shader lamp;

  Model m_Lights;
  Model pointLamp;
  Model spotLight;
 
  Model uvSphere;

  Shader pbrShader;
  
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;

  Framebuffer gFrameBuffer;
  Shader multipleRenderTargetShader;
  
  Framebuffer specularIrradianceBuffer;
  Shader equirectangularToCubemapShader;
  Shader irradianceShader;
  Shader prefilterShader;
  Shader brdfLookUpShader;
  Shader enviromentShader;


  Camera* cameraState;

  //Objects
  Model modelObject[10];
  GameObject testObject;
  IcoSphereGraphicsComponent gObject;
  IcoSpherePhysicsComponent pObject;
  DefaultInputComponent iObject;

  //Player
  Model playerModel;
  GameObject playerObject;
  PlayerGraphicsComponent playerGraphics;
  PlayerPhysicsComponent playerPhysics;
  PlayerInputComponent playerInput;
public:

  System();
  void SystemInit();
  void GameLoop();
  void Shutdown();

};

/*
  Framebuffer geometryBuffer;
  Shader multipleRenderTargetShader;
  Shader deferredLightingPass;*/
  /* Framebuffer pointShadowCastersBuffer[SHADOW_CASTING_POINT_LIGHTS];
  std::vector<glm::mat4> pointShadowMatrix;
  Shader depthShadowCubeShader;
*/
/*Model scene[9];*/

/*Model sceneP;*/

/* Model icoSphereModel;
 IcoSphereGraphicsComponent gComp;
 IcoSpherePhysicsComponent pComp;
 GameObject icoSphereObject;*/
 /*GameObject playerObject;
 Model playerModel;
 PlayerGraphicsComponent playerGraphicsComp;
 PlayerPhysicsComponent playerPhysicsComp;
 PlayerInputComponent playerInputComp;*/
#endif // !SYSTEM_H


