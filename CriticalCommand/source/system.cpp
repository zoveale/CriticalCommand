#include "system.h"

static LightFactory sceneLights;
static physx::Physics scenePhysics;

static LightFactory sceneShadowLights;

System::System() {
  model = glm::mat4(1.0f);
  view = glm::mat4(1.0f);
  projection = glm::mat4(1.0f);
}

void System::SystemInit(){
  render.StartUp();
  input.StartUp(render.Window());
  player.StartUp();
  physx::Physics::StartUp();
  scenePhysics.TestA();
  // \ or vise versa ?
  //camera.startup /
  
  //printf("OpenGl version: %s\n", glGetString(GL_VERSION));

  
  //light stuff
  shadowLights.LoadLights("resources/pbrTesting/scene/lights/shadowLights.dae", sceneShadowLights);
  shadowLights.LoadLights("resources/pbrTesting/scene/lights/lights.dae", sceneLights);
  lamp.Load("resources/shader/Lamp/lampV.glsl", "resources/shader/Lamp/lampF.glsl");
  pointLamp.LoadModel("resources/surface/pointLamp.dae");
  spotLight.LoadModel("resources/surface/spotLight.dae");
  ///

  //SceneStuff
  sceneP.Load("resources/pbrTesting/scene/floor/physicfloor.dae", sceneLights, scenePhysics, true);
  scene[0].LoadModel("resources/pbrTesting/scene/floor/floor.dae");
  scene[1].LoadModel("resources/pbrTesting/scene/barrel/barrel.dae");
  scene[2].LoadModel("resources/pbrTesting/scene/brasier/brasier.dae");
  scene[3].LoadModel("resources/pbrTesting/scene/doors/doors.dae");
  scene[4].LoadModel("resources/pbrTesting/scene/pillars/pillars.dae");
  scene[5].LoadModel("resources/pbrTesting/scene/torch/torch.dae");
  scene[6].LoadModel("resources/pbrTesting/scene/wall1/wall1.dae"); 
  scene[7].LoadModel("resources/pbrTesting/scene/wall2/wall2.dae");
  scene[8].LoadModel("resources/pbrTesting/scene/wall3/wall3.dae");
  //scene.LoadModel("resources/pbrTesting/models/sword/sword.dae");
  
  ///
  //skybox
  skyBox.Load("resources/cubemap/shaders/vertex.glsl", "resources/cubemap/shaders/fragment.glsl");
  skyBoxOne.Load(&skyBox);

  //gBuffer
  multipleRenderTargetShader.Load("resources/shader/gBuffer/vert.glsl" , "resources/shader/gBuffer/frag.glsl");
  gFrameBuffer.LoadGeometryBuffer();
  ///

  //Objects
  icoSphereModel.LoadModel("resources/pbrTesting/models/icoSphere/ico.dae");
  gComp.Load(&icoSphereModel, &multipleRenderTargetShader);
  pComp.Load(&scenePhysics);
  icoSphereObject.Load(&gComp, &pComp);
  ///
  //player object
  /*
  Model playerModel;
  PlayerGraphicsComponent playerGraphicsComp;
  PlayerPhysicsComponent playerPhysicsComp;
  PlayerInputComponent playerInputComp;
  */
  playerModel.LoadModel("resources/pbrTesting/models/icoSphere/ico.dae");
  playerGraphicsComp.Load(&icoSphereModel, &multipleRenderTargetShader);
  playerPhysicsComp.Load(&scenePhysics);
  playerInputComp.Load(); //temp function
  playerObject.Load(&playerGraphicsComp, &playerPhysicsComp, &playerInputComp);
  ///

  model = glm::mat4(1.0f); 
  view = glm::mat4(1.0f);
  projection = glm::mat4(1.0f);


  pbrShader.Load("resources/shader/PBR/vert.glsl", "resources/shader/PBR/frag.glsl");
  sceneShadowLights.SetFixedShadowAttributes(pbrShader);
  sceneLights.SetFixedAttributes(pbrShader);

  pbrShader.Use();
  pbrShader.SetVec3("camPos", player.position); 
   
  //Shadow Stuff
  depthShadowCubeShader.Load("resources/shader/Shadow/DepthCubemap/vert.glsl",
                             "resources/shader/Shadow/DepthCubemap/frag.glsl",
                             "resources/shader/Shadow/DepthCubemap/geo.glsl");

  float near_plane = 0.0010f, far_plane = 55.0f;//farplane == "radius" of point light
  for (unsigned int i = 0; i < SHADOW_CASTING_POINT_LIGHTS; ++i) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), 1.00f, near_plane, far_plane);
    pointShadowCastersBuffer[i].CreateDepthCubeMap();
    glm::vec3 pointLightPos = sceneLights.GetPointLightPos(i);
    glm::mat4 shadowTransforms[6];
    pointShadowCastersBuffer[i].SetPointLightDepthToCubemap(lightProjection, shadowTransforms, pointLightPos);
    for(unsigned int j = 0; j < 6; ++j)
      pointShadowMatrix.push_back(shadowTransforms[j]);
  }


  ///
}


