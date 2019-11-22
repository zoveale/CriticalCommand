#ifndef VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H
#define VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H


#include "mat4x4.hpp"
#include "vec3.hpp"


class PhysicsComponent;
class GraphicsComponent;

class GameObject {
public:
  float dt;
  float speed;
  float acc;
  float size;
  glm::vec3 position;
  glm::vec3 direction;

  GameObject();

  //TODO:: future pointers
  //input
  GameObject(GraphicsComponent* graphics, PhysicsComponent* physics);

  void Update(float dt, const glm::mat4 PV = glm::mat4(1.0f));
  void Draw();

  
private:
  GraphicsComponent* graphics;
  PhysicsComponent* physics;

  int timer;
};


#endif // !VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H
