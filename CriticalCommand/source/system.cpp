#include "system.h"

static LightFactory sceneLights;
static physx::Physics scenePhysics;

static LightFactory sceneShadowLights;

Overview Camera::overview;
ThirdPerson Camera::thirdPerson;

System::System() {
  model = glm::mat4(1.0f);
  view = glm::mat4(1.0f);
  projection = glm::mat4(1.0f);
}

void System::SystemInit() {
  render.StartUp();
  input.StartUp(render.Window());
  player.StartUp();

  //Camera::thirdPerson
  //Camera::overview
  cameraState = &Camera::overview;
  cameraState->StartUp(); 

  physx::Physics::StartUp();
  scenePhysics.TestA();
  // \ or vise versa ?
  //camera.startup /
  
  //printf("OpenGl version: %s\n", glGetString(GL_VERSION));

  
  //light stuff
  //m_Lights.LoadLights("resources/imagedBasedLighting/massLights.dae", sceneLights);
  lamp.Load("resources/shader/Lamp/lampV.glsl", "resources/shader/Lamp/lampF.glsl");
  pointLamp.LoadModel("resources/surface/pointLamp.dae");
  spotLight.LoadModel("resources/surface/spotLight.dae");
  ///

  //
  uvSphere.Load("resources/imagedBasedLighting/ground.dae", sceneLights, scenePhysics, true);
  
  //skybox
  skyBoxShader.Load("resources/cubemap/shaders/vertex.glsl", "resources/cubemap/shaders/fragment.glsl");
  skyBoxOne.Load(&skyBoxShader);

  //PBR shading stuff
  //Diffuse IBL
  equirectangularToCubemapShader.Load("resources/shader/PBR/DiffuseIBL/EquiRectMap/vert.glsl",
                                      "resources/shader/PBR/DiffuseIBL/EquiRectMap/frag.glsl");
  irradianceShader.Load("resources/shader/PBR/DiffuseIBL/IrradianceMap/vert.glsl",
                        "resources/shader/PBR/DiffuseIBL/IrradianceMap/frag.glsl");
  //Specular IBL
  prefilterShader.Load("resources/shader/PBR/SpecularIBL/Prefilter/vert.glsl",
                       "resources/shader/PBR/SpecularIBL/Prefilter/frag.glsl");
  brdfLookUpShader.Load("resources/shader/PBR/SpecularIBL/BRDFLookUpTexture/vert.glsl",
                        "resources/shader/PBR/SpecularIBL/BRDFLookUpTexture/frag.glsl");

  

  model = glm::mat4(1.0f); 
  view = glm::mat4(1.0f);
  projection = glm::mat4(1.0f);

  //gBuffer
  multipleRenderTargetShader.Load("resources/shader/gBuffer/vert.glsl", "resources/shader/gBuffer/frag.glsl");
  gFrameBuffer.LoadGeometryBuffer();
  ///

  pbrShader.Load("resources/shader/PBR/Main/vert.glsl", "resources/shader/PBR/Main/frag.glsl");
  sceneLights.SetFixedAttributes(pbrShader);
  sceneLights.SetFixedShadowAttributes(pbrShader);

  
  
  //reflection Buffers
  specularIrradianceBuffer.CreateEnvironmentMapFromHdrEquirectangularMap(equirectangularToCubemapShader,
    "resources/imagedBasedLighting/monoLake.hdr");
  specularIrradianceBuffer.CreateIrradianceMapFromEnvironmentMap(irradianceShader);
  specularIrradianceBuffer.CreatePrefilterMapFromEnvironmentMap(prefilterShader);
  /*
  specularIrradianceBuffer.CreateEnvironmentMapFromCubeMap("resources/cubemap/textures");
  specularIrradianceBuffer.CreateIrradianceMapFromCubemap(irradianceShader);
  specularIrradianceBuffer.CreatePrefilterMapFromCubemap(prefilterShader);*/
  specularIrradianceBuffer.CreateBRDFLookUpTextureMap(brdfLookUpShader);
  ///
  //Static render probe from enviroment cubemap
  specularIrradianceBuffer.SetIrradianceTexture(pbrShader);
  specularIrradianceBuffer.SetPrefilterTexture(pbrShader);
  specularIrradianceBuffer.SetBRDFLookUpTexture(pbrShader);
  ///

  //Objects
  modelObject[0].LoadModel("resources/imagedBasedLighting/object0/object.dae");
  modelObject[1].LoadModel("resources/imagedBasedLighting/object1/cube.dae");
  modelObject[2].LoadModel("resources/imagedBasedLighting/object2/diamond.dae");
  gObject[0].Load(&modelObject[0], &multipleRenderTargetShader, &sceneLights);
  gObject[1].Load(&modelObject[1], &multipleRenderTargetShader, &sceneLights);
  gObject[2].Load(&modelObject[2], &multipleRenderTargetShader, &sceneLights);
  pObjectCube.Load(&scenePhysics);
  pObjectSphere.Load(&scenePhysics);
  pObjectDiamond.Load(&scenePhysics);
  //iObject.Load();

  //testObject[0].position = glm::vec3(0.0f, 5.0f, 0.0f);
  //testObject[0].intialRotation = glm::vec3(0.0f , 5.0f, 0.0f);
  ////testObject.initalVelocity = glm::vec3(2.0f);
  //testObject[0].Load(&gObject, &pObject, &iObject);

  //for (unsigned int i = 0; i < MAX_OBJECTS; ++i) {
  //  testObject[i].position = glm::vec3(glm::sin(i * 0.50f) * 15 + 0.0f, i * (0.175) + 5.0f, glm::cos(i * 0.50f) * 15 + 0.0f);
  //  
  //  if((i % 3) == 0)
  //    testObject[i].Load(&gObject[0], &pObjectSphere);
  //  else if((i % 3) == 1)
  //    testObject[i].Load(&gObject[1], &pObjectCube);
  //  else
  //    testObject[i].Load(&gObject[2], &pObjectDiamond);
  //}
  /*playerModel.LoadModel("resources/imagedBasedLighting/object/object.dae");
  playerGraphics.Load(&playerModel, &multipleRenderTargetShader, &sceneLights);
  playerPhysics.Load(&scenePhysics);
  playerInput.Load();
  playerObject.position = glm::vec3(0.0f, 5.0f, 0.0f);
  playerObject.Load(&playerGraphics, &playerPhysics, &playerInput);*/
  
  mechModel.LoadModel("resources/Mechs/simpleTank.dae");
  gMechComponent.Load(&mechModel, &multipleRenderTargetShader);
  pMechComponent.Load(&scenePhysics, "resources/Mechs/simpleTank.dae");
  mechaTank.position = glm::vec3(0.0f, 7.0f, 0.0f);
  mechaTank.Load(&gMechComponent, &pMechComponent);


  dummyModel.LoadModel("resources/dummy/dummy.dae");
}


