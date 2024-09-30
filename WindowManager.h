#pragma once

#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "Coordinator.h"

class WindowManager {

private:
	GLFWwindow* window;

	bool createWindowSucceeded;
	bool loadGladSucceeded;

public:
	// constructors
	WindowManager();
	WindowManager(const unsigned int GLFW_MAJOR_VERS, const unsigned int GLFW_MINOR_VERS, const unsigned int SCREEN_WIDTH, const unsigned int SCREEN_HEIGHT, const std::string WINDOW_NAME);

	// glfw helpers
	void initialize(const unsigned int GLFW_MAJOR_VERS, const unsigned int GLFW_MINOR_VERS);
	void processInput();
	void configureStates();
	void update();
	void shutDown();

	bool createWindow(const unsigned int SCREEN_WIDTH, const unsigned int SCREEN_HEIGHT, const std::string WINDOW_NAME);
	bool loadGlad();
	bool isInitialized();
	bool isRunning();

	// getters
	GLFWwindow* getWindow();
};

// callbacks
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

#endif