#include "system.h"
#include "..//resources/data/testData.h"
//assimp test
#include "model.h"
///
//unsigned int loadTexture(char const* path);
System::System() {

}

void System::SystemInit(){

  render.StartUp();
  input.StartUp(render.Window());
  //player.startup \ or vise versa?
  //camera.startup /


  printf("OpenGl version: %s\n", glGetString(GL_VERSION));
}

void System::GameLoop(){
 //assimp test
  Shader ourShader("resources/shader/VmeshTest.glsl", "resources/shader/FmeshTest.glsl");
  Model ourModel_0("resources/skeleton/Skeleton2/CharacterRunning.dae");
  //Model ourModel_1("resources/skeleton/Skeleton2/skele2.obj");
 ///


  float deltaTime = 0.0f;	// Time between current frame and last frame
  float lastFrame = 0.0f; // Time of last frame
  float currentFrame = 0.0f;

  /* Loop until the user closes the window */
  while (!input.KEY.ESC) {
    //lightPos.z =  -4.0f - sin(glfwGetTime()) * 4.0f;

    input.Process();

    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    
    /* Render here */
    //clear screen and color background
    ClearScreen();

    player.HandleInput(input, deltaTime);

    // create transformations
    glm::mat4 model = glm::mat4(1.0f); 
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    
    //cool acceleration effect
    //player.Update(xpos, ypos); 
    ///
    ourShader.Use();
    projection = glm::perspective(glm::radians(55.0f), (float)1280 / (float)720, 0.1f, 100.0f);
    view = playerCamera.View();
    ourShader.SetMat4("projection", projection);
    ourShader.SetMat4("view", view);

    // render the loaded model
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
    ourShader.SetMat4("model", model);
    ourModel_0.Draw(ourShader);
    //ourModel_1.Draw(ourShader);


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

  //dShader.Shutdown();
  glfwTerminate();
}

///private fucntions
void System::ClearScreen() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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