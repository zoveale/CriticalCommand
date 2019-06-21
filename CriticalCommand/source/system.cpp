#include "system.h"
#include "..//resources/data/testData.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/include/stb_image.h"

System::System() {
}

void System::SystemInit(){
  render.Startup();
 
}

void System::GameLoop(){
  Shader dShader("resources/shader/zdVertexShader.glsl", "resources/shader/zdFragmentShader.glsl");

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
  
  /*
  
  */
  unsigned int VBO, VAO, UVBO;

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


  glBindVertexArray(0);
  glUseProgram(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);


  glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
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

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(render.Window())) {

    /* Render here */
    //clear screen and color background
    ClearScreen();

    input.Process(render.Window());

    player.HandleInput(input);

    // create transformations
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    float radius = 10.0f;
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;
   
    view = glm::lookAt(glm::vec3(camX, 0.0, camZ),
                       glm::vec3(0.0, 0.0, 0.0),
                       glm::vec3(0.0, 1.0, 0.0));

    
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
    projection = glm::perspective(glm::radians(55.0f),(float)1280/(float)720, 0.1f, 100.0f);
    
    // get matrix's uniform location and set matrix
    dShader.Use();
    //dShader.setMat4("model", model);
    dShader.setMat4("view", view);
    dShader.setMat4("projection", projection);

    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 1.0f));
      dShader.setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    

    player.Update();
    
    /* Swap front and back buffers */
    render.Display();

    /* Poll for and process events */
    input.PollEvents();
  }

  
}

void System::Shutdown() {

  //dShader.Shutdown();
  glfwTerminate();
}

///private fucntions
void System::ClearScreen() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}