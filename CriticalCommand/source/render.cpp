#include "render.h"

Render::Render() {
}

void Render::Startup(){
  /* Initialize the library */
  if (!glfwInit())
    printf("!glfwInit()");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(SC.WIDTH, SC.HEIGHT, "Welcome To Critical Command", NULL, NULL);
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

  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_BACK);
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  printf("OpenGl version: %s\n", glGetString(GL_VERSION));
}

void Render::Display() {
  glfwSwapBuffers(this->window);
}
