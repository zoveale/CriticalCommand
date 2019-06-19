

#include <iostream>

#include "system.h"

int main(void)
{
  System system;
 
  
  
  system.SystemInit();
  system.GameLoop();
  system.Shutdown();

  return 0;
}

/*
int nrAttributes;
glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
*/