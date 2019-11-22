#ifndef VEALE_AA1FE13E_F758_453D_B604_EF8EF26A9833_H
#define VEALE_AA1FE13E_F758_453D_B604_EF8EF26A9833_H

#include "physics.h"


class PhysicsComponent{
public:
  virtual ~PhysicsComponent() {}
  virtual void Update(GameObject& object) = 0;
};


class BombPhysicsComponent : public PhysicsComponent {
public:
  BombPhysicsComponent(physx::Physics* rootPhysics) {
  root = rootPhysics;
  timer = 0;
  }
  
  virtual void Update(GameObject& object) {
    timer++;
    if (timer == 250) {
      root->ExplosionEffect(object.position, 25.0f);
      timer = 0;
    }
  }

private:
  physx::Physics* root;
  int timer;
};

#endif // !VEALE_AA1FE13E_F758_453D_B604_EF8EF26A9833_H