void System::GameLoop(){
  //test values
  float x = 1.0f;
  float gamma = 1.2f;
  int perspective = 45;
  bool testBool = false;

  float deltaRate = 1.0f / 60.0f;
  float deltaTime = 0.0f;
  float currentFrame = 0.0f;
  float lastFrame = (float)glfwGetTime();

  projection = glm::perspective(glm::radians((float)perspective),
    (float)Render::Screen::WIDTH / (float)Render::Screen::HEIGHT, 0.1f, 1000.0f);
  model = glm::mat4(1.0f);
  view = glm::mat4(1.0f);
  ///

  bool stepPhysics = false;
  scenePhysics.StepPhysics(deltaRate);
  while (!input.KEY.ESC) {
    input.PollEvents();

    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    player.HandleInput(input, deltaTime);
   /* for (GameObject objects : testObject) {
      objects.Update(deltaTime, projection, view);
    }*/
   /* for (unsigned int i = 0; i < MAX_OBJECTS; ++i) {
      testObject[i].Update(deltaTime, projection, view);
    }*/
    mechaTank.Update(deltaTime, projection, view);
    //testObject[0].Update(deltaTime, projection, view);
    //playerObject.Update(deltaTime, projection, view);
    player.Update(deltaTime);
    cameraState->Update(player);

    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    view = cameraState->View();

   

    //TODO:: reimplement stencil testing
    //TODO:: Implement multisampling on framebuffers
    {
      //render.ClearScreen();
      
      gFrameBuffer.BindGeometryBuffer();
      glViewport(0, 0, (GLsizei)Render::Screen::WIDTH, (GLsizei)Render::Screen::HEIGHT);
      render.ClearScreen();
      multipleRenderTargetShader.Use();
      multipleRenderTargetShader.SetMat4("projection", projection);
      multipleRenderTargetShader.SetMat4("view", view);
      multipleRenderTargetShader.SetMat4("model", model);
      uvSphere.Draw(multipleRenderTargetShader); 
      dummyModel.Draw(multipleRenderTargetShader);
      mechaTank.Draw();

      /*for (unsigned int i = 0; i < MAX_OBJECTS; ++i) {
        testObject[i].Draw();
      }*/
      //playerObject.Draw();
      glBindFramebuffer(GL_FRAMEBUFFER, 0);


      pbrShader.Use();                 
      pbrShader.SetVec3("camPos", player.position);
      gFrameBuffer.SetDeferredShading(pbrShader);


      glBindFramebuffer(GL_READ_FRAMEBUFFER, gFrameBuffer.GetGeometryBufferFBO());
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
      glBlitFramebuffer(0, 0, (GLint)Render::Screen::WIDTH, (GLint)Render::Screen::HEIGHT,
        0, 0, (GLint)Render::Screen::WIDTH, (GLint)Render::Screen::HEIGHT,
        GL_DEPTH_BUFFER_BIT, GL_NEAREST);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      
      
      //Drawing Lights
      {
        lamp.Use();
        for (unsigned int i = 0; i < sceneLights.NumPointLights(); i++) {
          model = glm::mat4(1.0f);
          model = glm::translate(model, sceneLights.GetPointLightPos(i));
          model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
          lamp.SetVec3("lampColor", sceneLights.GetPointLightColor(i) * 0.01f);
          lamp.SetMat4("PVM", projection * view * model);
          pointLamp.DrawModelOnly(lamp);
        }
        for (unsigned int i = 0; i < sceneLights.NumSpotLights(); i++) {
          model = glm::mat4(1.0f);
          model = sceneLights.GetSpotLightTransformation(i);
          lamp.SetVec3("lampColor", sceneLights.GetSpotLightColor(i) * 0.1f);
          model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
          lamp.SetMat4("PVM", projection * view * model);
          spotLight.DrawModelOnly(lamp);
        }
      }

   
      //skybox
      //skyBoxOne.Draw(view, projection);
      specularIrradianceBuffer.DrawEnvironmentSkyBox(view, projection, skyBoxShader);
      //specularIrradianceBuffer.DrawPrefilterSkyBox(view, projection, skyBoxShader);
      //specularIrradianceBuffer.DrawIrradianceSkyBox(view, projection, skyBoxShader);
      ///

      /* Swap front and back buffers */
      render.Display();
    }

    input.IncrementDecrement(stepPhysics);
    if(stepPhysics)
      scenePhysics.StepPhysics(deltaRate);

    /* Poll for and process events */
    input.Process();
  }

  //FIXME:: quit function
  glfwWindowShouldClose(render.Window());
  
}

