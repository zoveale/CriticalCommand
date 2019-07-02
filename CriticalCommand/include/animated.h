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

  //vector<Joints> joints;
  //Joints joints;
  Joint rootJoint;


  /*  Functions  */

  typedef vector<Vertex> vec;
  typedef vector<unsigned int> iVec;
  typedef vector<Texture> tVec;
  //typedef vector<Joints> jVec;

  // constructor
  Animated(vec vertices, iVec indices, tVec textures, Joints joints) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    //FIXME::add joint
    this->rootJoint = joints.root;
    rootJoint.index = 0;
    this->jointCount = joints.jointCount;

    setupAnimated();
  }

  // render the mesh
  void Draw(Shader shader) {
    //printf("animated class\n");
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
      printf("texture size :%i\n", textures.size());
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

private:
  /*  Render data  */
  unsigned int VBO, EBO;
  unsigned int jointCount;
  /*  Functions    */
  // initializes all the buffer objects/arrays
  void setupAnimated() {
    rootJoint.InverseBindMatrix(glm::mat4(1.0));
    //printf("root joint: %s\n", (rootJoint.name).c_str());


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
    //vertex Joints

    //FIXME::add Joints
    //FIXME::Changed from use of struct to class
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 5, GL_INT, sizeof(Joint), (const GLvoid*)0);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 6, GL_FLOAT, GL_FALSE, sizeof(Joint), (const GLvoid*)16);
    ///
    glBindVertexArray(0);
  }

  //get array of all model space transfors of the joints
  glm::mat4 GetJointTransforms() {
               //FIXME::need to change size of joints dynamaically 
    glm::mat4 jointMatrices[16] = { glm::mat4(1.0) };
    AddJointsToArray(rootJoint, jointMatrices);
  }
  void AddJointsToArray(Joint root, glm::mat4 jointMatrices[]) {
    jointMatrices[root.index] = root.animateTransform;

    for (unsigned int i = 0; i < root.children.size(); i++) {
      AddJointsToArray(root.children[i], jointMatrices);
    }
  }
};
#endif //!ANIMATED_H