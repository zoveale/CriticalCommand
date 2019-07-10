#ifndef MODEL_H
#define MODEL_H




#include "mesh.h"
#include "animated.h"
#include "shader.h"
#include "modelUtility.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>

using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model {
public:
  /*  Model Data */
  vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
  vector<Mesh> meshes;
  vector<Animated> animatedMeshes;

  string directory;
  bool gammaCorrection;
  bool isAnimated;
  /*  Functions   */
  // constructor, expects a filepath to a 3D model.
  Model(string const& path, bool gamma = false) : gammaCorrection(gamma) {
    loadModel(path);
  }

  // draws the model, and thus all its meshes
  void Draw(Shader shader) {
      for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
        //printf("mesh[%i]\n", i);
      }
  }
  
  void Animate(Shader shader, float time) {

    vector<glm::mat4> transforms;
    BoneTransform(time, transforms);

    for (unsigned int i = 0; i < transforms.size(); i++) {
      shader.SetMat4(bonesGPU[i], transforms[i]); 
    }

    for (unsigned int i = 0; i < animatedMeshes.size(); i++) {
      animatedMeshes[i].Draw(shader);
      //printf("animatedMesh[%i]\n", i);
    }
  }
  void InitializeBones(Shader shader) {
    for (unsigned int i = 0; i < MAX_BONES; i++){
      string name = "gBones[" + to_string(i) + "]";// name like in shader
      bonesGPU[i] = shader.GetUniform(name);
    }
  }

