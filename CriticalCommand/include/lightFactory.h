#ifndef LIGHTFACTORY_H
#define LIGHTFACTORY_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "shader.h"

class PointLight;
class SpotLight;

class LightFactory {
private:
  
public:
  //FIXME:: make static
  static std::vector<PointLight> pointLights;
  static std::vector<SpotLight> spotLights;
  LightFactory();
  void AddLights(aiLight* light, aiNode* node);
  glm::vec3 GetPointLightPos(unsigned int i);
  glm::mat4 GetPointLightTransformation(unsigned int i);
  glm::vec3 GetSpotLightPos();
  unsigned int NumPointLights();
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
  void Draw(Shader shader, unsigned int i);
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
  float constant;
  float linear;
  float quadratic;
  //
  float innerCut;
  float outerCut;
public:
  SpotLight();
  SpotLight(aiLight* light, aiNode* node);
  void Draw(Shader shader, unsigned int i);
  glm::vec3 Position();
  //SpotLight Data();
  //void Transformation();
};
#endif // !LIGHTFACTORY_H
