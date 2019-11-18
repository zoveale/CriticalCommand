#ifndef GRAHPICS_H
#define GRAPHICS_H

#include "model.h"
static LightFactory sceneLights;
static physx::Physics scenePhysics;

class GraphicsComponet {
public:
  virtual ~GraphicsComponet(){}
  virtual void SetModelAndShaders() = 0;
  virtual void Update() = 0;
  static BombGraphicsComponet bombGraphicsComponet;
};

class BombGraphicsComponet : public GraphicsComponet {
public:
  virtual void SetModelAndShaders() {
    Model bombModel("resources/bomb/bomb.dae", sceneLights, scenePhysics);
    meshes = bombModel.meshes;

    this->bombShader = Shader(
      "resources/bomb/shader/fragment.glsl",
      "resources/bomb/shader/vertex.glsl",
      "resources/bomb/shader/geometry.glsl");

    
  }
  virtual void Update() {
    for (unsigned int i = 0; i < meshes.size(); i++) {
      meshes[i].Draw(bombShader);
    }
  }
  
private:

  vector<Mesh> meshes;
  Shader bombShader;
protected:

};
#endif // !GRAHPICS_H
