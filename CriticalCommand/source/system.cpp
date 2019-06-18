#include "system.h"


System::System() {
}

void System::SystemInit(){
  render.Startup();
}

void System::GameLoop(){


  float position[] = {
       -0.5f, -0.5f, 
        0.5f, -0.5f, 
        0.5f,  0.5f,
       -0.5f,  0.5f
       
  };

  unsigned int indices[] = {
    0,1,2,
    2,3,0
  };
  Shader dShader("resources/shader/zdVertexShader.glsl", "resources/shader/zdFragmentShader.glsl");
  
  GLuint buffer;         
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), position, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);

  //index Buffers
  GLuint ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

  dShader.Use();
  
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(render.Window())) {

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    

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



