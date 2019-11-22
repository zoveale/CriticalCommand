#ifndef VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H
#define VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H


#include "mat4x4.hpp"
#include "vec3.hpp"

namespace physx {
  class Physics;
}
class GraphicsComponent;

class GameObject {
public:
  float dt;
  float speed;
  float acc;
  glm::vec3 position;
  glm::vec3 direction;

  GameObject();
  //GameObject(GraphicsComponent* graphics);

  //TODO:: future pointers
  GameObject(GraphicsComponent* graphics, physx::Physics* physics);

  void Update(float dt, const glm::mat4 PV = glm::mat4(1.0f));
  void Draw();

  
private:
  GraphicsComponent* graphics;
  physx::Physics* physics;

  int timer;
};


#endif // !VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H
