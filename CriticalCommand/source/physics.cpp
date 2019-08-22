#include "physics.h"

Physics::Physics() {
  gFoundation = NULL;
  gPhysics = NULL;
  gDispatcher = NULL;
  gScene = NULL;
  gMaterial = NULL;
  gPvd = NULL;
}

void Physics::StartUp() {
  gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

  //Physx Visual Debugger TODO:: understand this better
  //gPvd = PxCreatePvd(*gFoundation);

  gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation,
    physx::PxTolerancesScale(), true, NULL);

  physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
  sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
  gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
  sceneDesc.cpuDispatcher = gDispatcher;
  sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
  gScene = gPhysics->createScene(sceneDesc);

 
  gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

  physx::PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, physx::PxPlane(0, 1, 0, 0), *gMaterial);
  gScene->addActor(*groundPlane);
}

void Physics::AddActor(physx::PxActor* actor) {
  gScene->addActor(*actor);
}

void Physics::StepPhysics() {
  gScene->simulate(1.0f / 60.0f);
  gScene->fetchResults(true);
}



void Physics::CleanUp() {
  
}
