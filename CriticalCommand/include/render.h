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
  void StartUp();
  void ClearScreen();
  GLFWwindow* Window() { return this->window; }
  
  void Display();

  struct Screen {
    static const int WIDTH = 1768;
    static const int HEIGHT = 992;
  };
};
#endif // !RENDER_H
