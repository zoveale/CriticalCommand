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

  bool UP = false;
  bool DOWN = false;
  bool LEFT = false;
  bool RIGHT = false;

  bool SPACE = false;
  bool ESC = false;
  bool LEFT_ALT = false;
};

class Input {
private:
  
  GLFWwindow* window;
  unsigned int timer;
public:

  static KEY_STATE KEY;
  double lastX, lastY, xpos, ypos;
  static double  xoffset, yoffset;

  Input();
  void StartUp(GLFWwindow* window);

  //Fix for circular class includes
  /*void MouseInput(Player& player) {
    
    
    player.look.x = (float)xoffset;
    player.look.y = (float)yoffset;
  }*/
  ///

  //void HandleInput();
  void Process();
  void PollEvents();

  //test function
  void IncrementDecrement(float &i);
  void IncrementDecrement(int &i);
  void IncrementDecrement(unsigned int& i);
  void IncrementDecrement(bool &i);
};
#endif // !INPUT_H
