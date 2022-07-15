#include <iostream>
#include <stdio.h>
#include <string>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "window.hpp"

void Window::FramebufferSizeCallback(GLFWwindow *Window, int Width, int Height)
{
    glViewport(0, 0, Width, Height);
}

Window::Window(int Width, int Height, const std::string &WindowName)
{
    glfwInit();

    m_WindowParams = {
        Width,
        Height,
        WindowName,
        NULL,
        NULL};

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = CreateWindow(m_WindowParams);
    if (!m_Window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    while (!glfwWindowShouldClose(m_Window))
    {
        ProcessInput(m_Window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    glfwTerminate();
}

GLFWwindow *Window::CreateWindow(WindowParams Params)
{
    return glfwCreateWindow(Params.m_Width, Params.m_Height, Params.m_Title.c_str(), Params.m_Monitor, Params.m_Share);
}

void Window::ProcessInput(GLFWwindow *Window)
{
    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(Window, true);
}