void System::GameLoop(){
  //test values
  float x = 1.0f;
  float gamma = 1.2f;
  int perspective = 45;
  bool testBool = false;

  float deltaRate = 1.0f / 60.0f;
  float deltaTime = 0.0f;
  float currentFrame = 0.0f;
  float lastFrame = (float)glfwGetTime();
 
  //projection = glm::mat4(1.0f);
  projection = glm::perspective(glm::radians((float)perspective),
    (float)Render::Screen::WIDTH / (float)Render::Screen::HEIGHT, 0.1f, 1000.0f);
  model = glm::mat4(1.0f);
  view = glm::mat4(1.0f);
  ///

  float near_plane = 0.0010f, far_plane = 55.0f;

  glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
  glStencilMask(0xFF);
  for (unsigned int i = 0; i < SHADOW_CASTING_POINT_LIGHTS; ++i) {
    pointShadowCastersBuffer[i].DepthMapViewPort();
    glBindFramebuffer(GL_FRAMEBUFFER, pointShadowCastersBuffer[i].GetDepthMapFBO());
    glClear(GL_DEPTH_BUFFER_BIT);
    depthShadowCubeShader.Use();
    depthShadowCubeShader.SetFloat("far_plane", far_plane);
    depthShadowCubeShader.SetVec3("lightPos", sceneShadowLights.GetPointLightPos(i));
    //pointLightOne.SetPointLightDepthToCubemap(lightProjection, shadowTransforms1, pointLightPos);
    for (unsigned int j = 0; j < 6; ++j)
      depthShadowCubeShader.SetMat4("shadowTransforms[" + std::to_string(j) + "]", pointShadowMatrix[i * 6 + j]);

    depthShadowCubeShader.SetMat4("model", glm::mat4(1.0f));
    for (unsigned int j = 0; j < 9; ++j)
      scene[j].DrawModelOnly(depthShadowCubeShader);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  while (!input.KEY.ESC) {
    input.PollEvents();

    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    //input.IncrementDecrement(testBool);
    //if (testBool)
    scenePhysics.StepPhysics(deltaRate * 0.1f);


    player.HandleInput(input, deltaTime);

    
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    view = firstPerson.View();
    ///

    render.ClearScreen();

   
    player.Update(deltaTime);
    icoSphereObject.Update(deltaTime, projection * view);
    playerObject.Update(deltaTime, projection * view);
   

    gFrameBuffer.BindGeometryBuffer();
    glViewport(0, 0, (GLsizei)Render::Screen::WIDTH, (GLsizei)Render::Screen::HEIGHT);
    render.ClearScreen();
    glDisable(GL_STENCIL_TEST);
    
    multipleRenderTargetShader.Use();
    multipleRenderTargetShader.SetMat4("projection", projection);
    multipleRenderTargetShader.SetMat4("view", view);
    multipleRenderTargetShader.SetMat4("model", model);
    multipleRenderTargetShader.SetMat4("inverseModel", glm::transpose(glm::inverse(model)));
    for (unsigned int i = 0; i < 9; ++i) {
      scene[i].Draw(multipleRenderTargetShader);
    }
    icoSphereObject.Draw();
    playerObject.Draw();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    pbrShader.Use();
    gFrameBuffer.SetDeferredShading(pbrShader);
    for (unsigned int i = 0; i < SHADOW_CASTING_POINT_LIGHTS; ++i)
      pointShadowCastersBuffer[i].SetShadowCubemap(pbrShader);
    glStencilMask(0xFF);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glBindFramebuffer(GL_READ_FRAMEBUFFER, gFrameBuffer.GetGeometryBufferFBO());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, (GLint)Render::Screen::WIDTH, (GLint)Render::Screen::HEIGHT,
      0, 0, (GLint)Render::Screen::WIDTH, (GLint)Render::Screen::HEIGHT,
      GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    

    glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
    glStencilMask(0xFF);
    lamp.Use();
    for (unsigned int i = 0; i < sceneShadowLights.NumPointLights(); i++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, sceneShadowLights.GetPointLightPos(i));
      model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
      lamp.SetVec3("lampColor", sceneShadowLights.GetPointLightColor(i) * 0.010f);
      lamp.SetMat4("PVM", projection * view * model);
      pointLamp.DrawModelOnly(lamp);
    }
    for (unsigned int i = 0; i < sceneShadowLights.NumSpotLights(); i++) {
      model = sceneShadowLights.GetSpotLightTransformation(i);
      model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
      lamp.SetVec3("lampColor", sceneShadowLights.GetSpotLightColor(i) * 0.010f);
      lamp.SetMat4("PVM", projection * view * model);
      spotLight.DrawModelOnly(lamp);
    }
    //skyBoxOne.Draw(view, projection);
    glStencilMask(0xFF);

    /* Swap front and back buffers */
    render.Display();

    /* Poll for and process events */
    input.Process();
  }

  //FIXME:: quit function
  glfwWindowShouldClose(render.Window());
  
}

