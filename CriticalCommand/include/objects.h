#ifndef VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H
#define VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H


#include "mat4x4.hpp"
#include "vec3.hpp"


class PhysicsComponent;
class GraphicsComponent;
class InputComponent;

//TODO::class of gameObjects that are only interactable through the physics class
//meaning: its position is only effected through force and gravity
//Probably rename DumbGameObject
class GameObject {
public:
  float dt;
  float velocity;
  float acc;
  glm::vec3 position;
  glm::vec3 direction;
  glm::mat4 modelMatrix;
  GameObject();
  //TODO:: future pointers
  void Load(GraphicsComponent* graphics, PhysicsComponent* physics, InputComponent* input = nullptr);
  void Update(float dt, const glm::mat4 P = glm::mat4(1.0f), const glm::mat4 V = glm::mat4(1.0f));
  void Draw();

private:
  GraphicsComponent* graphics;
  PhysicsComponent* physics;
  InputComponent* input;
};


#endif // !VEALE_89485D62_A5D9_499F_B124_717C2346DAAD_H
