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

  dynamicActorCount = -1;
  freeActors.reserve(MAX_ACTOR);
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
  sceneDesc.kineKineFilteringMode = PxPairFilteringMode::eKEEP;
  gScene = gPhysics->createScene(sceneDesc);


}

//TODO::make less awful
//Step physics in normal positon
//only one shape
void physx::Physics::ExplosionEffect(glm::vec3 pos, float radius) {
  
  //need a bool that is swaped when it is called?
  PxReal distance(radius);
  //TODO:: change to a capsle or cyilinder shape for more realistic explostion trajectory 
  PxSphereGeometry sphereOverlap(distance);
  //Move Sphere down so it solves for the upper part of the sphere
  PxTransform location(PxVec3(pos.x, pos.y - radius * 0.5f, pos.z));
  
 
  //
  //body->setMaxAngularVelocity(PxReal(0.01f));
  PxShape* shape = gPhysics->createShape(PxSphereGeometry(distance), *defaultMaterial);
  PxRigidDynamic* body = gPhysics->createRigidDynamic(location);
  body->attachShape(*shape);
  shape->release();
  body->setMaxDepenetrationVelocity(PxReal(35.0f));
  //body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
  PxRigidBodyExt::updateMassAndInertia(*body, 1.73f);

  PxShape* shapeA = gPhysics->createShape(PxSphereGeometry(distance * 1.5f), *defaultMaterial);
  PxRigidDynamic* bodyA = gPhysics->createRigidDynamic(location);
  bodyA->attachShape(*shapeA);
  shapeA->release();
  bodyA->setMaxDepenetrationVelocity(PxReal(25.0f));
  //bodyA->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
  PxRigidBodyExt::updateMassAndInertia(*bodyA, 1.73f);

  PxShape* shapeB = gPhysics->createShape(PxSphereGeometry(distance * 2.0f), *defaultMaterial);
  PxRigidDynamic* bodyB = gPhysics->createRigidDynamic(location);
  bodyB->attachShape(*shapeB);
  shapeB->release();
  bodyB->setMaxDepenetrationVelocity(PxReal(15.0f));
  //bodyB->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
  PxRigidBodyExt::updateMassAndInertia(*bodyB, 1.73f);
  
  //PxRigidActorExt::createExclusiveShape(*bodyB, shapeB, defaultMaterial, PxShapeFlag::eSIMULATION_SHAPE);
  gScene->addActor(*body);
  gScene->addActor(*bodyA);
  gScene->addActor(*bodyB);
  
  
  
  //PxSceneQueryUpdateMode
  //for (int i = 0; i < 3; i++) {
    gScene->simulate(1.0f / 60.0f);
    gScene->fetchResults(true);/*
    gScene->simulate(1.0f / 60.0f);
    gScene->fetchResults(true);
    /*
    gScene->simulate(1.0f / 60.0f);
    gScene->fetchResults(true);*/
  //}
    
  //
  

  gScene->removeActor(*body);
  gScene->removeActor(*bodyA);
  gScene->removeActor(*bodyB);
}

void physx::Physics::AddActor(PxActor* actor) {
  gScene->addActor(*actor);
}

