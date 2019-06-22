// OpenGL 3.3
#version 330

// Input from Vertex Shader
in vec4 frag_UV;

// Texture
uniform sampler2D u_texture_Map;

in vec4 frag_Position;
in vec4 frag_Normal;
in vec4 frag_Light_Direction;

// Output from Fragment Shader
out vec4 pixel_Colour;

// Light Source
uniform vec4 Ia = vec4(0.7f, 0.7f, 0.7f, 1.0f);
uniform vec4 Id = vec4(0.3f, 0.3f, 0.3f, 1.0f);
uniform vec4 Is = vec4(1.0f, 1.0f, 1.0f, 1.0f);


uniform float a = 5.5f;


void main () {

        vec4 Ka = frag_UV;
        vec4 Kd = vec4(0.2f, 0.2f, 0.2f, 1.0f);
        vec4 Ks = vec4(0.4f, 0.4f, 0.4f, 1.0f);

	//----------------------------------------------
	// Phong Reflection Model
	//----------------------------------------------

	// ---------- Calculate Vectors ----------
	// Direction to Light (normalised)
	vec4 l = normalize(-frag_Light_Direction);

	// Surface Normal (normalised)
	vec4 n = normalize(frag_Normal);

	// Reflected Vector
	vec4 r = reflect(-l, n);

	// View Vector
	vec4 v = normalize(-frag_Position);

	// ---------- Calculate Terms ----------
	// Ambient Term
	vec4 Ta = Ka * Ia;

	// Diffuse Term
	vec4 Td = Kd * max(dot(l, n), 0.0) * Id;

	// Specular Term
	vec4 Ts = Ks * pow((max(dot(r, v), 0.0)), a) * Is;

	pixel_Colour = Ta + Td + Ts;
	//pixel_Colour = frag_UV;
}