void System::Shutdown() {
 /* for (auto a : bombModels)
    delete a;
  bombModels.clear();*/

  scenePhysics.CleanUp();
  glfwTerminate();
}
////SceneStuff
  //sceneP.Load("resources/pbrTesting/scene/floor/physicfloor.dae", sceneLights, scenePhysics, true);
  //scene[0].LoadModel("resources/pbrTesting/scene/floor/floor.dae");
  //scene[1].LoadModel("resources/pbrTesting/scene/barrel/barrel.dae");
  //scene[2].LoadModel("resources/pbrTesting/scene/brasier/brasier.dae");
  //scene[3].LoadModel("resources/pbrTesting/scene/doors/doors.dae");
  //scene[4].LoadModel("resources/pbrTesting/scene/pillars/pillars.dae");
  //scene[5].LoadModel("resources/pbrTesting/scene/torch/torch.dae");
  //scene[6].LoadModel("resources/pbrTesting/scene/wall1/wall1.dae"); 
  //scene[7].LoadModel("resources/pbrTesting/scene/wall2/wall2.dae");
  //scene[8].LoadModel("resources/pbrTesting/scene/wall3/wall3.dae");
  ////scene.LoadModel("resources/pbrTesting/models/sword/sword.dae");
  //
  ///
  ////Objects
  //icoSphereModel.LoadModel("resources/pbrTesting/models/icoSphere/ico.dae");
  //gComp.Load(&icoSphereModel, &multipleRenderTargetShader);
  //pComp.Load(&scenePhysics);
  //icoSphereObject.Load(&gComp, &pComp);
  /////
  //player object
  /*
  Model playerModel;
  PlayerGraphicsComponent playerGraphicsComp;
  PlayerPhysicsComponent playerPhysicsComp;
  PlayerInputComponent playerInputComp;
  */
  //playerModel.LoadModel("resources/pbrTesting/models/icoSphere/ico.dae");
  //playerGraphicsComp.Load(&icoSphereModel, &multipleRenderTargetShader);
  //playerPhysicsComp.Load(&scenePhysics);
  //playerInputComp.Load(); //filler function
  //playerObject.Load(&playerGraphicsComp, &playerPhysicsComp, &playerInputComp);
  /////

  ////Shadow Stuff
  //depthShadowCubeShader.Load("resources/shader/Shadow/DepthCubemap/vert.glsl",
  //                           "resources/shader/Shadow/DepthCubemap/frag.glsl",
  //                           "resources/shader/Shadow/DepthCubemap/geo.glsl");

  //float near_plane = 0.0010f, far_plane = 55.0f;
  //for (unsigned int i = 0; i < SHADOW_CASTING_POINT_LIGHTS; ++i) {
  //  glClear(GL_DEPTH_BUFFER_BIT);
  //  glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), 1.00f, near_plane, far_plane);
  //  pointShadowCastersBuffer[i].CreateDepthCubeMap();
  //  glm::vec3 pointLightPos = sceneLights.GetPointLightPos(i);
  //  glm::mat4 shadowTransforms[6];
  //  pointShadowCastersBuffer[i].SetPointLightDepthToCubemap(lightProjection, shadowTransforms, pointLightPos);
  //  for(unsigned int j = 0; j < 6; ++j)
  //    pointShadowMatrix.push_back(shadowTransforms[j]);
  //}
  /////

