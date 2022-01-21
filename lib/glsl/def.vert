layout(location = 0) in vec2 ver;
layout(location = 1) in vec4 col;

uniform mat4 matrix;

out vec4 color;

void main(void){
    gl_Position = matrix * ver;
    color = col;
}
