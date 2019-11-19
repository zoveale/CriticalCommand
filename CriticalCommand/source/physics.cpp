#include "physics.h"

physx::PxDefaultAllocator       physx::Physics::gAllocator;
physx::PxDefaultErrorCallback   physx::Physics::gErrorCallback;
physx::PxFoundation*            physx::Physics::gFoundation;
physx::PxPhysics*               physx::Physics::gPhysics;
physx::PxCooking*               physx::Physics::gCooking;
physx::PxDefaultCpuDispatcher*  physx::Physics::gDispatcher;
physx::PxMaterial*              physx::Physics::defaultMaterial;
physx::PxPvd*                   physx::Physics::gPvd;

std::unordered_map<std::string, unsigned int> physx::Physics::geometryMap;

physx::Physics::Physics() {
  gFoundation = NULL;
  gPhysics = NULL;
  gDispatcher = NULL;
  gScene = NULL;
  defaultMaterial = NULL;
  gPvd = NULL;
  nbActors = NULL;
}


void physx::Physics::TestA() {
  //parsing assimp data
  for (unsigned int i = 0; i < GEOMETRY_IDS.size(); i++)
    geometryMap[GEOMETRY_IDS[i]] = GeometryTypes::StaticSphere + i;
    //GeometryMap.insert(std::make_pair(GeometryIds[i], i));
  ///

  //TODO::TEST SCENE remove testing physx code
  PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
  sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
  gDispatcher = PxDefaultCpuDispatcherCreate(2);
  sceneDesc.cpuDispatcher = gDispatcher;
  sceneDesc.filterShader = PxDefaultSimulationFilterShader;
  gScene = gPhysics->createScene(sceneDesc);

  

  CreateStack(PxTransform(PxVec3(0.0f, 10.0f, stackZ -= 20.0f)), (PxU32)10, (PxReal)2.0f);
}

//TODO::make less awful
void physx::Physics::ExplosionEffect(glm::vec3 pos, float radius, float dt) {
  PxReal distance(radius);
  PxSphereGeometry sphereOverlap(distance);
  PxTransform location(PxVec3(pos.x, pos.y, pos.z));

  PxShape* shape = gPhysics->createShape(PxSphereGeometry(distance), *defaultMaterial);
  PxRigidDynamic* body = gPhysics->createRigidDynamic(location);
  PxRigidDynamic* bodyA = gPhysics->createRigidDynamic(location);
  //body->setMaxAngularVelocity(PxReal(0.01f));
  body->attachShape(*shape);
  bodyA->attachShape(*shape);
  //some combo of these 2 to make a good explosion effect
  body->setMaxDepenetrationVelocity(PxReal(35.0f));
  bodyA->setMaxDepenetrationVelocity(PxReal(35.0f));

  PxRigidBodyExt::updateMassAndInertia(*body, 1.73f);
  PxRigidBodyExt::updateMassAndInertia(*bodyA, 1.73f);

  gScene->addActor(*body);
  gScene->addActor(*bodyA);
  shape->release();

  gScene->simulate(1.0f/60.0f);
  gScene->fetchResults(true);

  gScene->removeActor(*body);
  gScene->removeActor(*bodyA);

  //PxOverlapHit hits[MAX_ACTOR];
  //PxOverlapBuffer hitBuffer(hits, MAX_ACTOR);

  //PxQueryFilterData filterData(PxQueryFlag::eDYNAMIC | PxQueryFlag::eANY_HIT);
  //
  //
  //bool status = gScene->overlap(sphereOverlap, location, hitBuffer, filterData);

  //PxU32 x = hitBuffer.getNbAnyHits();
  ////hitb
  //for (PxU32 i = 0; i < x; ++i) {
  //  const PxOverlapHit& hit = hitBuffer.getAnyHit(i);
  //  PxShape* shape = hit.shape;
  //  PxRigidActor* actor = hit.actor;
  //  
  //  //gScene->ge
  //}
  //PxGeometryQuery::overlap();
}

void physx::Physics::AddActor(PxActor* actor) {
  gScene->addActor(*actor);
}

void physx::Physics::UpdateDynamicActorArray(/*PxActor** actor*/) {

  typedef PxActorTypeFlag FLAG;
  nbActors = gScene->getNbActors(FLAG::eRIGID_DYNAMIC);
  if (nbActors > MAX_ACTOR) {
    printf("\n # of actors > MAX_ACTORS\n");
    int i;
    scanf_s("%d", &i);
  }
  
  gScene->getActors(FLAG::eRIGID_DYNAMIC,
    reinterpret_cast<PxActor * *>(&actors[0]), nbActors);
  //printf("\n");
  for(PxU32 i = 0; i < nbActors; i++){
    globalPoseArray[i] = actors[i]->getGlobalPose();
    //printf("actor[%i].name: %s\n",i, actors[i]->getName());
    //globalPose = actors[i]->getGlobalPose();
    //pos[i] = globalPose.getPosition();
  }
}

