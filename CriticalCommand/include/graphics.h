#ifndef VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H
#define VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H

#include "model.h"


#include "objects.h"

class BombGraphicsComponent;
//TODO:: might need to have model be a public class to Grpahics component
class GraphicsComponent/*: public Model*/ {
public:
  virtual ~GraphicsComponent() {}
  virtual void SetModelAndShaders() = 0;
  virtual void Update(GameObject &object, const glm::mat4 PV) = 0;
  virtual void Draw() = 0;
  
};

static LightFactory sceneLights;
static physx::Physics scenePhysics;

class BombGraphicsComponent : public GraphicsComponent {
public:
  BombGraphicsComponent(std::vector<Model*> &bombModel, Shader &shader) {
    //bombModelIdel[0] = &bombModel;
    models = bombModel;
    bombShader = &shader;
    timer = 0.0f;
    model = glm::mat4(1.0f);
    bombModel_ = models[0];
  }
  virtual void SetModelAndShaders(){}

  virtual void Update(GameObject& object, const glm::mat4 PV = glm::mat4(1.0f)) {
    model[3][0] = object.position.x;
    model[3][1] = object.position.y;
    model[3][2] = object.position.z;
    //model *= object.position;
    if (timer > 20.0f) {
      bombModel_ = models[1];
    }
    bombShader->Use();
    bombShader->SetMat4("PVM", PV * model);
  }

  virtual void Draw() {
    timer += 0.1f;
    
    bombShader->Use();
    bombShader->SetFloat("iTime", timer);
    bombShader->SetVec2("iResolution", 1280, 720);
    bombShader->SetVec2("iMouse", 1280/2, 720/2);

    bombModel_->Draw(*bombShader);
  }

private:
  float timer;
  Model* bombModel_;
  std::vector<Model*> models;
  Shader* bombShader;

  glm::mat4 model;
};


#endif // !VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H
