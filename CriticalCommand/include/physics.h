#ifndef PHYSICS_H
#define PHYSICS_H

//#define _DEBUG 
///added to project config
#include <iostream>
#include <string>
#include <vector>

#include <geometric.hpp>
#include <vec3.hpp>
#include <mat4x4.hpp>
#include <unordered_map>

//TODO::fix this workaround
#include "modelUtility.h"

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
  static Physics scenePhysics;

  Physics();
  //same for all instances of physx
  //TODO::create more Component classes oh physics
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

    gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, scale);

    //TODO:: create default material
    defaultMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    
    
    /// 
  }
  
  void AddynamicActor(PxActor* actor);
  void AddActor(PxActor* actor);

  void UpdateDynamicActorArray();
  void StepPhysics(float dt);

  void CleanUp();

  PxU32 NumberOfActors();
  glm::mat4 GetAPose(int i);

  void SetGlobalPose(unsigned int index, glm::vec3 position, glm::quat rotation);

  bool AddPhysxObject(
    const std::string               &name,
    const float*                    vertex,
    const unsigned int*             indices,
    const unsigned int*             indicesSize,
    const float                     variables[]);

  
  void AddStaticTriangleMesh(
    const float*                    vertex,
    const unsigned int*             indices,
    const unsigned int*             indicesSize,
    const glm::vec3                 position) const;

  void AddStaticSphereActor(
    glm::vec3 pos,
    float radius,
    PxMaterial* material = defaultMaterial) const;

  void AddStaticBoxActor(
    glm::vec3 pos,
    glm::vec3 size,
    PxMaterial* material = defaultMaterial);

  //returns index for dynamic actor
  unsigned int AddDynamicSphereActor(
    glm::vec3 pos,
    float radius,
    glm::vec3 linearVelocity = glm::vec3(0.0f),
    glm::vec3 angularVelocity = glm::vec3(0.0f),
    PxMaterial* material = defaultMaterial);

  unsigned int AddDynamicConvexMesh(
    const float* vertex,
    const unsigned int* indices,
    const unsigned int* indicesSize,
    const glm::vec3                 position);

  //Add a created convex mesh otherwise it will create one from the mesh provided.
  //Must be condisdered a single mesh to be caluclated correctly
  unsigned int AddLoadedDynamicConvexMesh(const char* meshPath, const glm::vec3 position);
  

  //unsigned int AddQuickhullDynamicConvexMesh(const char* meshPath, const glm::vec3 position);

  void SetKinematicActorTarget(unsigned int index, glm::vec3 position, glm::vec4 rotation);
  
  unsigned int AddKinematicSphereActor(
    glm::vec3 pos,
    float radius,
    PxMaterial* material = defaultMaterial);

  unsigned int AddDynamicBoxActor(
    glm::vec3 pos,
    glm::vec3 size,
    PxMaterial* material = defaultMaterial);

  void ExplosionEffect(glm::vec3 pos, float radius);
  void ReleaseActor(unsigned int index);
  void DisableActorSimulation(unsigned int index);
  void DisableActorGravity(unsigned int index);
  //TODO:: test functions
  void TestA();
  //swap float to bool
  
  //void RayCastEffect()
  ///
  
  unsigned int GetDynamicActorCount();
  void SetDynamicActorKinematicTarget(unsigned int index, glm::vec3 pos);
  //Kinematic Character Stuff
  unsigned int CreateKinematicController(glm::vec3 position);
  void SetKinematicControllerPosition(glm::vec3 newPos, float dt);
  ///
  unsigned int AddKinematicConvexActor(const char* meshPath, const glm::vec3 position);
  glm::mat4 KinmaticActorPose(unsigned int);
private:
  PxTriangleMesh* CreateTriangleMesh(
    const float*                     vertex,
    const unsigned int*              indices,
    const unsigned int*              numFaces) const;

  PxConvexMesh* CreateConvexMesh(
    const float* vertex,
    const unsigned int* indices,
    const unsigned int* numFaces) const;

 /* PxConvexMesh* CreateConvexMeshCooking(
    const float* vertex,
    const unsigned int* indices,
    const unsigned int* indicesSize);*/

  static PxPhysics* gPhysics;
  static PxMaterial* defaultMaterial;
  static PxDefaultAllocator	gAllocator;
  static PxDefaultErrorCallback	gErrorCallback;
  static PxFoundation* gFoundation;
  static PxCooking* gCooking;
  static PxDefaultCpuDispatcher* gDispatcher;
  static PxPvd* gPvd;
  
  PxRigidDynamic* kinematicActor;
  PxControllerManager* CCTmanager;
  PxController* CCTcontroller;

  const PxFilterData* mFilterData;
  PxQueryFilterCallback* mFilterCallback;
  PxControllerFilterCallback* mCCTFilterCallback;
  const PxControllerFilters filters;

  //TODO:: recycleing actors vector
  std::vector<unsigned int> freeActors;

  PxScene* gScene;
  PxRigidActor* actors[MAX_ACTOR];
  PxU32 nbActors;
  int dynamicActorCount;

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
