#include "physics.h"

physx::Physics::Physics() {
  gFoundation = NULL;
  gPhysics = NULL;
  gDispatcher = NULL;
  gScene = NULL;
  gMaterial = NULL;
  gPvd = NULL;
}

void physx::Physics::StartUp() {
  gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

  //Physx Visual Debugger TODO:: understand this better
  gPvd = PxCreatePvd(*gFoundation);
  physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
  gPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

  gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, gPvd);

  //Can make new scale
  //pass in new scale for cenitmeters 
  physx::PxTolerancesScale scale;
  scale.length = 100;
  scale.speed = 981;
  ///

  //we pass in defualt scale
  gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation,
    physx::PxTolerancesScale(), true, NULL);

  gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation,
    physx::PxCookingParams(physx::PxTolerancesScale()));

  //if (!gCooking) TODO::error class
    //fatalError("PxCreateCooking failed!");

  physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
  sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
  gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
  sceneDesc.cpuDispatcher = gDispatcher;
  sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
  gScene = gPhysics->createScene(sceneDesc);

 
  gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
  CreateStack(physx::PxTransform(physx::PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);
  physx::PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, physx::PxPlane(0, 1, 0, 0), *gMaterial);
  gScene->addActor(*groundPlane);
}

void physx::Physics::AddActor(physx::PxActor* actor) {
  gScene->addActor(*actor);
}

void physx::Physics::StepPhysics() {
  gScene->simulate(1.0f / 60.0f);
  gScene->fetchResults(true);
}



void physx::Physics::CleanUp() {
  gPhysics->release();
  gFoundation->release();
}


/*
TODO::remove test funcitons
*/

void physx::Physics::CreateStack(const physx::PxTransform& t,
                                physx::PxU32 size,
                                physx::PxReal halfExtent) {
  physx::PxShape* shape = gPhysics->createShape(physx::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
  for (physx::PxU32 i = 0; i < size; i++) {
    for (physx::PxU32 j = 0; j < size - i; j++) {
      physx::PxTransform localTm(physx::PxVec3(physx::PxReal(j * 2) - physx::PxReal(size - i), physx::PxReal(i * 2 + 1), 0) * halfExtent);
      physx::PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
      body->attachShape(*shape);
      physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
      gScene->addActor(*body);
    }
  }
  shape->release();
}