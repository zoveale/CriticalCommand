#include "render.h"

//RENDERDOC_API_1_4_1* Render::DebugApi;



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
  //glfwGetPrimaryMonitor() for fullscreen
  window = glfwCreateWindow(Screen::WIDTH, Screen::HEIGHT, "Welcome To Critical Command", NULL, NULL);
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

  //Fix seams in cubemaps
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  //TODO::multisampling on framebuffers
  //glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  //TODO::reimplment stencil testing 
  //glEnable(GL_STENCIL_TEST);
  //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  
}
///private fucntions
void Render::ClearScreen() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
void Render::Display() {
  
  glfwSwapBuffers(this->window);
}
