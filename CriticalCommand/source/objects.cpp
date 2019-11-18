#include "objects.h"

GameObject::GameObject() {
}

void GameObject::Update(float dt) {
  graphics->Update(*this);
}

void GameObject::Draw(const glm::mat4 PV) {
  graphics->Draw(PV);
}


