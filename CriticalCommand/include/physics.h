#ifndef PHYSICS_H
#define PHYSICS_H

//#define _DEBUG 
///added to project config
#include <iostream>
#include <vector>

#include <geometric.hpp>
#include <vec3.hpp>
#include <mat4x4.hpp>
#include <unordered_map>



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

//TODO::static const
#define MAX_ACTOR 1<<8

namespace physx { class Physics; }

class physx::Physics {

public:
  Physics();
  //same for all instances of physx
  //TODO::create more componet classes oh physics
  static void StartUp() {

    //Can make new scale
    PxTolerancesScale scale = PxTolerancesScale();
    /*pass in new scale for cenitmeters for example
    scale.length = 100;
    scale.speed = 981;*/
    ///

    //Initialization of physx data types
    gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

    //Physx Visual Debugger info, TODO:: understand this better
    gPvd = PxCreatePvd(*gFoundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, scale, true, gPvd);
    gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(scale));

    //TODO:: create default material
    defaultMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    
    
    /// 
  }
  void TestA();
  void AddActor(PxActor* actor);
  void GetActors();
  void StepPhysics(float dt);

  void CleanUp();

  PxU32 NumberOfActors();
  glm::mat4 GetAPose(int i);

  bool AddPhysxObject(
    const std::string               &name,
    const std::vector<float>        &vertex,
    const std::vector<unsigned int> &indices,
    const unsigned int              &indicesSize) const;

  void AddStaticTriangleMesh(
    const std::vector<float>        &vertex,
    const std::vector<unsigned int> &indices,
    const unsigned int              &indicesSize) const;

  
  //TODO:: test functions
  void CreateStack(const PxTransform& t, PxU32 size, PxReal halfExtent);
  void AddCubeActor(glm::vec3 pos, float x = 1.0f, float y = 1.0f, float z = 1.0f);
  
  void ShootBall(glm::vec3 front, glm::vec3 pos);
  
  /*static PxTriangleMesh* createMeshGround();
  static void updateVertices(PxVec3* verts, float amplitude);*/
  
private:
  PxTriangleMesh* CreateTriangleMesh(
    const std::vector<float>        &vertex,
    const std::vector<unsigned int> &indices,
    const unsigned int              &numFaces) const;

  //TODO:: remove static?
  static PxDefaultAllocator	gAllocator;
  static PxDefaultErrorCallback	gErrorCallback;
  static PxFoundation* gFoundation;
  static PxPhysics* gPhysics;
  static PxCooking* gCooking;
  static PxDefaultCpuDispatcher* gDispatcher;
  static PxPvd* gPvd;
  static PxMaterial* defaultMaterial;

  PxScene* gScene;
  PxRigidActor* actors[MAX_ACTOR];
  PxU32 nbActors;

  /*
  TODO:: remove test variables
  remove static?
  */
  PxMat44 globalPoseArray[MAX_ACTOR];
  PxReal stackZ = 10.0f;

  //PxTriangleMesh* triMesh;
  //PxRigidStatic* meshActor;
  /*struct Triangle {
    PxU32 ind0, ind1, ind2;
  };*/


  static std::unordered_map<std::string, unsigned int> geometryMap;
  const enum GeometryTypes {
    StaticSphere,
    StaticCapsule,
    StaticBox,
    StaticPlane,
    StaticTriangleMesh,
    StaticConvexMesh,
    StaticConvexMeshCooking,
    StaticHeightField,
    DynamicSphere,
    DynamicCapsule,
    DynamicBox,
    DynamicConvexMesh,
    DynamicConvexMeshCooking,
    NoCollisionGeomety
  };
  const std::vector<std::string> GEOMETRY_IDS{
    "SS_",
    "SC_",
    "SB_",
    "SP_",
    "STM_",
    "SCM_",
    "SCMC_",
    "SHF_",
    "DS_",
    "DC_",
    "DB_",
    "DCM_",
    "DCMC_",
    "noID"
  };
  std::string GetIdKey(std::string name) const;
};






#endif // !PHYSICS_H
