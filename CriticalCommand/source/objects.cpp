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
  position = glm::vec3(FLT_MAX);

  up = glm::vec3(0.0f, 1.0f, 0.0f);
  right = glm::vec3(1.0f, 0.0f, 0.0f);
  front = glm::cross(glm::normalize(right), glm::normalize(up));
  direction = glm::cross(glm::normalize(right), glm::normalize(up));
  //deltaDirection = glm::cross(glm::normalize(right), glm::normalize(up));
  velocity = 10.0f;
  acc = 2.0f;
  modelMatrix = glm::mat4(1.0f);
  initalVelocity = glm::vec3(0.0f);
  intialRotation = glm::vec3(0.0f);
}


void GameObject::Load(GraphicsComponent* g, PhysicsComponent* p, InputComponent* i) {
  graphics = g;
  physics = p;
  input = i;

  //TODO:: fix this workaround?? or its a feature 
  //Graphics needs to be first to set up proper position
  graphics->SetUp(*this);
  physics->SetUp(*this);

  if (input != nullptr) {
    input->SetUp(*this);
  }
}


void GameObject::Update(float dt, const glm::mat4 P, const glm::mat4 V) {
  if (input != nullptr) {
    deltaTime = dt;
    input->Update(*this);
  }

  physics->Update(*this);
  graphics->Update(*this, P, V);
}

void GameObject::Draw() {
  graphics->Draw(*this);
}


