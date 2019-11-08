#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include "shader.h"
class Framebuffer {
public:
  Framebuffer(Shader screenShader);

  void Preprocess();
  void Postprocess(Shader screenShader);

  ~Framebuffer();
private:
  void Test();
  unsigned int framebuffer;
  unsigned int textureColorbuffer;
  unsigned int quadVAO;
  unsigned int quadVBO;
};
#endif // !FRAMEBUFFER_H
