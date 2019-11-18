#ifndef VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H
#define VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H



#include "graphics.h"
//class GraphicsComponet;

class GameObject {
public:
  float dt;
  float speed;
  float acc;
  glm::vec3 position;

  GameObject();
  //GameObject(std::string location);
  GameObject(GraphicsComponet* graphics, physx::Physics* physics) :
             graphics(graphics), physics(physics) {}

  void Update(float dt);
  void Draw(const glm::mat4 PV);

  
private:
  GraphicsComponet* graphics;
  physx::Physics* physics;
};


#endif // !VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H
