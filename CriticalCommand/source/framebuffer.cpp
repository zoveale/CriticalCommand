#include "framebuffer.h"
#include "render.h"
unsigned int Framebuffer::count = 0;



void Framebuffer::Load(Shader screenShader) {
  Test();

  screenShader.Use();
  screenShader.SetInt("hdrBuffer", 0);
  screenShader.SetInt("bloomBuffer", 1);

  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  // create a color attachment texture
  glGenTextures(2, textureColorbuffer);
  for (unsigned int i = 0; i < 2; i++) {
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer[i]);

    //TODO:: globally define screen height and width
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Render::Screen::WIDTH, Render::Screen::HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureColorbuffer[i], 0);
  }
  unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
  glDrawBuffers(2, attachments);
  //create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  //TODO::globally define screen height and width
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Render::Screen::WIDTH, Render::Screen::HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Preprocess() {
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void Framebuffer::Postprocess(Shader screenShader) {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilMask(0xFF);
  //CANT CLEAR SENTICL BUFFER HERE!
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  screenShader.Use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer[0]);
  screenShader.SetInt("hdrBuffer", 0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer[1]);
  screenShader.SetInt("bloomBuffer", 1);
  glBindVertexArray(quadVAO);
  //glViewport(0, 0, Render::Screen::WIDTH/2, Render::Screen::HEIGHT/2);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

unsigned int Framebuffer::GetFBO() {
  return framebuffer;
}

void Framebuffer::CreateDepthMap() {
  
}

unsigned int Framebuffer::GetDepthMapFBO() {
  return depthMapFBO;
}

void Framebuffer::CreateDepthCubeMap() {
  glGenFramebuffers(1, &depthMapFBO);
  // create depth cubemap texture
  //glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &depthCubemap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
  for (unsigned int i = 0; i < 6; ++i)
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  // attach depth texture as FBO's depth buffer
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  depthMapTextureKey = count;
  ++count;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glActiveTexture(GL_TEXTURE0);
}

void Framebuffer::SetPointLightDepthToCubemap(glm::mat4 lightProjection, 
                                              glm::mat4(&transformArray)[6],
                                              glm::vec3 lightPosition) {

  transformArray[0] = lightProjection * glm::lookAt(lightPosition,
    lightPosition + glm::vec3(1.0, 0.0, 0.0),glm::vec3(0.0, -1.0, 0.0));
  transformArray[1] = lightProjection * glm::lookAt(lightPosition,
    lightPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
  transformArray[2] = lightProjection * glm::lookAt(lightPosition,
    lightPosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
  transformArray[3] = lightProjection * glm::lookAt(lightPosition,
    lightPosition + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
  transformArray[4] = lightProjection * glm::lookAt(lightPosition,
    lightPosition + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
  transformArray[5] = lightProjection * glm::lookAt(lightPosition,
    lightPosition + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));

}

//Geometry Buffer Stuff
void Framebuffer::LoadGeometryBuffer() {
  Test();
  
  
  glGenFramebuffers(1, &geometryBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, geometryBuffer);
  // position color buffer
  glGenTextures(1, &geometryPosition);
  glBindTexture(GL_TEXTURE_2D, geometryPosition);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Render::Screen::WIDTH, Render::Screen::HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, geometryPosition, 0);
  // normal color buffer
  glGenTextures(1, &geometryNormal);
  glBindTexture(GL_TEXTURE_2D, geometryNormal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Render::Screen::WIDTH, Render::Screen::HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, geometryNormal, 0);
  // Metal Rough Ao
  glGenTextures(1, &geometrymMetalRoughAo);
  glBindTexture(GL_TEXTURE_2D, geometrymMetalRoughAo);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Render::Screen::WIDTH, Render::Screen::HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, geometrymMetalRoughAo, 0);
  //gAlbedo
  glGenTextures(1, &geometryAlbedo);
  glBindTexture(GL_TEXTURE_2D, geometryAlbedo);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Render::Screen::WIDTH, Render::Screen::HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, geometryAlbedo, 0);
  // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 

  unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
  glDrawBuffers(4, attachments);
  // create and attach depth buffer (renderbuffer)
  unsigned int rboDepth;
  glGenRenderbuffers(1, &rboDepth);
  glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Render::Screen::WIDTH, Render::Screen::HEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
  // finally check if framebuffer is complete
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "Framebuffer not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
unsigned int Framebuffer::GetGeometryBufferFBO() {
  return geometryBuffer;
}
void Framebuffer::BindGeometryBuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, geometryBuffer);
}
void Framebuffer::SetDeferredShading(Shader deferredShading) {
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilMask(0xFF);
  //CANT CLEAR SENTICL BUFFER HERE!
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  deferredShading.Use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, geometryPosition);
  deferredShading.SetInt("gPosition", 0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, geometryNormal);
  deferredShading.SetInt("gNormal", 1);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, geometrymMetalRoughAo);
  deferredShading.SetInt("metalRoughAo", 2);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, geometryAlbedo);
  deferredShading.SetInt("gAlbedo", 3);

  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
