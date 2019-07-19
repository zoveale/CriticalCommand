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

glm::vec3 LightFactory::GetSpotLightPos() {
  return glm::vec3();
}

unsigned int LightFactory::NumPointLights() {
  return pointLights.size();
}

void LightFactory::Draw(Shader shader) {
  for (unsigned int i = 0; i < pointLights.size(); i++) {
    pointLights[i].Draw(shader, i);
  }
  for (unsigned int i = 0; i < spotLights.size(); i++) {
    spotLights[i].Draw(shader, i);
  }
}

PointLight::PointLight() {

}

PointLight::PointLight(aiLight* light, aiNode* node) {
  transformation = aiToGlm(node->mTransformation);

  /*position = glm::vec3(node->mTransformation.a4,
                       node->mTransformation.c4,
                      -node->mTransformation.b4);*/
  
  
  position = glm::vec3(node->mTransformation.a4,
                       node->mTransformation.b4,
                       node->mTransformation.c4);
  
  ambient = glm::vec3(light->mColorAmbient.r,
                      light->mColorAmbient.g,
                      light->mColorAmbient.b);
  diffuse = glm::vec3(light->mColorDiffuse.r,
                      light->mColorDiffuse.g,
                      light->mColorDiffuse.b);
  specular = glm::vec3(light->mColorSpecular.r,
                       light->mColorSpecular.g,
                       light->mColorSpecular.b);

  constant = light->mAttenuationConstant;
  linear = light->mAttenuationLinear;
  quadratic = light->mAttenuationQuadratic;

}

void PointLight::Draw(Shader shader, unsigned int i) {
  std::string name = "pointLights[";
  name += std::to_string(i);
  name += "]";
  
    shader.SetVec3(name + ".position", this->position);
    //shader.SetVec3("pointLights[0].position", glm::vec3(1.0f, 1.0f, 1.0f));

    //FIXME:: amdient values = vec3(0.0) for some reason
    //shader.SetVec3(name + ".ambient", this->ambient);
    //shader.SetVec3(name + ".ambient", 0.5f, 0.5f, 0.5f);

    shader.SetVec3(name + ".diffuse", this->diffuse);
    shader.SetVec3(name + ".specular", this->specular);
    shader.SetFloat(name + ".constant", this->constant);
    shader.SetFloat(name + ".linear", this->linear);
    shader.SetFloat(name + ".quadratic", this->quadratic);
  
   
    /*if(i == 1){
      shader.SetVec3(name + ".ambient", 0.0f, 0.0f, 0.5f);
    }
    else{ 
      shader.SetVec3(name + ".ambient", 0.5f, 0.0f, 0.0f); }*/
    
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
  
  position = glm::vec3(node->mTransformation.a4,
                       node->mTransformation.b4,
                       node->mTransformation.c4);
  ambient = glm::vec3(light->mColorAmbient.r,
                      light->mColorAmbient.g,
                      light->mColorAmbient.b);
  diffuse = glm::vec3(light->mColorDiffuse.r,
                      light->mColorDiffuse.g,
                      light->mColorDiffuse.b);
  specular = glm::vec3(light->mColorSpecular.r,
                       light->mColorSpecular.g,
                       light->mColorSpecular.b);

  constant = light->mAttenuationConstant;
  linear = light->mAttenuationLinear;
  quadratic = light->mAttenuationQuadratic;

  innerCut = light->mAngleInnerCone;
  outerCut = light->mAngleOuterCone;
}

void SpotLight::Draw(Shader shader, unsigned int i) {
}
