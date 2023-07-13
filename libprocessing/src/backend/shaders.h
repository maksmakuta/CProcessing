#ifndef SHADERS_H
#define SHADERS_H

#include <string>

std::string vertexCode =
R"r(
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 col;
layout (location = 2) in vec2 tex;

out vec2 TexCoords;
out vec4 Colour;

uniform mat4 mat;

void main(){
    TexCoords = tex;
    Colour = col;
    gl_Position = mat * vec4(pos, 1.0);
}
)r";

std::string fragmentCode =
R"r(
#version 330 core
in vec4 Colour;
in vec2 TexCoords;

out vec4 color;

uniform sampler2D image;

void main(){
    color = Colour * texture(image, TexCoords);
}
)r";

#endif // SHADERS_H
