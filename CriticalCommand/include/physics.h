#ifndef PHYSICS_H
#define PHYSICS_H

#include "PxPhysicsAPI.h"


class Physics {
  Physics();
  void StartUp();

private:

  physx::PxDefaultAllocator	gAllocator;
  physx::PxDefaultErrorCallback	gErrorCallback;

  physx::PxFoundation* gFoundation = NULL;
  physx::PxPhysics* gPhysics = NULL;

  physx::PxDefaultCpuDispatcher* gDispatcher = NULL;
  physx::PxScene* gScene = NULL;

  physx::PxMaterial* gMaterial = NULL;

  physx::PxPvd* gPvd = NULL;
};
#endif // !PHYSICS_H
