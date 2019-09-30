#include "lightFactory.h"
#include "modelUtility.h"
std::vector <PointLight> LightFactory::pointLights;
std::vector <SpotLight> LightFactory::spotLights;

LightFactory::LightFactory() {
  
}

void LightFactory::AddLights(aiLight* light, aiNode* node) {

  if (light->mType == aiLightSource_POINT) {
    pointLights.push_back(PointLight(light, node));
  }
  else if (light->mType == aiLightSource_SPOT) {
    spotLights.push_back(SpotLight(light, node));
  }

}

glm::vec3 LightFactory::GetPointLightPos(unsigned int i) {
  return pointLights[i].Position();
}

glm::mat4 LightFactory::GetPointLightTransformation(unsigned int i) {
  return pointLights[i].Transformation();
}

glm::vec3 LightFactory::GetSpotLightPos(unsigned int i) {
  return spotLights[i].Position();
}

glm::mat4 LightFactory::GetSpotLightTransformation(unsigned int i) {
  return spotLights[i].Transformation();
}

unsigned int LightFactory::NumSpotLights() {
  return spotLights.size();
}

unsigned int LightFactory::NumPointLights() {
  return pointLights.size();
}

void LightFactory::SetDynamicAttributes(Shader shader) {
  for (unsigned int i = 0; i < pointLights.size(); i++) {
    pointLights[i].SetDynamicAttributes(shader, i);
  }
  for (unsigned int i = 0; i < spotLights.size(); i++) {
    spotLights[i].SetDynamicAttributes(shader, i);
  }
}

void LightFactory::SetFixedAttributes(Shader shader) {
  shader.SetUnsignedInt("numSpotLights", spotLights.size());
  shader.SetUnsignedInt("numPointLights", pointLights.size());

  for (unsigned int i = 0; i < pointLights.size(); i++) {
    pointLights[i].SetFixedAttributes(shader, i);
  }
  for (unsigned int i = 0; i < spotLights.size(); i++) {
    spotLights[i].SetFixedAttributes(shader, i);
  }
}

void LightFactory::Draw(Shader shader) {
  for (unsigned int i = 0; i < NumPointLights(); i++) {
    /*model = GetPointLightTransformation(i);
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    shader.SetMat4("PVM", projection * view * model);
    pointLamp.Draw(lamp);*/
  }
}

PointLight::PointLight() {

}

PointLight::PointLight(aiLight* light, aiNode* node) {
  transformation = aiToGlm(node->mTransformation);
  
  /*position = glm::vec3(-transformation[3][0],
                       transformation[3][2],
                      -transformation[3][1]);*/
  
  
  position = glm::vec3(transformation[3][0],
                       transformation[3][1],
                       transformation[3][2]);
  transformation = glm::mat4(1.0f);
  transformation[3][0] = position.x;
  transformation[3][1] = position.y;
  transformation[3][2] = position.z;
  //TODO:: Blender export rotates wrong way
  transformation = glm::rotate(transformation, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));

  ambient = glm::vec3(light->mColorAmbient.r,
                      light->mColorAmbient.g,
                      light->mColorAmbient.b);
  diffuse = glm::vec3(light->mColorDiffuse.r,
                      light->mColorDiffuse.g,
                      light->mColorDiffuse.b);
  specular = glm::vec3(light->mColorSpecular.r,
                       light->mColorSpecular.g,
                       light->mColorSpecular.b);

  //TODO:: light example in blender is much brighter than in opengl rendering
  diffuse /= 1000.0f;
  ambient /= 1000.0f;
  specular /= 1000.0f;

  constant = light->mAttenuationConstant;
  linear = light->mAttenuationLinear;
  quadratic = light->mAttenuationQuadratic;

}

void PointLight::SetFixedAttributes(Shader shader, unsigned int i) {
  std::string name = "pointLights[";
  name += std::to_string(i);
  name += "]";


  shader.SetVec3(name + ".ambient", this->ambient);
  shader.SetVec3(name + ".diffuse", this->diffuse);
  shader.SetVec3(name + ".specular", this->specular);
  shader.SetFloat(name + ".constant", this->constant);
  shader.SetFloat(name + ".linear", this->linear);
  shader.SetFloat(name + ".quadratic", this->quadratic);
}

void PointLight::SetDynamicAttributes(Shader shader, unsigned int i) {
  std::string name = "pointLights[";
  name += std::to_string(i);
  name += "]";
  
    shader.SetVec3(name + ".position", this->position);
}

glm::vec3 PointLight::Position() {
  return position;
}

glm::mat4 PointLight::Transformation() {
  return transformation;
}

SpotLight::SpotLight() {
}

SpotLight::SpotLight(aiLight* light, aiNode* node) {
  transformation = aiToGlm(node->mTransformation);

  position = glm::vec3(transformation[3][0],
                       transformation[3][1],
                       transformation[3][2]);
  //direction = glm::rotate(direction, glm::radians(90.0f), glm::vec3(1.0, 1.0, 0.0));
  //direction = glm::rotateX(direction, 90.0f);
  //TODO:: Blender export rotates wrong way
  transformation = glm::rotate(transformation, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
  direction = glm::vec3(transformation[2][0],
                        transformation[2][1],
                        transformation[2][2]);
  transformation = glm::rotate(transformation, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
  ///
  ambient = glm::vec3(light->mColorAmbient.r,
                      light->mColorAmbient.g,
                      light->mColorAmbient.b);
  diffuse = glm::vec3(light->mColorDiffuse.r,
                      light->mColorDiffuse.g,
                      light->mColorDiffuse.b);
  
  specular = glm::vec3(light->mColorSpecular.r,
                       light->mColorSpecular.g,
                       light->mColorSpecular.b);
  //TODO:: light example in blender is much brighter than in opengl rendering
  diffuse /= 1000.0f;
  specular /= 1000.0f;
  ambient /= 1000.0f;

  constant = light->mAttenuationConstant;
  linear = light->mAttenuationLinear;
  quadratic = light->mAttenuationQuadratic;
  
  //FIXME::Note creates neagative light space
 /* innerCut = glm::cos(light->mAngleInnerCone/1);
  outerCut = glm::cos(light->mAngleOuterCone/4);*/
  innerCut = glm::cos(light->mAngleInnerCone / 4);
  outerCut = glm::cos(light->mAngleOuterCone / 4);
}

void SpotLight::SetFixedAttributes(Shader shader, unsigned int i) {
  std::string name = "spotLights[";
  name += std::to_string(i);
  name += "]";

  shader.SetVec3(name + ".ambient", this->ambient);
  shader.SetVec3(name + ".diffuse", this->diffuse);
  shader.SetVec3(name + ".specular", this->specular);
  shader.SetFloat(name + ".constant", this->constant);
  shader.SetFloat(name + ".linear", this->linear);
  shader.SetFloat(name + ".quadratic", this->quadratic);
  shader.SetFloat(name + ".cutoff", this->innerCut);
  shader.SetFloat(name + ".outerCutoff", this->outerCut);
}

void SpotLight::SetDynamicAttributes(Shader shader, unsigned int i) {
  std::string name = "spotLights[";
  name += std::to_string(i);
  name += "]";

  shader.SetVec3(name + ".position", this->position);
  shader.SetVec3(name + ".direction", this->direction);

  

}

glm::vec3 SpotLight::Position() {
  return position;
}

glm::mat4 SpotLight::Transformation() {
  return transformation;
}
