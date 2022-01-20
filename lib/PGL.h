#ifndef PGL_H
#define PGL_H

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include "PShader.h"

struct Renderer{
    glm::mat4 matrix;
    PShader shader;
};

#endif
