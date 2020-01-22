#include "input.h"


Input::Input() {
  lastX = 0.0;
  lastY = 0.0;
  xpos = 0.0; 
  ypos = 0.0;
  xoffset = 0.0;
  yoffset = 0.0;
}

void Input::StartUp(GLFWwindow* window) {
  timer = 0;
  lastX = 1280 / 2;
  lastY = 720 / 2;
  this->window = window;
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwGetCursorPos(window, &xpos, &ypos);
}

//Fix for circular class includes
/*void MouseInput(Player& player) {


  player.look.x = (float)xoffset;
  player.look.y = (float)yoffset;
}*/
///

void Input::Process() {
  //Mouse input
  lastX = xpos;
  lastY = ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  xoffset = xpos - lastX;
  yoffset = lastY - ypos;
  //printf("mouse x, y : %f, %f\n", xpos, ypos);
  ///

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
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    KEY.UP = true;
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) {
    KEY.UP = false;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    KEY.DOWN = true;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE) {
    KEY.DOWN = false;
  }


}
void Input::PollEvents() {
  glfwPollEvents();
}

void Input::IncrementDecrement(float &i) {
  if (KEY.UP) {
    i += 0.01f;
  }
  if (KEY.DOWN) {
    i -= 0.01f;
  }
}

void Input::IncrementDecrement(int& i) {
  
  if (KEY.UP) {
    i++;
  }
  if (KEY.DOWN) {
    i--;
  }
}

void Input::IncrementDecrement(unsigned int& i) {
  
  if (KEY.UP) {
    timer += 1;
    if (timer > 100) {
      i++;
      timer = 0;
    }
  }
  if (KEY.DOWN) {
    timer += 1;
    if (timer > 100) {
      i--;
      timer = 0;
    }
    
  }
}

void Input::IncrementDecrement(bool& i) {
  if (KEY.UP) {
    i = true;
  }
  if (KEY.DOWN) {
    i = false;
  }
}
