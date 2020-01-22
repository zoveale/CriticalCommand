#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include "shader.h"

class Framebuffer {
public:
  Framebuffer() {}
  Framebuffer(Shader screenShader);

  void Preprocess();
  void Postprocess(Shader screenShader);

  void CreateDepthMap();
  unsigned int GetDepthMapFBO();
  
  void DepthMapViewPort();
  void BindDepthMap();
  void SetShadowMap(Shader shader);
  void SetShadowCubemap(Shader shader);

  void CreateDepthCubeMap();

  void SetPointLightDepthToCubemap(glm::mat4 lightProjection, glm::mat4 (&transformArray)[6], glm::vec3 lightPosition);

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
  const unsigned int SHADOW_WIDTH = 1 << 9;
  const unsigned int SHADOW_HEIGHT = 1 << 9;

  static unsigned int count;
};
#endif // !FRAMEBUFFER_H
