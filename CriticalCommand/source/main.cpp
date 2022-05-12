#include "system.h"

int main(int agrc, char** argv){
  System system;

  system.SystemInit();
  system.GameLoop();
  system.Shutdown();
  
  return 0;
}
