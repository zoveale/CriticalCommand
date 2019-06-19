#include "system.h"


System::System() {
}

void System::SystemInit(){
  render.Startup();
}

void System::GameLoop(){


 /* float position[] = {
       -0.5f, -0.5f, 
        0.5f, -0.5f, 
        0.5f,  0.5f,
       -0.5f,  0.5f
       
  };

  unsigned int indices[] = {
    0,1,2,
    2,3,0
  }*/;

  glm::vec3 position[] = {
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3(0.5f, -0.5f, -0.5f),
    glm::vec3(0.5f,  0.5f, -0.5f),
    glm::vec3(0.5f,  0.5f, -0.5f),
    glm::vec3(-0.5f,  0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),

    glm::vec3(-0.5f, -0.5f,  0.5f),
    glm::vec3(0.5f, -0.5f,  0.5f),
    glm::vec3(0.5f,  0.5f,  0.5f),
    glm::vec3(0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f, -0.5f,  0.5f),

    glm::vec3(-0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f,  0.5f),

    glm::vec3(0.5f,  0.5f,  0.5f),
    glm::vec3(0.5f,  0.5f, -0.5f),
    glm::vec3(0.5f, -0.5f, -0.5f),
    glm::vec3(0.5f, -0.5f, -0.5f),
    glm::vec3(0.5f, -0.5f,  0.5f),
    glm::vec3(0.5f,  0.5f,  0.5f),

    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3(0.5f, -0.5f, -0.5f),
    glm::vec3(0.5f, -0.5f,  0.5f),
    glm::vec3(0.5f, -0.5f,  0.5f),
    glm::vec3(-0.5f, -0.5f,  0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),

    glm::vec3(-0.5f,  0.5f, -0.5f),
    glm::vec3(0.5f,  0.5f, -0.5f),
    glm::vec3(0.5f,  0.5f,  0.5f),
    glm::vec3(0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f, -0.5f)
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

  dShader.Use();
  
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(render.Window())) {

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    
    //glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    ////transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
    //transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 1.0f));

    //// get matrix's uniform location and set matrix
    //dShader.Use();
    //dShader.setMat4("transform", transform);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
    
    ////transform = glm::mat4(1.0f); // reset it to identity matrix
    //transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
    //float scaleAmount = sin(glfwGetTime());
    //transform = glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
    //dShader.setMat4("transform", transform);
    //// now with the uniform matrix being replaced with new transformations, draw it again.
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    

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



