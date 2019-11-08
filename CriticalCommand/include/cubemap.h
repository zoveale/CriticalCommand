#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <GL/glew.h>
#include <vector>
#include <string>

class Cubemap {
public:
  Cubemap();
  unsigned int LoadCubemap(std::vector<std::string> faces);

private:

};
#endif // !CUBEMAP_H
