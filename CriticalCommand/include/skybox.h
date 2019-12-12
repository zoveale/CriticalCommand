#ifndef VEALE_909BC959_DC2C_4B6D_A279_3100195E4C83
#define VEALE_909BC959_DC2C_4B6D_A279_3100195E4C83

#include "model.h"

const float skyboxVertices[] = {
  // positions          
  -1.0f,  1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,

   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,

  -1.0f,  1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f,  1.0f
};

class Skybox {
public:
  Skybox(){}

  Skybox(Shader *shader) {
    faces.push_back("resources/cubemap/textures/frozendusk_rt.jpg");
    faces.push_back("resources/cubemap/textures/frozendusk_lf.jpg");
    faces.push_back("resources/cubemap/textures/frozendusk_up.jpg");
    faces.push_back("resources/cubemap/textures/frozendusk_dn.jpg");
    faces.push_back("resources/cubemap/textures/frozendusk_bk.jpg");
    faces.push_back("resources/cubemap/textures/frozendusk_ft.jpg");

    cubemapTexture = Texture::loadCubemap(faces);

    // skybox VAO
    
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    shaderPointer = shader;
    shaderPointer->Use();
    shaderPointer->SetInt("skybox", 0);
  }
  void Draw(glm::mat3 camera, glm::mat4 view, glm::mat4 projection) {
    // draw skybox as last
    //glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    shaderPointer->Use();
    //glm::lookAt(Position, Position + Front, Up)
    view = glm::mat4(glm::mat3(camera)); // remove translation from the view matrix
    shaderPointer->SetMat4("view", view);
    shaderPointer->SetMat4("projection", projection);
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    //glDepthFunc(GL_LESS);
  }
  
private:
  std::vector<std::string> faces;
  unsigned int cubemapTexture;
  unsigned int skyboxVAO, skyboxVBO;
  Shader *shaderPointer;

  
};
#endif // !VEALE_909BC959_DC2C_4B6D_A279_3100195E4C83
