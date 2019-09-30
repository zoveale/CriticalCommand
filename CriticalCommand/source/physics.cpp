#include "physics.h"

physx::PxDefaultAllocator physx::Physics::gAllocator;
physx::PxDefaultErrorCallback physx::Physics::gErrorCallback;

physx::PxFoundation* physx::Physics::gFoundation;
physx::PxPhysics*    physx::Physics::gPhysics;
physx::PxCooking*    physx::Physics::gCooking;
physx::PxMat44       physx::Physics::globalPoseArray[MAX_ACTOR];
physx::PxRigidActor* physx::Physics::actors[MAX_ACTOR];

physx::Physics::Physics() {
  gFoundation = NULL;
  gPhysics = NULL;
  gDispatcher = NULL;
  gScene = NULL;
  defaultMaterial = NULL;
  gPvd = NULL;
  triMesh = NULL;
  meshActor = NULL;

}

void physx::Physics::StartUp() {

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



  //TODO::TEST SCENE remove testing physx code
  PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
  sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
  gDispatcher = PxDefaultCpuDispatcherCreate(2);
  sceneDesc.cpuDispatcher = gDispatcher;
  sceneDesc.filterShader = PxDefaultSimulationFilterShader;
  gScene = gPhysics->createScene(sceneDesc);

  //TODO:: create default material
  defaultMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
  CreateStack(PxTransform(PxVec3(0, 10, stackZ -= 10.0f)), 10, 2.0f);

  //PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *defaultMaterial);
  //gScene->addActor(*groundPlane);
  
  /// 
}

void physx::Physics::AddActor(PxActor* actor) {
  gScene->addActor(*actor);
}

void physx::Physics::GetActors(/*PxActor** actor*/) {

  typedef PxActorTypeFlag FLAG;
  PxU32 nbActors = gScene->getNbActors(FLAG::eRIGID_DYNAMIC | FLAG::eRIGID_STATIC);
  /*printf("# of actors = %i\n", nbActors);
  std::cout << "# of actors = " << nbActors << std::endl;*/
  if (nbActors > MAX_ACTOR) {
    printf("\n # of actors > MAX_ACTORS\n");
    int i;
    scanf_s("%d", &i);
  }
  //std::vector<PxRigidActor*> actors(nbActors);
  
  gScene->getActors(FLAG::eRIGID_DYNAMIC | FLAG::eRIGID_STATIC,
    reinterpret_cast<PxActor * *>(&actors[0]), nbActors);

  for(PxU32 i = 0; i < nbActors; i++){
    globalPoseArray[i] = actors[i]->getGlobalPose();
    //globalPose = actors[i]->getGlobalPose();
    //pos[i] = globalPose.getPosition();
  }
  

  
}
/*
TODO:: use for cube shapes??
case PxGeometryType::eBOX:
    {
      const PxBoxGeometry& boxGeom = static_cast<const PxBoxGeometry&>(geom);
      glScalef(boxGeom.halfExtents.x, boxGeom.halfExtents.y, boxGeom.halfExtents.z);
      glutSolidCube(2);
    }
*/

void physx::Physics::StepPhysics() {
  gScene->simulate(1.0f / 60.0f);
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
      body->attachShape(*shape);
      PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
      gScene->addActor(*body);
      
    }
  }
  shape->release();
}

void physx::Physics::AddStaticTriangleMesh(const std::vector<float> vertex,
  const std::vector<unsigned int>          indices,
                                           const unsigned int           indicesSize) {
  printf("process tri mesh for shapes\n");

  //PxTriangleMesh* mesh = createMeshGround();
  PxTriangleMesh* mesh = CreateTriangleMesh(vertex, indices, indicesSize);
  triMesh = mesh;

  PxTriangleMeshGeometry triGeo(mesh);
  triGeo.isValid();
  //triGeo.triangleMesh = triMesh;

  PxTransform pos(PxVec3(0.0f, 0.0f, 0.0f));
  PxRigidStatic* staticActor = gPhysics->createRigidStatic(pos);
  //TODO:: can triangle meshes only be used on rigid static actors
  //PxRigidDynamic* staticActor = gPhysics->createRigidDynamic(pos);

  /*PxShape* triMeshShape = gPhysics->createShape(triGeo, *defaultMaterial, true);
  staticActor->attachShape(*triMeshShape);
  gScene->addActor(*staticActor);
  triMeshShape->release();*/
  PxMaterial* material = gPhysics->createMaterial(0.5f, 0.5f, 0.0f);
  PxRigidActorExt::createExclusiveShape(*staticActor, triGeo, *material);
  gScene->addActor(*staticActor);
  ///same as commented above

}

void physx::Physics::AddCubeActor(glm::vec3 pos, float scale) {
  PxShape* shape = gPhysics->createShape(PxBoxGeometry(scale/2, scale/2, scale/2), *defaultMaterial);
  
  PxTransform localTm(PxVec3(pos.x, pos.y, pos.z));

  PxRigidDynamic* body_0 = gPhysics->createRigidDynamic(localTm);
  body_0->attachShape(*shape);
  PxRigidBodyExt::updateMassAndInertia(*body_0, 10.0f);
  body_0->setLinearVelocity(PxVec3(0.0f, 0.0, 100.0f));
  PxActor* cube_0 = body_0;
  gScene->addActor(*body_0);
  shape->release();
}

