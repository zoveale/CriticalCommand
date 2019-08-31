#include "system.h"

System::System() {

}

void System::SystemInit(){

  render.StartUp();
  input.StartUp(render.Window());
  //player.startup \ or vise versa?
  //camera.startup /
  scenePhysics.StartUp();
  //sceneLights;
  printf("OpenGl version: %s\n", glGetString(GL_VERSION));
}

void System::GameLoop(){
  Shader animated("resources/shader/Vanimated.glsl", "resources/shader/Fanimated.glsl");
  Model ourModel_0("resources/cowboy/CharacterRunning4.dae", sceneLights);

  Shader fixed("resources/shader/Vmodel.glsl", "resources/shader/Fmodel.glsl");
  Model ourModel_1("resources/watchtower/tower.obj", sceneLights);

  //Model surface("resources/surface/floor.dae", sceneLights);
  Model default_0("resources/default/default5.dae", sceneLights);
  //Lamp models
  Shader lamp("resources/shader/lampV.glsl", "resources/shader/lampF.glsl");
  Model pointLamp("resources/surface/pointLamp.dae", sceneLights);
  Model spotLamp("resources/surface/spotLight.dae", sceneLights);

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  float deltaTime = 0.0f;	
  float lastFrame = 0.0f; 
  float currentFrame = 0.0f;
  fixed.Use();
  fixed.SetFloat("material.shininess", 32.0f);
  
  /* Loop until the user closes the window */
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

    model = glm::mat4(1.0f);

    fixed.Use();
    
    fixed.SetVec3("viewPos", player.position);
    
    //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));
    //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
    //model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    
    fixed.SetMat4("model", model);
    fixed.SetMat4("PVM", projection * view * model);
    sceneLights.Set(fixed);
    default_0.Draw(fixed);
    
    

    lamp.Use();
    for (unsigned int i = 0; i < sceneLights.NumPointLights(); i++) {
      model = sceneLights.GetPointLightTransformation(i);
      //model = glm::mat4(1.0f);
      //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));
      //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
      //model = glm::translate(model, sceneLights.GetPointLightPos(i));
      model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
      lamp.SetMat4("PVM", projection * view * model);
      pointLamp.Draw(lamp);
    }
    for (unsigned int i = 0; i < sceneLights.NumSpotLights(); i++) {
      model = sceneLights.GetSpotLightTransformation(i);
      //model = glm::mat4(1.0f);
      //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));
      //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
      //model = glm::translate(model, sceneLights.GetSpotLightPos(i));
      model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
      lamp.SetMat4("PVM", projection * view * model);
      spotLamp.Draw(lamp);
    }
    
    
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

//unsigned int loadTexture(char const* path) {
//  unsigned int textureID;
//  glGenTextures(1, &textureID);
//
//  int width, height, nrComponents;
//  unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
//  if (data) {
//    GLenum format;
//    if (nrComponents == 1)
//      format = GL_RED;
//    else if (nrComponents == 3)
//      format = GL_RGB;
//    else if (nrComponents == 4)
//      format = GL_RGBA;
//
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//    glGenerateMipmap(GL_TEXTURE_2D);
//
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    stbi_image_free(data);
//  }
//  else {
//    std::cout << "Texture failed to load at path: " << path << std::endl;
//    stbi_image_free(data);
//  }
//
//  return textureID;
//}

/*

    dShader.Use();
    dShader.SetVec3("objectColor", objectColor);
    dShader.SetVec3("lightColor", lightColor);


    dShader.SetVec3("viewPos", player.position);

    dShader.SetVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    dShader.SetVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
    dShader.SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
    dShader.SetFloat("material.shininess", 32.0f);

    dShader.SetVec3("light.ambient", 0.1f, 0.1f, 0.1f);
    dShader.SetVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
    dShader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

    dShader.SetVec3("light.position", player.position);
    dShader.SetVec3("light.direction", player.front);
    dShader.SetFloat("light.cutoff", glm::cos(glm::radians(12.5f)));

    dShader.SetFloat("light.outerCutoff", glm::cos(glm::radians(17.5f)));
    dShader.SetFloat("light.constant", 1.0f);
    dShader.SetFloat("light.linear", 0.09f);
    dShader.SetFloat("light.quadratic", 0.032f);
*/