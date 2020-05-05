#ifndef RENDER_H
#define RENDER_H

//RenderDoc
#include "windows.h"
#include "renderdoc_app.h"
#include <assert.h> 
///

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>




class Render {
private:
  GLFWwindow* window;
  
public:
  static RENDERDOC_API_1_4_1* DebugApi;

  static void RenderDocInit() {
    if (HMODULE mod = GetModuleHandleA("renderdoc.dll")) {
      pRENDERDOC_GetAPI RENDERDOC_GetAPI =
        (pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
      int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void**)& Render::DebugApi);
      assert(ret == 1);
    }
  }

  Render();
  void StartUp();
  void ClearScreen();
  GLFWwindow* Window() { return this->window; }
  
  void Display();

  struct Screen {
    static const int WIDTH = 1768;
    static const int HEIGHT = 992;
  };
};
#endif // !RENDER_H