//glm::vec3 physx::Physics::GetAPosition(int i) {
//  return glm::vec3(1.0f);//glm::vec3(pos[i].x, pos[i].y, pos[i].z);
//}

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

physx::PxTriangleMesh* physx::Physics::CreateTriangleMesh(const std::vector<float> vertex,
                                                   const std::vector<unsigned int> indices,
                                                      const unsigned int           numFaces) {

  PxCookingParams params = gCooking->getParams();
  //PxTolerancesScale scalex = PxTolerancesScale();
  /*params.midphaseDesc = PxMeshMidPhase::eBVH33;
  params.midphaseDesc.mBVH33Desc.meshCookingHint = PxMeshCookingHint::eSIM_PERFORMANCE;
  params.midphaseDesc.mBVH33Desc.meshSizePerformanceTradeOff = 1.0f;
*/


  params.midphaseDesc = PxMeshMidPhase::eBVH34;
  params.midphaseDesc.mBVH34Desc.numPrimsPerLeaf = 4; //default = 4, max = 15

  params.scale = gPhysics->getTolerancesScale();
  gCooking->setParams(params);

  PxTriangleMeshDesc meshDesc;
  meshDesc.points.data = &vertex[0];
  meshDesc.points.count = vertex.size();
  meshDesc.points.stride = sizeof(vertex[0]) * 3;

  meshDesc.triangles.data = &indices[0];
  meshDesc.triangles.count = numFaces;
  meshDesc.triangles.stride = sizeof(indices[0]) * 3;

  gCooking->validateTriangleMesh(meshDesc);

  PxTriangleMeshCookingResult::Enum result;
  PxTriangleMesh* triMesh = gCooking->createTriangleMesh(meshDesc, gPhysics->getPhysicsInsertionCallback(), &result);

  return triMesh;
  //TODO:: change to CreateTriangleMesh only
}

void physx::Physics::updateVertices(PxVec3* verts, float amplitude = 10.0f) {
  const PxU32 gridSize = GRID_SIZE;
  const PxReal gridStep = GRID_STEP;

  for (PxU32 a = 0; a < gridSize; a++) {
    const float coeffA = float(a) / float(gridSize);
    for (PxU32 b = 0; b < gridSize; b++) {
      const float coeffB = float(b) / float(gridSize);

      const float y = 20.0f + sinf(coeffA * PxTwoPi) * cosf(coeffB * PxTwoPi) * amplitude;
      //floats h and k effect position
      float h = -25.0f, k = -25.0f;
      verts[a * gridSize + b] = PxVec3(h + b * gridStep, y, k + a * gridStep);
    }
  }
}

physx::PxTriangleMesh* physx::Physics::createMeshGround() {
  
  const PxU32 gridSize = GRID_SIZE;

  PxVec3 verts[gridSize * gridSize];

  const PxU32 nbTriangles = 2 * (gridSize - 1) * (gridSize - 1);

  std::vector<Triangle> indices;
  indices.resize(nbTriangles);

  updateVertices(verts);
  
  for (PxU32 a = 0; a < (gridSize - 1); ++a) {
    for (PxU32 b = 0; b < (gridSize - 1); ++b) {
      Triangle& tri0 = indices[(a * (gridSize - 1) + b) * 2];
      Triangle& tri1 = indices[((a * (gridSize - 1) + b) * 2) + 1];

      tri0.ind0 = a * gridSize + b + 1;
      tri0.ind1 = a * gridSize + b;
      tri0.ind2 = (a + 1) * gridSize + b + 1;

      tri1.ind0 = (a + 1) * gridSize + b + 1;
      tri1.ind1 = a * gridSize + b;
      tri1.ind2 = (a + 1) * gridSize + b;
    }
  }
 
  PxCookingParams params = gCooking->getParams();
  //PxTolerancesScale scalex = PxTolerancesScale();
  params.midphaseDesc = PxMeshMidPhase::eBVH33;
  params.scale = gPhysics->getTolerancesScale();
  params.midphaseDesc.mBVH33Desc.meshCookingHint = PxMeshCookingHint::eSIM_PERFORMANCE;
  params.midphaseDesc.mBVH33Desc.meshSizePerformanceTradeOff = 0.5f;
  gCooking->setParams(params);

  PxTriangleMeshDesc meshDesc;
  meshDesc.points.data = verts;
  meshDesc.points.count = gridSize * gridSize;
  meshDesc.points.stride = sizeof(PxVec3);
  meshDesc.triangles.count = nbTriangles;
  /*
  std::vector<Triangle> test;
  test.resize(100);
  meshDesc.triangles.data = &test[0];
  */
  ///TODO::to set up with vector?
  meshDesc.triangles.data = &indices[0];
  ///yes
  meshDesc.triangles.stride = sizeof(Triangle);

  gCooking->validateTriangleMesh(meshDesc);
  
  PxTriangleMeshCookingResult::Enum result;
  PxTriangleMesh* triMesh = gCooking->createTriangleMesh(meshDesc, gPhysics->getPhysicsInsertionCallback(),&result);
  
  return triMesh;
}


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
    globalPoseArray[i].column3.w);*/