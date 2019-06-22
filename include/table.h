/*
Assignment 4, 159.709, 2019 S1
Wang, Zhichun(Zac), 19024898

This file defines a class: Table.

For more detail, see globj.cpp
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


#define RADIUS				0.2f

#define HOLE_RADIUS_RATIO  1.5f 		

#define TABLE_INNER_WIDTH	3.0f
#define TABLE_INNER_LENGTH	4.0f

#define TABLE_OUTER_WIDTH	3.2f
#define TABLE_OUTER_LENGTH	4.2f

#define TABLE_HIGHT         0.2f

class Table
{
private:
	GLuint vao, vbo;
	GLuint texture;
	int elementCount;
	GLuint program;

	
public:
    Table(GLuint program);
	~Table();
	void Draw();
};


class Cue
{
private:
	GLuint vao, vbo, ebo;
	int elementCount;
	GLuint program;
	
public:
    Cue(GLuint program);
	~Cue();
	void Draw(float cueball_x, float cueball_z, float hitdirection);
};