void System::Shutdown() {
 /* for (auto a : bombModels)
    delete a;
  bombModels.clear();*/

  scenePhysics.CleanUp();
  glfwTerminate();
}


//pbrShader.Use();
//pbrShader.SetMat4("projection", projection);
////model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.5f));
//pbrShader.SetMat4("model", model);
//pbrShader.SetMat4("view", view);
//pbrShader.SetVec3("camPos", player.position);
//pbrShader.SetFloat("radius", 10.0f);// +rand() % 3 + 0.1);
//for (unsigned int i = 0; i < sceneLights.NumPointLights(); i++) {
//  pbrShader.SetVec3("lightPositions[" + std::to_string(i) + "]", sceneLights.GetPointLightPos(i));
//  pbrShader.SetVec3("lightColors[" + std::to_string(i) + "]", sceneLights.GetPointLightColor(i));
//}
//for (unsigned int i = 0; i < 9; ++i) {
//  scene[i].Draw(pbrShader);
//}

/*render.ClearScreen();
pointLightOne.DepthMapViewPort();
glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
glStencilMask(0xFF);
glBindFramebuffer(GL_FRAMEBUFFER, pointLightOne.GetDepthMapFBO());
glClear(GL_DEPTH_BUFFER_BIT);
cubemapDepthShader.Use();
cubemapDepthShader.SetFloat("far_plane", far_plane);
cubemapDepthShader.SetVec3("lightPos", pointLightPos);
pointLightOne.SetPointLightDepthToCubemap(lightProjection, shadowTransforms1, pointLightPos);
for (unsigned int i = 0; i < 6; ++i)
  cubemapDepthShader.SetMat4("shadowTransforms[" + std::to_string(i) + "]", shadowTransforms1[i]);
cubemapDepthShader.SetMat4("model", glm::mat4(1.0f));
magicStoneCircle.DepthDraw(cubemapDepthShader);

simpleTorch.DepthDraw(cubemapDepthShader);
largeRock0.DepthDraw(cubemapDepthShader);
deadTree0.DepthDraw(cubemapDepthShader);
default_0.DepthDraw(cubemapDepthShader);
glBindFramebuffer(GL_FRAMEBUFFER, 0);*/


