#include "model.h"

// draws the model, and thus all its meshes
void Model::Draw(Shader shader) {
  for (unsigned int i = 0; i < meshes.size(); i++) {
    meshes[i].Draw(shader);
  }
}
void Model::DepthDraw(Shader shader) {
  for (unsigned int i = 0; i < meshes.size(); i++) {
    meshes[i].DepthDraw(shader);
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
    if (shader.GetUniform(name) != -1)
      bonesGPU[i] = shader.GetUniform(name);
    else {
      //TODO::print error
    }
  }
}

glm::vec3 Model::Position() {
  return modelPosition;
}

//private
  /*  Functions   */
  // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void Model::loadModel(string const& path, LightFactory& light, physx::Physics& physicsScene) {
  //FIXME::add type file reading, so if .obj it isnt animated
  isAnimated = false;

#ifdef PRINT_ASSIMP_INFO
  printf("(1)Read Assimp file : %s\n", path.c_str());
#endif // PRINT_ASSIMP_INFO

  // read file via ASSIMP
  //This combo seems to work best for Physx Triangle Mesh creation
  //Much more Post Processing information
  //sir-kimmi.de/assimp/lib_html/postprocess_8h.htm
  this->scene = importer.ReadFile(path,
    aiProcess_FlipUVs |
    aiProcess_CalcTangentSpace |
    aiProcess_FindInvalidData |
    aiProcess_ImproveCacheLocality
  );


  // check for errors
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
    return;
  }

  //retrieve the directory path 
  directory = path.substr(0, path.find_last_of('/'));

#ifdef PRINT_ASSIMP_INFO
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
#endif // PRINT_ASSIMP_INFO
  // process ASSIMP's root node recursively

#ifdef PRINT_ASSIMP_INFO   
  printf("(2)processNodes\n");
#endif // PRINT_ASSIMP_INFO


  if (scene->HasLights()) {
#ifdef PRINT_ASSIMP_INFO   
    printf("\t(2a)Process %i Lights\n", scene->mNumLights);
#endif // PRINT_ASSIMP_INFO
    ProcessLights(scene, light);
  }
  if (scene->HasAnimations()) {
    //TODO::array of different ticks for other animations?
    for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
      ticksPerSecond = scene->mAnimations[i]->mTicksPerSecond;
      animationDuration = scene->mAnimations[i]->mDuration;
    }
    isAnimated = true;

#ifdef PRINT_ASSIMP_INFO   
    printf("\t(2b)Process Animated Node\n");
#endif // PRINT_ASSIMP_INFO
    ProcessAnimatedNode(scene->mRootNode, scene);
  }
  else {
    //ticksPerSecond = 25.0f;
#ifdef PRINT_ASSIMP_INFO   
    printf("\t(2b)Process non-animated node\n");
#endif // PRINT_ASSIMP_INFO
    processNode(scene->mRootNode, scene, physicsScene);
  }


}

void Model::LoadLightsOnly(std::string const& path, LightFactory& light) {
  this->scene = importer.ReadFile(path,
    aiProcess_FlipUVs |
    aiProcess_CalcTangentSpace |
    aiProcess_FindInvalidData |
    aiProcess_ImproveCacheLocality
  );
  // check for errors
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
    return;
  }
  //retrieve the directory path 
  directory = path.substr(0, path.find_last_of('/'));

  //ProcessNodesOnly(scene->mRootNode, scene);
  ProcessLights(scene, light);
}


