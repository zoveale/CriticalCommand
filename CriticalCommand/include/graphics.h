#ifndef VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H
#define VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H

#include "model.h"
#include "objects.h"


class GraphicsComponent {
public:

  virtual ~GraphicsComponent() {}
  virtual void Update(GameObject &object, const glm::mat4 PV) = 0;
  virtual void Draw() = 0;
  
};

class BombGraphicsComponent : public GraphicsComponent {
public:

//TODO::make less awful
  BombGraphicsComponent(std::vector<Model*> &modelPointers, Shader &shader) {
    this->modelPointers = modelPointers;
    bombShader = &shader;
    timer = 0.0f;
    model = glm::mat4(1.0f);

    bombModel = this->modelPointers[0];
  }
  virtual void Update(GameObject& object, const glm::mat4 PV = glm::mat4(1.0f)) {
    timer += 0.1f;
    if (timer > 20.0f) {
      //bombModel = modelPointers[1];
    }
    bombShader->Use();
    bombShader->SetMat4("PVM", PV * object.modelMatrix);
    
  }

  virtual void Draw() {
    
    
    bombShader->Use();
    bombShader->SetFloat("iTime", timer);
    bombShader->SetFloat("timer", timer);
    bombShader->SetVec2("iResolution", 1280, 720);
    bombShader->SetVec2("iMouse", 1280/2, 720/2);

    bombModel->Draw(*bombShader);
  }

private:
  float timer;
  Model* bombModel;
  std::vector<Model*> modelPointers;
  Shader* bombShader;

  glm::mat4 model;
};

class IcoSphereGraphicsComponent : public GraphicsComponent {
public:
  IcoSphereGraphicsComponent(Model* modelPointers, Shader* shader, LightFactory* lightContainer = nullptr) {
    modelPointer = modelPointers;
    icoShader = shader;
    lights = lightContainer;
    model = glm::mat4(1.0f);
  }

  virtual void Update(GameObject& object, const glm::mat4 PV) {
    icoShader->Use();
    icoShader->SetMat4("model", object.modelMatrix);
    icoShader->SetMat4("PVM", PV * object.modelMatrix);
    lights->SetDynamicAttributes(*icoShader);
  }

  virtual void Draw() {
    //icoShader->Use();
    modelPointer->Draw(*icoShader);
  }

private:
  Model* modelPointer;
  Shader* icoShader;
  LightFactory* lights;
  glm::mat4 model;
};

#endif // !VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H