//hdrFramebuffer.Preprocess();
//geometryBuffer.BindGeometryBuffer();
//glStencilFunc(GL_ALWAYS, 1, 0xFF);
//glStencilMask(0xFF);
//render.ClearScreen();
//glViewport(0, 0, (float)Render::Screen::WIDTH, (float)Render::Screen::HEIGHT);
//simple.Use();
////input.IncrementDecrement(heightScale);
//simple.SetFloat("heightScale", heightScale);
////printf("heightScale = %f\n", heightScale);
//simple.SetFloat("gamma", gamma);
//simple.SetMat4("PVM", projection* view* model);
//simple.SetFloat("far_plane", far_plane);
//simple.SetVec3("viewPos", player.position);
//simple.SetMat4("model", model);
//simple.SetVec3("lightPos", pointLightPos);
////SET CUBEMAP SHADOWS
//pointLightOne.SetShadowCubemap(simple);
////pointLightTwo.SetShadowCubemap(simple);
/////
//sceneLights.SetDynamicAttributes(simple);
//simpleTorch.Draw(simple);
//magicStoneCircle.Draw(simple);
//largeRock0.Draw(simple);
//deadTree0.Draw(simple);
//default_0.Draw(simple);

/*glm::mat4 pvMatrix = projection * view;
    for (GameObject dgo: dynamicObjects) {
      dgo.Update(deltaTime, pvMatrix);
      dgo.Draw();
    }*/
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    /*bomb.Update(deltaTime, pvMatrix);
    bomb.Draw();
    icoSphere.Update(deltaTime, pvMatrix);
    icoSphere.Draw();*/
    ///
   /* glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
    glStencilMask(0x01);
    lamp.Use();
    for (unsigned int i = 0; i < sceneLights.NumPointLights(); i++) {
      model = sceneLights.GetPointLightTransformation(i);
      model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
      lamp.SetVec3("lampColor", sceneLights.GetPointLightColor(i));
      lamp.SetMat4("PVM", projection * view * model);
      pointLamp.Draw(lamp);
    }
    for (unsigned int i = 0; i < sceneLights.NumSpotLights(); i++) {
      model = sceneLights.GetSpotLightTransformation(i);
      model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
      lamp.SetVec3("lampColor", sceneLights.GetSpotLightColor(i));
      lamp.SetMat4("PVM", projection * view * model);
      spotLamp.Draw(lamp);
    }
    glStencilMask(0xFF);*/
    ///
    /*glStencilFunc(GL_NOTEQUAL, 0x01, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    skyboxOne.Draw(view, projection);
    glEnable(GL_DEPTH_TEST);
    glStencilMask(0xFF);*/
    ///

/*
    normalShader.Use();
    normalShader.SetMat4("projection", projection);
    normalShader.SetMat4("view", view);
    normalShader.SetMat4("model", model);
    simpleTorch.Draw(normalShader);
    magicStoneCircle.Draw(normalShader);
    largeRock0.Draw(normalShader);
    deadTree0.Draw(normalShader);
    default_0.Draw(normalShader);*/


//glBindFramebuffer(GL_FRAMEBUFFER, pointLightTwo.GetDepthMapFBO());
//glClear(GL_DEPTH_BUFFER_BIT);
//cubemapDepthShader.Use();
//cubemapDepthShader.SetFloat("far_plane", far_plane);
//pointLightPos = sceneLights.GetPointLightPos(1);
//cubemapDepthShader.SetVec3("lightPos", pointLightPos);
//pointLightTwo.SetPointLightDepthToCubemap(lightProjection, shadowTransforms2, pointLightPos);
//for (unsigned int i = 0; i < 6; ++i)
//  cubemapDepthShader.SetMat4("shadowTransforms[" + std::to_string(i) + "]", shadowTransforms2[i]);
//cubemapDepthShader.SetMat4("model", glm::mat4(1.0f));
//default_0.Draw(cubemapDepthShader);
////TODO::multiple light sources and shadows.
//glBindFramebuffer(GL_FRAMEBUFFER, 1);
//


