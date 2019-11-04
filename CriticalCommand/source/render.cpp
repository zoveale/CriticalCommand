#include "render.h"

Render::Render() {

}

void Render::StartUp(){
  /* Initialize the library */
  if (!glfwInit())
    printf("!glfwInit()");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //MSAA-multisampled anti-aliasing
  glfwWindowHint(GLFW_SAMPLES, 4);
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
  //glDepthFunc(GL_LESS);

  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  glEnable(GL_CULL_FACE);
  //glCullFace(GL_FRONT);
  //glFrontFace(GL_CCW);

  glEnable(GL_MULTISAMPLE);
}
///private fucntions
void Render::ClearScreen() {
  glClearColor(0.0f, 0.05f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
void Render::Display() {
  
  glfwSwapBuffers(this->window);
}
