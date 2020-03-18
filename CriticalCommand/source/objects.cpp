#include "objects.h"

#include "../include/Components/Graphics/graphicsComponent.h"
#include "../include/Components/Physics/physicsComponent.h"
#include "../include/Components/Input/inputComponent.h"

GameObject::GameObject() {
  this->graphics = nullptr;
  this->physics = nullptr;
  this->input = nullptr;
  look = glm::vec2(0.0f);
  deltaTime = 0.0f;
  front = glm::vec3(0.0f);
  position = glm::vec3(0.0f);
  direction = glm::vec3(0.0f, 0.0f, 0.0f);
  velocity = 0.0f;
  acc = 0.0f;
  modelMatrix = glm::mat4(1.0f);
  initalVelocity = glm::vec3(0.0f);
  intialRotation = glm::vec3(0.0f);
}


void GameObject::Load(GraphicsComponent* g, PhysicsComponent* p, InputComponent* i) {
  graphics = g;
  physics = p;
  input = i;
  if (input == nullptr) {
    input = new DefaultInputComponent();
  }
 
  deltaTime = 0.0f;
  right = glm::vec3(0.0f, 0.0f, 1.0f);
  //position = glm::vec3(0.0f);
  direction = glm::vec3(0.0f, -1.0f, 0.0f);
  front = glm::vec3(1.0f, 0.0f, 0.0f);
  velocity = 10.0f;
  acc = 2.0f;
  modelMatrix = glm::mat4(1.0f);

  //TODO:: fix this workaround?? or its a feature 
  //Graphics needs to be first to set up proper position
  graphics->SetUp(*this);
  physics->SetUp(*this);
  input->SetUp(*this);
}


void GameObject::Update(float dt, const glm::mat4 P, const glm::mat4 V) {
  deltaTime = dt;
  input->Update(*this);
  physics->Update(*this);
  graphics->Update(*this, P, V);
}

void GameObject::Draw() {
  graphics->Draw();
}


