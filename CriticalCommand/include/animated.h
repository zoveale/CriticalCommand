#ifndef ANIMATED_H
#define ANIMATED_H

#include <GL/glew.h> 

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "shader.h"
#include "modelUtility.h"

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
    //FIXME::add joint
    indentity = glm::mat4(1.0);

    setupAnimated();
  }

  // render the mesh
  void Draw(Shader shader) {

    
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
   /* for (int i = 0; i < bones.size(); i++) {
      for (int j = 0; j < 4; j++) {
        printf("bone[%i]:id[%i]: weight: %f\n", i, bones[i].ids[j], bones[i].weights[j]);
      }
    }*/
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
  
};
#endif //!ANIMATED_H