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
}


void System::GameLoop(){
  Shader simple("resources/shader/Model/Vmodel.glsl",
                "resources/shader/Model/Fmodel.glsl");

  Shader depthTestShader("resources/shader/Shadow/Depth/vert.glsl",
    "resources/shader/Shadow/Depth/frag.glsl");

  Shader cubemapDepthShader("resources/shader/Shadow/DepthCubemap/vert.glsl",
                            "resources/shader/Shadow/DepthCubemap/frag.glsl",
                            "resources/shader/Shadow/DepthCubemap/geo.glsl");

  std::vector<Model*> bombModels;
  Model bombModelIdel("resources/bomb/bomb.dae", sceneLights, scenePhysics);
  //Model bombModelBig("resources/bomb/bomb1.dae", sceneLights, scenePhysics);

  bombModels.push_back(&bombModelIdel);
  //bombModels.push_back(&bombModelBig);

  Shader bombShader("resources/bomb/shaders/vertex.glsl", "resources/bomb/shaders/fragment.glsl");

  BombGraphicsComponent bombGraphics(bombModels, &simple);
  BombPhysicsComponent bombPhysics(&scenePhysics);

  GameObject bomb(&bombGraphics, &bombPhysics);

  //TODO::HEIGHTMAP LOADing
  Model testShpere("resources/brickSphere/bb.dae");
  ///

  //TODO:: PHYSX testing
  //Model ico_80("resources/default/ico_80.dae", sceneLights, scenePhysics);

  IcoSphereGraphicsComponent icoGraphics(&testShpere, &simple, &sceneLights);
  IcoSpherePhysicsComponent icoPhysics(&scenePhysics);
  //std::vector<GameObject> icoSpheres;
  GameObject icoSphere(&icoGraphics, &icoPhysics);


  Shader normalShader("resources/shader/NormalShader/Vnormal.glsl",
                      "resources/shader/NormalShader/Fnormal.glsl",
                      "resources/shader/NormalShader/Gnormal.glsl");

  //Framebuffer testing
  /*Shader basicFramebufferShader("resources/shader/Framebuffer/Vframebuffer.glsl",
                           "resources/shader/Framebuffer/Fframebuffer.glsl");
  Framebuffer basicFramebuffer(basicFramebufferShader);*/
  ///
  Shader hdrShader("resources/shader/HDR/vert.glsl",
                        "resources/shader/HDR/frag.glsl");
    hdrShader.Use();
    hdrShader.SetInt("hdrBuffer", 0);
  Framebuffer hdrFramebuffer(hdrShader);

  Model default_0("resources/SimpleGround/snowGround.dae", sceneLights, scenePhysics , true);
  Model deadTree0("resources/DeadTree0/deadTrees0.dae");// , sceneLights, scenePhysics, true);
  Model magicStoneCircle("resources/MagicStoneCircle/magicStoneCircle.dae");
  Model largeRock0("resources/LargeRock0/largeRock0.dae");
  Model simpleTorch("resources/SimpleTorch/torch.dae");
  Model lights("resources/SnowMap/physxTestLightsTestTextureTest1.dae", sceneLights, scenePhysics);
  //Lamp models
  Shader lamp("resources/shader/Lamp/lampV.glsl", "resources/shader/Lamp/lampF.glsl");
  Model pointLamp("resources/surface/pointLamp.dae", sceneLights, scenePhysics);
  Model spotLamp("resources/surface/spotLight.dae", sceneLights, scenePhysics);

  Shader skyboxShader("resources/cubemap/shaders/vertex.glsl",
                      "resources/cubemap/shaders/fragment.glsl");
  Skybox skyboxOne(&skyboxShader);

  

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);
  


  simple.Use();
  simple.SetFloat("material.shininess", 32.0f);
  sceneLights.SetFixedAttributes(simple);

  //test values
  float x = 1.0f;
  float gamma = 1.2f;
  int perspective = 55;
  bool testBool = false;
  ///
  // draw as wireframe
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  float deltaRate = 1.0f / 60.0f;
  float deltaTime = 0.0f;
  float currentFrame = 0.0f;
  float lastFrame = (float)glfwGetTime();
  //
  
  //float aspect = (float)1024 / (float)1024;
  //float near = 10.0f;
  //float far = 100.0f;
  //glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), aspect, near, far);
