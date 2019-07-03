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
  unsigned int VAO;

  /*  Functions  */

  typedef std::vector<Vertex> vec;
  typedef std::vector<unsigned int> iVec;
  typedef std::vector<Texture> tVec;
  typedef std::vector<VertexBoneData> bVec;
  // constructor
  Animated(vec vertices, iVec indices, tVec textures, bVec weights) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    //FIXME::add joint

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

private:
  /*  Render data  */
  unsigned int VBO, EBO;

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
    //glEnableVertexAttribArray(5);
    //glVertexAttribIPointer(5, 5, GL_INT, sizeof(Joint), (const GLvoid*)0);
    //glEnableVertexAttribArray(6);
    //glVertexAttribPointer(6, 6, GL_FLOAT, GL_FALSE, sizeof(Joint), (const GLvoid*)16);
    ///
    glBindVertexArray(0);
  }

  
};
#endif //!ANIMATED_H