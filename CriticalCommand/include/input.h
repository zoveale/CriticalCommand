#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct KEY_STATE {

  bool W = false;
  bool S = false;
  bool A = false;
  bool D = false;
  bool SPACE = false;
  bool ESC = false;
};

class Input {
private:
  
public:
  void StartUp(GLFWwindow* window) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
  KEY_STATE KEY;
  void Process(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      KEY.ESC = true;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      KEY.W = true;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
      KEY.W = false;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      KEY.S = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
      KEY.S = false;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      KEY.A = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
      KEY.A = false;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      KEY.D = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
      KEY.D = false;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
      KEY.SPACE = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
      KEY.SPACE = false;
    }

 
     
  }
  void PollEvents() {
    glfwPollEvents();
  }

};
#endif // !INPUT_H
