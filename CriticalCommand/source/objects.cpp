#include "objects.h"
#include "graphics.h"

GameObject::GameObject() {
}

GameObject::GameObject(GraphicsComponet* graphics) :
  graphics(graphics) {

  graphics->SetModelAndShaders();
}


void GameObject::Update(float dt, const glm::mat4 PV) {
  graphics->Update(*this, PV);
}

void GameObject::Draw() {
  graphics->Draw();
}