//pbrShader.Use();
//pbrShader.SetMat4("projection", projection);
////model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.5f));
//pbrShader.SetMat4("model", model);
//pbrShader.SetMat4("view", view);
//pbrShader.SetVec3("camPos", player.position);
//pbrShader.SetFloat("radius", 10.0f);// +rand() % 3 + 0.1);
//for (unsigned int i = 0; i < sceneLights.NumPointLights(); i++) {
//  pbrShader.SetVec3("lightPositions[" + std::to_string(i) + "]", sceneLights.GetPointLightPos(i));
//  pbrShader.SetVec3("lightColors[" + std::to_string(i) + "]", sceneLights.GetPointLightColor(i));
//}
//for (unsigned int i = 0; i < 9; ++i) {
//  scene[i].Draw(pbrShader);
//}

/*render.ClearScreen();
pointLightOne.DepthMapViewPort();
glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
glStencilMask(0xFF);
glBindFramebuffer(GL_FRAMEBUFFER, pointLightOne.GetDepthMapFBO());
glClear(GL_DEPTH_BUFFER_BIT);
cubemapDepthShader.Use();
cubemapDepthShader.SetFloat("far_plane", far_plane);
cubemapDepthShader.SetVec3("lightPos", pointLightPos);
pointLightOne.SetPointLightDepthToCubemap(lightProjection, shadowTransforms1, pointLightPos);
for (unsigned int i = 0; i < 6; ++i)
  cubemapDepthShader.SetMat4("shadowTransforms[" + std::to_string(i) + "]", shadowTransforms1[i]);
cubemapDepthShader.SetMat4("model", glm::mat4(1.0f));
magicStoneCircle.DepthDraw(cubemapDepthShader);

simpleTorch.DepthDraw(cubemapDepthShader);
largeRock0.DepthDraw(cubemapDepthShader);
deadTree0.DepthDraw(cubemapDepthShader);
default_0.DepthDraw(cubemapDepthShader);
glBindFramebuffer(GL_FRAMEBUFFER, 0);*/


