#include "objects.h"
#include "graphics.h"

GameObject::GameObject() {
}

GameObject::GameObject(GraphicsComponent* graphics, physx::Physics* physics) :
  graphics(graphics), physics(physics){
  timer = 0;
  position = glm::vec3(0.0f);
}


void GameObject::Update(float dt, const glm::mat4 PV) {
  timer++;
  if (timer > 200 ) {
    timer = 0;
    physics->ExplosionEffect(position, 25.0f, dt);
    //physics->ExplosionEffect(position, 25.0f, dt);
  }

  graphics->Update(*this, PV);
}

void GameObject::Draw() {
  graphics->Draw();
}


