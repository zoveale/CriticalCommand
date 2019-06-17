#ifndef RENDER_H
#define RENDER_H

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Render {
private:
  GLFWwindow* window;

public:
  Render();

  void Startup();
  GLFWwindow* Window() { return this->window; }
  
  void Display();
};
#endif // !RENDER_H