private:
  //
  //aiMatrix4x4 inverseRootNode;
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
  void loadModel(string const& path) {
    //FIXME::add type file reading, so if .obj it isnt animated
    isAnimated = false;
    // read file via ASSIMP
    
    printf("(1)Read Assimp file : %s\n", path.c_str());
    //FIXME:: need static scene?
    this->scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
      cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
      return;
    }
    
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));
    //FIXME::why tho
    //inverse root node
    this->inverseRootNode = ai4x4ToGlm4x4(scene->mRootNode->mTransformation);
    
    ///
    //printf("Root node named: %s\n",scene->mRootNode->mName.data);
    /*if (scene->mRootNode->FindNode("Armature_Torso")) {
      printf("find node named: %s\n", scene->mRootNode->FindNode("Armature_Torso"));
    }*/
    if (scene->HasMaterials()) {
      aiString name;
      printf("it has %i material\n", scene->mNumMaterials);
      for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        scene->mMaterials[i]->Get(AI_MATKEY_NAME, name);
        printf("\tname: %s\n", name.C_Str());
        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, name);
        printf("\tDiffuse: %s\n", name.C_Str());
        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, name);
        printf("\tSpecular: %s\n", name.C_Str());
      }

    }
    // process ASSIMP's root node recursively
    printf("(2)processNodes\n");
    if(scene->HasAnimations()){
      //FIXME::array of different ticks for other animations?
      for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
        ticksPerSecond = scene->mAnimations[i]->mTicksPerSecond;
        animationDuration = scene->mAnimations[i]->mDuration;
      }

      isAnimated = true;
      printf("\t(2a)Process Animated Node\n");
      ProcessAnimatedNode(scene->mRootNode, scene);
    }
    else {
      //ticksPerSecond = 25.0f;
      printf("\t(2a)Process non-animated node\n");
      processNode(scene->mRootNode, scene);
    }
    
  }
  void ProcessAnimatedNode(aiNode* node, const aiScene* scene) {
    // process each mesh located at the current node
    //printf("node name: %s, num of children: %i, number of meshes: %i\n", node->mName.data, node->mNumChildren, node->mNumMeshes);
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      // the node object only contains indices to index the actual objects in the scene. 
      // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      //printf("mesh name: %s\n", mesh->mName.data);
      animatedMeshes.push_back(ProcessAnimatedMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    /*for (unsigned int i = 0; i < node->mNumChildren; i++) {
      printf("\tchild name: %s\n", node->mChildren[i]->mName.data);
    }*/
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      ProcessAnimatedNode(node->mChildren[i], scene);
    }


  }
  
  Animated ProcessAnimatedMesh(aiMesh* mesh, const aiScene* scene) {
    printf("(3)processMesh\n");
    // data to fill
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    ///
    PrintAnimationInfo(scene);
    ///
    //
    std::vector<VertexBoneData> weights;
    weights.resize(mesh->mNumVertices);
    LoadBones(mesh, weights);
    ///
    printf("\n");
    if (mesh->HasTextureCoords(0)) {
      printf("has texture coordinates!\n");

    }
    else printf("no texture coordinates!\n");

    //
    printf("# of vertices in mesh: %i\n", mesh->mNumVertices);
    // Walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;
      glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
      // positions
      //FIXME:: swapping y and z because of .dae file.
      /*
      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].z;
      vector.z = -mesh->mVertices[i].y;
      vertex.Position = vector;
      */
      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].y;
      vector.z = mesh->mVertices[i].z;
      vertex.Position = vector;
      // normals
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.Normal = vector;

      // texture coordinates

      if (mesh->HasTextureCoords(0)) // does the mesh contain texture coordinates?
      {
        glm::vec2 vec;
        // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
        // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
        vec.x = mesh->mTextureCoords[0][i].x;
        vec.y = mesh->mTextureCoords[0][i].y;
        vertex.TexCoords = vec;
      }
      else {
        vertex.TexCoords = glm::vec2(0.0f, 0.0f);
      }
      // tangent
      vector.x = mesh->mTangents[i].x;
      vector.y = mesh->mTangents[i].y;
      vector.z = mesh->mTangents[i].z;
      vertex.Tangent = vector;
      // bitangent
      vector.x = mesh->mBitangents[i].x;
      vector.y = mesh->mBitangents[i].y;
      vector.z = mesh->mBitangents[i].z;
      vertex.Bitangent = vector;
      vertices.push_back(vertex);
    }

    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      //printf("face #: %i\n", face.mNumIndices);
      // retrieve all indices of the face and store them in the indices vector
      for (unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }


    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    // return a mesh object created from the extracted mesh data

  
    

    return Animated(vertices, indices, textures, weights);
  }
  //FIXME::Joint Loading
  void LoadBones(aiMesh* mesh, std::vector<VertexBoneData>& weights) {
    
    for (unsigned int i = 0; i < mesh->mNumBones; i++) {
      unsigned index = 0;
      std::string name(mesh->mBones[i]->mName.data);
      //printf("bone name: %s\n", name.c_str());

      if (boneMap.find(name) == boneMap.end()) {

        index = numBones;
        numBones++;
        BoneData bd;
        
        boneData.push_back(bd);
        boneData[index].offsetTransform = ai4x4ToGlm4x4(mesh->mBones[i]->mOffsetMatrix);
        boneMap[name] = index;
      }
      else {
        index = boneMap[name];
      }
      for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {

        
        unsigned int vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
        float weight = mesh->mBones[i]->mWeights[j].mWeight;
        
        weights[vertexID].addVertexBoneData(index, weight);
      }
    }
  }
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
  void processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
  }

  Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
    printf("(3)processMesh\n");
    // data to fill
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    ///
    if (mesh->HasTextureCoords(0)) {
      printf("has texture coordinates!\n");
      
    }else printf("no texture coordinates!\n");
    
    //
    //LoadBones(mesh, joints);
    ///

    printf("# of vertices in mesh: %i\n", mesh->mNumVertices);
    // Walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;
      glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
      // positions
      //FIXME:: swapping y and z because of dae file.
      /*
      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].z;
      vector.z = -mesh->mVertices[i].y;
      vertex.Position = vector;
      */
      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].y;
      vector.z = mesh->mVertices[i].z;
      vertex.Position = vector;
      // normals
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.Normal = vector;
      
      // texture coordinates
      
      if (mesh->HasTextureCoords(0)) // does the mesh contain texture coordinates?
      {
        glm::vec2 vec;
        // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
        // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
        vec.x = mesh->mTextureCoords[0][i].x;
        vec.y = mesh->mTextureCoords[0][i].y;
        vertex.TexCoords = vec;
      }
      else {
        vertex.TexCoords = glm::vec2(0.0f, 0.0f);
      }
      // tangent
      vector.x = mesh->mTangents[i].x;
      vector.y = mesh->mTangents[i].y;
      vector.z = mesh->mTangents[i].z;
      vertex.Tangent = vector;
      // bitangent
      vector.x = mesh->mBitangents[i].x;
      vector.y = mesh->mBitangents[i].y;
      vector.z = mesh->mBitangents[i].z;
      vertex.Bitangent = vector;
      vertices.push_back(vertex);
    }
    
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      // retrieve all indices of the face and store them in the indices vector
      for (unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }
    

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    // return a mesh object created from the extracted mesh data

    return Mesh(vertices, indices, textures);
  }
  ///
  
  //
  void PrintBones(const aiNode* bones,  aiMesh* mesh) {
    
  }
  ///

  // checks all material textures of a given type and loads the textures if they're not loaded yet.
  // the required info is returned as a Texture struct.
  vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
    vector<Texture> textures;
    printf("texture count %i\n", mat->GetTextureCount(type));
    printf("texture name %s\n", typeName.c_str());
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
      aiString str;
      mat->GetTexture(type, i, &str);
      // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
      bool skip = false;
      for (unsigned int j = 0; j < textures_loaded.size(); j++) {
        if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
          textures.push_back(textures_loaded[j]);
          skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
          break;
        }
      }
      if (!skip) {   // if texture hasn't been loaded already, load it
        Texture texture;
        texture.id = TextureFromFile(str.C_Str(), this->directory);
        texture.type = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
        textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
      }
    }
    return textures;
  }
  ///

  //
  void BoneTransform(double timeInSec, vector<glm::mat4>& transforms) {
    
    double timeInTicks = timeInSec * ticksPerSecond;
    //FIXME:: have array of durations for different animations
    double animationTime = fmod(timeInTicks, animationDuration);

    ReadNodeHierarchy((float)animationTime, scene->mRootNode, glm::mat4(1.0));

    transforms.resize(numBones);

    for (unsigned int i = 0; i < numBones; i++) {
      transforms[i] = boneData[i].finalTransform;
    }
  }
  void ReadNodeHierarchy(float animationTime, const aiNode* parent, glm::mat4 pTransform) {
    
    //get parent node name as string
    string nodeName = parent->mName.data;
    glm::mat4 nodeTransform(1.0);
    
    if (!scene->mAnimations[0]) {
      string error = importer.GetErrorString();
      printf("error: %s\n", error.c_str());
    }
    //FIXME: support multiple animations!
    //FIXME:: make static
    const aiAnimation* parentAnimation = scene->mAnimations[0];
    
    //FIXME:: make static
    const aiNodeAnim* nodeAnimation = FindNodeAnim(parentAnimation, nodeName);

   
    //if nodeAnimation does not return nullptr
    if (nodeAnimation) {
      //FIXME:: make static
      glm::vec3 scaling;
      CalcInterpolatedScaling(scaling, animationTime, nodeAnimation);
      glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0), scaling);
      //FIXME:: make static
      glm::quat rotation;
      CalcInterpolatedRotation(rotation, animationTime, nodeAnimation);
      glm::mat4 rotationMatrix = glm::toMat4(rotation);
      //FIXME:: make static
      glm::vec3 traslation;
      CalcInterpolatedPosition(traslation, animationTime, nodeAnimation);
      glm::mat4 traslationMatrix = glm::translate(glm::mat4(1.0), traslation);
      
      //FIXME::may need to reverse * order because of glm
      nodeTransform = traslationMatrix * rotationMatrix * scalingMatrix;
      //nodeTransform = scalingMatrix * rotationMatrix * traslationMatrix;
    }
    //FIXME::may need to reverse * order because of glm
    glm::mat4 globalTransform = pTransform * nodeTransform;

    if (boneMap.find(nodeName) != boneMap.end()) {
      unsigned int index = boneMap[nodeName];

      //FIXME:: dont need to inverse because of glm?
      boneData[index].finalTransform =   globalTransform * boneData[index].offsetTransform;
    }

    for (unsigned int i = 0; i < parent->mNumChildren; i++) {
      ReadNodeHierarchy(animationTime, parent->mChildren[i], globalTransform);
    }
  }
  //HelperFunction for ReadNodeHierarchy
  const aiNodeAnim* FindNodeAnim(const aiAnimation* parent, const string name) {
    for (unsigned int i = 0; i < parent->mNumChannels; i++) {
      if (parent->mChannels[i]->mNodeName.data == name) {
        return parent->mChannels[i];
      }
    }
    return nullptr;
  }
  ///
  typedef const aiNodeAnim* aiAnim;
  void CalcInterpolatedScaling(glm::vec3& scaling, float animationTime, aiAnim parent) {
    const aiVector3D& out = parent->mScalingKeys[0].mValue;
    if (parent->mNumScalingKeys == 1) {
      scaling = aiVec3ToGlmVec3(out);
    }
    unsigned int index = FindScaling(animationTime, parent);
    unsigned int nextIndex = index + 1;
    assert(nextIndex < parent->mNumScalingKeys);
    float deltaTime = (float)(parent->mScalingKeys[nextIndex].mTime - parent->mScalingKeys[index].mTime);
    float factor = (animationTime - parent->mScalingKeys[index].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);
    const aiVector3D& start = parent->mScalingKeys[index].mValue;
    const aiVector3D& end = parent->mScalingKeys[nextIndex].mValue;
    const aiVector3D& delta = end - start;
    scaling = aiVec3ToGlmVec3(start + factor * delta);
    //printf("scaling x,y,z : %f, %f, %f\n", scaling.x, scaling.y, scaling.z);
  }
  void CalcInterpolatedRotation(glm::quat& rotation, float animationTime, aiAnim parent) {
    aiQuaternion out = parent->mRotationKeys[0].mValue;
    if (parent->mNumPositionKeys == 1) {
      rotation = aiQuatToGlmQuat(out);
    }
    out.x = rotation.x;
    out.y = rotation.y;
    out.z = rotation.z;
    out.w = rotation.w;

    unsigned int index = FindRotation(animationTime, parent);
    unsigned int nextIndex = index + 1;
    assert(nextIndex < parent->mNumRotationKeys);

    float deltaTime = (float)(parent->mRotationKeys[nextIndex].mTime - parent->mRotationKeys[index].mTime);
    float factor = (animationTime - parent->mRotationKeys[index].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);
    const aiQuaternion& start = parent->mRotationKeys[index].mValue;
    const aiQuaternion& end = parent->mRotationKeys[nextIndex].mValue;
    aiQuaternion::Interpolate(out, start, end, factor);
    out.Normalize();
    rotation = aiQuatToGlmQuat(out);
    //printf("rotation x,y,z, w : %f, %f, %f, %f\n", rotation.x, rotation.y, rotation.z, rotation.w);
  }
  void CalcInterpolatedPosition(glm::vec3& translation, float animationTime, aiAnim parent) {
    const aiVector3D& out = parent->mScalingKeys[0].mValue;
    if (parent->mNumPositionKeys == 1) {
      translation = aiVec3ToGlmVec3(out);
    }
    unsigned int index = FindPosition(animationTime, parent);
    unsigned int nextIndex = index + 1;
    assert(nextIndex < parent->mNumPositionKeys);
    float deltaTime = (float)(parent->mPositionKeys[nextIndex].mTime - parent->mPositionKeys[index].mTime);
    float factor = (animationTime - parent->mPositionKeys[index].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);
    const aiVector3D& start = parent->mPositionKeys[index].mValue;
    const aiVector3D& end = parent->mPositionKeys[nextIndex].mValue;
    const aiVector3D& delta = end - start;
    translation = aiVec3ToGlmVec3(start + factor * delta);
    //printf("translation x,y,z : %f, %f, %f\n", translation.x, translation.y, translation.z);
  }
  unsigned int FindScaling(float animationTime, aiAnim parent) {
    assert(parent->mNumScalingKeys > 0);

    for (unsigned int i = 0; i < parent->mNumScalingKeys - 1; i++) {
      if (animationTime < (float)parent->mScalingKeys[i + 1].mTime) {
        return i;
      }
    }

    assert(0);

    return 0;
  }
  unsigned int FindRotation(float animationTime, aiAnim parent) {
    assert(parent->mNumRotationKeys > 0);

    for (unsigned int i = 0; i < parent->mNumRotationKeys - 1; i++) {
      if (animationTime < (float)parent->mRotationKeys[i + 1].mTime) {
        return i;
      }
    }

    assert(0);

    return 0;
  }
  unsigned int FindPosition(float animationTime, aiAnim parent) {
    for (unsigned int i = 0; i < parent->mNumPositionKeys - 1; i++) {
      if (animationTime < (float)parent->mPositionKeys[i + 1].mTime) {
        return i;
      }
    }

    assert(0);

    return 0;
  }
};

#endif //MODEL_H