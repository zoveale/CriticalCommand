#include "objects.h"
#include "graphics.h"
#include "physicsComponent.h"

GameObject::GameObject() {
}

GameObject::GameObject(GraphicsComponent* graphics, PhysicsComponent* physics, InputComponent* input) :
  graphics(graphics), physics(physics), input(input){
  timer = 0;
  dt = 0.0f;
  position = glm::vec3(0.0f, 1.0f, 0.0f);
  direction = glm::vec3(0.0f, -1.0f, 0.0f);
  speed = 10.0f;
  acc = 2.0f;

  modelMatrix = glm::mat4(1.0f);
}


void GameObject::Update(float dt, const glm::mat4 PV) {
  physics->Update(*this);
  graphics->Update(*this, PV);
}

void GameObject::Draw() {
  graphics->Draw();
}