void physx::Physics::UpdateDynamicActorArray(/*PxActor** actor*/) {

  //typedef PxActorTypeFlag FLAG;
  nbActors = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
  if (nbActors > MAX_ACTOR) {
    printf("\n # of actors > MAX_ACTORS\n");
    int i;
    scanf_s("%d", &i);
  }
  
  gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC,
    reinterpret_cast<PxActor * *>(&actors[0]), nbActors);
  for(PxU32 i = 0; i < nbActors; i++){
    globalPoseArray[i] = actors[i]->getGlobalPose();
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
  CCTcontroller->release();
  CCTmanager->release();

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


void physx::Physics::AddStaticTriangleMesh(
                                            const float*                  vertex,
                                            const unsigned int*           indices,
                                            const unsigned int*           indicesSize,
                                            const glm::vec3               position) const {

  PxTriangleMesh* mesh = CreateTriangleMesh(vertex, indices, indicesSize);

  PxTriangleMeshGeometry triGeo = mesh;

  PxTransform pos(PxVec3(position.x, position.y, position.z));
  //PxTransform pos(PxVec3(2.0f));
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

unsigned int physx::Physics::GetDynamicActorCount() {
  return dynamicActorCount;
}

unsigned int physx::Physics::CreateKinematicController(glm::vec3 position) {
  CCTmanager = PxCreateControllerManager(*gScene);
  PxUserControllerHitReport* mReportCallback = nullptr;
  PxControllerBehaviorCallback* mBehaviorCallback = nullptr;
  PxControllerDesc* cDesc;
  PxCapsuleControllerDesc capsuleDesc;
  
  capsuleDesc.height = PxF32(1.0f);
  capsuleDesc.radius = PxF32(0.5f);
  capsuleDesc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
  cDesc = &capsuleDesc;
  cDesc->density = PxF32(10.0f);
  cDesc->scaleCoeff = PxF32(0.8f);
  cDesc->position.x = position.x;
  cDesc->position.y = position.y;
  cDesc->position.z = position.z;
  cDesc->slopeLimit = PxF32(0.707f);
  cDesc->contactOffset = PxF32(0.1f);
  cDesc->stepOffset = PxF32(0.5f);
  cDesc->invisibleWallHeight = PxF32(0.0f);
  cDesc->maxJumpHeight = PxF32(0.0f);
  cDesc->volumeGrowth = PxF32(1.5f);
  cDesc->upDirection = PxVec3(0.0f, 1.0f, 0.0f);
  cDesc->material = defaultMaterial;
  cDesc->reportCallback = mReportCallback;
  cDesc->behaviorCallback = mBehaviorCallback;
  CCTcontroller = CCTmanager->createController(capsuleDesc);
  
  return ++dynamicActorCount;
}

void physx::Physics::SetKinematicControllerPosition(glm::vec3 newPos, float dt) {
  PxTransform setPosition(PxVec3(newPos.x, newPos.y, newPos.z));
  CCTcontroller->getActor()->setKinematicTarget(setPosition); 
}

void physx::Physics::ReleaseActor(unsigned int index) {
  gScene->removeActor(*actors[index]);
  dynamicActorCount--;
}

void physx::Physics::DisableActorSimulation(unsigned int index) {
  actors[index]->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
  freeActors.push_back(index);
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

physx::PxConvexMesh* physx::Physics::CreateConvexMesh(const float* vertex,
                                                 const unsigned int* indices,
                                                 const unsigned int* numFaces) const {
 

  PxConvexMeshDesc convexDesc;
  convexDesc.points.count = *numFaces * 3;
  convexDesc.points.stride = sizeof(PxVec3);
  convexDesc.points.data = vertex;
  convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

  PxDefaultMemoryOutputStream buf;
  PxConvexMeshCookingResult::Enum result;
  if (!gCooking->cookConvexMesh(convexDesc, buf, &result))
    return NULL;
  PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
  PxConvexMesh* convexMesh = gPhysics->createConvexMesh(input);

  return convexMesh;
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

bool physx::Physics::AddPhysxObject(const std::string  &name, 
                                    const float        *vertex, 
                                    const unsigned int *indices,
                                    const unsigned int *indicesSize,
                                    const float        variables[]){

  std::string mapKey = GetIdKey(name);
  glm::vec3 position(variables[0], variables[1], variables[2]);
  glm::vec3 size(variables[3], variables[4], variables[5]);

  switch (geometryMap.at(mapKey)) {
    case GeometryTypes::StaticSphere:
      AddStaticSphereActor(position, variables[3]); 
      return false;
    case GeometryTypes::StaticCapsule:
      return false;
    case GeometryTypes::StaticBox:
      AddStaticBoxActor(position, size);
      return false;
    case GeometryTypes::StaticPlane:
      return false;
    case GeometryTypes::StaticTriangleMesh:
      AddStaticTriangleMesh(vertex, indices, indicesSize, position);
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
      AddDynamicBoxActor(position, size);
      return false;
    case GeometryTypes::DynamicConvexMesh:
      return false;
    case GeometryTypes::DynamicConvexMeshCooking:
      return false;
    case GeometryTypes::NoCollisionGeomety:
      return true;
   }

  return false;
}

//printf("\tAdd Static Sphere for %s\n", name.c_str());
//printf("\tno collision data found for %s\n", name.c_str());
//TODO:: only false to only draw physx objects 
//in future will be true for so all meshes with 
//no collision data will be drawn

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




void physx::Physics::AddStaticSphereActor(glm::vec3 pos, float radius, PxMaterial* material) const{
  PxReal distance(radius);
  PxSphereGeometry sphere(distance);
  PxTransform location(PxVec3(pos.x, pos.y, pos.z));


  PxRigidStatic* body = gPhysics->createRigidStatic(location);
  PxShape* sphereShape = PxRigidActorExt::createExclusiveShape(*body, sphere, *defaultMaterial);
  gScene->addActor(*body);
}

void physx::Physics::AddStaticBoxActor(glm::vec3 pos, glm::vec3 size, PxMaterial* material) {
  //Physx uses "Halfextents" for length claculation
  //so inputing appropriate size into the function should
  //return the correct size;
  PxBoxGeometry box(PxReal(size.x), PxReal(size.y), PxReal(size.z));
  PxTransform location(PxVec3(pos.x, pos.y, pos.z));

  PxShape* shape = gPhysics->createShape(box, *defaultMaterial);
  PxRigidStatic* body = gPhysics->createRigidStatic(location);
  body->attachShape(*shape);
  shape->release();

  gScene->addActor(*body);
  

}



void physx::Physics::SetKinematicActorTarget(unsigned int index, glm::vec3 position) {
  PxVec3 nextPosition(position.x, position.y, position.z);
  PxTransform target(nextPosition);
  kinematicActor->setKinematicTarget(target);
}

unsigned int physx::Physics::AddKinematicSphereActor(glm::vec3 pos, float radius, PxMaterial* material) {

  //Physx uses "Halfextents" for length claculation
  //so inputing appropriate size into the function should
  //return the correct size;
  PxReal distance(radius * 0.5f);
  PxSphereGeometry sphere(distance);
  PxTransform location(PxVec3(pos.x, pos.y, pos.z));


  PxRigidDynamic* body = gPhysics->createRigidDynamic(location);
  body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

  PxShape* sphereShape = PxRigidActorExt::createExclusiveShape(*body, sphere, *defaultMaterial);
  PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);

  //rolls slower
  body->setMaxAngularVelocity(PxReal(3.0f));
  kinematicActor = body;
  gScene->addActor(*body);
  
  
  //UpdateDynamicActorArray();
  return ++dynamicActorCount;
}

//TODO:: reduces rotational velocity 
unsigned int physx::Physics::AddDynamicSphereActor(glm::vec3 pos, float radius,
  glm::vec3 linearVelocity, glm::vec3 angularVelocity,
  PxMaterial* material) {
  //Physx uses "Halfextents" for length claculation
  //so inputing appropriate size into the function should
  //return the correct size;
  PxReal size(radius * 0.5f);
  PxSphereGeometry sphere(size);

  PxQuat rotation(0.0f, PxVec3(1.0f, 0.0f, 0.0f));

  PxTransform location(PxVec3(pos.x, pos.y, pos.z), rotation);


  PxRigidDynamic* body = gPhysics->createRigidDynamic(location);

  PxShape* sphereShape = PxRigidActorExt::createExclusiveShape(*body, sphere, *defaultMaterial);
  PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);

  //rolls slower
  //body->setMaxAngularVelocity(PxReal(3.0f));
  body->setLinearVelocity(PxVec3(linearVelocity.x, linearVelocity.y, linearVelocity.z));
  body->setAngularVelocity(PxVec3(angularVelocity.x, angularVelocity.y, angularVelocity.z));
  gScene->addActor(*body);


  //UpdateDynamicActorArray();
  return ++dynamicActorCount;
}

unsigned int physx::Physics::AddDynamicConvexMesh(const float* vertex,
                                          const unsigned int* indices,
                                          const unsigned int* indicesSize,
                                          const glm::vec3 position) {
  PxConvexMesh* mesh = CreateConvexMesh(vertex, indices, indicesSize);

  PxTransform location(PxVec3(position.x, position.y, position.z));
  
  PxRigidDynamic* aConvexActor = gPhysics->createRigidDynamic(location);
  

  PxShape* aConvexShape = PxRigidActorExt::createExclusiveShape(*aConvexActor,
    PxConvexMeshGeometry(mesh), *defaultMaterial);


  gScene->addActor(*aConvexActor);

  ///same as commented above
  return ++dynamicActorCount;
}

unsigned int physx::Physics::AddLoadedDynamicConvexMesh(const char* meshPath, const glm::vec3 position) {
  Assimp::Importer importer;
  const aiScene* scene;
  std::vector<unsigned int> indices;
  scene = importer.ReadFile(meshPath, aiProcess_FindInvalidData);
  aiNode* rootNode = scene->mRootNode;
  aiMesh* mesh = scene->mMeshes[0];

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    // retrieve all indices of the face and store them in the indices vector
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  PxTransform location(PxVec3(position.x, position.y, position.z));
  PxRigidDynamic* aConvexActor = gPhysics->createRigidDynamic(location);

  PxConvexMesh* convexMesh = CreateConvexMesh(&mesh->mVertices[0].x, &indices[0], &mesh->mNumFaces);
  PxShape* aConvexShape = PxRigidActorExt::createExclusiveShape(*aConvexActor, PxConvexMeshGeometry(convexMesh), *defaultMaterial);

  gScene->addActor(*aConvexActor);

  return ++dynamicActorCount;
}

unsigned int physx::Physics::AddDynamicBoxActor(glm::vec3 pos, glm::vec3 size, PxMaterial* material) {


  //Physx uses "Halfextents" for length claculation
  //so inputing appropriate size into the function should
  //return the correct size;
  PxBoxGeometry box(PxReal(size.x ), PxReal(size.y), PxReal(size.z));

  //45 degrees around the x axis
  PxQuat rotation(0.382683456f, 0.0f, 0.0f, 0.923879564f);

  PxTransform location(PxVec3(pos.x, pos.y, pos.z), rotation);
  PxShape* shape = gPhysics->createShape(box, *defaultMaterial);
  PxRigidDynamic* body = gPhysics->createRigidDynamic(location);
  body->attachShape(*shape);
  shape->release();

  PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
  gScene->addActor(*body);

  return ++dynamicActorCount;
}


//
////void physx::Physics::ShootBall(glm::vec3 front, glm::vec3 pos) {
////
////  PxShape* shape = gPhysics->createShape(PxBoxGeometry(PxVec3(1.0)), *defaultMaterial);
//// 
////  PxTransform localTm(PxVec3(pos.x, pos.y, pos.z));
////
////  PxRigidDynamic* body_0 = gPhysics->createRigidDynamic(localTm);
////  body_0->attachShape(*shape);
////  PxRigidBodyExt::updateMassAndInertia(*body_0, 100.0f);
////  body_0->setAngularDamping(0.5f);
////  glm::vec3 dot = glm::cross(front, pos);
////  body_0->setLinearVelocity(25*PxVec3(dot.x, dot.y, dot.z));
////  PxActor* cube_0 = body_0;
////  //TODO:: cube or body?
////  gScene->addActor(*cube_0);
////  shape->release();
////}
////
////void physx::Physics::updateVertices(PxVec3* verts, float amplitude = 10.0f) {
////  const PxU32 gridSize = GRID_SIZE;
////  const PxReal gridStep = GRID_STEP;
////
////  for (PxU32 a = 0; a < gridSize; a++) {
////    const float coeffA = float(a) / float(gridSize);
////    for (PxU32 b = 0; b < gridSize; b++) {
////      const float coeffB = float(b) / float(gridSize);
////
////      const float y = 20.0f + sinf(coeffA * PxTwoPi) * cosf(coeffB * PxTwoPi) * amplitude;
////      //floats h and k effect position
////      float h = -25.0f, k = -25.0f;
////      verts[a * gridSize + b] = PxVec3(h + b * gridStep, y, k + a * gridStep);
////    }
////  }
////}
////
////physx::PxTriangleMesh* physx::Physics::createMeshGround() {
////  
////  const PxU32 gridSize = GRID_SIZE;
////
////  PxVec3 verts[gridSize * gridSize];
////
////  const PxU32 nbTriangles = 2 * (gridSize - 1) * (gridSize - 1);
////
////  std::vector<Triangle> indices;
////  indices.resize(nbTriangles);
////
////  updateVertices(verts);
////  
////  for (PxU32 a = 0; a < (gridSize - 1); ++a) {
////    for (PxU32 b = 0; b < (gridSize - 1); ++b) {
////      Triangle& tri0 = indices[(a * (gridSize - 1) + b) * 2];
////      Triangle& tri1 = indices[((a * (gridSize - 1) + b) * 2) + 1];
////
////      tri0.ind0 = a * gridSize + b + 1;
////      tri0.ind1 = a * gridSize + b;
////      tri0.ind2 = (a + 1) * gridSize + b + 1;
////
////      tri1.ind0 = (a + 1) * gridSize + b + 1;
////      tri1.ind1 = a * gridSize + b;
////      tri1.ind2 = (a + 1) * gridSize + b;
////    }
////  }
//// 
////  PxCookingParams params = gCooking->getParams();
////  //PxTolerancesScale scalex = PxTolerancesScale();
////  params.midphaseDesc = PxMeshMidPhase::eBVH33;
////  params.scale = gPhysics->getTolerancesScale();
////  params.midphaseDesc.mBVH33Desc.meshCookingHint = PxMeshCookingHint::eSIM_PERFORMANCE;
////  params.midphaseDesc.mBVH33Desc.meshSizePerformanceTradeOff = 0.5f;
////  gCooking->setParams(params);
////
////  PxTriangleMeshDesc meshDesc;
////  meshDesc.points.data = verts;
////  meshDesc.points.count = gridSize * gridSize;
////  meshDesc.points.stride = sizeof(PxVec3);
////  meshDesc.triangles.count = nbTriangles;
////  /*
////  std::vector<Triangle> test;
////  test.resize(100);
////  meshDesc.triangles.data = &test[0];
////  */
////  ///TODO::to set up with vector?
////  meshDesc.triangles.data = &indices[0];
////  ///yes
////  meshDesc.triangles.stride = sizeof(Triangle);
////
////  gCooking->validateTriangleMesh(meshDesc);
////  
////  PxTriangleMeshCookingResult::Enum result;
////  PxTriangleMesh* triMesh = gCooking->createTriangleMesh(meshDesc, gPhysics->getPhysicsInsertionCallback(),&result);
////  
////  return triMesh;
////}
// return glm::mat4(
//    globalPoseArray[i].column0.x,
//    globalPoseArray[i].column1.x,
//    globalPoseArray[i].column2.x,
//    globalPoseArray[i].column3.x,
//    globalPoseArray[i].column0.y,
//    globalPoseArray[i].column1.y,
//    globalPoseArray[i].column2.y,
//    globalPoseArray[i].column3.y,
//    globalPoseArray[i].column0.z,
//    globalPoseArray[i].column1.z,
//    globalPoseArray[i].column2.z,
//    globalPoseArray[i].column3.z,
//    globalPoseArray[i].column0.w,
//    globalPoseArray[i].column1.w,
//    globalPoseArray[i].column2.w,
//    globalPoseArray[i].column3.w);
//    
//    
//    static const physx::PxU32 GRID_SIZE = 8;
//    static const physx::PxReal GRID_STEP = 56.0f / physx::PxReal(GRID_SIZE - 1);
//
//  