///


void Framebuffer::DepthMapViewPort() {
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
}

void Framebuffer::BindDepthMap() {
   
}

void Framebuffer::SetShadowMap(Shader shader) {
  /*glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, depthMapTextureKey);
  shader.SetInt("shadowMap", 1);*/ 
}

void Framebuffer::SetShadowCubemap(Shader shader) {
  glActiveTexture(GL_TEXTURE5 + depthMapTextureKey);
  glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
  
  shader.Use();
  shader.SetInt("shadowCastingPointLights[" +  std::to_string(depthMapTextureKey) + "]"
                 + ".shadowMap", 
                 5 + depthMapTextureKey);
}

//PBR Area sampling
//pbr: convert HDR equirectangular environment map to cubemap equivalent
void Framebuffer::CreateEnvironmentMapFromHdrEquirectangularMap(Shader equirectangularToCubemapShader, std::string hdrPath, unsigned int resolution){

  hdrTexture = Texture::LoadHDR(hdrPath.c_str());
  basicCube.LoadCubeOnly();

  glGenFramebuffers(1, &captureFBO);
  glGenRenderbuffers(1, &captureRBO);

  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

  glGenTextures(1, &envCubemap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
  for (unsigned int i = 0; i < 6; ++i) {
    // note that we store each face with 16 bit floating point values
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
      resolution, resolution, 0, GL_RGB, GL_FLOAT, nullptr);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
  glm::mat4 captureViews[] =
  {
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
  };

  equirectangularToCubemapShader.Use();
  equirectangularToCubemapShader.SetInt("equirectangularMap", 0);
  equirectangularToCubemapShader.SetMat4("projection", captureProjection);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, hdrTexture);

  glViewport(0, 0, resolution, resolution); // don't forget to configure the viewport to the capture dimensions.
  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  for (unsigned int i = 0; i < 6; ++i) {
    equirectangularToCubemapShader.SetMat4("view", captureViews[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    basicCube.RenderCube();
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Framebuffer::CreateIrradianceMapFromEnvironmentMap(Shader irradianceShader, unsigned int resolution) {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glGenTextures(1, &irradianceMap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
  for (unsigned int i = 0; i < 6; ++i) {
    // note that we store each face with 16 bit floating point values
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
      resolution, resolution, 0, GL_RGB, GL_FLOAT, nullptr);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
  glm::mat4 captureViews[] =
  {
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
  };

  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);

  irradianceShader.Use();
  irradianceShader.SetInt("environmentMap", 0);
  irradianceShader.SetMat4("projection", captureProjection);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

  glViewport(0, 0, resolution, resolution); // don't forget to configure the viewport to the capture dimensions.
  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  for (unsigned int i = 0; i < 6; ++i) {
    irradianceShader.SetMat4("view", captureViews[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    basicCube.RenderCube();
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  depthMapTextureKey = count;
  ++count;
}


void Framebuffer::DrawEnvironmentSkyBox(glm::mat4 view, glm::mat4 projection, Shader skyboxShader) {
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_FALSE);

  skyboxShader.Use();
  skyboxShader.SetMat4("view", glm::mat4(glm::mat3(view)));
  skyboxShader.SetMat4("projection", projection);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
  basicCube.RenderCube();
 
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);
}

void Framebuffer::DrawIrradianceSkyBox(glm::mat4 view, glm::mat4 projection, Shader skyboxShader) {
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_FALSE);

  skyboxShader.Use();
  skyboxShader.SetMat4("view", glm::mat4(glm::mat3(view)));
  skyboxShader.SetMat4("projection", projection);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
  basicCube.RenderCube();

  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);
}

void Framebuffer::SetIrradianceTexture(Shader pbrShader) {
  
  glActiveTexture(GL_TEXTURE4 + depthMapTextureKey);
  glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
  pbrShader.Use();
  pbrShader.SetInt("irradianceMap", 4 + depthMapTextureKey);


}

Framebuffer::~Framebuffer() {
  //delete buffers
  --count;
}

void Framebuffer::Test() {
  float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
       // positions   // texCoords
       -1.0f,  1.0f,  0.0f, 1.0f,
       -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

       -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
  };
  
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glBindVertexArray(quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}