void Model::ProcessLights(const aiScene* scene, LightFactory& lights) {
  std::vector<aiLight*> light;
  std::vector< aiNode*> lightNode;
  for (unsigned int i = 0; i < scene->mNumLights; i++) {
    //TODO:: vector not needed
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
#ifdef PRINT_ASSIMP_INFO   
  printf("(3)processMesh\n");
#endif // PRINT_ASSIMP_INFO
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
#ifdef PRINT_ASSIMP_INFO   
  printf("\n");
  if (mesh->HasTextureCoords(0)) {
    printf("has texture coordinates!\n");

  }
  else printf("no texture coordinates!\n");
  printf("# of vertices in mesh: %i\n", mesh->mNumVertices);
#endif // PRINT_ASSIMP_INFO
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

  //wak through each of the mesh's faces
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
  FillTextureVector(textures);
  // 1. diffuse maps
  //vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
  //textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
  //// 2. specular maps
  //vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
  //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  //// 3. normal maps
  //std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
  //textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
  //// 4. height maps
  //std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
  //textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
  //// return a mesh object created from the extracted mesh data



  return Animated(vertices, indices, textures, weights);
}


// processes a node in a recursive fashion. Processes 
// each individual mesh located at the node and repeats 
// this process on its children nodes (if any).
//TODO:: remove physics comments
void Model::processNode(aiNode* node, const aiScene* scene, physx::Physics& physicsScene/*, again*/) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    nodeTransform = aiToGlm(node->mTransformation);
    meshes.push_back(processMesh(mesh, scene, physicsScene));

  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene, physicsScene);
  }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, physx::Physics& physicsScene/*, again*/) {

#ifdef PRINT_ASSIMP_INFO   
  printf("(3)processMesh\n");
  printf("%s\n", mesh->mName.C_Str());
#endif // PRINT_ASSIMP_INFO


  // data to fill
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  //TODO:: possibly switch to a set of pre allocated arrays with max limit
  ///Swapped to pointers
  //std::vector<float> triMeshPos;
  //triMeshPos.reserve(mesh->mNumVertices * 3);

  //TODO:: values do not need to be this large
  //for size equation solving
  /*std::vector<float> physxPosition;
  physxPosition.reserve(mesh->mNumVertices);*/
  float maxX = -1 * FLT_MAX;
  float minX = FLT_MAX;

  float maxY = -1 * FLT_MAX;
  float minY = FLT_MAX;

  float maxZ = -1 * FLT_MAX;
  float minZ = FLT_MAX;
  ///

#ifdef PRINT_ASSIMP_INFO   
  if (mesh->HasTextureCoords(0)) {
    printf("has texture coordinates!\n");

  }
  else printf("no texture coordinates!\n");
  printf("# of vertices in mesh: %i\n", mesh->mNumVertices);
#endif // PRINT_ASSIMP_INFO
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
    //position realtive to scene
    //vector += glm::vec3(nodeTransform[3][0], nodeTransform[3][1], nodeTransform[3][2]);
   /* physxPosition.push_back(vector.x + nodeTransform[3][0]);
    physxPosition.push_back(vector.y + nodeTransform[3][1]);
    physxPosition.push_back(vector.z + nodeTransform[3][2]);*/
    /*
    TODO: to find height estimate of object for PHYSX primatives.
    maybe similar to find width and length
    no idea how to solve for capsules right now
    but should work for Sphere, Boxes, and Planes
    TODO::most likely going to need the vector position for each min max position
    for capsule primative shape
    */
    if (vector.x > maxX)
      maxX = vector.x;
    if (vector.x < minX)
      minX = vector.x;
    if (vector.y > maxY)
      maxY = vector.y;
    if (vector.y < minY)
      minY = vector.y;
    if (vector.z > maxZ)
      maxZ = vector.z;
    if (vector.z < minZ)
      minZ = vector.z;

    vertex.Position = vector;
    //TODO:: TRIMESH FOR PHYSICS
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
  //indices.resize(mesh->mNumFaces * 3);
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    // retrieve all indices of the face and store them in the indices vector
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  //Physx primative sizeing
  
  float xHalfextent = ((maxX - minX) / 2.0f);
  float yHalfextent = ((maxY - minY) / 2.0f);
  float zHalfextent = ((maxZ - minZ) / 2.0f);
  /*float xPos = xHalfextent + minX +nodeTransform[3][0];
  float yPos = yHalfextent + minY +nodeTransform[3][1];
  float zPos = zHalfextent + minZ +nodeTransform[3][2];*/
  modelPosition = glm::vec3(nodeTransform[3][0], nodeTransform[3][1], nodeTransform[3][2]);
  const float minMaxXYZ[] = { modelPosition.x , modelPosition.y, modelPosition.z, xHalfextent, yHalfextent , zHalfextent};
  ///
  
  if (collisions) {

    /*for (unsigned int i = 0; i < vertices.size(); ++i) {
      vertices[i].Position -= glm::vec3(nodeTransform[3][0], nodeTransform[3][1], nodeTransform[3][2]);
    }*/
    bool buildMesh = physicsScene.AddPhysxObject(
                                                 mesh->mName.C_Str(),
                                                 &mesh->mVertices[0].x,
                                                 &indices[0],
                                                 &mesh->mNumFaces,
                                                 minMaxXYZ);
    //If static mesh need to move graphics appropriately
    for (unsigned int i = 0; i < vertices.size(); ++i) {
      vertices[i].Position -= glm::vec3(nodeTransform[3][0], nodeTransform[3][1], nodeTransform[3][2]);
    }

    if (!buildMesh)
      return Mesh::Empty();
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
  FillPBRTextureVector(textures);

  /*std::vector<Texture> diffuseMaps = LoadATexture(aiTextureType_DIFFUSE, "material.texture_diffuse");
  textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
  std::vector<Texture> specularMaps = LoadATexture(aiTextureType_SPECULAR, "material.texture_specular");
  textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  std::vector<Texture> normalMaps = LoadATexture(aiTextureType_NORMALS, "material.texture_normal");
  textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
  std::vector<Texture> heightMaps = LoadATexture(aiTextureType_HEIGHT, "material.texture_height");
  textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());*/
  /*
  Albedo Map
  std::vector<Texture> diffuseMaps = LoadATexture(aiTextureType_DIFFUSE, "material.texture_diffuse");
  textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

  Normal Map
  std::vector<Texture> normalMaps = LoadATexture(aiTextureType_NORMALS, "material.texture_normal");
  textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

  Metallic Map
  std::vector<Texture> metallicMaps = LoadATexture(aiTextureType_METALNESS, "material.texture_metallic");
  textures.insert(textures.end(), metallicMaps.begin(), metallicMaps.end());

  Roughness Map
  std::vector<Texture> roughnessMaps = LoadATexture(aiTextureType_SHININESS, "material.texture_roughness");
  textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());

  AO Map
  std::vector<Texture> aoMaps = LoadATexture(aiTextureType_LIGHTMAP, "material.texture_ao");
  textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());
  */
  return Mesh(vertices, indices, textures);
}
///

//
///



void Model::FillTextureVector(std::vector<Texture> &textureVector) {
  std::vector<Texture> diffuseMaps = LoadATexture(aiTextureType_DIFFUSE, "material.texture_diffuse");
  textureVector.insert(textureVector.end(), diffuseMaps.begin(), diffuseMaps.end());
  std::vector<Texture> specularMaps = LoadATexture(aiTextureType_SPECULAR, "material.texture_specular");
  textureVector.insert(textureVector.end(), specularMaps.begin(), specularMaps.end());
  std::vector<Texture> normalMaps = LoadATexture(aiTextureType_NORMALS, "material.texture_normal");
  textureVector.insert(textureVector.end(), normalMaps.begin(), normalMaps.end());
  std::vector<Texture> heightMaps = LoadATexture(aiTextureType_HEIGHT, "material.texture_height");
  textureVector.insert(textureVector.end(), heightMaps.begin(), heightMaps.end());

}

void Model::FillPBRTextureVector(std::vector<Texture>& pbrTextureVector) {
  //Albedo Map
  std::vector<Texture> diffuseMaps = LoadPBRTexture(aiTextureType_DIFFUSE, "material.texture_diffuse");
  pbrTextureVector.insert(pbrTextureVector.end(), diffuseMaps.begin(), diffuseMaps.end());

  //Normal Map
  std::vector<Texture> normalMaps = LoadPBRTexture(aiTextureType_NORMALS, "material.texture_normal");
  pbrTextureVector.insert(pbrTextureVector.end(), normalMaps.begin(), normalMaps.end());

  //Metallic Map
  std::vector<Texture> metallicMaps = LoadPBRTexture(aiTextureType_METALNESS, "material.texture_metallic");
  pbrTextureVector.insert(pbrTextureVector.end(), metallicMaps.begin(), metallicMaps.end());

  //Roughness Map
  std::vector<Texture> roughnessMaps = LoadPBRTexture(aiTextureType_SHININESS, "material.texture_roughness");
  pbrTextureVector.insert(pbrTextureVector.end(), roughnessMaps.begin(), roughnessMaps.end());

  //AO Map
  std::vector<Texture> aoMaps = LoadPBRTexture(aiTextureType_LIGHTMAP, "material.texture_ao");
  pbrTextureVector.insert(pbrTextureVector.end(), aoMaps.begin(), aoMaps.end());
}

vector<Texture> Model::LoadPBRTexture(aiTextureType type, string typeName) {

  std::string textureType;
  switch (type) {
  case aiTextureType_DIFFUSE:
    textureType = "albedo.png";
    break;
  case aiTextureType_NORMALS:
    textureType = "normal.png";
    break;
  case aiTextureType_METALNESS:
    textureType = "metallic.png";
    //TODO:: if no metallic, dont use albedo in shader
    break;
  case aiTextureType_SHININESS:
    textureType = "roughness.png";
    break;
  case aiTextureType_LIGHTMAP:
    textureType = "ao.png";
    break;
  default:
    break;
  }
  vector<Texture> textures;
  bool skip = false;
  for (unsigned int j = 0; j < textures_loaded.size(); j++) {
    if (std::strcmp(textures_loaded[j].path.data(), textureType.c_str()) == 0) {
      textures.push_back(textures_loaded[j]);
      skip = true;
      break;
    }
  }
  if (!skip) {
    // if texture hasn't been loaded already, load it
    Texture texture;
    texture.id = Texture::Load(textureType.c_str(), this->directory + "/images");
    if (texture.id < 0)
      return textures;
    texture.type = typeName;
    texture.path = textureType.c_str();
    textures.push_back(texture);
    textures_loaded.push_back(texture);
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
void Model::BoneTransform(double timeInSec, std::vector<glm::mat4>& transforms) {

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
  const aiNodeAnim* nodeAnimation = Animated::FindNodeAnim(parentAnimation, nodeName);


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

    Animated::CalcInterpolatedScaling(scaling, animationTime, nodeAnimation);
    Animated::CalcInterpolatedRotation(rotation, animationTime, nodeAnimation);
    Animated::CalcInterpolatedPosition(traslation, animationTime, nodeAnimation);
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


/*----------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------*/
void Model::LoadModelOnly(std::string const& path) {
  this->scene = importer.ReadFile(path,
    aiProcess_FlipUVs |
    aiProcess_CalcTangentSpace |
    aiProcess_FindInvalidData |
    aiProcess_ImproveCacheLocality
  );
  // check for errors
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
    return;
  }
  //retrieve the directory path 
  directory = path.substr(0, path.find_last_of('/'));

  ProcessNodesOnly(scene->mRootNode, scene);
}

void Model::LoadModelNoTextures(std::string const& path) {
}

void Model::ProcessNodesOnly(aiNode* node, const aiScene* scene) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    nodeTransform = aiToGlm(node->mTransformation);
    meshes.push_back(ProcessMeshOnly(mesh, scene));
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    ProcessNodesOnly(node->mChildren[i], scene);
  }
}

Mesh Model::ProcessMeshOnly(aiMesh* mesh, const aiScene* scene) {

  std::vector<Vertex> vertices;
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
    if (mesh->HasNormals()) {
      vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
    }
    else {
      vertex.Normal = glm::vec3(1.0f);
    }
    if (mesh->HasTextureCoords(0)){
      vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }
    else {
      vertex.TexCoords = glm::vec2(0.0f);
    }
    if (mesh->HasTangentsAndBitangents()) {
      vertex.Tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
      vertex.Bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
    }
    else {
      vertex.Tangent = glm::vec3(1.0f);
      vertex.Bitangent = glm::vec3(1.0f);
    }
    vertices.push_back(vertex);
  }

  std::vector<unsigned int> indices;
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    // retrieve all indices of the face and store them in the indices vector
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  std::vector<Texture> textures;
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];


  FillPBRTextureVector(textures);
  //TODO::add emission textures
  modelPosition = glm::vec3(nodeTransform[3][0], nodeTransform[3][1], nodeTransform[3][2]);

  return Mesh(vertices, indices, textures);
}


