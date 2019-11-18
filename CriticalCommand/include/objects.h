#ifndef OBJECTS_H
#define OBJECTS_H

#include "graphics.h"


class GameObject {
public:
  GameObject();
  GameObject(std::string location);
  GameObject(GraphicsComponet* graphics, physx::Physics* physics) :
             graphics(graphics), physics(physics) {}

  void Update(float dt);
private:
  GraphicsComponet* graphics;
  physx::Physics* physics;

  
  float dt;
  float speed;
  float acc;
  glm::vec3 position;
};

class BombModel : public Model {
public:

};
class BombShaders : public Shader {
public:

};
class BombPhysics : public physx::Physics {
public:

};
#endif // !OBJECTS_H
