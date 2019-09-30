#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/include/stb_image.h"
#include <string>
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);


#include "model.h"
#include "modelUtility.h"
//Model::Model(std::string const& path, LightFactory& light, bool gamma = false) : gammaCorrection(gamma) {
//  
//}

// draws the model, and thus all its meshes
void Model::Draw(Shader shader) {
  
  for (unsigned int i = 0; i < meshes.size(); i++) {
    meshes[i].Draw(shader);
  }
}

void Model::Animate(Shader shader, float time) {

  vector<glm::mat4> transforms;
  //TODO:: time is slower
  BoneTransform(time, transforms);

  for (unsigned int i = 0; i < transforms.size(); i++) {
    //TODO::
    string name = "gBones[" + to_string(i) + "]";
    shader.SetMat4(name, transforms[i]);
  }
  for (unsigned int i = 0; i < animatedMeshes.size(); i++) {
    animatedMeshes[i].Draw(shader);
  }
}
//TODO::
void Model::InitializeBones(Shader shader) {
  for (unsigned int i = 0; i < MAX_BONES; i++) {
    string name = "gBones[" + to_string(i) + "]";
    if(shader.GetUniform(name) != -1)
      bonesGPU[i] = shader.GetUniform(name);
    else {
      //TODO::print error
    }
  }
}