void physx::Physics::StepPhysics(float dt) {
  UpdateDynamicActorArray();
  gScene->simulate(dt);
  gScene->fetchResults(true);
}

void physx::Physics::CleanUp() {
  /*
  Must be in this order to exit correctly with out errors
  */
  gScene->release();
  gDispatcher->release();
  gPhysics->release();
  gCooking->release();
  if (gPvd) {
    PxPvdTransport* transport = gPvd->getTransport();
    gPvd->release();	
    gPvd = NULL;
    transport->release();
  }
  gFoundation->release();

  printf("\n----------\nPHYSX DONE.\n----------\n");
}

physx::PxU32 physx::Physics::NumberOfActors() {
  return nbActors;
}


/*
TODO::remove test funcitons
*/

void physx::Physics::CreateStack(const PxTransform& t,
                                PxU32 size,
                                PxReal halfExtent) {
  /*
  PxSphereGeometry(radius)
  plane geo PxBoxGeometry(PxVec3(halfExtent, halfExtent, 0.01f or 0.1f)
  
  PxBoxGeometry(PxVec3(halfExtent))
  PxCapsuleGeometry(radius, halfExtent)
  PxPlaneGeometry
    */
  PxShape* shape = gPhysics->createShape(PxSphereGeometry(halfExtent), *defaultMaterial);
  for (PxU32 i = 0; i < size; i++) {
    for (PxU32 j = 0; j < size - i; j++) {
      PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2), i) * halfExtent);
      PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
      //std::string actorA = "actor" + std::to_string((unsigned int)i);
      const char* actor = "actor_";

      body->setName(actor);
      body->attachShape(*shape);
      PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
      gScene->addActor(*body);
      
    }
  }
  shape->release();
}

void physx::Physics::AddStaticTriangleMesh(
                                            const float*                  vertex,
                                            const unsigned int*           indices,
                                            const unsigned int*           indicesSize) const {
  //PxTriangleMesh* mesh = createMeshGround();
  PxTriangleMesh* mesh = CreateTriangleMesh(vertex, indices, indicesSize);
  //triMesh = mesh;

  PxTriangleMeshGeometry triGeo = mesh;

  //triGeo.triangleMesh = triMesh;

  PxTransform pos(PxVec3(0.0f, 0.0f, 0.0f));
  PxRigidStatic* staticActor = gPhysics->createRigidStatic(pos);
  //TODO:: can triangle meshes only be used on rigid static actors
  //PxRigidDynamic* staticActor = gPhysics->createRigidDynamic(pos);
  /*
    PxShape* triMeshShape = gPhysics->createShape(triGeo, *defaultMaterial, true);
    staticActor->attachShape(*triMeshShape);
    gScene->addActor(*staticActor);
    triMeshShape->release();
  */
  //PxMaterial* material = gPhysics->createMaterial(0.5f, 0.5f, 0.0f);
  PxRigidActorExt::createExclusiveShape(*staticActor, triGeo, *defaultMaterial);
  gScene->addActor(*staticActor);
  ///same as commented above
}

physx::PxTriangleMesh* physx::Physics::CreateTriangleMesh(
                                                      const float           *vertex,
                                                      const unsigned int    *indices,
                                                      const unsigned int    *numFaces) const {

  PxCookingParams params = gCooking->getParams();
  /*PxTolerancesScale scalex = PxTolerancesScale();
  params.midphaseDesc = PxMeshMidPhase::eBVH33;
  params.midphaseDesc.mBVH33Desc.meshCookingHint = PxMeshCookingHint::eSIM_PERFORMANCE;
  params.midphaseDesc.mBVH33Desc.meshSizePerformanceTradeOff = 1.0f;
  */


  params.midphaseDesc = PxMeshMidPhase::eBVH34;
  params.midphaseDesc.mBVH34Desc.numPrimsPerLeaf = 4; //default = 4, max = 15
  params.scale = gPhysics->getTolerancesScale();
  // disable mesh cleaning - perform mesh validation on development configurations
  //params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
  // disable edge precompute, edges are set for each triangle, slows contact generation
  //params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
  //enables vertex welding during triangle mesh cooking.
  //params.meshPreprocessParams |= PxMeshPreprocessingFlag::eWELD_VERTICES;

  gCooking->setParams(params);

  PxTriangleMeshDesc meshDesc;
  meshDesc.points.data = vertex;
  meshDesc.points.count = *numFaces * 3;
  meshDesc.points.stride = sizeof(PxF32) * 3;
  
  meshDesc.triangles.data = &indices[0];
  meshDesc.triangles.count = *numFaces;
  meshDesc.triangles.stride = sizeof(PxU32) * 3;
  
  /*
  bool validate;
  validate = gCooking->validateTriangleMesh(meshDesc);
  PX_ASSERT(validate);
  if (validate) {
    printf("");
  }
  PxTriangleMeshCookingResult::Enum result;
  PxTriangleMesh* triMesh = gCooking->createTriangleMesh(meshDesc,
                            gPhysics->getPhysicsInsertionCallback(), &result);
  if (!result) printf("");
  return triMesh;
  //PxTriangleMeshCookingResult::Enum result;
  //PxTriangleMesh* triMesh = gCooking->createTriangleMesh(meshDesc, gPhysics->getPhysicsInsertionCallback(), &result);
  */

  return gCooking->createTriangleMesh(meshDesc, gPhysics->getPhysicsInsertionCallback());
}



