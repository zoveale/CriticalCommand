#ifndef ANIMATED_H
#define ANIMATED_H

#include <GL/glew.h> 

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "shader.h"
#include "modelUtility.h"
#include "skeletal.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;


class Animated {
public:
  /*  Mesh Data  */
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  vector<Texture> textures;
  vector<VertexBoneData> bones;
  unsigned int VAO;

  /*  Functions  */

  typedef std::vector<Vertex> Vec;
  typedef std::vector<unsigned int> iVec;
  typedef std::vector<Texture> tVec;
  typedef std::vector<VertexBoneData> bVec;
  // constructor
  Animated(Vec vertices, iVec indices, tVec textures, bVec weights) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->bones = weights;
    
    indentity = glm::mat4(1.0);

    setupAnimated();
  }

  // render the mesh
  void Draw(Shader shader) {
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
      glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
      // retrieve texture number (the N in diffuse_textureN)
      string number;
      string name = textures[i].type;
      if (name == "texture_diffuse")
        number = std::to_string(diffuseNr++);
      else if (name == "texture_specular")
        number = std::to_string(specularNr++); // transfer unsigned int to stream
      else if (name == "texture_normal")
        number = std::to_string(normalNr++); // transfer unsigned int to stream
      else if (name == "texture_height")
        number = std::to_string(heightNr++); // transfer unsigned int to stream

                         // now set the sampler to the correct texture unit
      glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
      // and finally bind the texture
      glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
  }

  //glm::mat4 BoneTransform(float timeInSec, std::vector<glm::mat4>& transforms) {}


private:
  glm::mat4 indentity;
  /*  Render data  */
  unsigned int VBO, EBO, BONES;
  
  static const int MAX_BONES = 100;
  unsigned int bonesGPU[MAX_BONES];


  /*  Functions    */
  // initializes all the buffer objects/arrays
  void setupAnimated() {
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    //FIXME::replace 0,1,2.. with Enum
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    //SizeofVertex
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    

    //FIXME::add Joints
    ////FIXME::Changed from use of struct to class

    glGenBuffers(1, &BONES);
    glBindBuffer(GL_ARRAY_BUFFER, BONES);
    glBufferData(GL_ARRAY_BUFFER, bones.size() * sizeof(bones[0]), &bones[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(VertexBoneData), (void*)0);
    //weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData),
                                 (void*)offsetof(VertexBoneData, weights));
    ///
    glBindVertexArray(0);
  }

  //FIXME::might as well do this for all uniforms in the shader
  public:
    //HelperFunction for ReadNodeHierarchy
    static const aiNodeAnim* FindNodeAnim(const aiAnimation* parent, const string name) {
      for (unsigned int i = 0; i < parent->mNumChannels; i++) {
        if (std::string(parent->mChannels[i]->mNodeName.data) == name) {
          return parent->mChannels[i];
        }
      }
      return nullptr;
    }
    ///
    typedef const aiNodeAnim* aiAnim;
    static void CalcInterpolatedScaling(aiVector3D& scaling, float animationTime, aiAnim parent) {
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
    static void CalcInterpolatedRotation(aiQuaternion& rotation, float animationTime, aiAnim parent) {
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
    static void CalcInterpolatedPosition(aiVector3D& translation, float animationTime, aiAnim parent) {
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
    static unsigned int FindScaling(float animationTime, aiAnim parent) {
      assert(parent->mNumScalingKeys > 0);

      for (unsigned int i = 0; i < parent->mNumScalingKeys - 1; i++) {
        if (animationTime < (float)parent->mScalingKeys[i + 1].mTime) {
          return i;
        }
      }

      assert(0);

      return 0;
    }
    static unsigned int FindRotation(float animationTime, aiAnim parent) {
      assert(parent->mNumRotationKeys > 0);

      for (unsigned int i = 0; i < parent->mNumRotationKeys - 1; i++) {
        if (animationTime < (float)parent->mRotationKeys[i + 1].mTime) {
          return i;
        }
      }

      assert(0);

      return 0;
    }
    static unsigned int FindPosition(float animationTime, aiAnim parent) {
      assert(parent->mNumPositionKeys > 0);
      for (unsigned int i = 0; i < parent->mNumPositionKeys - 1; i++) {
        if (animationTime < (float)parent->mPositionKeys[i + 1].mTime) {
          return i;
        }
      }

      assert(0);

      return 0;
    }

};
#endif //!ANIMATED_H