//private
  /*  Functions   */
  // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
  void Model::loadModel(string const& path, LightFactory& light, physx::Physics& physicsScene) {
    //FIXME::add type file reading, so if .obj it isnt animated
    isAnimated = false;
    // read file via ASSIMP

    printf("(1)Read Assimp file : %s\n", path.c_str());
    //FIXME:: need static scene?
    this->scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
      aiProcess_CalcTangentSpace | aiProcess_SplitLargeMeshes);// | aiProcess_GenSmoothNormals |
      //aiProcess_JoinIdenticalVertices | aiProcess_FindDegenerates |
      //aiProcess_ValidateDataStructure);

    //TODO:: this Property corresponds to the aiProcess_SplitLargeMeshes post process
    //It should help physx when making a triangle mesh out of larger meshes.
    //Need more testing to see if it is needed to change the default value.
    //importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, )
    ///

    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
      cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
      return;
    }

    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));
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
        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_EMISSIVE, name);
        printf("\tEmissive: %s\n", name.C_Str());
      }

    }
    // process ASSIMP's root node recursively
    printf("(2)processNodes\n");
    if (scene->HasLights()) {
      printf("\t(2a)Process %i Lights\n", scene->mNumLights);
      ProcessLights(scene, light);
    }
    if (scene->HasAnimations()) {
      //TODO::array of different ticks for other animations?
      for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
        ticksPerSecond = scene->mAnimations[i]->mTicksPerSecond;
        animationDuration = scene->mAnimations[i]->mDuration;
      }

      isAnimated = true;
      printf("\t(2b)Process Animated Node\n");
      ProcessAnimatedNode(scene->mRootNode, scene);
    }
    else {
      //ticksPerSecond = 25.0f;
      printf("\t(2b)Process non-animated node\n");
      
      processNode(scene->mRootNode, scene, physicsScene);
    }


  }
  void Model::ProcessLights(const aiScene* scene, LightFactory& lights) {
    std::vector<aiLight*> light;
    std::vector< aiNode*> lightNode;
    for (unsigned int i = 0; i < scene->mNumLights; i++) {
      //FIXME:: vector not needed
      light.push_back(scene->mLights[i]);
      lightNode.push_back(scene->mRootNode->FindNode(light[i]->mName.data));
      lights.AddLights(light[i], lightNode[i]);
    }
  }
  void Model::ProcessAnimatedNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      animatedMeshes.push_back(ProcessAnimatedMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      ProcessAnimatedNode(node->mChildren[i], scene);
    }


  }

  Animated Model::ProcessAnimatedMesh(aiMesh* mesh, const aiScene* scene) {
    printf("(3)processMesh\n");
    // data to fill
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    ///
    //PrintAnimationInfo(scene);
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
      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].y;
      vector.z = mesh->mVertices[i].z;
      vertex.Position = vector;
      // normals
      if (mesh->HasNormals()) {
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
      }
      else {
        vector = glm::vec3(0.0);
      }
      vertex.Normal = vector;
      // texture coordinates
      if (mesh->HasTextureCoords(0)) {
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
      if (mesh->HasTangentsAndBitangents()) {
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
      else {
        // tangent
        vector = glm::vec3(0.0);
        vertex.Tangent = vector;
        vertex.Bitangent = vector;
        vertices.push_back(vertex);
      }
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
  //TODO:: remove physics comments
  void Model::processNode(aiNode* node, const aiScene* scene, physx::Physics& physicsScene/*, again*/) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene, physicsScene));
      
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene, physicsScene);
    }
  }

  Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, physx::Physics& physicsScene/*, again*/) {
    printf("(3)processMesh\n");
    // data to fill
    printf("%s\n", mesh->mName.C_Str());
    
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    //TODO:: possibly switch to a set of pre allocated arrays with max limit
    vector<float> triMeshPos;
    ///
    if (mesh->HasTextureCoords(0)) {
      printf("has texture coordinates!\n");

    }
    else printf("no texture coordinates!\n");

    
    

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
      //TODO:: TRIMESH FOR PHYSICS
      triMeshPos.push_back(vector.x);
      triMeshPos.push_back(vector.y);
      triMeshPos.push_back(vector.z);
      /* 
      PxTriangleMeshDesc meshDesc;
      meshDesc.points.data = PxVec3((float)vector.x, (float)vector.y, (float)vector.z);
      meshDesc.points.count = (unsigned int)mesh->mNumVertices;
      meshDesc.points.stride = sizeof(PxVec3);
      */
      ///
      // normals
      if (mesh->HasNormals()) {
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
      }
      else {
        vector.x = 0.0f;
        vector.y = 0.0f;
        vector.z = 0.0f;
      }
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
      if (mesh->HasTangentsAndBitangents()) {
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
      else {
        // tangent
        vector.x = 0.0f;
        vector.y = 0.0f;
        vector.z = 0.0f;
        vertex.Tangent = vector;
        // bitangent
        vector.x = 0.0f;
        vector.y = 0.0f;
        vector.z = 0.0f;
        vertex.Bitangent = vector;
        vertices.push_back(vertex);
      }
    }
    
    // now wak through each of the mesh's faces (a face is a mesh its triangle)
    // and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      // retrieve all indices of the face and store them in the indices vector
      for (unsigned int j = 0; j < face.mNumIndices; j++) {
        indices.push_back(face.mIndices[j]);
      }
      //std::vector<Triangles> trimesh
      //trimesh[i].pushback(mesh->mFaces[i].mIndices)
    }
    /*
    meshDesc.triangles.count = mesh->mNumFaces;
    meshDesc.triangles.data = indices;
    meshDesc.triangles.stride = 3*  sizeof(indices);
    */
    //TODO:: TRIMESH FOR PHYSICS
    if (collisions) {
      //get all indices
      std::vector<unsigned int> indices;
      for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
          indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
      }
      indices;
      physicsScene.AddStaticTriangleMesh(triMeshPos, indices, mesh->mNumFaces);
    }
    ///

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "material.texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "material.texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    //FIXME:: .obj needs: aiTextureType_HEIGHT, while .dae needs: aiTextureType_NORMALS
    vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "material.texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "material.texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return Mesh(vertices, indices, textures);
  }
  ///

  //
  ///

  // checks all material textures of a given type and loads the textures if they're not loaded yet.
  // the required info is returned as a Texture struct.
  vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {

    vector<Texture> textures;
    /*printf("texture count %i\n", mat->GetTextureCount(type));
    printf("texture name %s\n", typeName.c_str());*/
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
  //FIXME::Joint Loading
  void Model::LoadBones(aiMesh* mesh, std::vector<VertexBoneData>& weights) {

    for (unsigned int i = 0; i < mesh->mNumBones; i++) {
      unsigned index = 0;
      std::string name(mesh->mBones[i]->mName.data);

      if (boneMap.find(name) == boneMap.end()) {
        index = numBones;
        numBones++;

        BoneData bd;
        boneData.push_back(bd);
        boneMap[name] = index;
        boneData[index].offsetTransform = glm::mat4(aiToGlm(mesh->mBones[i]->mOffsetMatrix));

      }
      else {
        index = boneMap[name];
      }
      for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
        unsigned int vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
        float weight = mesh->mBones[i]->mWeights[j].mWeight;

        if (index > numBones) assert(0);
        if (weight <= 0.1f) continue;
        weights[vertexID].addVertexBoneData(index, weight);

      }
    }

  }
  //
  void Model::BoneTransform(double timeInSec, vector<glm::mat4>& transforms) {

    const double ticksPerSecond = scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 25.0f;
    double timeInTicks = timeInSec * ticksPerSecond;
    //FIXME:: have array of durations for different animations
    double animationTime = fmod(timeInTicks, animationDuration);

    ReadNodeHierarchy((float)animationTime, scene->mRootNode, glm::mat4(1.0));

    transforms.resize(numBones);

    for (unsigned int i = 0; i < numBones; i++) {
      transforms[i] = boneData[i].finalTransform;
    }
  }

  void Model::ReadNodeHierarchy(float animationTime, const aiNode* parent, glm::mat4 pTransform) {

    //get parent node name as string
    string nodeName = parent->mName.data;
    glm::mat4 nodeTransform(aiToGlm(parent->mTransformation));
    //FIXME: support multiple animations!
    //FIXME:: make static?
    const aiAnimation* parentAnimation = scene->mAnimations[0];

    //FIXME:: make static?
    const aiNodeAnim* nodeAnimation = FindNodeAnim(parentAnimation, nodeName);


    //if nodeAnimation does not return nullptr
    if (nodeAnimation) {
      //FIXME:: make static
      glm::mat4 scalingMatrix(1.0);
      glm::mat4 rotationMatrix(1.0);
      glm::mat4 traslationMatrix(1.0);
      //FIXME:: make static
      aiVector3D scaling;
      aiQuaternion rotation;
      aiVector3D traslation;

      CalcInterpolatedScaling(scaling, animationTime, nodeAnimation);
      CalcInterpolatedRotation(rotation, animationTime, nodeAnimation);
      CalcInterpolatedPosition(traslation, animationTime, nodeAnimation);
      scalingMatrix = glm::scale(scalingMatrix, glm::vec3(scaling.x, scaling.y, scaling.z));
      rotationMatrix = glm::toMat4(aiToGlm(rotation));
      traslationMatrix = glm::translate(traslationMatrix, glm::vec3(traslation.x, traslation.y, traslation.z));

      nodeTransform = traslationMatrix * rotationMatrix * scalingMatrix;

    }
    glm::mat4 globalTransform = pTransform * nodeTransform;

    if (boneMap.find(nodeName) != boneMap.end()) {
      unsigned int index = boneMap[nodeName];
      boneData[index].finalTransform = globalTransform * boneData[index].offsetTransform;
    }
    for (unsigned int i = 0; i < parent->mNumChildren; i++) {
      ReadNodeHierarchy(animationTime, parent->mChildren[i], globalTransform);
    }
  }
  //HelperFunction for ReadNodeHierarchy
  const aiNodeAnim* Model::FindNodeAnim(const aiAnimation* parent, const string name) {
    for (unsigned int i = 0; i < parent->mNumChannels; i++) {
      if (std::string(parent->mChannels[i]->mNodeName.data) == name) {
        return parent->mChannels[i];
      }
    }
    return nullptr;
  }
  ///
  typedef const aiNodeAnim* aiAnim;
  void Model::CalcInterpolatedScaling(aiVector3D& scaling, float animationTime, aiAnim parent) {
    if (parent->mNumScalingKeys == 1) {
      scaling = parent->mScalingKeys[0].mValue;
      return;
    }

    auto scaling_index = FindScaling(animationTime, parent);
    auto nex_sca_index = scaling_index + 1;

    assert(nex_sca_index < parent->mNumScalingKeys);

    float delta_time = (float)(parent->mScalingKeys[nex_sca_index].mTime - parent->mScalingKeys[scaling_index].mTime);

    float factor = (animationTime - (float)parent->mScalingKeys[scaling_index].mTime) / delta_time;

    assert(factor >= 0.0f && factor <= 1.0f);

    const aiVector3D& start = parent->mScalingKeys[scaling_index].mValue;
    const aiVector3D& end = parent->mScalingKeys[nex_sca_index].mValue;

    scaling = start + factor * (end - start);
  }
  void Model::CalcInterpolatedRotation(aiQuaternion& rotation, float animationTime, aiAnim parent) {
    if (parent->mNumRotationKeys == 1) {
      // There is only one Position.
      rotation = parent->mRotationKeys[0].mValue;
      return;
    }

    unsigned int rotation_index = FindRotation(animationTime, parent);
    unsigned int next_rot_index = rotation_index + 1;
    assert(next_rot_index < parent->mNumRotationKeys);

    // The Difference between two key frames.
    float delta_time = (float)(parent->mRotationKeys[next_rot_index].mTime - parent->mRotationKeys[rotation_index].mTime);

    // The Factor by which the current frame has transitioned into the next frame.
    float factor = (animationTime - (float)parent->mRotationKeys[rotation_index].mTime) / delta_time;

    assert(factor >= 0.0f && factor <= 1.0f);

    const aiQuaternion& start = parent->mRotationKeys[rotation_index].mValue;
    const aiQuaternion& end = parent->mRotationKeys[next_rot_index].mValue;

    aiQuaternion::Interpolate(rotation, start, end, factor);

    rotation = rotation.Normalize();
  }
  void Model::CalcInterpolatedPosition(aiVector3D& translation, float animationTime, aiAnim parent) {
    if (parent->mNumPositionKeys == 1) {
      // There is only one Position.
      translation = parent->mPositionKeys[0].mValue;
      return;
    }

    unsigned int index = FindPosition(animationTime, parent);
    unsigned int next_pos_index = index + 1;
    assert(next_pos_index < parent->mNumPositionKeys);

    // The Difference between two key frames.
    float delta_time = (float)(parent->mPositionKeys[next_pos_index].mTime - parent->mPositionKeys[index].mTime);

    // The Factor by which the current frame has transitioned into the next frame.
    float factor = (animationTime - (float)parent->mPositionKeys[index].mTime) / delta_time;

    assert(factor >= 0.0f && factor <= 1.0f);

    const aiVector3D& start = parent->mPositionKeys[index].mValue;
    const aiVector3D& end = parent->mPositionKeys[next_pos_index].mValue;

    translation = start + factor * (end - start);
  }
  //
  unsigned int Model::FindScaling(float animationTime, aiAnim parent) {
    assert(parent->mNumScalingKeys > 0);

    for (unsigned int i = 0; i < parent->mNumScalingKeys - 1; i++) {
      if (animationTime < (float)parent->mScalingKeys[i + 1].mTime) {
        return i;
      }
    }

    assert(0);

    return 0;
  }
  unsigned int Model::FindRotation(float animationTime, aiAnim parent) {
    assert(parent->mNumRotationKeys > 0);

    for (unsigned int i = 0; i < parent->mNumRotationKeys - 1; i++) {
      if (animationTime < (float)parent->mRotationKeys[i + 1].mTime) {
        return i;
      }
    }

    assert(0);

    return 0;
  }
  unsigned int Model::FindPosition(float animationTime, aiAnim parent) {
    assert(parent->mNumPositionKeys > 0);
    for (unsigned int i = 0; i < parent->mNumPositionKeys - 1; i++) {
      if (animationTime < (float)parent->mPositionKeys[i + 1].mTime) {
        return i;
      }
    }

    assert(0);

    return 0;
  }

  unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma) {
    std::string filename = std::string(path);
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
  void VertexBoneData::addVertexBoneData(unsigned int bone_id, float weight) {
    for (unsigned int i = 0; i < sizeof(ids) / sizeof(unsigned int); i++) {

      if (weights[i] == 0.0f) {

        ids[i] = bone_id;
        weights[i] = weight;
        return;
      }
    }
    // should never get here - more bones than we have space for
    /*count++;
    printf("assert(0) %i times\n", count);*/
  }
  BoneData::BoneData() {
    offsetTransform = glm::mat4(0.0f);
    finalTransform = glm::mat4(0.0f);
  }
  BoneData::BoneData(const aiMatrix4x4 offset, const aiMatrix4x4 final) {
    this->offsetTransform = aiToGlm(offset);
    this->finalTransform = aiToGlm(final);
  }


  glm::mat3 aiToGlm(const aiMatrix3x3& load) {
    glm::mat3 store;
    //row major to column major
    store[0][0] = load.a1; store[1][0] = load.a2; store[2][0] = load.a3;
    store[0][1] = load.b1; store[1][1] = load.b2; store[2][1] = load.b3;
    store[0][2] = load.c1; store[1][2] = load.c2; store[2][2] = load.c3;
    return store;
  }

  glm::mat4 aiToGlm(const aiMatrix4x4& load) {
    glm::mat4 store;
    //row major to column major
    store[0][0] = load.a1; store[1][0] = load.a2; store[2][0] = load.a3; store[3][0] = load.a4;
    store[0][1] = load.b1; store[1][1] = load.b2; store[2][1] = load.b3; store[3][1] = load.b4;
    store[0][2] = load.c1; store[1][2] = load.c2; store[2][2] = load.c3; store[3][2] = load.c4;
    store[0][3] = load.d1; store[1][3] = load.d2; store[2][3] = load.d3; store[3][3] = load.d4;
    //FIXME:: change to transposed glm
    //store = glm::transpose(glm::make_mat4(&load));
    /*for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        store[i][j] = load[i][j];
      }
    }*/
    return store;
  }

  glm::vec3 aiToGlm(const aiVector3D& vec) {
    /*glm::vec3 store;
    store = glm::vec3(vec.x, vec.y, vec.z);*/
    return glm::vec3(vec.x, vec.y, vec.z);
  }
  glm::quat aiToGlm(const aiQuaternion& quat) {
    /*glm::quat store;
    store = glm::quat(quat.x, quat.y, quat.z, quat.w);*/
    return glm::quat(quat.w, quat.x, quat.y, quat.z);
  }