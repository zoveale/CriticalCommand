#include "system.h"

System::System() {

}

void System::SystemInit(){

  render.StartUp();
  input.StartUp(render.Window());
  //player.startup \ or vise versa?
  //camera.startup /
  
  scenePhysics.StartUp();
  
  printf("OpenGl version: %s\n", glGetString(GL_VERSION));
}

void System::GameLoop(){
  //Shader animated("resources/shader/Vanimated.glsl", "resources/shader/Fanimated.glsl");
  //Model ourModel_0("resources/cowboy/CharacterRunning4.dae", sceneLights,scenePhysics);

  
  //Model ourModel_1("resources/watchtower/tower.obj", sceneLights, scenePhysics);
  Shader fixed("resources/shader/Vmodel.glsl", "resources/shader/Fmodel.glsl");
  //TODO:: PHYSX testing
  Model ico_80("resources/default/ico_80.dae", sceneLights, scenePhysics);
  //Model ico_80_Big("resources/default/ico_80.dae", sceneLights, scenePhysics);
  
  ///
  //Model surface("resources/surface/floor.dae", sceneLights);
  //TODO:: warning : TriangleMesh: triangles are too big, 
  //reduce their size to increase simulation stability!
  Model default_0("resources/default/default7.dae", sceneLights, scenePhysics , true);
  
  //Lamp models
  Shader lamp("resources/shader/lampV.glsl", "resources/shader/lampF.glsl");
  Model pointLamp("resources/surface/pointLamp.dae", sceneLights, scenePhysics);
  Model spotLamp("resources/surface/spotLight.dae", sceneLights, scenePhysics);

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  float deltaTime = 0.0f;	
  float lastFrame = 0.0f; 
  float currentFrame = 0.0f;
  fixed.Use();
  fixed.SetFloat("material.shininess", 32.0f);
  sceneLights.SetFixedAttributes(fixed);

  float x = 1.0;

  while (!input.KEY.ESC) {

    input.Process();
    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    /* Render here */
    //clear screen and color background
    ClearScreen();
    player.HandleInput(input, deltaTime);
    //cool acceleration effect
    //player.Update(xpos, ypos); 
    ///
    //reset models
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);
    ///
    projection = glm::perspective(glm::radians(55.0f), (float)1280 / (float)720, 0.1f, 100.0f);
    view = playerCamera.View();

   /* animated.Use();
    animated.SetMat4("projection", projection);
    animated.SetMat4("view", view);
    model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	
    animated.SetMat4("PVM", projection * view * model);
    animated.SetMat4("model", model);
    ourModel_0.Animate(animated, currentFrame);*/

    

    fixed.Use();
    fixed.SetVec3("viewPos", player.position);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    fixed.SetMat4("model", model);
    fixed.SetMat4("PVM", projection * view * model);
    sceneLights.SetDynamicAttributes(fixed);
    default_0.Draw(fixed);
    
    
    
    for (int i = 0; i <55; i++) {
      model = glm::mat4(1.0f);
      model = scenePhysics.GetAPose(i); 
      model = glm::scale(model, glm::vec3(2.0));
      fixed.SetMat4("model", model);
      fixed.SetMat4("PVM", projection * view * model);
      sceneLights.SetDynamicAttributes(fixed);
      ico_80.Draw(fixed);
    }

  
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
    
    scenePhysics.GetActors();
    scenePhysics.StepPhysics();
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

///private fucntions
void System::ClearScreen() {
  glClearColor(0.0f, 0.05f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
