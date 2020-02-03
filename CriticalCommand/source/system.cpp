#include "system.h"

static LightFactory sceneLights;
static physx::Physics scenePhysics;

System::System() {

}

void System::SystemInit(){
  render.StartUp();
  input.StartUp(render.Window());
  player.StartUp();
  // \ or vise versa ?
  //camera.startup /
  physx::Physics::StartUp();
  scenePhysics.TestA();
  printf("OpenGl version: %s\n", glGetString(GL_VERSION));

  simple.Load("resources/shader/Model/Vmodel.glsl", 
    "resources/shader/Model/Fmodel.glsl");
  depthTestShader.Load("resources/shader/Shadow/Depth/vert.glsl", 
    "resources/shader/Shadow/Depth/frag.glsl");
  cubemapDepthShader.Load("resources/shader/Shadow/DepthCubemap/vert.glsl",
    "resources/shader/Shadow/DepthCubemap/frag.glsl",
    "resources/shader/Shadow/DepthCubemap/geo.glsl");
  bombShader.Load("resources/bomb/shaders/vertex.glsl", "resources/bomb/shaders/fragment.glsl");
  normalShader.Load("resources/shader/NormalShader/Vnormal.glsl",
    "resources/shader/NormalShader/Fnormal.glsl",
    "resources/shader/NormalShader/Gnormal.glsl");
  hdrShader.Load("resources/shader/HDR/vert.glsl",
    "resources/shader/HDR/frag.glsl");
  lamp.Load("resources/shader/Lamp/lampV.glsl", 
    "resources/shader/Lamp/lampF.glsl");
  skyboxShader.Load("resources/cubemap/shaders/vertex.glsl",
    "resources/cubemap/shaders/fragment.glsl");

  bombModels.push_back(new Model("resources/bomb/bomb.dae", sceneLights, scenePhysics));
  //TODO::HEIGHTMAP LOADing
  testShpere.Load("resources/brickSphere/bb.dae");
  //default_0.Load("resources/SimpleGround/snowGround.dae", sceneLights, scenePhysics, true);
  deadTree0.Load("resources/DeadTree0/deadTrees0.dae");// , sceneLights, scenePhysics, true);
  magicStoneCircle.Load("resources/MagicStoneCircle/magicStoneCircle.dae");
  largeRock0.Load("resources/LargeRock0/largeRock0.dae");
  simpleTorch.Load("resources/SimpleTorch/torch.dae");
  lights.Load("resources/SnowMap/physxTestLightsTestTextureTest1.dae", sceneLights, scenePhysics);
  pointLamp.Load("resources/surface/pointLamp.dae", sceneLights, scenePhysics);
  spotLamp.Load("resources/surface/spotLight.dae", sceneLights, scenePhysics);
  testDefferedRenderSpheres.Load("resources/default/SceneInProgressDefferedRenderTest2.dae", sceneLights, scenePhysics, true);

  hdrFramebuffer.Load(hdrShader);
  //lightProjection = glm::perspective(glm::radians(90.0f), 1.00f, near_plane, far_plane);
  //pointLightOne.CreateDepthCubeMap();
  //pointLightPos = sceneLights.GetPointLightPos(0);
  //pointLightOne.SetPointLightDepthToCubemap(lightProjection, shadowTransforms1, pointLightPos);

  skyboxOne.Load(&skyboxShader);

  model = glm::mat4(1.0f);
  view = glm::mat4(1.0f);
  projection = glm::mat4(1.0f);

  simple.Use();
  simple.SetFloat("material.shininess", 16.0f);
  //sceneLights.SetFixedAttributes(simple);

  //gBuffer stuff
  geometryBuffer.LoadGeometryBuffer();
  multipleRenderTargetShader.Load("resources/shader/Gbuffer/vert.glsl",
                                  "resources/shader/Gbuffer/frag.glsl");
  deferredLightingPass.Load("resources/shader/DeferredShading/vert.glsl",
                            "resources/shader/DeferredShading/frag.glsl");

  deferredLightingPass.Use();
  deferredLightingPass.SetInt("gPosition", 0);
  deferredLightingPass.SetInt("gNormal", 1);
  deferredLightingPass.SetInt("gAlbedoSpec", 2);
  deferredLightingPass.SetVec3("viewPos", player.position);
  sceneLights.SetFixedAttributes(deferredLightingPass);

}


void System::GameLoop(){
  

  //test values
  float x = 1.0f;
  float gamma = 1.2f;
  int perspective = 55;
  bool testBool = false;

  float deltaRate = 1.0f / 60.0f;
  float deltaTime = 0.0f;
  float currentFrame = 0.0f;
  float lastFrame = (float)glfwGetTime();
 
  
  unsigned int index = 0;
  float heightScale = 0.01;

  while (!input.KEY.ESC) {
 
    input.Process();

    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    input.IncrementDecrement(testBool);
    //if (testBool)
    scenePhysics.StepPhysics(deltaRate);
       

    player.HandleInput(input, deltaTime);

    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);
    ///

    player.Update(deltaTime);

    projection = glm::perspective(glm::radians((float)perspective),
      (float)Render::Screen::WIDTH / (float)Render::Screen::HEIGHT, 0.1f, 1000.0f);
    view = firstPerson.View();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    geometryBuffer.BindGeometryBuffer();
    glViewport(0, 0, (float)Render::Screen::WIDTH, (float)Render::Screen::HEIGHT);

    
    render.ClearScreen();
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    multipleRenderTargetShader.Use();
    multipleRenderTargetShader.SetMat4("projection", projection);
    multipleRenderTargetShader.SetMat4("view", view);
    multipleRenderTargetShader.SetMat4("model", model);
    multipleRenderTargetShader.SetMat4("inverseModel", glm::inverse(model));

    testDefferedRenderSpheres.Draw(multipleRenderTargetShader);
    /*simpleTorch.Draw(multipleRenderTargetShader);
    magicStoneCircle.Draw(multipleRenderTargetShader);
    largeRock0.Draw(multipleRenderTargetShader);
    deadTree0.Draw(multipleRenderTargetShader);
    default_0.Draw(multipleRenderTargetShader);*/
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //hdrFramebuffer.Preprocess();
    deferredLightingPass.Use();
    deferredLightingPass.SetVec3("viewPos", player.position);
    geometryBuffer.SetDeferredShading(deferredLightingPass);
    //hdrFramebuffer.Postprocess(hdrShader);
    glStencilMask(0xFF);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //framebuffer test
    ///
    glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
    glStencilMask(0x01);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, geometryBuffer.GetGeometryBufferFBO());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, (float)Render::Screen::WIDTH, (float)Render::Screen::HEIGHT,
                      0, 0, (float)Render::Screen::WIDTH, (float)Render::Screen::HEIGHT,
                                                        GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    lamp.Use();
    //sceneLights.NumPointLights()
    for (unsigned int i = 0; i < sceneLights.NumPointLights(); i++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, sceneLights.GetPointLightPos(i));
      model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
      lamp.SetVec3("lampColor", sceneLights.GetPointLightColor(i) * 10.0f);
      lamp.SetMat4("PVM", projection * view * model);
      pointLamp.Draw(lamp);
    }
    glStencilMask(0xFF);

    /* Swap front and back buffers */
    render.Display();

    /* Poll for and process events */
    input.PollEvents();
  }

  //FIXME:: quit function
  glfwWindowShouldClose(render.Window());
  
}

void System::Shutdown() {
  for (auto a : bombModels)
    delete a;
  bombModels.clear();

  scenePhysics.CleanUp();
  glfwTerminate();
}


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

