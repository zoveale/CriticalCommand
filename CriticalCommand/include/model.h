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
    //if(isAnimated)
    //  for (unsigned int i = 0; i < animatedMeshes.size(); i++) {
    //    animatedMeshes[i].Draw(shader);
    //    //printf("animatedMesh[%i]\n", i);
    //  }
    //else {
      for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
        //printf("mesh[%i]\n", i);
      }
    //}
      
  }
  void Animate(Shader shader) {
    /*
    vector<aiMatrix4x4> transforms;
	  boneTransform((double) SDL_GetTicks() / 1000.0f, transforms);

	  for (uint i = 0; i < transforms.size(); i++) // move all matrices for actual model position to shader
	    {
		glUniformMatrix4fv(m_bone_location[i], 1, GL_TRUE, (const GLfloat*)&transforms[i]);
	  }
    */
    vector<glm::mat4> transforms;
    float gameTime = 0.0f;
    BoneTransform(gameTime, transforms);


    for (unsigned int i = 0; i < animatedMeshes.size(); i++) {
      animatedMeshes[i].Draw(shader);
      //printf("animatedMesh[%i]\n", i);
    }
  }

private:
  //
  aiMatrix4x4 inverseRootNode;
  const aiScene* scene;
  std::map<std::string, unsigned int> boneMap;
  std::vector<BoneData> boneData;
  unsigned int numBones = 0;
  //FIXME::make an array for multiple animations
  //ticksPerSecond[MAX_ANIMATIONS], animationDuration[MAX_ANIMATIONS]... etc
  static const unsigned int MAX_ANIMATIONS = 10;
  double ticksPerSecond;
  double animationDuration;
  ///
  /*  Functions   */
  // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
  void loadModel(string const& path) {
    //FIXME::add type file reading, so if .obj it isnt animated
    isAnimated = false;
    // read file via ASSIMP
    Assimp::Importer importer;
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
    this->inverseRootNode = scene->mRootNode->mTransformation;
    inverseRootNode.Inverse();
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

  void BoneTransform(double timeInSec, vector<glm::mat4>& transforms) {
    
    double timeInTicks = timeInSec * ticksPerSecond;
    //FIXME:: have array of durations for different animations
    float animationTime = fmod(timeInTicks, animationDuration);

    ReadNodeHierarchy(animationTime, scene->mRootNode, glm::mat4(1.0));

    transforms.resize(numBones);

    for (unsigned int i = 0; i < numBones; i++) {
      transforms[i] = boneData[i].finalTransform;
    }
  }
  void ReadNodeHierarchy(float animationTime, const aiNode* parent, glm::mat4 pTransform) {

  }
};

#endif //MODEL_H