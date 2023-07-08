#include "backend/gl/gl_backend.h"
#include "processing.h"
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

backend *gl = null;

void todo(const std::string& text){
    std::cout << "TODO: " << text << std::endl;
}

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(){
    gl = new GLBackend();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Construct the window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Template", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create the GLFW window\n";
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // Handle view port dimensions
    gl->viewport(800, 600);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height){
        gl->viewport( width, height);
    });

    // This is the render loop
    while (!glfwWindowShouldClose(window)){
        ProcessInput(window);

        gl->clearColor(1.00f, 0.2f, 0.9f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    delete gl;
    return 0;
}
