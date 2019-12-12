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
  std::vector<Model*> bombModels;
  Model bombModelIdel("resources/bomb/bomb.dae", sceneLights, scenePhysics);
  //Model bombModelBig("resources/bomb/bomb1.dae", sceneLights, scenePhysics);
  bombModels.push_back(&bombModelIdel);
  //bombModels.push_back(&bombModelBig);
  Shader bombShader("resources/bomb/shaders/vertex.glsl", "resources/bomb/shaders/fragment.glsl");

  BombGraphicsComponent bombGraphics(bombModels, bombShader);
  BombPhysicsComponent bombPhysics(&scenePhysics);

  GameObject bomb(&bombGraphics, &bombPhysics);


  Shader simple("resources/shader/Model/Vmodel.glsl",
    "resources/shader/Model/Fmodel.glsl");
  //TODO:: PHYSX testing
  Model ico_80("resources/default/ico_80.dae", sceneLights, scenePhysics);

  IcoSphereGraphicsComponent icoGraphics(&ico_80, &simple, &sceneLights);
  IcoSpherePhysicsComponent icoPhysics(&scenePhysics);
  //std::vector<GameObject> icoSpheres;
  GameObject icoSphere(&icoGraphics, &icoPhysics);


  Shader normalShader("resources/shader/NormalShader/Vnormal.glsl",
                      "resources/shader/NormalShader/Fnormal.glsl",
                      "resources/shader/NormalShader/Gnormal.glsl");

  //Framebuffer testing
  Shader framebufferShader("resources/shader/Framebuffer/Vframebuffer.glsl",
                           "resources/shader/Framebuffer/Fframebuffer.glsl");
  Framebuffer framebuffertest(framebufferShader);
  ///

  ////Cubemap Testing
  //Shader cubeMapShader("resources/shader/CubeMap/Vcubemap.glsl",
  //                     "resources/shader/CubeMap/Fcubemap.glsl",
  //                     "resources/shader/CubeMap/Gcubemap.glsl");
  //Model cubemap("resources/default/cubemapbox.dae", sceneLights, scenePhysics);
  /////

  //Model ourModel_1("resources/watchtower/tower.obj", sceneLights, scenePhysics);

 
  ///
  Model default_0("resources/default/physxTestLightsTestTextureTest1.dae", sceneLights, scenePhysics , true);
  
  //Lamp models
  Shader lamp("resources/shader/Lamp/lampV.glsl", "resources/shader/Lamp/lampF.glsl");
  Model pointLamp("resources/surface/pointLamp.dae", sceneLights, scenePhysics);
  Model spotLamp("resources/surface/spotLight.dae", sceneLights, scenePhysics);

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
  bool testBool = true;
  ///
  // draw as wireframe
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  float deltaRate = 1.0f / 60.0f;
  float deltaTime = 0.0f;
  float currentFrame = 0.0f;
  float lastFrame = (float)glfwGetTime();

  //

  while (!input.KEY.ESC) {
     
    input.Process();

    //
    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    //printf("deltaTime = %f\n", deltaTime);
    ///
    //TODO:update this somehow
    input.IncrementDecrement(testBool);
    if (testBool)
      scenePhysics.StepPhysics(deltaRate);

    //framebuffer test
    framebuffertest.Preprocess();
    ///

    /* Render here */
    //clear screen and color background
    render.ClearScreen();
    player.HandleInput(input, deltaTime);
    ///
    //set to identity matrix
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);
    ///

    //TODO::TEST FUNCTIONS
    //input.IncrementDecrement(perspective);
    projection = glm::perspective(glm::radians((float)perspective),
                (float)Render::Screen::WIDTH /(float)Render::Screen::HEIGHT, 0.1f, 1000.0f);
    view = firstPerson.View();
    
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

    glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
    glStencilMask(0x00);
    //input.IncrementDecrement(gamma);
    
    simple.Use();
    simple.SetFloat("gamma", gamma);
    simple.SetVec3("viewPos", player.position);
    //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    simple.SetMat4("model", model);
    simple.SetMat4("PVM", projection * view * model);
    sceneLights.SetDynamicAttributes(simple);
    default_0.Draw(simple);
    
    normalShader.Use();
    normalShader.SetMat4("projection", projection);
    normalShader.SetMat4("view", view);
    normalShader.SetMat4("model", model);
    default_0.Draw(normalShader);
    
    glm::mat4 pvMatrix = projection * view;
    player.Update(deltaTime);
    bomb.Update(deltaTime, pvMatrix);
    bomb.Draw();
    
    icoSphere.Update(deltaTime, pvMatrix);
    icoSphere.Draw();
  
    glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
    glStencilMask(0xFF);

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
    
    //framebuffer test
    framebuffertest.Postprocess(framebufferShader);
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

