#ifndef VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H
#define VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H

#include "model.h"
#include "objects.h"


//class BombGraphicsComponent;
//TODO:: might need to have model be a public class to Grpahics component
class GraphicsComponent/*: public Model*/ {
public:
  virtual ~GraphicsComponent() {}
  virtual void Update(GameObject &object, const glm::mat4 PV) = 0;
  virtual void Draw() = 0;
  
};

class BombGraphicsComponent : public GraphicsComponent {
public:

//TODO::make less awful
//Step physics in normal positon
//only one shape
  BombGraphicsComponent(std::vector<Model*> &modelPointers, Shader &shader) {
    //bombModelIdel[0] = &bombModel;
    this->modelPointers = modelPointers;
    bombShader = &shader;
    timer = 0.0f;
    model = glm::mat4(1.0f);

    bombModel = this->modelPointers[0];
  }
  virtual void Update(GameObject& object, const glm::mat4 PV = glm::mat4(1.0f)) {
    model[3][0] = object.position.x;
    model[3][1] = object.position.y;
    model[3][2] = object.position.z;
    if (timer > 20.0f) {
      bombModel = modelPointers[1];
    }

    bombShader->Use();
    bombShader->SetMat4("PVM", PV * model);
  }

  virtual void Draw() {
    timer += 0.1f;
    
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


#endif // !VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H
