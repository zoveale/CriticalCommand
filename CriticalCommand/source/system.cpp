#include "system.h"

void System::SystemInit(){
  render.Startup();
}

void System::GameLoop(){


  GLfloat positions[6] = {
       -0.5f, -0.5f, 
        0.0f,  0.5f, 
        0.5f, -0.5f 
  };

  GLuint buffer;         
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), positions, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (const void*)0);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(render.Window())) {

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    

    /* Swap front and back buffers */
    render.Display();

    /* Poll for and process events */
    glfwPollEvents();
  }
}

void System::Shutdown() {


  glfwTerminate();
}



