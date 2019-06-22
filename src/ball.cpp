/*
Assignment 4, 159.709, 2019 S1
Wang, Zhichun(Zac), 19024898

This file implements the Ball class

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
#include "ball.h"
#include "image.h"


Ball::Ball(float radius, const char* textureMapFile, GLuint sky_texture, GLuint program)
{
	onTable = true;
	velocity = 0;
	direction = 0;
	rotateMatrix = glm::rotate(glm::mat4(1.0f), float(-M_PI/2), glm::vec3(0.0f, 0.0f, 1.0f));
	
	this->program 		= program;
	this->sky_texture	= sky_texture;
	this->radius		= radius;
	
	// Another approach is that all the planets share the same VAO/VBO/EBO, which would save GL buffers.
	// This can be achieved by declaring a static member in class Ball,
	// and the static member can be of another class type, whose constructor can do things like creating VAO/VBO/EBO.
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	
	// Bind VAO, VBO & EBO
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	std::vector<glm::vec4> buffer;
	std::vector<glm::ivec3> indexes;	

	// Use degrees instead of radians to avoid comparison between the values of two floating-point numbers, which might not produce a result you expected
	// I know 'longitude' should be -180 to 180 degrees in geography, but I prefer 0-360.
	#define MERIDIAN_INTERVAL  6  // 360 must be divisible ty it
	#define LATITUDE_INTERVAL  6  // 180 must be divisible by it
	for (int longitude = 0; longitude <= 360; longitude += MERIDIAN_INTERVAL) // Both 0 and 360 are included to avoid texture tearing
		for (int latitude = 90; latitude >= -90; latitude -= LATITUDE_INTERVAL)
		{

			glm::vec3 pos = glm::vec3(glm::cos(glm::radians((float)latitude)) * glm::cos(glm::radians((float)longitude)), 
							glm::sin(glm::radians((float)latitude)),
							-glm::cos(glm::radians((float)latitude)) * glm::sin(glm::radians((float)longitude))) 
							* radius;

			buffer.push_back(glm::vec4(pos, 1.0f));
			buffer.push_back(glm::vec4(glm::normalize(pos), 0.0f));			
			buffer.push_back(glm::vec4(longitude/360.0f, (latitude+90)/180.0f, 0.0f, 0.0f)); // UV coordinates

		}
		
	// Load Vertex Data
	glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(glm::vec4), buffer.data(), GL_STATIC_DRAW);

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


	// Load Element Data
	#define VERTEX_NUM_ON_EACH_MERIDIAN  (180/LATITUDE_INTERVAL + 1)
	for (int i = 0; i < 360/MERIDIAN_INTERVAL; i++)
	{
		// For the area between every two adjacent meridians, draw triangles from top (latitude 90) to bottom (latitude -90)
		indexes.push_back(glm::ivec3(i*VERTEX_NUM_ON_EACH_MERIDIAN, i*VERTEX_NUM_ON_EACH_MERIDIAN+1, (i+1)*VERTEX_NUM_ON_EACH_MERIDIAN+1));

		int j;
		
		for (j = 1; j < 180/LATITUDE_INTERVAL-1; j++)
		{
			indexes.push_back(glm::ivec3(i*VERTEX_NUM_ON_EACH_MERIDIAN+j, i*VERTEX_NUM_ON_EACH_MERIDIAN+j+1, (i+1)*VERTEX_NUM_ON_EACH_MERIDIAN+j));
			indexes.push_back(glm::ivec3(i*VERTEX_NUM_ON_EACH_MERIDIAN+j+1, (i+1)*VERTEX_NUM_ON_EACH_MERIDIAN+j, (i+1)*VERTEX_NUM_ON_EACH_MERIDIAN+j+1));			
		}
		
		indexes.push_back(glm::ivec3(i*VERTEX_NUM_ON_EACH_MERIDIAN+j, i*VERTEX_NUM_ON_EACH_MERIDIAN+j+1, (i+1)*VERTEX_NUM_ON_EACH_MERIDIAN+j));
	}	// Actually the last vertex (at longitude 360, latitude -90) is not used, but it doesn't matter.

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(glm::ivec3), indexes.data(), GL_STATIC_DRAW);
	elementCount = indexes.size() * 3;

	int x, y, n;
	texture = loadTexture2D(textureMapFile, x, y, n);
	
	// Unbind VAO, VBO & EBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
	
Ball::~Ball()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Ball::SetVelocity(float velocity, float direction)
{
	this->velocity 	= velocity;
	this->direction	= direction;
}

void Ball::Draw()
{	
	if (!onTable)
		return;
	
	glUseProgram(program);

	float t = glfwGetTime();
	
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(location_x, radius, location_z)) * rotateMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "u_Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sky_texture);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (GLvoid*)NULL);
	glBindVertexArray(0);

	// The following unblinding may be optional.
	
	// Set active Texture Unit 0
	glActiveTexture(GL_TEXTURE0);

	// Unbind Texture Map
	glBindTexture(GL_TEXTURE_2D, 0);
}

