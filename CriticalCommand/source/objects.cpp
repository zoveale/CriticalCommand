#include "objects.h"
#include "graphics.h"
#include "physicsComponent.h"

GameObject::GameObject() {
  this->graphics = nullptr;
  this->physics = nullptr;
  this->input = nullptr;

  dt = 0.0f;
  position = glm::vec3(0.0f);
  direction = glm::vec3(0.0f, 0.0f, 0.0f);
  velocity = 0.0f;
  acc = 0.0f;
  modelMatrix = glm::mat4(1.0f);
}


void GameObject::Load(GraphicsComponent* g, PhysicsComponent* p, InputComponent* i){
  this->graphics = g;
  this->physics = p;
  this->input = i;

  dt = 0.0f;
  position = glm::vec3(0.0f);
  direction = glm::vec3(0.0f, -1.0f, 0.0f);
  velocity = 10.0f;
  acc = 2.0f;
  modelMatrix = glm::mat4(1.0f);
  //TODO:: fix this workaround
  //Graphics needs to be first to set up proper position
  this->graphics->SetUp(*this);
  this->physics->SetUp(*this);
}


void GameObject::Update(float dt, const glm::mat4 P, const glm::mat4 V) {
  physics->Update(*this);
  graphics->Update(*this, P, V);
}

void GameObject::Draw() {
  graphics->Draw();
}


