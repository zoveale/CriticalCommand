

//#include <iostream>

#include "system.h"

int main(int agrc, char** argv){
  
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

//#define MAX_ACTORS 1<<8
//  int array[MAX_ACTORS] = { 1,2,3,4,5,6};
//  array[MAX_ACTORS] = -INT_MAX;
//  for (int i = 0; i < MAX_ACTORS; i++) {
//    if (array[i] == -INT_MAX) {
//      printf("no more actors to pass to shader\n");
//      break;
//    }
//    printf("array[%i] = %i\n", i, array[i]);
//    
//  }
//  printf("for loop has been exited\n");
//  std::cin.get();