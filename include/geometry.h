/*
Assignment 4, 159.709, 2019 S1
Wang, Zhichun(Zac), 19024898

This file is from an example program provided by the lecturer of 159.709.
*/

#ifndef GEOMETRY_H
#define GEOMETRY_H

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

// Create Tetrahedron with Positions and Normals
void createTetrahedron(std::vector<glm::vec4> &buffer, std::vector<glm::ivec3> &indexes);

// Create Skybox (Cube with Positions only)
void createSkybox(std::vector<glm::vec4> &buffer, std::vector<glm::ivec3> &indexes);

// Create Cube with Positions and Normals
void createCube(std::vector<glm::vec4> &buffer, std::vector<glm::ivec3> &indexes);

// Create Cube with Positions, Normals and UVs
void createTexturedCube(std::vector<glm::vec4> &buffer, std::vector<glm::ivec3> &indexes);

// Create Torus with Positions and Normals
void createTorus(std::vector<glm::vec4> &buffer, std::vector<glm::ivec3> &indexes, float r1, float r2, int sub1, int sub2);

// Create Sphere with Positions and Normals
void createSphere(std::vector<glm::vec4> &buffer, std::vector<glm::ivec3> &indexes, float r, int sub1, int sub2);

#endif // GEOMETRY_H
