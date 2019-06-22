/*
Assignment 4, 159.709, 2019 S1
Wang, Zhichun(Zac), 19024898

The main function is here. What it does include:
	create shader programs
	render balls, table, cue, sky
	render reflection map on balls 

														How to play	
1. Press Z X C V to change the orientation of the cue
2. Press Space to make the cue hit the cue ball 
3. Press UP/DOWN to zoom in/out
4. Press W S to pitch
5. Press A D to yaw

When all the object balls have entered the holes or the cue ball enters a hole, the game finishes and restart again .
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
	#include <OpenGL/gl3ext.h>
#elif defined(__linux__)
	// GLEW Headers
	#include <GL/glew.h>
	#include <GL/gl.h>

	// GLFW Headers
	#define GLFW_INCLUDE_GLCOREARB
	#include <GLFW/glfw3.h>
#endif


// GLM Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project Headers
#include "shader.h"
#include "utils.h"
#include "geometry.h"
#include "image.h"
#include "ball.h"
#include "table.h"
#include "skybox.h"
#include <iostream>
// --------------------------------------------------------------------------------
// GLFW Callbacks
// --------------------------------------------------------------------------------

// Called on Error Event
void onError(int error, const char *description) {
	// Print Error message
	std::cerr << "Error: " << error << " : " << description << std::endl;
}

// Called on Window Close Event
void onWindowClose(GLFWwindow *window) {
	// Nothing to do right now
	// Do not call glfwDestroyWindow from here
}

// Called on Window Size Event
void onFramebufferSize(GLFWwindow *window, int width, int height) {
	// Set-up the window/screen coordinates
	glfwMakeContextCurrent(window);
	glViewport(0, 0, width, height);
	glfwMakeContextCurrent(NULL);
}

float DistanceOf2Balls(Ball &ball1, Ball &ball2)
{
	return sqrt(pow(ball1.location_x-ball2.location_x, 2) + pow(ball1.location_z-ball2.location_z, 2));
}

float GetTheta(float x, float z)
{
	float theta = acos(x/sqrt(pow(x, 2) + pow(z, 2)));

	if (-z < 0)
		theta = -theta;

	return theta;
}

/* return [0, M_PI]
*/
float AnglesDifference(float theta1, float theta2)
{
	float difference = theta1 - theta2;

	if (difference < -M_PI)
		difference += 2 * M_PI;
	else if (difference > M_PI)
		difference -= 2 * M_PI;

	return fabs(difference);
}

