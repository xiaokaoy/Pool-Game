/*
Assignment 4, 159.709, 2019 S1
Wang, Zhichun(Zac), 19024898

This file implements a class: Skybox. 

Part of the code is from an example program provided by  the lecturer of 159.709.

This class has only one member function: Draw.
*/

// System Headers
#include <iostream>
#include <fstream>
#include <cmath>

// System Headers
#include <iostream>
#include <vector>

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


// GLM Headers
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"


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
#include "skybox.h"
#include "image.h"
#include "geometry.h"


Skybox::Skybox(const char* textureMapFiles[6], GLuint program)
{
	this->program 		= program;

	// Craate VAO,VBO,EBO
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	
	// Bind VAO, VBO & EBO
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Load Texture Map
	int x, y, n;
	
	// Load Cubemap
	texture = loadTextureCubeMap(textureMapFiles, x, y, n);

	// ----------------------------------------
	// Skybox
	// ----------------------------------------

	// Skybox Program
	glUseProgram(program);

	// Vertex and Index buffers (host)
	std::vector<glm::vec4> skybox_buffer;
	std::vector<glm::ivec3> skybox_indexes;

	// Create Skybox
	createSkybox(skybox_buffer, skybox_indexes);

	// Load Vertex Data
	glBufferData(GL_ARRAY_BUFFER, skybox_buffer.size() * sizeof(glm::vec4), skybox_buffer.data(), GL_STATIC_DRAW);

	// Load Element Data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, skybox_indexes.size() * sizeof(glm::ivec3), skybox_indexes.data(), GL_STATIC_DRAW);
	elementCount = skybox_indexes.size() * 3;
	
	// Get Position Attribute location (must match name in shader)
	GLuint skybox_posLoc = glGetAttribLocation(program, "vert_Position");

	// Set Vertex Attribute Pointers
	glVertexAttribPointer(skybox_posLoc, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), NULL);

	// Enable Vertex Attribute Arrays
	glEnableVertexAttribArray(skybox_posLoc);

	// ----------------------------------------
	// Set Texture Unit
	glUniform1i(glGetUniformLocation(program, "u_texture_Map"), 0);

	// Unbind VAO, VBO & EBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
	
Skybox::~Skybox()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Skybox::Draw()
{	
	glUseProgram(program);

	#if 1	
	
	// Set active Texture Unit 0
	glActiveTexture(GL_TEXTURE0);

	// Bind Texture Map
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (GLvoid*)NULL);
	glBindVertexArray(0);

	// The following unblinding may be optional.
	
	// Set active Texture Unit 0
	glActiveTexture(GL_TEXTURE0);

	// Unbind Texture Map
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		#endif
}

