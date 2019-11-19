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
  BombGraphicsComponent(Model &bombModel, Shader &shader) {
    bombModelIdel = &bombModel;
    bombShader = &shader;

    model = glm::mat4(1.0f);
  }
  virtual void SetModelAndShaders(){}

  virtual void Update(GameObject& object, const glm::mat4 PV = glm::mat4(1.0f)) {

    model = glm::translate(model, object.position);

    bombShader->Use();
    bombShader->SetMat4("PVM", PV * model);
  }

  virtual void Draw() {
    bombShader->Use();
    bombModelIdel->Draw(*bombShader);
  }

private:

  Model* bombModelIdel;
  
  Shader* bombShader;

  glm::mat4 model;
};


#endif // !VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H
