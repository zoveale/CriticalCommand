#ifndef MODEL_H
#define MODEL_H

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/include/stb_image.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "animated.h"
#include "shader.h"
#include "modelUtility.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <vector>

#include <map>
using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model {
public:
  /*  Model Data */
  vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
  vector<Mesh> meshes;
  vector<Animated> animatedMeshes;
  vector<Joint> jointTest;

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
    if(isAnimated)
      for (unsigned int i = 0; i < animatedMeshes.size(); i++) {
        animatedMeshes[i].Draw(shader);
        //printf("animatedMesh[%i]\n", i);
      }
    else {
      for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
        //printf("mesh[%i]\n", i);
      }
    }
      
  }

private:
  //
  aiMatrix4x4 inverseRootNode;
  const aiScene* scene;
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
      for (int i = 0; i < scene->mNumMaterials; i++) {
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
      isAnimated = true;
      ProcessAnimatedNode(scene->mRootNode, scene);
    }
    else {
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
  // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
  void processNode(aiNode* node, const aiScene* scene) {
    // process each mesh located at the current node
    //printf("node name: %s, num of children: %i, number of meshes: %i\n", node->mName.data, node->mNumChildren, node->mNumMeshes);
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      // the node object only contains indices to index the actual objects in the scene. 
      // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      //printf("mesh name: %s\n", mesh->mName.data);
      meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    /*for (unsigned int i = 0; i < node->mNumChildren; i++) {
      printf("\tchild name: %s\n", node->mChildren[i]->mName.data);
    }*/
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
    

  }
  Animated ProcessAnimatedMesh(aiMesh* mesh, const aiScene* scene) {
    printf("(3)processMesh\n");
    // data to fill
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    Joints joints;
    ///
    
    LoadBones(mesh, joints);
    ///
    vector <Joint> jointTest;
    
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
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    // return a mesh object created from the extracted mesh data

    return Animated(vertices, indices, textures, joints);
  }
  //FIXME::Joint Loading
  void LoadBones(aiMesh* mesh, Joints& joints) {

    const char* rootBoneName = scene->mRootNode->FindNode(mesh->mBones[0]->mName.data)->mName.data;
    joints.root.name = rootBoneName;
    joints.jointCount = mesh->mNumBones;
    
    const aiNode* bones = scene->mRootNode;

    /*for (unsigned int i = 0; i < joints.jointCount; i++) {
      
      printf("joint name: %s\n", jointTest.at(i).name.c_str());
    }*/

    printf("has %i bones:\n", mesh->mNumBones);
    PrintBones(bones, mesh);//FIXME::also loads bones in joint test
    
  }
  ///
  //
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
    for (unsigned int i = 0; i < mesh->mNumBones; i++) {
      const char* boneName;
      printf("\t%s :", mesh->mBones[i]->mName.data);
      glm::mat4 offset = ai4x4ToGlmMat4x4(mesh->mBones[i]->mOffsetMatrix);
      boneName = mesh->mBones[i]->mName.data;
      jointTest.push_back(Joint(i, boneName, offset));
      if (bones->FindNode(jointTest.at(i).name.c_str())) {
        printf("is in the scene's hierarchy\n");
        if (bones->FindNode(jointTest.at(i).name.c_str())->mChildren) {
          unsigned int numChildren = bones->FindNode(jointTest.at(i).name.c_str())->mNumChildren;
          if (numChildren == 1)
            printf("\t\thas %i child :", numChildren);
          else
            printf("\t\thas %i children :", numChildren);
          for (unsigned int j = 0; j < numChildren; j++) {
            const char* childBoneName = bones->FindNode(boneName)->mChildren[j]->mName.data;
            
            jointTest.at(i).children.push_back(Joint(j, childBoneName, glm::mat4(1.0)));
            printf(" %s, ", jointTest.at(i).children.at(j).name.c_str());
          }
          printf("\n");
        }
      }
      else { printf("\n"); }
    }
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
  glm::mat3 ai3x3ToGlmMat3x3(const aiMatrix3x3& load) {
    glm::mat3 store;
    //row major to column major
    store[0][0] = load.a1; store[1][0] = load.a2; store[2][0] = load.a3;
    store[0][1] = load.b1; store[1][1] = load.b2; store[2][1] = load.b3;
    store[0][2] = load.c1; store[1][2] = load.c2; store[2][2] = load.c3;
    return store;
  }

  glm::mat4 ai4x4ToGlmMat4x4(const aiMatrix4x4& load) {
    glm::mat4 store;
    //row major to column major
    store[0][0] = load.a1; store[1][0] = load.a2; store[2][0] = load.a3; store[3][0] = load.a4;
    store[0][1] = load.b1; store[1][1] = load.b2; store[2][1] = load.b3; store[3][1] = load.b4;
    store[0][2] = load.c1; store[1][2] = load.c2; store[2][2] = load.c3; store[3][2] = load.c4;
    store[0][3] = load.d1; store[1][3] = load.d2; store[2][3] = load.d3; store[3][3] = load.d4;
    return store;
  }
  ///
};

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma) {
  string filename = string(path);
  filename = directory + '/' + filename;

  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}
#endif //MODEL_H