void physx::Physics::AddCubeActor(glm::vec3 pos, float x, float y, float z) {
  PxShape* shape = gPhysics->createShape(PxBoxGeometry(x/2, y/2, z/2), *defaultMaterial);
  
  PxTransform localTm(PxVec3(pos.x, pos.y, pos.z));

  PxRigidDynamic* body_0 = gPhysics->createRigidDynamic(localTm);
  body_0->attachShape(*shape);
  PxRigidBodyExt::updateMassAndInertia(*body_0, 10.0f);
  body_0->setLinearVelocity(PxVec3(0.0f, 0.0, 100.0f));
  PxActor* cube_0 = body_0;
  gScene->addActor(*body_0);
  shape->release();
}


glm::mat4 physx::Physics::GetAPose(int i) {

 

  return glm::mat4(globalPoseArray[i].column0.x,
    globalPoseArray[i].column0.y,
    globalPoseArray[i].column0.z,
    globalPoseArray[i].column0.w,
                   globalPoseArray[i].column1.x,
    globalPoseArray[i].column1.y,
    globalPoseArray[i].column1.z,
    globalPoseArray[i].column1.w, 
                   globalPoseArray[i].column2.x,
    globalPoseArray[i].column2.y,
    globalPoseArray[i].column2.z,
    globalPoseArray[i].column2.w,
                   globalPoseArray[i].column3.x,
    globalPoseArray[i].column3.y,
    globalPoseArray[i].column3.z,
    globalPoseArray[i].column3.w);
}

bool physx::Physics::AddPhysxObject(const std::string &name, 
                                    const float        *vertex, 
                                    const unsigned int *indices,
                                    const unsigned int *indicesSize) const {
  std::string mapKey = GetIdKey(name);

  switch (geometryMap.at(mapKey)) {
    case GeometryTypes::StaticSphere:
      //printf("\tAdd Static Sphere for %s\n", name.c_str());
      return false;
    case GeometryTypes::StaticCapsule:
      //printf("\tAdd Static Capsule mesh for %s\n", name.c_str());
      return false;
    case GeometryTypes::StaticBox:
      //printf("\tAdd Static Box mesh for %s\n", name.c_str());
      return false;
    case GeometryTypes::StaticPlane:
      //printf("\tAdd Static Plane mesh for %s\n", name.c_str());
      return false;
    case GeometryTypes::StaticTriangleMesh:
      //printf("\tAdd Triangle mesh for %s\n", name.c_str());
      AddStaticTriangleMesh(vertex, indices, indicesSize);
      return true;
    case GeometryTypes::StaticConvexMesh:
      return false;
    case GeometryTypes::StaticConvexMeshCooking:
      return false;
    case GeometryTypes::StaticHeightField:
      return false;
    case GeometryTypes::DynamicSphere:
      return false;
    case GeometryTypes::DynamicCapsule:
      return false;
    case GeometryTypes::DynamicBox:
      return false;
    case GeometryTypes::DynamicConvexMesh:
      return false;
    case GeometryTypes::DynamicConvexMeshCooking:
      return false;
    case GeometryTypes::NoCollisionGeomety:
      //printf("\tno collision data found for %s\n", name.c_str());
      //TODO:: only false to only draw physx objects 
      //in future will be true for so all meshes with 
      //no collision data will be drawn
      return false;
   }

  return false;
}

std::string physx::Physics::GetIdKey(std::string name) const {
  std::string geomID;
  for (char x : name) {
    geomID += x;
    if (x == '_')
      return geomID;
    if(geomID.size() > 5)
      return "noID";
  }
  return "noID";
}

