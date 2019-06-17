#include "render.h"

Render::Render() {
}

void Render::Startup(){
  /* Initialize the library */
  if (!glfwInit())
    printf("!glfwInit()");

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Welcome To Critical Command", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    printf("!glfwCreateWindow\n");
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    printf("Error GLEW_OK = %i", GLEW_OK);
  }

  printf("OpenGl version: %s\n", glGetString(GL_VERSION));
}

void Render::Display() {
  glfwSwapBuffers(this->window);
}
