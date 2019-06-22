/*
Assignment 4, 159.709, 2019 S1
Wang, Zhichun(Zac), 19024898

This file implement classes for the table and the cue
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
#include "table.h"
#include "image.h"


Table::Table(GLuint program)
{
	this->program 		= program;
	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	
	// Bind VAO, VBO & EBO	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);


#define OUTER_COLOR 0.1f, 0.1f, 0.1f, 1.0f
#define INNER_WALL_COLOR 1.0f, 0, 0, 1.0f
#define BOTTOM_COLOR 0, 1.0f, 0, 1.0f

	float buffer[] =
	{
		// ---------------------------------OUTER wall----------------------------------------
		
		// front
		-TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		OUTER_COLOR,
			
		-TABLE_OUTER_WIDTH/2, 0, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		OUTER_COLOR,		
			
		TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		OUTER_COLOR,

		-TABLE_OUTER_WIDTH/2, 0, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		OUTER_COLOR,		
			
		TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		OUTER_COLOR,
	
		TABLE_OUTER_WIDTH/2, 0, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		OUTER_COLOR,

		//back
		-TABLE_OUTER_WIDTH/2, TABLE_HIGHT, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 0, -1.0f, 0, // normal
		OUTER_COLOR,
			
		-TABLE_OUTER_WIDTH/2, 0, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 0, -1.0f, 0, // normal
		OUTER_COLOR,		
			
		TABLE_OUTER_WIDTH/2, TABLE_HIGHT, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 0, -1.0f, 0, // normal
		OUTER_COLOR,

		-TABLE_OUTER_WIDTH/2, 0, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 0, -1.0f, 0, // normal
		OUTER_COLOR,		
			
		TABLE_OUTER_WIDTH/2, TABLE_HIGHT, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 0, -1.0f, 0, // normal
		OUTER_COLOR,
	
		TABLE_OUTER_WIDTH/2, 0, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 0, -1.0f, 0, // normal
		OUTER_COLOR,

		//left
		-TABLE_OUTER_WIDTH/2, TABLE_HIGHT, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		OUTER_COLOR,
			
		-TABLE_OUTER_WIDTH/2, 0, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		OUTER_COLOR,
			
		-TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		OUTER_COLOR,			
			
		-TABLE_OUTER_WIDTH/2, 0, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		OUTER_COLOR,
			
		-TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		OUTER_COLOR,
			
		-TABLE_OUTER_WIDTH/2, 0, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		OUTER_COLOR,

		// rigght
		TABLE_OUTER_WIDTH/2, TABLE_HIGHT, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		OUTER_COLOR,
			
		TABLE_OUTER_WIDTH/2, 0, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		OUTER_COLOR,
			
		TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		OUTER_COLOR,			
			
		TABLE_OUTER_WIDTH/2, 0, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		OUTER_COLOR,
			
		TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		OUTER_COLOR,
			
		TABLE_OUTER_WIDTH/2, 0, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		OUTER_COLOR,

		// ---------------------------------INNER wall----------------------------------------
		
		// front
		-TABLE_INNER_WIDTH/2, TABLE_HIGHT, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		INNER_WALL_COLOR,
			
		-TABLE_INNER_WIDTH/2, 0, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		INNER_WALL_COLOR,		
			
		TABLE_INNER_WIDTH/2, TABLE_HIGHT, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		INNER_WALL_COLOR,

		-TABLE_INNER_WIDTH/2, 0, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		INNER_WALL_COLOR,		
			
		TABLE_INNER_WIDTH/2, TABLE_HIGHT, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		INNER_WALL_COLOR,
	
		TABLE_INNER_WIDTH/2, 0, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		INNER_WALL_COLOR,

		//back
		-TABLE_INNER_WIDTH/2, TABLE_HIGHT, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 0, -1.0f, 0, // normal
		INNER_WALL_COLOR,
			
		-TABLE_INNER_WIDTH/2, 0, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 0, -1.0f, 0, // normal
		INNER_WALL_COLOR,		
			
		TABLE_INNER_WIDTH/2, TABLE_HIGHT, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 0, -1.0f, 0, // normal
		INNER_WALL_COLOR,

		-TABLE_INNER_WIDTH/2, 0, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 0, -1.0f, 0, // normal
		INNER_WALL_COLOR,		
			
		TABLE_INNER_WIDTH/2, TABLE_HIGHT, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 0, -1.0f, 0, // normal
		INNER_WALL_COLOR,
	
		TABLE_INNER_WIDTH/2, 0, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 0, -1.0f, 0, // normal
		INNER_WALL_COLOR,

		//left
		-TABLE_INNER_WIDTH/2, TABLE_HIGHT, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		INNER_WALL_COLOR,
			
		-TABLE_INNER_WIDTH/2, 0, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		INNER_WALL_COLOR,
			
		-TABLE_INNER_WIDTH/2, TABLE_HIGHT, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		INNER_WALL_COLOR,			
			
		-TABLE_INNER_WIDTH/2, 0, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		INNER_WALL_COLOR,
			
		-TABLE_INNER_WIDTH/2, TABLE_HIGHT, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		INNER_WALL_COLOR,
			
		-TABLE_INNER_WIDTH/2, 0, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		INNER_WALL_COLOR,

		// rigght
		TABLE_INNER_WIDTH/2, TABLE_HIGHT, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		INNER_WALL_COLOR,
			
		TABLE_INNER_WIDTH/2, 0, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		INNER_WALL_COLOR,
			
		TABLE_INNER_WIDTH/2, TABLE_HIGHT, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		INNER_WALL_COLOR,			
			
		TABLE_INNER_WIDTH/2, 0, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		INNER_WALL_COLOR,
			
		TABLE_INNER_WIDTH/2, TABLE_HIGHT, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		INNER_WALL_COLOR,
			
		TABLE_INNER_WIDTH/2, 0, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		INNER_WALL_COLOR,

		// ---------------------------------BOTTOM----------------------------------------
		-TABLE_INNER_WIDTH/2, 0, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		BOTTOM_COLOR,
		
		-TABLE_INNER_WIDTH/2, 0, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		BOTTOM_COLOR,
		
		TABLE_INNER_WIDTH/2, 0, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		BOTTOM_COLOR,
		
		-TABLE_INNER_WIDTH/2, 0, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		BOTTOM_COLOR,
		
		TABLE_INNER_WIDTH/2, 0, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		BOTTOM_COLOR,
		
		TABLE_INNER_WIDTH/2, 0, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		BOTTOM_COLOR,


#define HOLE_HEIGHT 0.00001f
#define HOLE_COLOR	0.0f, 0.0f, 0.0f, 1.0f

		//  top left hole
		-TABLE_INNER_WIDTH/2, HOLE_HEIGHT, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		-TABLE_INNER_WIDTH/2, HOLE_HEIGHT, -TABLE_INNER_LENGTH/2+RADIUS*HOLE_RADIUS_RATIO, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		-TABLE_INNER_WIDTH/2+RADIUS*HOLE_RADIUS_RATIO, HOLE_HEIGHT, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		-TABLE_INNER_WIDTH/2, HOLE_HEIGHT, -TABLE_INNER_LENGTH/2+RADIUS*HOLE_RADIUS_RATIO, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		-TABLE_INNER_WIDTH/2+RADIUS*HOLE_RADIUS_RATIO, HOLE_HEIGHT, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		-TABLE_INNER_WIDTH/2+RADIUS*HOLE_RADIUS_RATIO, HOLE_HEIGHT, -TABLE_INNER_LENGTH/2+RADIUS*HOLE_RADIUS_RATIO, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		
		//  bottom left hole
		-TABLE_INNER_WIDTH/2, HOLE_HEIGHT, TABLE_INNER_LENGTH/2-RADIUS*HOLE_RADIUS_RATIO, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		-TABLE_INNER_WIDTH/2, HOLE_HEIGHT, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		-TABLE_INNER_WIDTH/2+RADIUS*HOLE_RADIUS_RATIO, HOLE_HEIGHT, TABLE_INNER_LENGTH/2-RADIUS*HOLE_RADIUS_RATIO, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		-TABLE_INNER_WIDTH/2, HOLE_HEIGHT, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		-TABLE_INNER_WIDTH/2+RADIUS*HOLE_RADIUS_RATIO, HOLE_HEIGHT, TABLE_INNER_LENGTH/2-RADIUS*HOLE_RADIUS_RATIO, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		-TABLE_INNER_WIDTH/2+RADIUS*HOLE_RADIUS_RATIO, HOLE_HEIGHT, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		
		//  top right hole
		TABLE_INNER_WIDTH/2-RADIUS*HOLE_RADIUS_RATIO, HOLE_HEIGHT, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		TABLE_INNER_WIDTH/2-RADIUS*HOLE_RADIUS_RATIO, HOLE_HEIGHT, -TABLE_INNER_LENGTH/2+RADIUS*HOLE_RADIUS_RATIO, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		TABLE_INNER_WIDTH/2, HOLE_HEIGHT, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		TABLE_INNER_WIDTH/2-RADIUS*HOLE_RADIUS_RATIO, HOLE_HEIGHT, -TABLE_INNER_LENGTH/2+RADIUS*HOLE_RADIUS_RATIO, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		TABLE_INNER_WIDTH/2, HOLE_HEIGHT, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		TABLE_INNER_WIDTH/2, HOLE_HEIGHT, -TABLE_INNER_LENGTH/2+RADIUS*HOLE_RADIUS_RATIO, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		
		//  bottom right hole
		TABLE_INNER_WIDTH/2-RADIUS*HOLE_RADIUS_RATIO, HOLE_HEIGHT, TABLE_INNER_LENGTH/2-RADIUS*HOLE_RADIUS_RATIO, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		TABLE_INNER_WIDTH/2-RADIUS*HOLE_RADIUS_RATIO, HOLE_HEIGHT, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		TABLE_INNER_WIDTH/2, HOLE_HEIGHT, TABLE_INNER_LENGTH/2-RADIUS*HOLE_RADIUS_RATIO, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		TABLE_INNER_WIDTH/2-RADIUS*HOLE_RADIUS_RATIO, HOLE_HEIGHT, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		TABLE_INNER_WIDTH/2, HOLE_HEIGHT, TABLE_INNER_LENGTH/2-RADIUS*HOLE_RADIUS_RATIO, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		TABLE_INNER_WIDTH/2, HOLE_HEIGHT, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		HOLE_COLOR,
		
		
		
		// ---------------------------------horizontal outer surface----------------------------------------

		// left
		-TABLE_OUTER_WIDTH/2, TABLE_HIGHT, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		-TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		-TABLE_INNER_WIDTH/2, TABLE_HIGHT, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,		
		
		-TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		-TABLE_INNER_WIDTH/2, TABLE_HIGHT, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		-TABLE_INNER_WIDTH/2, TABLE_HIGHT, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,

		// right
		TABLE_OUTER_WIDTH/2, TABLE_HIGHT, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		TABLE_INNER_WIDTH/2, TABLE_HIGHT, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,		
		
		TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		TABLE_INNER_WIDTH/2, TABLE_HIGHT, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		TABLE_INNER_WIDTH/2, TABLE_HIGHT, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,

		//near
		-TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		-TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,		
		
		-TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		TABLE_OUTER_WIDTH/2, TABLE_HIGHT, TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		//far
		-TABLE_OUTER_WIDTH/2, TABLE_HIGHT, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		-TABLE_OUTER_WIDTH/2, TABLE_HIGHT, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		TABLE_OUTER_WIDTH/2, TABLE_HIGHT, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,		
		
		-TABLE_OUTER_WIDTH/2, TABLE_HIGHT, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		TABLE_OUTER_WIDTH/2, TABLE_HIGHT, -TABLE_INNER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
		TABLE_OUTER_WIDTH/2, TABLE_HIGHT, -TABLE_OUTER_LENGTH/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		OUTER_COLOR,
		
	};
	
	// Load Vertex Data	
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);

	// Get Position Attribute location (must match name in shader)
	GLuint posLoc = glGetAttribLocation(program, "vert_Position");

	// Get Normal Attribute location (must match name in shader)
	GLuint norLoc = glGetAttribLocation(program, "vert_Normal");

	// Get Texture Attribute location (must match name in shader)
	GLuint texLoc = glGetAttribLocation(program, "vert_UV");

	// Set Vertex Attribute Pointers
	glVertexAttribPointer(posLoc, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), NULL);
	glVertexAttribPointer(norLoc, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(4*sizeof(float)));
	glVertexAttribPointer(texLoc, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(8*sizeof(float)));

	// Enable Vertex Attribute Arrays
	glEnableVertexAttribArray(posLoc);
	glEnableVertexAttribArray(norLoc);
	glEnableVertexAttribArray(texLoc);
	elementCount = sizeof(buffer) / (sizeof(float)*4*3);
}	
		


	
Table::~Table()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Table::Draw()
{	
	glUseProgram(program);

	glm::mat4 modelMatrix = glm::mat4(1.0f);

	glUniformMatrix4fv(glGetUniformLocation(program, "u_Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));


	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, elementCount);
	glBindVertexArray(0);
}

Cue::Cue(GLuint program)
{
	this->program 		= program;
	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	
	// Bind VAO, VBO & EBO	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);


#define STICK_HEIGHT 1.5f
#define STICK_THICKNESS 0.04f
#define STICK_COLOR 1.0f, 1.0f, 1.0f, 1.0f

	float buffer[] =
	{
		// front
		-STICK_THICKNESS/2, STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		STICK_COLOR,
			
		-STICK_THICKNESS/2, -STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		STICK_COLOR,
			
		STICK_THICKNESS/2, STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		STICK_COLOR,
		
		-STICK_THICKNESS/2, -STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		STICK_COLOR,
			
		STICK_THICKNESS/2, STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		STICK_COLOR,
			
		STICK_THICKNESS/2, -STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		STICK_COLOR,
			
		//back
		-STICK_THICKNESS/2, STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		STICK_COLOR,
			
		-STICK_THICKNESS/2, -STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		STICK_COLOR,
			
		STICK_THICKNESS/2, STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		STICK_COLOR,
		
		-STICK_THICKNESS/2, -STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		STICK_COLOR,
			
		STICK_THICKNESS/2, STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		STICK_COLOR,
			
		STICK_THICKNESS/2, -STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		0, 0, 1.0f, 0, // normal
		STICK_COLOR,			

		//left
		-STICK_THICKNESS/2, STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		STICK_COLOR,			
			
		-STICK_THICKNESS/2, -STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		STICK_COLOR,		

		-STICK_THICKNESS/2, STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		STICK_COLOR,					
			
		-STICK_THICKNESS/2, -STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		STICK_COLOR,		

		-STICK_THICKNESS/2, STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		STICK_COLOR,			
			
		-STICK_THICKNESS/2, -STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		-1.0f, 0, 0, 0, // normal
		STICK_COLOR,		

		// rigght
		STICK_THICKNESS/2, STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		STICK_COLOR,			
			
		STICK_THICKNESS/2, -STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		STICK_COLOR,		

		STICK_THICKNESS/2, STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		STICK_COLOR,					
			
		STICK_THICKNESS/2, -STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		STICK_COLOR,		

		STICK_THICKNESS/2, STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		STICK_COLOR,			
			
		STICK_THICKNESS/2, -STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		1.0f, 0, 0, 0, // normal
		STICK_COLOR,

		// top
		-STICK_THICKNESS/2, STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		STICK_COLOR,					

		-STICK_THICKNESS/2, STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		STICK_COLOR,	
		
		STICK_THICKNESS/2, STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		STICK_COLOR,					
		
		-STICK_THICKNESS/2, STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		STICK_COLOR,	
		
		STICK_THICKNESS/2, STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		STICK_COLOR,	

		STICK_THICKNESS/2, STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		STICK_COLOR,			
			
		
		// bottom
		-STICK_THICKNESS/2, -STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		STICK_COLOR,					

		-STICK_THICKNESS/2, -STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		STICK_COLOR,	
		
		STICK_THICKNESS/2, -STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		STICK_COLOR,					
		
		-STICK_THICKNESS/2, -STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		STICK_COLOR,	
		
		STICK_THICKNESS/2, -STICK_HEIGHT/2, -STICK_THICKNESS/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		STICK_COLOR,	

		STICK_THICKNESS/2, -STICK_HEIGHT/2, STICK_THICKNESS/2, 1.0f, // coordinates
		0, 1.0f, 0, 0, // normal
		STICK_COLOR,	
	};

	// Load Vertex Data	
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);

	// Get Position Attribute location (must match name in shader)
	GLuint posLoc = glGetAttribLocation(program, "vert_Position");

	// Get Normal Attribute location (must match name in shader)
	GLuint norLoc = glGetAttribLocation(program, "vert_Normal");

	// Get Texture Attribute location (must match name in shader)
	GLuint texLoc = glGetAttribLocation(program, "vert_UV");

	// Set Vertex Attribute Pointers
	glVertexAttribPointer(posLoc, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), NULL);
	glVertexAttribPointer(norLoc, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(4*sizeof(float)));
	glVertexAttribPointer(texLoc, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(8*sizeof(float)));

	// Enable Vertex Attribute Arrays
	glEnableVertexAttribArray(posLoc);
	glEnableVertexAttribArray(norLoc);
	glEnableVertexAttribArray(texLoc);
	elementCount = sizeof(buffer) / (sizeof(float)*4*3);
}	
		


	
Cue::~Cue()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Cue::Draw(float cueball_x, float cueball_z, float hitdirection)
{	
	glUseProgram(program);
	
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), 
								glm::vec3(cueball_x, RADIUS*1.5f, cueball_z)) *	
							glm::rotate(glm::mat4(1.0f), (float)M_PI+hitdirection, glm::vec3(0.0f, 1.0f, 0.0f)) *	
							glm::rotate(glm::mat4(1.0f), (float)-M_PI/2.5f, glm::vec3(0.0f, 0.0f, 1.0f)) *
							glm::translate(glm::mat4(1.0f), 
								glm::vec3(0.0f, STICK_HEIGHT/2+RADIUS*2, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(program, "u_Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, elementCount);
	glBindVertexArray(0);
}

