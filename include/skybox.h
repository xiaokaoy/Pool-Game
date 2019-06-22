/*
Assignment 4, 159.709, 2019 S1
Wang, Zhichun(Zac), 19024898

This file defines a class: Skybox.

For more detail, see skybox.cpp
*/

// System Headers
#include <iostream>
#include <fstream>
#include <cmath>

// OpenGL Headers
#if defined(_WIN32)
	#include <GL/glew.h>
	#if defined(GLEW_EGL)
		#include <GL/eglew.h>
	#elif defined(GLEW_OSMESA)
		#define GLAPI extern
		#include <GL/osmesa.h>
	#elif defined(_WIN32)
		#include <GL/wglew.h>
	#elif !defined(__APPLE__) && !defined(__HAIKU__) || defined(GLEW_APPLE_GLX)
		#include <GL/glxew.h>
	#endif

	// OpenGL Headers
	#define GLFW_INCLUDE_GLCOREARB
	#include <GLFW/glfw3.h>
#elif defined(__APPLE__)
	#define GLFW_INCLUDE_GLCOREARB
	#include <GLFW/glfw3.h>
	#include <OpenGL/gl3.h>
#endif

// Project Headers
#include "shader.h"
#include "utils.h"

class Skybox
{
public:
	GLuint vao, vbo, ebo;
	GLuint texture;
	int elementCount;
	GLuint program;
	

public:
    Skybox(const char* textureMapFiles[6], GLuint program);
	~Skybox();
	void Draw();
};

