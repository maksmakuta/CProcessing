#ifndef SHADERS_H
#define SHADERS_H

#include <string>

std::string vertexCode =
R"r(
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 col;
layout (location = 2) in vec2 tex;
layout (location = 3) in float texID;

out vec2 TexCoords;
out vec4 Colour;
flat out int id;

uniform mat4 mat;

void main(){
    TexCoords = tex;
    id = int(texID);
    Colour = col;
    gl_Position = mat * vec4(pos, 1.0);
}
)r";

std::string fragmentCode =
R"r(
#version 330 core
in vec4 Colour;
in vec2 TexCoords;
flat in int id;

out vec4 color;

uniform sampler2DArray images;

void main(){
    if(id >= 0){
        color = Colour * texture(images, vec3(TexCoords,float(id)));
    }else
        color = Colour;
}
)r";

#endif // SHADERS_H
