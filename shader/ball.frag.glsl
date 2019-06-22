// OpenGL 3.3
#version 400

// Input from Vertex Shader
in vec4 frag_UV;

// Texture
uniform sampler2D u_texture_Map;

in vec4 frag_Position;
in vec4 frag_Normal;
in vec4 frag_Light_Direction;


// Light Source
uniform vec4 Ia = vec4(0.2f, 0.2f, 0.2f, 1.0f);
uniform vec4 Id = vec4(0.7f, 0.7f, 0.7f, 1.0f);
uniform vec4 Is = vec4(1.0f, 1.0f, 1.0f, 1.0f);

// Transform Matrices
uniform mat4 u_IView;

// Texture
uniform samplerCube u_cube_texture_Map;

// Material
// uniform vec4 Kr = vec4(0.780392, 0.568627, 0.113725, 1.0);
uniform vec4 Kr = vec4(0.90164, 0.75648, 0.32648, 1.0);
float roughness = 0.9;

// Output from Fragment Shader
out vec4 pixel_Colour;

void main () {
	//----------------------------------------------
	// Calculate Vectors
	//----------------------------------------------

	// View Vector (eye space)
	vec3 v = normalize(frag_Position.xyz);

	// Normal Vector (eye space)
	vec3 n = normalize(frag_Normal.xyz);

	// Reflection Vector (eye space)
	vec3 r = reflect(v, n);

	// Reflected Vector (world space)
	vec3 str = (u_IView * vec4(r, 0.0f)).xyz * vec3(-1,-1,1);
	
        vec4 Ka = texture(u_texture_Map, frag_UV.xy);
	pixel_Colour = 0.8f*Ka + 0.5f*texture(u_cube_texture_Map, str);
}