/*
  float near = 1.0f, far = 25.0f;
  glm::mat4 lightProjectionOrtho = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near, far);

  glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f));*/
  //glm::mat4 lightView = glm::lookAt(pointLightPos,
  //pointLightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

  Framebuffer pointLightOne;
  float near_plane = 1.0f, far_plane = 15.0f;//farplane == "radius" of point light
  glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), 1.00f, near_plane, far_plane);
  pointLightOne.CreateDepthCubeMap();
  glm::vec3 pointLightPos = sceneLights.GetPointLightPos(0);
  glm::mat4 shadowTransforms1[6];
  pointLightOne.SetPointLightDepthToCubemap(lightProjection, shadowTransforms1, pointLightPos);
     
  //Framebuffer pointLightTwo;
  ////float near_plane = 1.0f, far_plane = 65.0f;//farplane == "radius" of point light
  ////glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), 1.00f, near_plane, far_plane);
  //pointLightTwo.CreateDepthCubeMap();
  //pointLightPos = sceneLights.GetPointLightPos(1);
  //glm::mat4 shadowTransforms2[6];
  //pointLightTwo.SetPointLightDepthToCubemap(lightProjection, shadowTransforms2, pointLightPos);

  std::vector<GameObject> dynamicObjects;
  dynamicObjects.push_back(icoSphere);
  dynamicObjects.push_back(bomb);
  
  bool swap = true;
  unsigned int index = 0;
  float heightScale = 0.01;

  while (!input.KEY.ESC) {
 
    input.Process();

    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    input.IncrementDecrement(testBool);
    if (testBool)
      scenePhysics.StepPhysics(deltaRate);
       

    player.HandleInput(input, deltaTime);

    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);
    ///

    projection = glm::perspective(glm::radians((float)perspective),
      (float)Render::Screen::WIDTH / (float)Render::Screen::HEIGHT, 0.1f, 1000.0f);
    view = firstPerson.View();

    render.ClearScreen();

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
    ////TODO::multiple light sources and shadows.
    
   
    
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
    hdrFramebuffer.Preprocess();

    render.ClearScreen();
    glViewport(0, 0, (float)Render::Screen::WIDTH, (float)Render::Screen::HEIGHT);
    simple.Use();
    //input.IncrementDecrement(heightScale);
    simple.SetFloat("heightScale", heightScale);
    //printf("heightScale = %f\n", heightScale);
    simple.SetFloat("gamma", gamma);
    simple.SetMat4("PVM", projection* view* model);
    simple.SetFloat("far_plane", far_plane);
    simple.SetVec3("viewPos", player.position);
    simple.SetMat4("model", model);
    simple.SetVec3("lightPos", pointLightPos);
    //SET CUBEMAP SHADOWS
    pointLightOne.SetShadowCubemap(simple);
    //pointLightTwo.SetShadowCubemap(simple);
    ///
    sceneLights.SetDynamicAttributes(simple);

    simpleTorch.Draw(simple);
    magicStoneCircle.Draw(simple);
    largeRock0.Draw(simple);
    deadTree0.Draw(simple);
    default_0.Draw(simple);


   /* normalShader.Use();
    normalShader.SetMat4("projection", projection);
    normalShader.SetMat4("view", view);
    normalShader.SetMat4("model", model);
    simpleTorch.Draw(normalShader);
    magicStoneCircle.Draw(normalShader);
    largeRock0.Draw(normalShader);
    deadTree0.Draw(normalShader);
    default_0.Draw(normalShader);*/


    player.Update(deltaTime);
    glm::mat4 pvMatrix = projection * view;
    for (GameObject dgo: dynamicObjects) {
      dgo.Update(deltaTime, pvMatrix);
      dgo.Draw();
    }
    /*bomb.Update(deltaTime, pvMatrix);
    bomb.Draw();
    icoSphere.Update(deltaTime, pvMatrix);
    icoSphere.Draw();*/

    glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
    glStencilMask(0x01);
    lamp.Use();
    for (unsigned int i = 0; i < sceneLights.NumPointLights(); i++) {
      model = sceneLights.GetPointLightTransformation(i);
      model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
      lamp.SetMat4("PVM", projection * view * model);
      pointLamp.Draw(lamp);
    }
    for (unsigned int i = 0; i < sceneLights.NumSpotLights(); i++) {
      model = sceneLights.GetSpotLightTransformation(i);
      model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
      lamp.SetMat4("PVM", projection * view * model);
      spotLamp.Draw(lamp);
    }
    glStencilMask(0xFF);

    glStencilFunc(GL_NOTEQUAL, 0x01, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    skyboxOne.Draw(view, projection);
    glEnable(GL_DEPTH_TEST);
    glStencilMask(0xFF);

    //framebuffer test
    hdrFramebuffer.Postprocess(hdrShader);
    ///
    
    /* Swap front and back buffers */
    render.Display();

    /* Poll for and process events */
    input.PollEvents();
  }

  //FIXME:: quit function
  glfwWindowShouldClose(render.Window());
  
}

void System::Shutdown() {
  scenePhysics.CleanUp();
  glfwTerminate();
}




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

