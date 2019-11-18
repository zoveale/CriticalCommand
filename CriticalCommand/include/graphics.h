#ifndef VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H
#define VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H

#include "model.h"


//#include "objects.h"
//class GameObject;

//
class BombGraphicsComponet;

class GraphicsComponet {
public:
  virtual ~GraphicsComponet() {}
  virtual void SetModelAndShaders() = 0;
  virtual void Update(GameObject &object/*, Model &objectModel*/) = 0;
  virtual void Draw(const glm::mat4 PV) = 0;

  static BombGraphicsComponet bombGraphicsComponet;
};

class BombGraphicsComponet : public GraphicsComponet {
public:
  virtual void SetModelAndShaders();
  virtual void Update(GameObject& object/*, Model &objectModel*/);
  virtual void Draw(const glm::mat4 PV);

private:
  std::vector<Mesh> idel;
  Shader bombShader;

  glm::mat4 model;
};


#endif // !VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H