//render.ClearScreen(); 
//glViewport(0, 0, (float)Render::Screen::WIDTH, (float)Render::Screen::HEIGHT);
//glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
//glStencilMask(0xFF);
////input.IncrementDecrement(gamma);
//simple.Use();
//simple.SetFloat("gamma", gamma);
//simple.SetMat4("PVM", projection* view* model);
//simple.SetFloat("far_plane", far_plane);
//simple.SetVec3("viewPos", player.position);
//simple.SetMat4("model", model);
//
//sceneLights.SetDynamicAttributes(simple);
////simple.SetInt("pointLightNum", 0);
//default_0.Draw(simple);

//basicFramebuffer.DepthMapViewPort();
//TODO::animation stuff
//animated.Use();
    //animated.SetMat4("projection", projection);
    //animated.SetMat4("view", view);
    //model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
    ////model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	
    //animated.SetMat4("model", model);
    //ourModel_0.Animate(animated, currentFrame);
    //normalShader.Use();
    //normalShader.SetMat4("projection", projection);
    //normalShader.SetMat4("view", view);
    //normalShader.SetMat4("model", model);
    //ourModel_0.Draw(normalShader);
    //model = glm::mat4(1.0f);
//TODO:: setting ico80 models the physics deformations
    //for (int i = 0; i < 55; i++) {
    //  glDepthMask(GL_TRUE);
    //  
    //  glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
    //  glStencilMask(0xFF);
    //  simple.Use();
    //  model = glm::mat4(1.0f);
    //  model = scenePhysics.GetAPose(i); 
    //  model = glm::scale(model, glm::vec3(2.0));
    //  simple.SetMat4("model", model);
    //  simple.SetMat4("PVM", projection * view * model);
    //  sceneLights.SetDynamicAttributes(simple);
    //  ico_80.Draw(simple);
    //  
    //  glDepthMask(GL_FALSE);
    //  glStencilFunc(GL_GREATER, 0x01, 0xFF);
    //  glStencilMask(0x00);
    //  stencilShader.Use();
    //  model = glm::scale(model, glm::vec3(1.1f));
    //  stencilShader.SetMat4("model", model);
    //  stencilShader.SetMat4("PVM", projection* view* model);
    //  ico_80.Draw(stencilShader);
    //  /*normalShader.Use();
    //  normalShader.SetMat4("projecion", projection);
    //  normalShader.SetMat4("view", view);
    //  normalShader.SetMat4("model", model);
    //  ico_80.Draw(normalShader);*/
    //}
    //glStencilMask(0xFF);
    //cool acceleration effect
    //player.Update(xpos, ypos); 

//Framebuffer pointLightOne;
//float near_plane = 1.0f, far_plane = 115.0f;//farplane == "radius" of point light
//glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), 1.00f, near_plane, far_plane);
//pointLightOne.CreateDepthCubeMap();
//glm::vec3 pointLightPos = sceneLights.GetPointLightPos(0);
//glm::mat4 shadowTransforms1[6];
//pointLightOne.SetPointLightDepthToCubemap(lightProjection, shadowTransforms1, pointLightPos);
//pointLightOne.DepthMapViewPort();
//glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
//glStencilMask(0xFF);
//glBindFramebuffer(GL_FRAMEBUFFER, pointLightOne.GetDepthMapFBO());
//glClear(GL_DEPTH_BUFFER_BIT);
//cubemapDepthShader.Use();
//cubemapDepthShader.SetFloat("far_plane", far_plane);
//cubemapDepthShader.SetVec3("lightPos", pointLightPos);
//pointLightOne.SetPointLightDepthToCubemap(lightProjection, shadowTransforms1, pointLightPos);
//for (unsigned int i = 0; i < 6; ++i)
//  cubemapDepthShader.SetMat4("shadowTransforms[" + std::to_string(i) + "]", shadowTransforms1[i]);
//cubemapDepthShader.SetMat4("model", glm::mat4(1.0f));
//magicStoneCircle.DepthDraw(cubemapDepthShader);