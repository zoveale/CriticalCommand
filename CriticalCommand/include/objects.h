#ifndef VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H
#define VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H


#include "mat4x4.hpp"
#include "vec3.hpp"

//#include "physics.h"
//class physx::Physics;
namespace physx {
  class Physics;
}
class GraphicsComponet;
//class BombGraphicsComponet;

//static BombGraphicsComponet bombGraphicsComponet;
class GameObject {
public:
  float dt;
  float speed;
  float acc;
  glm::vec3 position;

  GameObject();
  //GameObject(std::string location);
  /*GameObject(GraphicsComponet* graphics, physx::Physics* physics) :
             graphics(graphics), physics(physics) {}*/
  GameObject(GraphicsComponet* graphics);

  //GameObject(GraphicsComponet* graphics);

  void Update(float dt, const glm::mat4 PV);
  void Draw();

  
private:
  GraphicsComponet* graphics;
  physx::Physics* physics;
};


#endif // !VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H
