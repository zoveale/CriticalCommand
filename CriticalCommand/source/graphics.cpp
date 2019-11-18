#include "graphics.h"

static LightFactory sceneLights;
static physx::Physics scenePhysics;


void BombGraphicsComponet::SetModelAndShaders() {

  Model bombModelIdel("resources/bomb/bomb.dae", sceneLights, scenePhysics);
  idel = bombModelIdel.meshes;

  this->bombShader = Shader(
    "resources/bomb/shader/fragment.glsl",
    "resources/bomb/shader/vertex.glsl",
    "resources/bomb/shader/geometry.glsl");

  model = glm::mat4(1.0f);
}

void BombGraphicsComponet::Update(GameObject& object) {
  //idel = model.meshes;
  model = glm::translate(model, object.position);
}

void BombGraphicsComponet::Draw(const glm::mat4 PV) {
  bombShader.Use();

  bombShader.SetMat4("PVM", PV * model);
  for (unsigned int i = 0; i < idel.size(); i++) {
    idel[i].Draw(bombShader);
  }
}


