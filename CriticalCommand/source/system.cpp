#include "system.h"

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
  //Shader animated("resources/shader/Vanimated.glsl", "resources/shader/Fanimated.glsl");
  //Model ourModel_0("resources/cowboy/CharacterRunning4.dae", sceneLights,scenePhysics);
  Shader normalShader("resources/shader/NormalShader/Vnormal.glsl",
                      "resources/shader/NormalShader/Fnormal.glsl",
                      "resources/shader/NormalShader/Gnormal.glsl");

  Shader framebufferShader("resources/shader/Framebuffer/Vframebuffer.glsl",
                           "resources/shader/Framebuffer/Fframebuffer.glsl");
  Framebuffer framebuffertest(framebufferShader);
  Model framebufferModel("resources/default/framebuffertest.dae", sceneLights, scenePhysics);

  Shader cubeMapShader("resources/shader/CubeMap/Vcubemap.glsl",
                       "resources/shader/CubeMap/Fcubemap.glsl",
                       "resources/shader/CubeMap/Gcubemap.glsl");
  Model cubemap("resources/default/cubemapbox.dae", sceneLights, scenePhysics);
  //Model ourModel_1("resources/watchtower/tower.obj", sceneLights, scenePhysics);
  Shader fixed("resources/shader/Model/Vmodel.glsl",
               "resources/shader/Model/Fmodel.glsl");
  //TODO:: PHYSX testing
  Model ico_80("resources/default/ico_80.dae", sceneLights, scenePhysics);
  //Model ico_80_Big("resources/default/ico_80.dae", sceneLights, scenePhysics);
  
  ///
  //TODO:: warning : TriangleMesh: triangles are too big, 
  //reduce their size to increase simulation stability! wtf is this
  Model default_0("resources/default/default7.dae", sceneLights, scenePhysics , true);
  
  //Lamp models
  Shader lamp("resources/shader/lampV.glsl", "resources/shader/lampF.glsl");
  Model pointLamp("resources/surface/pointLamp.dae", sceneLights, scenePhysics);
  Model spotLamp("resources/surface/spotLight.dae", sceneLights, scenePhysics);

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  
  fixed.Use();
  fixed.SetFloat("material.shininess", 32.0f);
  sceneLights.SetFixedAttributes(fixed);

  //test values
  float x = 1.0;
  float gamma = 1.2;
  int perspective = 55;
  bool testBool = true;
  ///
  // draw as wireframe
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  float deltaRate = 1.0f / 60.0f;
  float deltaTime = 0.0f;
  float currentFrame = 0.0f;
  float lastFrame = (float)glfwGetTime();
  while (!input.KEY.ESC) {

    input.Process();
    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    printf("deltaTime = %f\n", deltaTime);

    //framebuffer test
    framebuffertest.Preprocess();
    ///

    /* Render here */
    //clear screen and color background
    render.ClearScreen();
    player.HandleInput(input, deltaTime);
    //cool acceleration effect
    //player.Update(xpos, ypos); 
    ///
    //set to identity matrix
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);
    ///

    //TODO::TEST FUNCTIONS
    //input.IncrementDecrement(perspective);
    projection = glm::perspective(glm::radians((float)perspective), (float)1280 / (float)720, 0.1f, 1000.0f);
    view = playerCamera.View();

   /* animated.Use();
    animated.SetMat4("projection", projection);
    animated.SetMat4("view", view);
    model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	
    animated.SetMat4("PVM", projection * view * model);
    animated.SetMat4("model", model);
    ourModel_0.Animate(animated, currentFrame);*/

   
    //TODO::TEST FUNCTIONS

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0x00);
    //input.IncrementDecrement(gamma);
    fixed.Use();
    fixed.SetFloat("gamma", gamma);
    fixed.SetVec3("viewPos", player.position);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    fixed.SetMat4("model", model);
    fixed.SetMat4("PVM", projection * view * model);
    sceneLights.SetDynamicAttributes(fixed);
    default_0.Draw(fixed);

    glDepthMask(GL_FALSE);
    normalShader.Use();
    normalShader.SetMat4("projection", projection);
    normalShader.SetMat4("view", view);
    normalShader.SetMat4("model", model);
    default_0.Draw(normalShader);
    glDepthMask(GL_TRUE);
    
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    //TODO:: setting ico80 models the physics deformations
    for (int i = 0; i < 55; i++) {
      fixed.Use();
      model = glm::mat4(1.0f);
      model = scenePhysics.GetAPose(i); 
      model = glm::scale(model, glm::vec3(2.0));
      fixed.SetMat4("model", model);
      fixed.SetMat4("PVM", projection * view * model);
      sceneLights.SetDynamicAttributes(fixed);
      ico_80.Draw(fixed);

      /*normalShader.Use();
      normalShader.SetMat4("projecion", projection);
      normalShader.SetMat4("view", view);
      normalShader.SetMat4("model", model);
      ico_80.Draw(normalShader);*/
    }
    
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    for (int i = 0; i < 55; i++) {
      lamp.Use();
      model = glm::mat4(1.0f);
      model = scenePhysics.GetAPose(i);
      model = glm::scale(model, glm::vec3(2.2));
      lamp.SetMat4("model", model);
      lamp.SetMat4("PVM", projection * view * model);
      sceneLights.SetDynamicAttributes(lamp);
      ico_80.Draw(lamp);
    }
    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);

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

    
    //framebuffer test
    framebuffertest.Postprocess(framebufferShader);
    ///

    //TODO:update this somehow
    input.TrueFalse(testBool);
    if(testBool)
      scenePhysics.StepPhysics(deltaTime < deltaRate ? deltaTime : deltaRate);

    player.Update();

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
  //dShader.Shutdown();
  glfwTerminate();
}


