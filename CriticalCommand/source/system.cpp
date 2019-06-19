#include "system.h"


System::System() {
}

void System::SystemInit(){
  render.Startup();
 
}

void System::GameLoop(){
  Shader dShader("resources/shader/zdVertexShader.glsl", "resources/shader/zdFragmentShader.glsl");

  float position[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
  };

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  dShader.Use();
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(render.Window())) {

    /* Render here */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    // create transformations
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(1.0f, 0.0f, -4.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 100.0f);
    
   
    

    // get matrix's uniform location and set matrix
    dShader.Use();
    dShader.setMat4("model", model);
    dShader.setMat4("view", view);
    dShader.setMat4("projection", projection);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
    
    ////transform = glm::mat4(1.0f); // reset it to identity matrix
    //model = glm::translate(model, glm::vec3(-0.5f, 0.5f, 0.0f));
    //float scaleAmount = sin(glfwGetTime());
    //model = glm::scale(model, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
    //dShader.setMat4("model", model);
    //// now with the uniform matrix being replaced with new transformations, draw it again.
    //glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    
    
    /* Swap front and back buffers */
    render.Display();

    /* Poll for and process events */
    glfwPollEvents();
  }

  
}

void System::Shutdown() {

  //dShader.Shutdown();
  glfwTerminate();
}



/* float position[] = {
       -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f,  0.5f,
       -0.5f,  0.5f

  };

  unsigned int indices[] = {
    0,1,2,
    2,3,0

    glm::vec3 position[] = {
     glm::vec3(2.0000f, -1.0000f, -2.0000f),
     glm::vec3(0.0000f, -1.0000f, 0.0000f),
     glm::vec3(0.0000f, -1.0000f, -2.0000f),
     glm::vec3(2.0000f, 1.0000f, -2.0000f),
     glm::vec3(0.0000f, 1.0000f, 0.0000f),
     glm::vec3(2.0000f, 1.0000f, 0.0000f),
     glm::vec3(2.0000f, -1.0000f, -2.0000f),
     glm::vec3(2.0000f, 1.0000f, 0.0000f),
     glm::vec3(2.0000f, -1.0000f, 0.0000f),
     glm::vec3(2.0000f, -1.0000f, 0.0000f),
     glm::vec3(0.0000f, 1.0000f, 0.0000f),
     glm::vec3(0.0000f, -1.0000f, 0.0000f),
     glm::vec3(0.0000f, -1.0000f, 0.0000f),
     glm::vec3(0.0000f, 1.0000f, -2.0000f),
     glm::vec3(0.0000f, -1.0000f, -2.0000f),
     glm::vec3(2.0000f, 1.0000f, -2.0000f),
     glm::vec3(0.0000f, -1.0000f, -2.0000f),
     glm::vec3(0.0000f, 1.0000f, -2.0000f),
     glm::vec3(2.0000f, -1.0000f, 0.0000f),
     glm::vec3(0.0000f, 1.0000f, -2.0000f),
     glm::vec3(2.0000f, 1.0000f, -2.0000f),
     glm::vec3(2.0000f, 1.0000f, 0.0000f),
     glm::vec3(0.0000f, 1.0000f, 0.0000f),
     glm::vec3(2.0000f, -1.0000f, -2.0000f)
  };


  unsigned short indices[] = {
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 18, 1,
     3, 19, 4, 6, 20, 7, 9, 21, 10, 12, 22, 13, 15, 23, 16
  };

  Shader dShader("resources/shader/zdVertexShader.glsl", "resources/shader/zdFragmentShader.glsl");

  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 36, &position[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void*)0);


  //index Buffers
  GLuint ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,36 , &indices[0], GL_STATIC_DRAW);


  glBindVertexArray(0);
  }*/