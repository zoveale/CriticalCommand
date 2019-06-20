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
};

class Input {
private:
  
public:
  KEY_STATE KEY;
  void Process(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      KEY.W = true;
      //printf("(vW) ");
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
      KEY.W = false;
      //printf("(^W) ");
    }
      
    
    
    //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
     
    //if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
     
  }
  void PollEvents() {
    glfwPollEvents();
  }

};
#endif // !INPUT_H
