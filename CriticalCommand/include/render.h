#ifndef RENDER_H
#define RENDER_H

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Render {
private:
  GLFWwindow* window;

public:
  void Startup();
  GLFWwindow* Window() { return this->window; }
};
#endif // !RENDER_H
