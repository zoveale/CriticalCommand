

#include <iostream>

#include "system.h"

int main(void)
{
  System system;
 
  int nrAttributes;
  
  system.SystemInit();
  system.GameLoop();
  system.Shutdown();

  return 0;
}

/*
glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
*/