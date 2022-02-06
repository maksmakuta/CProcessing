#version 330 core
in vec3 ver;
uniform mat4 matrix;
void main(void){
    gl_Position = matrix * vec4(ver,1.0f);
}