void physx::Physics::ShootBall(glm::vec3 front, glm::vec3 pos) {

  PxShape* shape = gPhysics->createShape(PxBoxGeometry(PxVec3(1.0)), *defaultMaterial);
 
  PxTransform localTm(PxVec3(pos.x, pos.y, pos.z));

  PxRigidDynamic* body_0 = gPhysics->createRigidDynamic(localTm);
  body_0->attachShape(*shape);
  PxRigidBodyExt::updateMassAndInertia(*body_0, 100.0f);
  body_0->setAngularDamping(0.5f);
  glm::vec3 dot = glm::cross(front, pos);
  body_0->setLinearVelocity(25*PxVec3(dot.x, dot.y, dot.z));
  PxActor* cube_0 = body_0;
  //TODO:: cube or body?
  gScene->addActor(*cube_0);
  shape->release();
}


//
//void physx::Physics::updateVertices(PxVec3* verts, float amplitude = 10.0f) {
//  const PxU32 gridSize = GRID_SIZE;
//  const PxReal gridStep = GRID_STEP;
//
//  for (PxU32 a = 0; a < gridSize; a++) {
//    const float coeffA = float(a) / float(gridSize);
//    for (PxU32 b = 0; b < gridSize; b++) {
//      const float coeffB = float(b) / float(gridSize);
//
//      const float y = 20.0f + sinf(coeffA * PxTwoPi) * cosf(coeffB * PxTwoPi) * amplitude;
//      //floats h and k effect position
//      float h = -25.0f, k = -25.0f;
//      verts[a * gridSize + b] = PxVec3(h + b * gridStep, y, k + a * gridStep);
//    }
//  }
//}
//
//physx::PxTriangleMesh* physx::Physics::createMeshGround() {
//  
//  const PxU32 gridSize = GRID_SIZE;
//
//  PxVec3 verts[gridSize * gridSize];
//
//  const PxU32 nbTriangles = 2 * (gridSize - 1) * (gridSize - 1);
//
//  std::vector<Triangle> indices;
//  indices.resize(nbTriangles);
//
//  updateVertices(verts);
//  
//  for (PxU32 a = 0; a < (gridSize - 1); ++a) {
//    for (PxU32 b = 0; b < (gridSize - 1); ++b) {
//      Triangle& tri0 = indices[(a * (gridSize - 1) + b) * 2];
//      Triangle& tri1 = indices[((a * (gridSize - 1) + b) * 2) + 1];
//
//      tri0.ind0 = a * gridSize + b + 1;
//      tri0.ind1 = a * gridSize + b;
//      tri0.ind2 = (a + 1) * gridSize + b + 1;
//
//      tri1.ind0 = (a + 1) * gridSize + b + 1;
//      tri1.ind1 = a * gridSize + b;
//      tri1.ind2 = (a + 1) * gridSize + b;
//    }
//  }
// 
//  PxCookingParams params = gCooking->getParams();
//  //PxTolerancesScale scalex = PxTolerancesScale();
//  params.midphaseDesc = PxMeshMidPhase::eBVH33;
//  params.scale = gPhysics->getTolerancesScale();
//  params.midphaseDesc.mBVH33Desc.meshCookingHint = PxMeshCookingHint::eSIM_PERFORMANCE;
//  params.midphaseDesc.mBVH33Desc.meshSizePerformanceTradeOff = 0.5f;
//  gCooking->setParams(params);
//
//  PxTriangleMeshDesc meshDesc;
//  meshDesc.points.data = verts;
//  meshDesc.points.count = gridSize * gridSize;
//  meshDesc.points.stride = sizeof(PxVec3);
//  meshDesc.triangles.count = nbTriangles;
//  /*
//  std::vector<Triangle> test;
//  test.resize(100);
//  meshDesc.triangles.data = &test[0];
//  */
//  ///TODO::to set up with vector?
//  meshDesc.triangles.data = &indices[0];
//  ///yes
//  meshDesc.triangles.stride = sizeof(Triangle);
//
//  gCooking->validateTriangleMesh(meshDesc);
//  
//  PxTriangleMeshCookingResult::Enum result;
//  PxTriangleMesh* triMesh = gCooking->createTriangleMesh(meshDesc, gPhysics->getPhysicsInsertionCallback(),&result);
//  
//  return triMesh;
//}


/* return glm::mat4(
    globalPoseArray[i].column0.x,
    globalPoseArray[i].column1.x,
    globalPoseArray[i].column2.x,
    globalPoseArray[i].column3.x,
    globalPoseArray[i].column0.y,
    globalPoseArray[i].column1.y,
    globalPoseArray[i].column2.y,
    globalPoseArray[i].column3.y,
    globalPoseArray[i].column0.z,
    globalPoseArray[i].column1.z,
    globalPoseArray[i].column2.z,
    globalPoseArray[i].column3.z,
    globalPoseArray[i].column0.w,
    globalPoseArray[i].column1.w,
    globalPoseArray[i].column2.w,
    globalPoseArray[i].column3.w);
    
    
    static const physx::PxU32 GRID_SIZE = 8;
    static const physx::PxReal GRID_STEP = 56.0f / physx::PxReal(GRID_SIZE - 1);

    */