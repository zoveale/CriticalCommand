#ifndef LIGHTFACTORY_H
#define LIGHTFACTORY_H

#include <glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/rotate_vector.hpp>
#include <gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "shader.h"

class PointLight;
class SpotLight;

extern glm::mat4 aiToGlm(const aiMatrix4x4& load);

class LightFactory {
private:
  
public:
  //TODO:: testing component system,
  static LightFactory sceneLights;
  //FIXME:: make static
  static std::vector<PointLight> pointLights;
  static std::vector<SpotLight> spotLights;
  LightFactory();
  void AddLights(aiLight* light, aiNode* node);
  glm::vec3 GetPointLightPos(unsigned int i);
  glm::mat4 GetPointLightTransformation(unsigned int i);
  glm::vec3 GetPointLightColor(unsigned int i);
  unsigned int NumPointLights();

  glm::vec3 GetSpotLightPos(unsigned int i);
  glm::mat4 GetSpotLightTransformation(unsigned int i);
  glm::vec3 GetSpotLightColor(unsigned int i);
  unsigned int NumSpotLights();
  
  void SetDynamicAttributes(Shader shader);
  void SetFixedAttributes(Shader shader);
  void Draw(Shader shader);
  //virtual void Transformation();
};

class PointLight : LightFactory {
private:
  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  glm::mat4 transformation;
  float constant;
  float linear;
  float quadratic;

public:
  PointLight();
  PointLight(aiLight* light, aiNode* node);
  void SetFixedAttributes(Shader shader, unsigned int i);
  void SetDynamicAttributes(Shader shader, unsigned int i);
  glm::vec3 DiffuseColor();
  glm::vec3 Position();
  glm::mat4 Transformation();
};

class SpotLight : LightFactory {
private:
  glm::vec3 position;
  glm::vec3 direction;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  glm::mat4 transformation;
  float constant;
  float linear;
  float quadratic;
  //
  float innerCut;
  float outerCut;
public:
  SpotLight();
  SpotLight(aiLight* light, aiNode* node);
  void SetFixedAttributes(Shader shader, unsigned int i);
  void SetDynamicAttributes(Shader shader, unsigned int i);
  glm::vec3 DiffuseColor();
  glm::vec3 Position();
  glm::mat4 Transformation();
  //SpotLight Data();
  //void Transformation();
};
#endif // !LIGHTFACTORY_H
