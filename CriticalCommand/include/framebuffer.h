#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include "shader.h"

class Framebuffer {
public:
  Framebuffer(Shader screenShader);

  void Preprocess();
  void Postprocess(Shader screenShader);

  void CreateDepthMap();
  unsigned int GetDepthMapFBO();
  

  void BindDepthMap();
  void SetShadowMap(Shader shader);
  void SetShadowCubemap(Shader shader);
  void CreateDepthCubeMap();

  ~Framebuffer();
private:
  void Test();
  unsigned int framebuffer;
  unsigned int textureColorbuffer;
  unsigned int quadVAO;
  unsigned int quadVBO;

  unsigned int depthCubemap;
  unsigned int depthMapTextureKey;

  const unsigned int TEXTURE_ZERO = 0;

  unsigned int depthMapFBO;
  const unsigned int SHADOW_WIDTH = 1 << 7;
  const unsigned int SHADOW_HEIGHT = 1 << 7;


};
#endif // !FRAMEBUFFER_H
