#include "lightFactory.h"

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

PointLight::PointLight() {
}

PointLight::PointLight(aiLight* light, aiNode* node) {
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
