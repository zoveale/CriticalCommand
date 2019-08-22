#ifndef PHYSICS_H
#define PHYSICS_H

//#define _DEBUG
#include "PxConfig.h"
#include "PxPhysicsAPI.h"


//class PxAllocatorCallback {
//public:
//  virtual ~PxAllocatorCallback() {}
//  virtual void* allocate(size_t size, const char* typeName, const char* filename,
//    int line) = 0;
//  virtual void deallocate(void* ptr) = 0;
//};

//class UserErrorCallback : public physx::PxErrorCallback {
//public:
//  virtual void reportError(physx::PxErrorCode::Enum code,
//    const char* message, const char* file, int line) {}
//};

class Physics {
public:
  Physics();
  void StartUp();
  void AddActor(physx::PxActor* actor);
  void StepPhysics();
  
  void CleanUp();

private:

  physx::PxDefaultAllocator	gAllocator;
  physx::PxDefaultErrorCallback	gErrorCallback;

  physx::PxFoundation* gFoundation;
  physx::PxPhysics* gPhysics;

  physx::PxDefaultCpuDispatcher* gDispatcher;
  physx::PxScene* gScene;

  physx::PxMaterial* gMaterial;

  physx::PxPvd* gPvd;
};
#endif // !PHYSICS_H
