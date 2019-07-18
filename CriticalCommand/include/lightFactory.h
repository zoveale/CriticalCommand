#ifndef LIGHTFACTORY_H
#define LIGHTFACTORY_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

class PointLight;
class SpotLight;

class LightFactory {
public:
  //FIXME:: make static
  std::vector<PointLight> pointLights;
  std::vector<SpotLight> spotLights;
  LightFactory();
  void AddLights(aiLight* light, aiNode* node);
  //virtual void Transformation();
};

class PointLight : LightFactory {
private:
  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  float constant;
  float linear;
  float quadratic;

public:
  PointLight();
  PointLight(aiLight* light, aiNode* node);

  //void Transformation();
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
  //SpotLight Data();
  //void Transformation();
};
#endif // !LIGHTFACTORY_H
