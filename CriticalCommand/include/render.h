#ifndef RENDER_H
#define RENDER_H

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#define SC_WIDTHX 1280
struct SCREEN {
  static const int WIDTH = 1280;
  static const int HEIGHT = 720;
};

class Render {
private:
  GLFWwindow* window;
public:
  Render();
  SCREEN SC;
  void Startup();
  GLFWwindow* Window() { return this->window; }
  
  void Display();
};
#endif // !RENDER_H