//hdrFramebuffer.Preprocess();
//geometryBuffer.BindGeometryBuffer();
//glStencilFunc(GL_ALWAYS, 1, 0xFF);
//glStencilMask(0xFF);
//render.ClearScreen();
//glViewport(0, 0, (float)Render::Screen::WIDTH, (float)Render::Screen::HEIGHT);
//simple.Use();
////input.IncrementDecrement(heightScale);
//simple.SetFloat("heightScale", heightScale);
////printf("heightScale = %f\n", heightScale);
//simple.SetFloat("gamma", gamma);
//simple.SetMat4("PVM", projection* view* model);
//simple.SetFloat("far_plane", far_plane);
//simple.SetVec3("viewPos", player.position);
//simple.SetMat4("model", model);
//simple.SetVec3("lightPos", pointLightPos);
////SET CUBEMAP SHADOWS
//pointLightOne.SetShadowCubemap(simple);
////pointLightTwo.SetShadowCubemap(simple);
/////
//sceneLights.SetDynamicAttributes(simple);
//simpleTorch.Draw(simple);
//magicStoneCircle.Draw(simple);
//largeRock0.Draw(simple);
//deadTree0.Draw(simple);
//default_0.Draw(simple);

/*glm::mat4 pvMatrix = projection * view;
    for (GameObject dgo: dynamicObjects) {
      dgo.Update(deltaTime, pvMatrix);
      dgo.Draw();
    }*/
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    /*bomb.Update(deltaTime, pvMatrix);
    bomb.Draw();
    icoSphere.Update(deltaTime, pvMatrix);
    icoSphere.Draw();*/
    ///
   /* glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
    glStencilMask(0x01);
    lamp.Use();
    for (unsigned int i = 0; i < sceneLights.NumPointLights(); i++) {
      model = sceneLights.GetPointLightTransformation(i);
      model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
      lamp.SetVec3("lampColor", sceneLights.GetPointLightColor(i));
      lamp.SetMat4("PVM", projection * view * model);
      pointLamp.Draw(lamp);
    }
    for (unsigned int i = 0; i < sceneLights.NumSpotLights(); i++) {
      model = sceneLights.GetSpotLightTransformation(i);
      model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
      lamp.SetVec3("lampColor", sceneLights.GetSpotLightColor(i));
      lamp.SetMat4("PVM", projection * view * model);
      spotLamp.Draw(lamp);
    }
    glStencilMask(0xFF);*/
    ///
    /*glStencilFunc(GL_NOTEQUAL, 0x01, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    skyboxOne.Draw(view, projection);
    glEnable(GL_DEPTH_TEST);
    glStencilMask(0xFF);*/
    ///

/*
    normalShader.Use();
    normalShader.SetMat4("projection", projection);
    normalShader.SetMat4("view", view);
    normalShader.SetMat4("model", model);
    simpleTorch.Draw(normalShader);
    magicStoneCircle.Draw(normalShader);
    largeRock0.Draw(normalShader);
    deadTree0.Draw(normalShader);
    default_0.Draw(normalShader);*/


//glBindFramebuffer(GL_FRAMEBUFFER, pointLightTwo.GetDepthMapFBO());
//glClear(GL_DEPTH_BUFFER_BIT);
//cubemapDepthShader.Use();
//cubemapDepthShader.SetFloat("far_plane", far_plane);
//pointLightPos = sceneLights.GetPointLightPos(1);
//cubemapDepthShader.SetVec3("lightPos", pointLightPos);
//pointLightTwo.SetPointLightDepthToCubemap(lightProjection, shadowTransforms2, pointLightPos);
//for (unsigned int i = 0; i < 6; ++i)
//  cubemapDepthShader.SetMat4("shadowTransforms[" + std::to_string(i) + "]", shadowTransforms2[i]);
//cubemapDepthShader.SetMat4("model", glm::mat4(1.0f));
//default_0.Draw(cubemapDepthShader);
////TODO::multiple light sources and shadows.
//glBindFramebuffer(GL_FRAMEBUFFER, 1);
//


