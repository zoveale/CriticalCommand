#ifndef VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H
#define VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H

#include "model.h"


#include "objects.h"
//class GameObject;

//
class BombGraphicsComponet;

class GraphicsComponet: public Model {
public:
  virtual ~GraphicsComponet() {}
  virtual void SetModelAndShaders() = 0;
  virtual void Update(GameObject &object, const glm::mat4 PV) = 0;
  virtual void Draw() = 0;
  
};




static LightFactory sceneLights;
static physx::Physics scenePhysics;
//static Model bombModel;

class BombGraphicsComponet : public GraphicsComponet {
public:
  BombGraphicsComponet(std::vector<Mesh> &bombMesh, Shader &shader) {

    for (int i = 0; i < 1; i++) {
      bombModelIdel.push_back(&bombMesh[i]);
    }
    

    
    bombShader = &shader;

    model = glm::mat4(1.0f);
  }
  virtual void SetModelAndShaders(){
    
  }

  virtual void Update(GameObject& object, const glm::mat4 PV) {
    //idel = model.meshes;
    model = glm::translate(model, object.position);
    bombShader->SetMat4("PVM", PV * model);
  }

  virtual void Draw() {
    bombShader->Use();
    //bombModelIdel->Draw(bombShader);
    for (unsigned int i = 0; i < bombModelIdel.size(); i++) {
      bombModelIdel[i]->Draw(*bombShader);
    }
    
  }

private:
  std::vector<Mesh*> bombModelIdel;
  Shader* bombShader;

  glm::mat4 model;
};


#endif // !VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H
