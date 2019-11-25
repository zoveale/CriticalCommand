#include "objects.h"
#include "graphics.h"
#include "physicsComponent.h"

GameObject::GameObject() {
}

GameObject::GameObject(GraphicsComponent* g, PhysicsComponent* p, InputComponent* i) :
  graphics(g), physics(p), input(i){
  dt = 0.0f;
  position = glm::vec3(0.0f, 1.0f, 0.0f);
  direction = glm::vec3(0.0f, -1.0f, 0.0f);
  velocity = 10.0f;
  acc = 2.0f;
  
  //TODO:: fix this workaround
  //Graphics needs to be first to set up proper position
  this->graphics->SetUp(*this);
  this->physics->SetUp(*this);

  modelMatrix = glm::mat4(1.0f);
}


void GameObject::Update(float dt, const glm::mat4 PV) {
  physics->Update(*this);
  graphics->Update(*this, PV);
}

void GameObject::Draw() {
  graphics->Draw();
}


