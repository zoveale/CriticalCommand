#ifndef VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H
#define VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H

#include "mat4x4.hpp"
#include "vec3.hpp"

class PhysicsComponent;
class GraphicsComponent;
class InputComponent;

class GameObject {
public:
  float deltaTime;
  float velocity;
  float acc;
  glm::vec3 front;
  glm::vec3 right;
  //Starting position of an object
  glm::vec3 position;
  glm::vec3 direction;
  glm::vec2 look;
  glm::mat4 modelMatrix;
  //Starting velocity of an object
  glm::vec3 initalVelocity;
  //Starting rotational velocity of an object
  glm::vec3 intialRotation;
  unsigned int index;
  GameObject();
  void Load(GraphicsComponent *graphics = nullptr,
            PhysicsComponent  *physics  = nullptr,
            InputComponent    *input    = nullptr);
  void Update(float dt,
              const glm::mat4 P = glm::mat4(1.0f),
              const glm::mat4 V = glm::mat4(1.0f));
  void Draw();

private:
  GraphicsComponent *graphics;
  PhysicsComponent  *physics;
  InputComponent    *input;
};

//protected:
//  GameObject* down;
//  GameObject* branch;
//class NodeBasedGameObject : public GameObject {
//  NodeBasedGameObject();
//  void Load(GameObject** gameObjects) {
//  }
//  void AddDownNode(GameObject* node) {
//  }
//};

#endif // !VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H
