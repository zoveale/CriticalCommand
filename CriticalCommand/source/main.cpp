

#include <iostream>

#include "render.h"

int main(void)
{
  Render render;
  render.Startup();


  

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(render.Window()))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();

    /* Swap front and back buffers */
    glfwSwapBuffers(render.Window());

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}