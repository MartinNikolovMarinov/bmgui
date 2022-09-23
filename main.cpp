#include <core.h>
#include <plt.h>
#include <fmt/core.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace coretypes;

void ProcessInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

i32 main(i32, const char **, const char **) {
    if (!glfwInit()) {
        fmt::print("Failed to initialize GLFW");
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For MacOS

    defer { glfwTerminate(); };

    auto window = glfwCreateWindow(1024, 768, "EXAMPLE WINDOW", nullptr, nullptr);
    if (!window) {
        fmt::print("Failed to create window");
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = true; // needed in core profile
    if (glewInit() != GLEW_OK) {
        fmt::print("Failed to initialize GLEW");
        return 1;
    }

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        ProcessInput(window);

        glfwSwapBuffers(window);
        glfwPollEvents();

        core::plt::OsThreadSleep(50);
    }

    return 0;
}
