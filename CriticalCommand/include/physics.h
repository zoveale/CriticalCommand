#ifndef PHYSICS_H
#define PHYSICS_H

//#define _DEBUG
#include "PxConfig.h"
#include "PxPhysicsAPI.h"
#define PVD_HOST "127.0.0.1"

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
namespace physx { class Physics; }

class physx::Physics {
  
public:
  Physics();
  void StartUp();
  void AddActor(physx::PxActor* actor);
  void StepPhysics();
  
  void CleanUp();
  void CreateStack(const physx::PxTransform& t,
    PxU32 size, 
    physx::PxReal halfExtent);
private:

  //TODO:: make static
  physx::PxDefaultAllocator	gAllocator;
  physx::PxDefaultErrorCallback	gErrorCallback;


  physx::PxFoundation* gFoundation;
  physx::PxPhysics* gPhysics;
  physx::PxCooking* gCooking;

  physx::PxDefaultCpuDispatcher* gDispatcher;
  physx::PxScene* gScene;

  physx::PxMaterial* gMaterial;

  physx::PxPvd* gPvd;
  /*
  TODO:: remove test variables 
  */

  physx::PxReal stackZ = 10.0f;
};

#endif // !PHYSICS_H
