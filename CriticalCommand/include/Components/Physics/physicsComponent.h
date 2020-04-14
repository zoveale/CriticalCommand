#ifndef VEALE_AA1FE13E_F758_453D_B604_EF8EF26A9833_H
#define VEALE_AA1FE13E_F758_453D_B604_EF8EF26A9833_H

#include "physics.h"
#include "objects.h"

class PhysicsComponent{
public:
  PhysicsComponent() {}
  virtual ~PhysicsComponent() {}
  virtual void Update(GameObject& object) = 0;
  virtual void SetUp(GameObject& object) = 0;
};


class BombPhysicsComponent : public PhysicsComponent {
public:
  BombPhysicsComponent(physx::Physics* rootPhysics):root(rootPhysics), index(0), timer(0){}

  virtual void SetUp(GameObject& object) {
    index = root->AddDynamicSphereActor(object.position, 1.5f);
  }
  
  virtual void Update(GameObject& object) {
    object.modelMatrix = root->GetAPose(index);
  //  timer++;
  //  if (timer == 250) {
  //    //TODO:: when an actor gets released all other
  //    //actors get their component index messed up, because,
  //    //once the GetDynamicActors() is called it redistribute 
  //    //the actors in the array
  //    //root->ReleaseActor(index);
  //    //DisableActorSimulation is a work around for now,
  //    //does exatly what it sounds like
  //    //root->DisableActorSimulation(index);
  //    root->ExplosionEffect(glm::vec3(
  //      object.modelMatrix[3][0],
  //      object.modelMatrix[3][1],
  //      object.modelMatrix[3][2]), 55.0f);
  //    //timer = 0;
  //  }
  }

private:
  physx::Physics* root;
  unsigned int index;
  int timer;
};



class IcoSpherePhysicsComponent : public PhysicsComponent{
public:
  IcoSpherePhysicsComponent() : root(nullptr) {}

  void Load(physx::Physics* rootPhysics) {
    this->root = rootPhysics;
  }
  virtual void SetUp(GameObject& object) {
    object.index = root->AddDynamicSphereActor(object.position, 2.0f, object.initalVelocity, object.intialRotation);
    object.modelMatrix = root->GetAPose(object.index);
  }

  virtual void Update(GameObject &object) {
    object.modelMatrix = root->GetAPose(object.index);
  }
private:
  physx::Physics* root;
};


class CubePhysicsComponent : public PhysicsComponent {
public:
  CubePhysicsComponent() : root(nullptr) {}

  void Load(physx::Physics* rootPhysics) {
    this->root = rootPhysics;
  }
  virtual void SetUp(GameObject& object) {
    object.index = root->AddDynamicBoxActor(object.position, glm::vec3(1.0f));
  }

  virtual void Update(GameObject& object) {
    object.modelMatrix = root->GetAPose(object.index);
  }
private:
  physx::Physics* root;
};

class DiamondPhysicsComponent : public PhysicsComponent {
public:
  DiamondPhysicsComponent() : root(nullptr) {}

  void Load(physx::Physics* rootPhysics) {
    this->root = rootPhysics;
  }
  virtual void SetUp(GameObject& object) {
    object.index = root->AddLoadedDynamicConvexMesh("resources/imagedBasedLighting/object1/cubeConvex.dae", object.position);
  }

  virtual void Update(GameObject& object) {
    object.modelMatrix = root->GetAPose(object.index);
  }
private:
  physx::Physics* root;
};

class PlayerPhysicsComponent : public PhysicsComponent {
public:
  PlayerPhysicsComponent() : root(nullptr), index(unsigned int(0)) {}

  void Load(physx::Physics* rootPhysics) {
    this->root = rootPhysics;
    index = 0;
  }
  virtual void SetUp(GameObject& object) {
    index = root->CreateKinematicController(object.position);
  }

  virtual void Update(GameObject& object) {
    root->SetKinematicControllerPosition(object.position, object.deltaTime);
    object.modelMatrix = root->GetAPose(index);
  }
private:
  physx::Physics* root;
  unsigned int index;
};

class DefaultPhysicsComponent : public PhysicsComponent {
public:
  DefaultPhysicsComponent() : root(nullptr), index(unsigned int(0)) {}

  void Load(physx::Physics* rootPhysics) {
  }
  virtual void SetUp(GameObject& object) {
  }

  virtual void Update(GameObject& object) {
  }
private:
  physx::Physics* root;
  unsigned int index;
};

class ConvexPhysicsComponent : public PhysicsComponent {
public:
  ConvexPhysicsComponent() : root(nullptr), convexDataFileLocation(""){}

  void Load(physx::Physics* rootPhysics, std::string convexDataFileLocation) {
    this->root = rootPhysics;
    this->convexDataFileLocation = convexDataFileLocation;
   
  }
  virtual void SetUp(GameObject& object) {
    object.index = root->AddLoadedDynamicConvexMesh(convexDataFileLocation.c_str(), object.position);
    //root->DisableActorGravity(object.index);
  }

  virtual void Update(GameObject& object) {
    object.modelMatrix = root->GetAPose(object.index);
  }
private:
  physx::Physics* root;
  std::string convexDataFileLocation;

};



#endif // !VEALE_AA1FE13E_F758_453D_B604_EF8EF26A9833_H
