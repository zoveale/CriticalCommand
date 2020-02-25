#ifndef VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H
#define VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H

#include "model.h"
#include "objects.h"

class GraphicsComponent {
public:

  GraphicsComponent() {}
  virtual ~GraphicsComponent() {}
  virtual void Load(Model* modelPointers, Shader* shader, LightFactory* lightContainer) = 0;
  virtual void Update(GameObject &object, const glm::mat4 P, const glm::mat4 V) = 0;
  virtual void SetUp(GameObject& object) = 0;
  virtual void Draw() = 0;
  
  
};

//class BombGraphicsComponent : public GraphicsComponent {
//public:
//
////TODO::make less awful
//  virtual void Load(std::vector<Model*> modelPointers, Shader* shader) {
//    this->modelPointers = modelPointers;
//    this->bombShader = shader;
//    timer = 0.0f;
//    bombModel = this->modelPointers[0]; 
//  }
//  virtual void SetUp(GameObject& object) {
//    object.position = modelPointers[0]->Position();
//  }
//
//  virtual void Update(GameObject& object, const glm::mat4 PV = glm::mat4(1.0f)) {
//    timer += 0.1f;
//    if (timer > 25.0f) {
//      //bombModel = modelPointers[1];
//    }
//    else {
//      //object.modelMatrix = glm::translate(object.modelMatrix, -object.position);
//    }
//    bombShader->Use();
//    bombShader->SetMat4("PVM", PV * object.modelMatrix);
//    bombShader->SetFloat("iTime", (timer - 30.0f) * 0.25f);
//    bombShader->SetFloat("timer", timer);
//    //bombShader->SetVec2("iResolution", 1280, 720);
//    //bombShader->SetVec2("iMouse", 0, 0);
//  }
//
//  virtual void Draw() {
//    bombModel->Draw(*bombShader);
//  }
//
//private:
//  Model* bombModel;
//  Shader* bombShader;
//  std::vector<Model*> modelPointers;
//  float timer;
//};


class IcoSphereGraphicsComponent : public GraphicsComponent {
public:
  IcoSphereGraphicsComponent():modelPointer(nullptr), icoShader(nullptr), lights(nullptr) {}

  virtual void Load(Model* modelPointers, Shader* shader, LightFactory* lightContainer = nullptr) {
    modelPointer = modelPointers;
    icoShader = shader;
    lights = lightContainer;
    model = glm::mat4(1.0f);
  }
  virtual void SetUp(GameObject& object) {
    object.position = modelPointer->Position();
  }
  virtual void Update(GameObject& object, const glm::mat4 P, const glm::mat4 V) {
    model = object.modelMatrix;
    //object.modelMatrix = glm::translate(object.modelMatrix, object.position);
    /*icoShader->Use();
    icoShader->SetMat4("model", object.modelMatrix);
    icoShader->SetMat4("view", V);
    icoShader->SetMat4("projection", P);*/
    //icoShader->SetMat4("PVM", PV * object.modelMatrix);
    //lights->SetDynamicAttributes(*icoShader);
  }

  virtual void Draw() {
    icoShader->Use();
    icoShader->SetMat4("model", model);
    icoShader->SetMat4("inverseModel", glm::inverse(model));
    modelPointer->Draw(*icoShader);
  }

private:
  Model* modelPointer;
  Shader* icoShader;
  LightFactory* lights;
  glm::mat4 model;
};


class PlayerGraphicsComponent : public GraphicsComponent {
public:
  PlayerGraphicsComponent() : modelData(nullptr), shader(nullptr),
                              lights(nullptr), model(glm::mat4(1.0f)) {}

  virtual void Load(Model* m, Shader* s, LightFactory* l = nullptr) {
    modelData = m;
    shader = s;
    lights = l;
    model = glm::mat4(1.0f);
  }
  virtual void SetUp(GameObject& object) {
    object.position = glm::vec3(1.0f);
    object.modelMatrix = glm::translate(glm::mat4(1.0f), object.position);
  } 
  virtual void Update(GameObject& object, const glm::mat4 P, const glm::mat4 V) {
    model = object.modelMatrix;
    model = glm::translate(model, object.position);
    //object.modelMatrix = model;
  }

  virtual void Draw() {
    shader->Use();
    shader->SetMat4("model", model);
    shader->SetMat4("inverseModel", glm::inverse(model));
    modelData->Draw(*shader);
  }

private:
  Model* modelData;
  Shader* shader;
  LightFactory* lights;
  glm::mat4 model;
};

class DefaultGraphicsComponent : public GraphicsComponent {
public:
  DefaultGraphicsComponent() : modelData(nullptr), shader(nullptr), lights(nullptr) {}

  virtual void Load(Model* m, Shader* s, LightFactory* l = nullptr) {
    modelData = m;
    shader = s;
    lights = l;
    model = glm::mat4(1.0f);
  }
  virtual void SetUp(GameObject& object) {
    object.position = modelData->Position();
  }
  virtual void Update(GameObject& object, const glm::mat4 P, const glm::mat4 V) {
    model = object.modelMatrix;
  }

  virtual void Draw() {
    shader->Use();
    shader->SetMat4("model", model);
    shader->SetMat4("inverseModel", glm::inverse(model));
    modelData->Draw(*shader);
  }

private:
  Model* modelData;
  Shader* shader;
  LightFactory* lights;
  glm::mat4 model;
};
#endif // !VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H
