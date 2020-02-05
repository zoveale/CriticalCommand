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

  //light stuff
  lights.LoadLights("resources/pbrTesting/lights/lights.dae", sceneLights);
  lamp.Load("resources/shader/Lamp/lampV.glsl",
    "resources/shader/Lamp/lampF.glsl");
  pointLamp.LoadModel("resources/surface/pointLamp.dae");
  ///

  //SceneStuff
  scene.LoadModel("resources/pbrTesting/models/pbr_scene.dae");
  pbrShader.Load("resources/shader/PBR/vert.glsl", "resources/shader/PBR/frag.glsl");
  ///
  model = glm::mat4(1.0f);
  view = glm::mat4(1.0f);
  projection = glm::mat4(1.0f);

 
  //sceneLights.SetFixedAttributes( SHADER STUFF );

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
  pbrShader.Use();
  pbrShader.SetVec3("albedo", 0.5f, 0.0f, 0.0f);
  pbrShader.SetFloat("ao", 1.0f);

  float metallic = 0.05;
  float roughness = 0.25;

  while (!input.KEY.ESC) {
 
    input.Process();

    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    //input.IncrementDecrement(testBool);
    //if (testBool)
    //scenePhysics.StepPhysics(deltaRate);
       

    player.HandleInput(input, deltaTime);

    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);
    ///

    player.Update(deltaTime);

    render.ClearScreen();

    projection = glm::perspective(glm::radians((float)perspective),
      (float)Render::Screen::WIDTH / (float)Render::Screen::HEIGHT, 0.1f, 1000.0f);
    view = firstPerson.View();

    glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
    glStencilMask(0xFF);
    pbrShader.Use();
    pbrShader.SetMat4("projection", projection);
    pbrShader.SetMat4("model", model);
    pbrShader.SetMat4("view", view);
    pbrShader.SetVec3("camPos", player.position);
    for (unsigned int i = 0; i < sceneLights.NumPointLights(); i++) {
      pbrShader.SetVec3("lightPositions[" + std::to_string(i) + "]", sceneLights.GetPointLightPos(i));
      pbrShader.SetVec3("lightColors[" + std::to_string(i) + "]", glm::vec3(300.0f, 300.0f, 300.0f));
    }
    scene.Draw(pbrShader);

    
    glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
    glStencilMask(0xFF);
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
 /* for (auto a : bombModels)
    delete a;
  bombModels.clear();*/

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