//render.ClearScreen(); 
//glViewport(0, 0, (float)Render::Screen::WIDTH, (float)Render::Screen::HEIGHT);
//glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
//glStencilMask(0xFF);
////input.IncrementDecrement(gamma);
//simple.Use();
//simple.SetFloat("gamma", gamma);
//simple.SetMat4("PVM", projection* view* model);
//simple.SetFloat("far_plane", far_plane);
//simple.SetVec3("viewPos", player.position);
//simple.SetMat4("model", model);
//
//sceneLights.SetDynamicAttributes(simple);
////simple.SetInt("pointLightNum", 0);
//default_0.Draw(simple);

//basicFramebuffer.DepthMapViewPort();
//TODO::animation stuff
//animated.Use();
    //animated.SetMat4("projection", projection);
    //animated.SetMat4("view", view);
    //model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
    ////model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	
    //animated.SetMat4("model", model);
    //ourModel_0.Animate(animated, currentFrame);
    //normalShader.Use();
    //normalShader.SetMat4("projection", projection);
    //normalShader.SetMat4("view", view);
    //normalShader.SetMat4("model", model);
    //ourModel_0.Draw(normalShader);
    //model = glm::mat4(1.0f);
//TODO:: setting ico80 models the physics deformations
    //for (int i = 0; i < 55; i++) {
    //  glDepthMask(GL_TRUE);
    //  
    //  glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
    //  glStencilMask(0xFF);
    //  simple.Use();
    //  model = glm::mat4(1.0f);
    //  model = scenePhysics.GetAPose(i); 
    //  model = glm::scale(model, glm::vec3(2.0));
    //  simple.SetMat4("model", model);
    //  simple.SetMat4("PVM", projection * view * model);
    //  sceneLights.SetDynamicAttributes(simple);
    //  ico_80.Draw(simple);
    //  
    //  glDepthMask(GL_FALSE);
    //  glStencilFunc(GL_GREATER, 0x01, 0xFF);
    //  glStencilMask(0x00);
    //  stencilShader.Use();
    //  model = glm::scale(model, glm::vec3(1.1f));
    //  stencilShader.SetMat4("model", model);
    //  stencilShader.SetMat4("PVM", projection* view* model);
    //  ico_80.Draw(stencilShader);
    //  /*normalShader.Use();
    //  normalShader.SetMat4("projecion", projection);
    //  normalShader.SetMat4("view", view);
    //  normalShader.SetMat4("model", model);
    //  ico_80.Draw(normalShader);*/
    //}
    //glStencilMask(0xFF);
    //cool acceleration effect
    //player.Update(xpos, ypos); 

//Framebuffer pointLightOne;
//float near_plane = 1.0f, far_plane = 115.0f;//farplane == "radius" of point light
//glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), 1.00f, near_plane, far_plane);
//pointLightOne.CreateDepthCubeMap();
//glm::vec3 pointLightPos = sceneLights.GetPointLightPos(0);
//glm::mat4 shadowTransforms1[6];
//pointLightOne.SetPointLightDepthToCubemap(lightProjection, shadowTransforms1, pointLightPos);
//pointLightOne.DepthMapViewPort();
//glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
//glStencilMask(0xFF);
//glBindFramebuffer(GL_FRAMEBUFFER, pointLightOne.GetDepthMapFBO());
//glClear(GL_DEPTH_BUFFER_BIT);
//cubemapDepthShader.Use();
//cubemapDepthShader.SetFloat("far_plane", far_plane);
//cubemapDepthShader.SetVec3("lightPos", pointLightPos);
//pointLightOne.SetPointLightDepthToCubemap(lightProjection, shadowTransforms1, pointLightPos);
//for (unsigned int i = 0; i < 6; ++i)
//  cubemapDepthShader.SetMat4("shadowTransforms[" + std::to_string(i) + "]", shadowTransforms1[i]);
//cubemapDepthShader.SetMat4("model", glm::mat4(1.0f));
//magicStoneCircle.DepthDraw(cubemapDepthShader);