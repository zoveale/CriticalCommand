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

  //TODO:: get position data from game object
  //Perhaps a Virutal "SetUp" function
  index = root->AddDynamicSphereActor(glm::vec3(0.0f, 7.0f, 0.0f), 1.0f);
  }
  
  virtual void Update(GameObject& object) {
    object.modelMatrix = root->GetAPose(index);

    timer++;
    if (timer == 250) {
      //TODO:: when an actor gets released all other
      //actors get their component index messed up, because,
      //once the GetDynamicActors() is called it redistribute 
      //the actors in the array
      //root->ReleaseActor(index);
      //DisableActorSimulation is a work around for now,
      //does exatly what it sounds like
      root->DisableActorSimulation(index);

      root->ExplosionEffect(glm::vec3(
        object.modelMatrix[3][0],
        object.modelMatrix[3][1],
        object.modelMatrix[3][2]), 25.0f);
      //timer = 0;
    }
  }

private:
  physx::Physics* root;
  unsigned int index;
  int timer;
};


class IcoSpherePhysicsComponent : public PhysicsComponent{
public:
  IcoSpherePhysicsComponent(physx::Physics* rootPhysics) {
    root = rootPhysics;
    index = root->AddDynamicSphereActor(glm::vec3(4.0f), 2.0f);
  }
  virtual void Update(GameObject &object) {
    object.modelMatrix = root->GetAPose(index);
  }
private:
  physx::Physics* root;
  unsigned int index;
};

#endif // !VEALE_AA1FE13E_F758_453D_B604_EF8EF26A9833_H
