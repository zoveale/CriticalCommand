#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "animated.h"
#include "shader.h"

#include "lightFactory.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>





class Model {
public:
  /*  Model Data */
  vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
  vector<Mesh> meshes;
  vector<Animated> animatedMeshes;
  float baseVertexIDs;
  std::string directory;
  bool gammaCorrection;
  bool isAnimated;
  /*  Functions   */
  // constructor, expects a filepath to a 3D model.
  Model(std::string const& path, LightFactory& light, bool gamma = false) : gammaCorrection(gamma) {
    loadModel(path, light);
  }

  // draws the model, and thus all its meshes
  void Draw(Shader shader);
  
  void Animate(Shader shader, float time);
  //FIXME::
  void InitializeBones(Shader shader);

private:
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
  void loadModel(std::string const& path, LightFactory& light);

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
  void processNode(aiNode* node, const aiScene* scene);

  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
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
  //HelperFunction for ReadNodeHierarchy
  const aiNodeAnim* FindNodeAnim(const aiAnimation* parent, const std::string name);
  ///
  typedef const aiNodeAnim* aiAnim;
  void CalcInterpolatedScaling(aiVector3D& scaling, float animationTime, aiAnim parent);
  void CalcInterpolatedRotation(aiQuaternion& rotation, float animationTime, aiAnim parent);
  void CalcInterpolatedPosition(aiVector3D& translation, float animationTime, aiAnim parent);
  //
  unsigned int FindScaling(float animationTime, aiAnim parent);
  unsigned int FindRotation(float animationTime, aiAnim parent);
  unsigned int FindPosition(float animationTime, aiAnim parent);
};

#endif //MODEL_H