void HandleCollision(Ball &ball1, Ball &ball2)
{
	Ball *faster, *slower;

	if (ball1.velocity > ball2.velocity)
	{
		faster = &ball1;
		slower = &ball2;
	}
	else 
	{
		faster = &ball2;
		slower = &ball1;
	}

	float theta = GetTheta(slower->location_x- faster->location_x, slower->location_z- faster->location_z);

	if (AnglesDifference(theta, faster->direction) >= M_PI/2) // not really collide
	{
		return;
	}
		
	slower->velocity	= faster->velocity * cos(theta - faster->direction);
	slower->direction 	= theta;

	faster->velocity	= faster->velocity * fabs(sin(theta - faster->direction));
	faster->direction	= sin(theta - faster->direction) > 0 ? theta - M_PI/2 : theta + M_PI/2;	
	
	// Keep it between [-M_PI, M_PI]
	if (faster->direction > M_PI)
		faster->direction -= 2 * M_PI;
	else if (faster->direction < -M_PI)
		faster->direction += 2 * M_PI;
}
// --------------------------------------------------------------------------------
// Example 07 - Texture Mapping
// --------------------------------------------------------------------------------
int main() {
	// Set Error Callback
	glfwSetErrorCallback(onError);

	// Initialise GLFW
	if (!glfwInit()) {
		// Return Error
		return 1;
	}

	// Create Window
	GLFWwindow *window = createWindow(800, 800, "Z X C V Redirects Cue. SPACE hits Cue Ball. W S A D UP DOWN change View", 3, 2);

	// Check Window 
	if (window == NULL) {
		// Print Error Message
		std::cerr << "Error: create window or context failed." << std::endl;

		// Return Error
		return 1;
	}

	#if defined(_WIN32) || defined(__linux__)
		// Initialise GLEW
		if (glewInit() != GLEW_OK) {
			// Return Error
			return 1;
		}
	#endif

	// Set window callback functions
	glfwSetFramebufferSizeCallback(window, onFramebufferSize);
	glfwSetWindowCloseCallback(window, onWindowClose);

	// ----------------------------------------
	// Initialise OpenGL
	// ----------------------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// ----------------------------------------
	// Create GLSL Program 
	// ----------------------------------------

	// Load GLSL Program
	GLuint ball_program  = loadProgram("./shader/ball.vert.glsl",  NULL, NULL, NULL, "./shader/ball.frag.glsl");
	GLuint table_program     = loadProgram("./shader/table.vert.glsl",  NULL, NULL, NULL, "./shader/table.frag.glsl");	
	GLuint skybox_program  = loadProgram("./shader/skybox.vert.glsl",  NULL, NULL, NULL, "./shader/skybox.frag.glsl");
	const char *skyboxfile[6] = {"images/skybox/posx.jpg",
	 							"images/skybox/negx.jpg",
	 							"images/skybox/negy.jpg",
	 							"images/skybox/posy.jpg",
	 							"images/skybox/posz.jpg",
	 							"images/skybox/negz.jpg"};
	Skybox skybox(skyboxfile, skybox_program);
	Table table(table_program);
	Cue cue(table_program);
	
	#define OBJECT_BALL_INIT_Z  (-TABLE_INNER_LENGTH/4)
	#define CUE_BALL_INIT_Z  	( TABLE_INNER_LENGTH/4)

	#define ACCELERATION		0.0005f
	
	Ball balls[] = 
	{	
		Ball(RADIUS, "./images/BallCue.jpg", 	skybox.texture, ball_program),
		Ball(RADIUS, "./images/Ball1.jpg", 		skybox.texture, ball_program),
		Ball(RADIUS, "./images/Ball2.jpg", 		skybox.texture, ball_program),
		Ball(RADIUS, "./images/Ball3.jpg", 		skybox.texture, ball_program),
		Ball(RADIUS, "./images/Ball4.jpg", 		skybox.texture, ball_program),
		Ball(RADIUS, "./images/Ball5.jpg", 		skybox.texture, ball_program),
		Ball(RADIUS, "./images/Ball6.jpg", 		skybox.texture, ball_program),
		Ball(RADIUS, "./images/Ball7.jpg", 		skybox.texture, ball_program),
	};

	Ball *cueBall = &balls[0];

#define INIT_BALLS \
	do { \
		for (int i = 0; i < sizeof(balls) / sizeof(balls[0]); i++) \
		{ \
			balls[i].location_x = balls[i].radius * 2.01 * (i-1-(int)(sizeof(balls) / sizeof(balls[0])-1)/2); \
			balls[i].location_z = -TABLE_INNER_WIDTH/4; \
			balls[i].velocity	= 0; \
			balls[i].onTable	= true;\
			balls[i].rotateMatrix = glm::rotate(glm::mat4(1.0f), float(-M_PI/2), glm::vec3(0.0f, 0.0f, 1.0f)); \
		} \
		cueBall->location_x = 0;\
		cueBall->location_z = TABLE_INNER_WIDTH/4; \
	} while(0)
	
	INIT_BALLS;

	GLuint textureMapLoc;
	
	glUseProgram(table_program);
	textureMapLoc = glGetUniformLocation(table_program, "u_texture_Map");
	glUniform1i(textureMapLoc, 0);
	

	glUseProgram(skybox_program);
	textureMapLoc = glGetUniformLocation(skybox_program, "u_texture_Map");
	glUniform1i(textureMapLoc, 0);

	glUseProgram(ball_program);
	textureMapLoc = glGetUniformLocation(ball_program, "u_texture_Map");
	glUniform1i(textureMapLoc, 0);
	textureMapLoc = glGetUniformLocation(ball_program, "u_cube_texture_Map");
	glUniform1i(textureMapLoc, 1);

	// ----------------------------------------
	// Projection Matrix
	// ----------------------------------------
	glm::mat4 projectionMatrix(1.0f);
	
	// Calculate Perspective Projection
	projectionMatrix = glm::perspective(glm::radians(67.0f), 1.0f, 0.2f, 50.0f);

	// Copy Projection Matrix to Shader
	glUseProgram(table_program);	
	glUniformMatrix4fv(glGetUniformLocation(table_program, "u_Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUseProgram(ball_program);
	glUniformMatrix4fv(glGetUniformLocation(ball_program, "u_Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUseProgram(skybox_program);
	glUniformMatrix4fv(glGetUniformLocation(skybox_program, "u_Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	float hitStrength 	= 2.5;
	float hitDirection	= M_PI/2;

	glm::vec3 viewPosition(0.0f,  0.0f,  5.0f); // Initial location of the camera
	float yaw = 0, pitch = -M_PI/3;
	float key_pressed_last_time = 0.0f;

	printf("%s",
"------------------------how to play-----------------\n"
"1. Press Z X C V to change the orientation of the cue\n"
"2. Press Space to make the cue hit the cue ball\n"
"3. Press UP/DOWN to zoom in/out\n"
"4. Press W S to pitch\n"
"5. Press A D to yaw\n\n"
"When all the object balls have entered the holes or the cue ball enters a hole, the game finishes and restarts automatically.\n");

	while (!glfwWindowShouldClose(window)) {
		// Make the context of the given window current on the calling thread
		glfwMakeContextCurrent(window);

		// Set clear (background) colour to dark 
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		// Clear Screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static float time_last_read;
		float t = glfwGetTime();
		float time_difference = t - time_last_read;
		time_last_read = t;

		bool allBallsStatic = true;

		for (int i = 0; i < sizeof(balls) / sizeof(balls[0]); i++)
			if (balls[i].onTable && balls[i].velocity > 0)
			{
				allBallsStatic = false;
				break;
			}

		// Even when a key is pressed only once, glfwGetKey may think the key has been pressed many times within a short period of time
		// So we must filter this error.
		#define KEY_PRESSED_MIN_INTERVAL  0.01f 
		// ----------------------------------------
		// Update View Matrix
		// ----------------------------------------
		if(glfwGetKey(window, GLFW_KEY_UP)) {
			// Move Closer
			if (t-key_pressed_last_time > KEY_PRESSED_MIN_INTERVAL)
				viewPosition = viewPosition*0.8f;
			key_pressed_last_time = t;
		}

		if(glfwGetKey(window, GLFW_KEY_DOWN)) {
			// Move Away
			if (t-key_pressed_last_time > KEY_PRESSED_MIN_INTERVAL)
				viewPosition = viewPosition/0.8f;
			key_pressed_last_time = t;
		}

		if(glfwGetKey(window, GLFW_KEY_A)) {
			// Yaw Left
			if (t-key_pressed_last_time > KEY_PRESSED_MIN_INTERVAL)
				yaw += M_PI/10;
			key_pressed_last_time = t;
		}

		if(glfwGetKey(window, GLFW_KEY_D)) {
			// Yaw Right
			if (t-key_pressed_last_time > KEY_PRESSED_MIN_INTERVAL)
				yaw -= M_PI/10;
			key_pressed_last_time = t;
		}

		if(glfwGetKey(window, GLFW_KEY_S)) {
			// Pitch Up
			if (t-key_pressed_last_time > KEY_PRESSED_MIN_INTERVAL)
				pitch += M_PI/6;
			key_pressed_last_time = t;
		}

		if(glfwGetKey(window, GLFW_KEY_W)) {
			// Pitch Down
			if (t-key_pressed_last_time > KEY_PRESSED_MIN_INTERVAL)
				pitch -= M_PI/6;
			key_pressed_last_time = t;
		}

		// hit the cue ball
		if(glfwGetKey(window, GLFW_KEY_SPACE)) {			
			if (t-key_pressed_last_time > KEY_PRESSED_MIN_INTERVAL)
			{
				if (allBallsStatic)
					cueBall->SetVelocity(hitStrength, hitDirection);
			}
			key_pressed_last_time = t;
		}

		 
		if(glfwGetKey(window, GLFW_KEY_Z)) {
			 // Pitch Down
			 if (t-key_pressed_last_time > KEY_PRESSED_MIN_INTERVAL)
			 {
				 hitDirection -= M_PI/6;
				 if (hitDirection < -M_PI)
				 	hitDirection += 2 * M_PI;
			 }
			 key_pressed_last_time = t;
		 }
		 		 
		if(glfwGetKey(window, GLFW_KEY_X)) {
			 // Pitch Down
			 if (t-key_pressed_last_time > KEY_PRESSED_MIN_INTERVAL)
			 {
				 hitDirection -= M_PI/30;
				 if (hitDirection < -M_PI)
				 	hitDirection += 2 * M_PI;
			 }
			 key_pressed_last_time = t;
		 }
		 		 
		if(glfwGetKey(window, GLFW_KEY_C)) {
			 // Pitch Down
			 if (t-key_pressed_last_time > KEY_PRESSED_MIN_INTERVAL)
			 {
				 hitDirection += M_PI/30;
				 if (hitDirection > M_PI)
				 	hitDirection -= 2 * M_PI;
			 }
			 key_pressed_last_time = t;
		 }
		 		 
		if(glfwGetKey(window, GLFW_KEY_V)) {
			 // Pitch Down
			 if (t-key_pressed_last_time > KEY_PRESSED_MIN_INTERVAL)
			 {
				 hitDirection += M_PI/6;
				 if (hitDirection > M_PI)
				 	hitDirection -= 2 * M_PI;
			 }
			 key_pressed_last_time = t;
		 }
		 

		// Calculate Pitch and Yaw
		glm::mat4 yawMatrix   = glm::rotate(glm::mat4(1.0f), yaw,   glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 pitchMatrix = glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1.0f, 0.0f, 0.0f));

		// Calculate View Vectors
		glm::vec3 viewUp      = glm::normalize(yawMatrix * pitchMatrix * glm::vec4(0.0f, 1.0f,  0.0f, 0.0f));
		glm::vec3 viewForward = glm::normalize(yawMatrix * pitchMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));

		// Copy View Matrix to ball Shader	
		viewPosition = viewForward * (-glm::length(viewPosition)); // The camera always looks straightly at the origin (in world space), that is, the center of the table.
		glm::mat4 viewMatrix = glm::lookAt(viewPosition, viewPosition + viewForward, viewUp);
		glUseProgram(table_program);
		glUniformMatrix4fv(glGetUniformLocation(table_program, "u_View"), 1, GL_FALSE, glm::value_ptr(viewMatrix));


		glUseProgram(ball_program);
		glUniformMatrix4fv(glGetUniformLocation(ball_program, "u_View"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(ball_program, "u_IView"), 1, GL_FALSE, glm::value_ptr(glm::inverse(viewMatrix)));

		// fall into a hole?
		static int onTableCnt = sizeof(balls) / sizeof(balls[0]);
		
		for (int i = 0; i < sizeof(balls) / sizeof(balls[0]); i++)
		{
			if (!balls[i].onTable)
				continue;
			
			if ((balls[i].location_x-balls[i].radius*HOLE_RADIUS_RATIO < -TABLE_INNER_WIDTH/2  || TABLE_INNER_WIDTH/2  < balls[i].location_x+balls[i].radius*HOLE_RADIUS_RATIO) &&
				(balls[i].location_z-balls[i].radius*HOLE_RADIUS_RATIO < -TABLE_INNER_LENGTH/2 || TABLE_INNER_LENGTH/2 < balls[i].location_z+balls[i].radius*HOLE_RADIUS_RATIO))
			{
				printf("#%d falls into a hole\n", i);
				balls[i].onTable = false;
				if (--onTableCnt <= 1 || &balls[i] == cueBall) // restart the game
				{
					INIT_BALLS;
					onTableCnt = sizeof(balls) / sizeof(balls[0]);
					hitDirection = M_PI/2;

					break;
				}
			}
		}
		
		// collision between two balls?
		for (int i = 0; i < sizeof(balls) / sizeof(balls[0]) - 1; i++)
		{
			if (!balls[i].onTable)
				continue;
			
			for (int j = i + 1; j < sizeof(balls) / sizeof(balls[0]); j++)
			{
				if (!balls[j].onTable)
					continue;

				if (DistanceOf2Balls(balls[i], balls[j]) < balls[i].radius + balls[j].radius)
					HandleCollision(balls[i], balls[j]);				
			}
		}

		// collide with the wall of the table?
		for (int i = 0; i < sizeof(balls) / sizeof(balls[0]); i++)
		{
			if (!balls[i].onTable)
				continue;
			
			if (balls[i].location_x-balls[i].radius < -TABLE_INNER_WIDTH/2 && fabs(balls[i].direction) > M_PI/2 ||
				TABLE_INNER_WIDTH/2  < balls[i].location_x+balls[i].radius && fabs(balls[i].direction) < M_PI/2)
			{
				balls[i].direction = M_PI - balls[i].direction;
				if (balls[i].direction > M_PI)
					balls[i].direction -= 2 * M_PI;
			}
			
			if (balls[i].location_z-balls[i].radius < -TABLE_INNER_LENGTH/2 && (balls[i].direction) > 0 || 
				TABLE_INNER_LENGTH/2 < balls[i].location_z+balls[i].radius  && (balls[i].direction) < 0) 			
				balls[i].direction = -balls[i].direction;
				
		}
		
		// Update location, rotateMatrix,  and velocity (Note that even if no collision happens, the velocity of a moving ball needs to be updated since it will decrease)
		for (int i = 0; i < sizeof(balls) / sizeof(balls[0]); i++)
		{	
			if (!balls[i].onTable)
				continue;
						
			if (balls[i].velocity > 0)
			{
				balls[i].location_x += balls[i].velocity * time_difference *  cos(balls[i].direction);
				balls[i].location_z += balls[i].velocity * time_difference * -sin(balls[i].direction);	
				balls[i].rotateMatrix = glm::rotate(glm::mat4(1.0f), 
													balls[i].velocity * time_difference / balls[i].radius, 
													glm::vec3(-sin(balls[i].direction), 0.0f, -cos(balls[i].direction))) 
										* balls[i].rotateMatrix;

				balls[i].velocity -= ACCELERATION;
				if (balls[i].velocity < 0)
					balls[i].velocity = 0;
			}				
		}


		// Draw
		for (int i = 0; i < sizeof(balls) / sizeof(balls[0]); i++)
			balls[i].Draw();		

		// ----------------------------------------------------table & cue-----------------------------------------------------
		
		glUseProgram(table_program);
		glUniformMatrix4fv(glGetUniformLocation(table_program, "u_View"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
		table.Draw();
		if (allBallsStatic && cueBall->onTable)
			cue.Draw(cueBall->location_x, cueBall->location_z, hitDirection);
		
		
		// ------------------------------------------------------sky------------------------------------------------------
		// Fix View Position to (0,0,0) for sky
		glm::vec3 viewPositionSky = glm::vec3(0.0f, 0.0f, 0.0f);

		// Construct View Matrix
		viewMatrix = glm::lookAt(viewPositionSky, viewPositionSky + viewForward, viewUp);

		// Copy View Matrix to Shader		
		glUseProgram(skybox_program);
		glUniformMatrix4fv(glGetUniformLocation(skybox_program, "u_View"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

		skybox.Draw();
		
		// Swap the back and front buffers
		glfwSwapBuffers(window);

		// Poll window events
		glfwPollEvents();
	}

	// Delete Program
	glDeleteProgram(ball_program);	
	glDeleteProgram(table_program);
	glDeleteProgram(skybox_program);	
	
	// Stop receiving events for the window and free resources; this must be
	// called from the main thread and should not be invoked from a callback
	glfwDestroyWindow(window);

	// Terminate GLFW
	glfwTerminate();

	return 0;
}


