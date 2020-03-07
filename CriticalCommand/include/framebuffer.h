#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include "shader.h"
#include "modelUtility.h"

class Framebuffer {
public:
  Framebuffer() {}
  void Load(Shader screenShader);
  void Preprocess();
  void Postprocess(Shader screenShader);
  unsigned int GetFBO();

  void CreateDepthMap();
  unsigned int GetDepthMapFBO();
  
  void DepthMapViewPort();
  void BindDepthMap();
  void SetShadowMap(Shader shader);
  void SetShadowCubemap(Shader shader);
  void CreateDepthCubeMap();
  void SetPointLightDepthToCubemap(glm::mat4 lightProjection, glm::mat4 (&transformArray)[6], glm::vec3 lightPosition);

  //
  void LoadGeometryBuffer();
  unsigned int GetGeometryBufferFBO();
  void BindGeometryBuffer();
  void SetDeferredShading(Shader deferredShading);
  ///

  //PBR Area sampling
  //pbr: convert HDR equirectangular environment map to cubemap equivalent
  void CreateEnvironmentMapBuffer(Shader equiShader, std::string hdrPath);
  ///
  ~Framebuffer();
private:
  void Test();


  //Gbuffer Stuff
  unsigned int geometryBuffer;
  unsigned int geometryPosition, geometryNormal, geometrymMetalRoughAo, geometryAlbedo;
  unsigned int attachments[3];
  unsigned int rboDepth;
  ///

  //
  unsigned int captureFBO, captureRBO;
  unsigned int envCubemap;
  unsigned int hdrTexture;
  ///
  unsigned int framebuffer;
  unsigned int textureColorbuffer[2];
  unsigned int quadVAO;
  unsigned int quadVBO;

  unsigned int depthCubemap;
  unsigned int depthMapTextureKey;

  const unsigned int TEXTURE_ZERO = 0;

  unsigned int depthMapFBO;
  const unsigned int SHADOW_WIDTH = 1 << 10;
  const unsigned int SHADOW_HEIGHT = 1 << 10;

  static unsigned int count;
};
#endif // !FRAMEBUFFER_H
