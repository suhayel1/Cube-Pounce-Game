#include "WindowManager.h"

#include <iostream>

extern Coordinator coordinator;

// constructors
WindowManager::WindowManager() {}

WindowManager::WindowManager(const unsigned int GLFW_MAJOR_VERS, const unsigned int GLFW_MINOR_VERS, const unsigned int SCREEN_WIDTH, const unsigned int SCREEN_HEIGHT, const std::string WINDOW_NAME) {
	this->initialize(GLFW_MAJOR_VERS, GLFW_MINOR_VERS);
    this->createWindowSucceeded = this->createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME);
    this->loadGladSucceeded = (createWindowSucceeded) ? this->loadGlad() : false;
    this->configureStates();
}


// glfw helpers
void WindowManager::initialize(const unsigned int GLFW_MAJOR_VERS, const unsigned int GLFW_MINOR_VERS) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_MAJOR_VERS);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_MINOR_VERS);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

bool WindowManager::createWindow(const unsigned int SCREEN_WIDTH, const unsigned int SCREEN_HEIGHT, const std::string WINDOW_NAME) {
    this->window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME.c_str(), NULL, NULL);
    if (this->window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;

        return false;
    }
    glfwMakeContextCurrent(this->window);
    glfwSetFramebufferSizeCallback(this->window, framebufferSizeCallback);


    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return true;
}

bool WindowManager::loadGlad() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;

        return false;
    }

    return true;
}

bool WindowManager::isInitialized() {
    return this->createWindowSucceeded && this->loadGladSucceeded;
}

bool WindowManager::isRunning() {
    return !glfwWindowShouldClose(this->window);
}

void WindowManager::configureStates() {
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_MULTISAMPLE);
}

void WindowManager::processInput() {
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);
}

void WindowManager::update() {
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(this->window);
    glfwPollEvents();
}

void WindowManager::shutDown() {
    glfwTerminate();
}

// getters
GLFWwindow* WindowManager::getWindow() {
    return this->window;
}


// callback functions
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}