#ifndef PHYSICS_H
#define PHYSICS_H

//#define _DEBUG 
///added to project config
#include <iostream>
#include <vector>

#include <geometric.hpp>
#include <vec3.hpp>
#include <mat4x4.hpp>




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

#define MAX_ACTOR 1<<8
namespace physx { class Physics; }

static const physx::PxU32 GRID_SIZE = 8;
static const physx::PxReal GRID_STEP = 56.0f / physx::PxReal(GRID_SIZE - 1);

class physx::Physics {

public:
  Physics();
  void StartUp();
  void AddActor(PxActor* actor);
  void GetActors();
  void StepPhysics();

  void CleanUp();


  void AddStaticTriangleMesh(
    const std::vector<float>        vertex,
    const std::vector<unsigned int> indices,
    const unsigned int              indicesSize);

  
  //TODO:: test functions
  void CreateStack(const PxTransform& t,
    PxU32 size,
    PxReal halfExtent);
  void AddCubeActor(glm::vec3 pos, float scale = 1.0f);
  //glm::vec3 GetAPosition(int i);
  glm::mat4 GetAPose(int i);
  void ShootBall(glm::vec3 front, glm::vec3 pos);
  static PxTriangleMesh* createMeshGround();
  static void updateVertices(PxVec3* verts, float amplitude);

private:
  PxTriangleMesh* CreateTriangleMesh(
    const std::vector<float>        vertex,
    const std::vector<unsigned int> indices,
    const unsigned int              numFaces);
  //TODO:: remove static
  static PxDefaultAllocator	gAllocator;
  static PxDefaultErrorCallback	gErrorCallback;



  static PxFoundation* gFoundation;
  static PxPhysics* gPhysics;
  static PxCooking* gCooking;

  PxDefaultCpuDispatcher* gDispatcher;
  PxScene* gScene;

  PxMaterial* defaultMaterial;

  PxPvd* gPvd;
  /*
  TODO:: remove test variables
  */
  static PxMat44 globalPoseArray[MAX_ACTOR];
  static PxRigidActor* actors[MAX_ACTOR];
  PxReal stackZ = 10.0f;

  PxTriangleMesh* triMesh;
  PxRigidStatic* meshActor;
  struct Triangle {
    PxU32 ind0, ind1, ind2;
  };

};



#endif // !PHYSICS_H
