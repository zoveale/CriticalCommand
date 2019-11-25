#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "animated.h"
#include "shader.h"
#include "modelUtility.h"
#include "skeletal.h"
#include "lightFactory.h"
#include "physics.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>



//#define PRINT_ASSIMP_INFO


class Model {
public:

  Model() {}
  /*  Model Data */
  std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
  std::vector<Mesh> meshes;
  std::vector<Animated> animatedMeshes;
  float baseVertexIDs;
  std::string directory;
  bool gammaCorrection;
  bool isAnimated;
  bool collisions;
  /*  Functions   */
  // constructor, expects a filepath to a 3D model.
  Model(std::string const& path, 
        LightFactory &light,
        physx::Physics &physicScene,
        bool physics = false,
        bool gamma = false) 
        :collisions(physics),gammaCorrection(gamma) {

    loadModel(path, light, physicScene);
  }

  // draws the model, and thus all its meshes
  void Draw(Shader shader);
  void DrawStencil(Shader shader);
  void Animate(Shader shader, float time);
  //FIXME::
  void InitializeBones(Shader shader);
  
  glm::vec3 Position();
private:
  glm::vec3 modelPosition;

  glm::mat4 inverseRootNode;
  Assimp::Importer importer;
  const aiScene* scene;
  std::map<std::string, unsigned int> boneMap;
  std::vector<BoneData> boneData;
  unsigned int numBones = 0;
  //FIXME::make an array for multiple animations
  //ticksPerSecond[MAX_ANIMATIONS], animationDuration[MAX_ANIMATIONS]... etc
  static const unsigned int MAX_ANIMATIONS = 10;
  double ticksPerSecond;
  double animationDuration;
  //
  static const int MAX_BONES = 100;
  unsigned int bonesGPU[MAX_BONES];
  ///
  /*  Functions   */
  // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
  void loadModel(std::string const& path, LightFactory& light, physx::Physics& physicsScene);

  void ProcessLights(const aiScene* scene, LightFactory& lights);
  void ProcessAnimatedNode(aiNode* node, const aiScene* scene);
  
  Animated ProcessAnimatedMesh(aiMesh* mesh, const aiScene* scene);
  
  ///
  //
  
  /*
  /
  /
  /non animated meshes below
  */

  // processes a node in a recursive fashion. Processes 
  // each individual mesh located at the node and repeats 
  // this process on its children nodes (if any).
  void processNode(aiNode* node, const aiScene* scene, physx::Physics& physicsScene);

  Mesh processMesh(aiMesh* mesh, const aiScene* scene, physx::Physics& physicsScene);
  ///
  
  //
  ///

  // checks all material textures of a given type and loads the textures if they're not loaded yet.
  // the required info is returned as a Texture struct.
  vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
  ///
  //FIXME::Joint Loading
  void LoadBones(aiMesh* mesh, std::vector<VertexBoneData>& weights);
  //
  void BoneTransform(double timeInSec, vector<glm::mat4>& transforms);

  void ReadNodeHierarchy(float animationTime, const aiNode* parent, glm::mat4 pTransform);
 
};

#endif //MODEL_H