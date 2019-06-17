

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