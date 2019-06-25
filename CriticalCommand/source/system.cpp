#include "system.h"
#include "..//resources/data/testData.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/include/stb_image.h"

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
  //
  Shader dShader("resources/shader/zdVertexShader.glsl", "resources/shader/zdFragmentShader.glsl");
  Shader lamp("resources/shader/vLamp.glsl", "resources/shader/fLamp.glsl");
  //lamp.Print();
  ///
  /*
  texture test
  */
  unsigned int texture1;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  // set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate the texture
  int width, height, nrChannels;
  unsigned char* data = stbi_load("resources/texture/wall.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  ///
  /*
  
  */
  unsigned int VBO, VAO, UVBO, NormalVB;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

 
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
  // position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glGenBuffers(1, &UVBO);
  glBindBuffer(GL_ARRAY_BUFFER, UVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
  // position attribute
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glGenBuffers(1, &NormalVB);
  glBindBuffer(GL_ARRAY_BUFFER, NormalVB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
  // position attribute
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


  glBindVertexArray(0);
  glUseProgram(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);


  //glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3 cubePositions[] = {
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
  };

  /*
  lamp light
  */
  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);
  glm::vec3 lightPos(-4.2f, -1.0f, -5.0f);
  ///

  float deltaTime = 0.0f;	// Time between current frame and last frame
  float lastFrame = 0.0f; // Time of last frame
  float currentFrame = 0.0f;

 /* double xpos, ypos;
  glfwGetCursorPos(render.Window(), &xpos, &ypos);
  float lastX = 1280/2;
  float lastY = 720/2;
*/
  glm::vec3 objectColor = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

  /* Loop until the user closes the window */
  while (!input.KEY.ESC) {
    /*lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
    lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
    lightColor = glm::vec3(1.0f + sin(glfwGetTime()) * 2.0f, sin(glfwGetTime() / 2.0f) * 1.0f,0.0);*/

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
    
    
   
    

    dShader.Use();
    dShader.setVec3("objectColor", objectColor);
    dShader.setVec3("lightColor", lightColor);
    dShader.setVec3("lightPos", lightPos);
    dShader.setVec3("viewPos", player.position);
   
    
    
    projection = glm::perspective(glm::radians(55.0f), (float)1280 / (float)720, 0.1f, 100.0f);
    view = playerCamera.View();
    dShader.setMat4("projection", projection);
    dShader.setMat4("view", view);
    
    
    for (unsigned int i = 0; i < 9; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      //model = glm::rotate(model, (float)glfwGetTime()/4, glm::vec3(0.0f, 1.0f, 1.0f));
      dShader.setMat4("model", model);
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    
   
    lamp.Use();
    model = glm::mat4(1.0f);
    
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.3));
    lamp.setVec3("color", lightColor);
    lamp.setMat4("model", model);
    lamp.setMat4("view", view);
    lamp.setMat4("projection", projection);
    
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

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