vector<Texture> Model::LoadATexture(aiTextureType type, string typeName) {

  std::string textureType;
  switch (type) {
    case aiTextureType_DIFFUSE:
      textureType = "diffuse.png";
      break;
    case aiTextureType_SPECULAR:
      textureType = "specular.png";
      break;
    case aiTextureType_NORMALS:
      textureType = "normal.png";
      break;
    case aiTextureType_HEIGHT:
      textureType = "height.png";
      break;
    default:
      break;
  }
  vector<Texture> textures;
  bool skip = false;
  for (unsigned int j = 0; j < textures_loaded.size(); j++) {
    if (std::strcmp(textures_loaded[j].path.data(), textureType.c_str()) == 0) {
      textures.push_back(textures_loaded[j]);
      skip = true; 
      break;
    } 
  }
  if (!skip) {
    // if texture hasn't been loaded already, load it
    Texture texture;
    texture.id = Texture::Load(textureType.c_str(), this->directory + "/images");
    if(texture.id < 0)
      return textures;
    texture.type = typeName;
    texture.path = textureType.c_str();
    textures.push_back(texture);
    textures_loaded.push_back(texture);
  }

  return textures;
}




//// checks all material textures of a given type and loads the textures if they're not loaded yet.
//// the required info is returned as a Texture struct.
//vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
//
//
//  vector<Texture> textures;
//  //TODO:: only loads one texture per texture type
//  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
//    aiString str;
//    mat->GetTexture(type, i, &str);
//    // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
//    bool skip = false;
//    for (unsigned int j = 0; j < textures_loaded.size(); j++) {
//      if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
//        textures.push_back(textures_loaded[j]);
//        skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
//        break;
//      }
//    }
//    if (!skip) {
//        // if texture hasn't been loaded already, load it
//        Texture texture;
//        texture.id = Texture::Load(str.C_Str(), this->directory);
//        texture.type = typeName;
//        texture.path = str.C_Str();
//        textures.push_back(texture);
//        textures_loaded.push_back(texture);// store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
//      }
//    }
//
//  /* aiString str;
//   mat->GetTexture(type, 0, &str);
//   if (type == aiTextureType_NORMALS) {
//     Texture texture;
//     std::string normalDir = this->directory + '/' + "snow";
//     texture.id = Texture::Load("normal.jpg", normalDir);
//     texture.type = "material.texture_normal";
//     texture.path = str.C_Str();
//     textures.push_back(texture);
//     textures_loaded.push_back(texture);
//   }*/
//  return textures;
//}