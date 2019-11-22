#include "objects.h"
#include "graphics.h"

GameObject::GameObject() {
}

GameObject::GameObject(GraphicsComponent* graphics, physx::Physics* physics) :
  graphics(graphics), physics(physics){
  timer = 0;
  position = glm::vec3(0.0f, 1.0f, 0.0f);
  direction = glm::vec3(0.0f, -1.0f, 0.0f);
  speed = 10.0f;
  acc = 2.0f;
}


void GameObject::Update(float dt, const glm::mat4 PV) {
  
  //TODO:: TestFunctions will be moved to componets 
  //position += direction * (speed*dt + 0.5f*acc*acc*dt);// *acc;
  timer++;
  if (timer > 250 ) {
    timer = 0;
    /*speed = 0.0f;
    acc = 0.0f;*/
    //direction *= -1.0f;
    physics->ExplosionEffect(position, 25.0f, dt);
  }

  graphics->Update(*this, PV);
}

void GameObject::Draw() {
  graphics->Draw();
}


