#ifndef INPUT_H
#define INPUT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Input {
private:
public:
  int Process(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      printf("W ");
      return glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
      
      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        printf("(W->S) ");
      }
    }
      
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      printf("S ");
    
    //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
     
    //if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
     
  }
  void PollEvents() {
    glfwPollEvents();
  }

};
#endif // !INPUT_H
