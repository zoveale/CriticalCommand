#include "input.h"


void Input::StartUp(GLFWwindow* window) {
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



}
void Input::PollEvents() {
  glfwPollEvents();
}