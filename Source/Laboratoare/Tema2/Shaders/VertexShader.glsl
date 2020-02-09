#version 330

// DONE: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_normal;
layout(location = 2) in vec3 v_texture_coord;
layout(location = 1) in vec3 v_color;
// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float myTime;
uniform vec3 thisColor;
uniform int isSea;

// DONE: output values to fragment shader
out vec3 frag_color;
out vec3 frag_normal;
out vec3 frag_texture_coord;
out vec3 frag_position;

void main()
{
	// DONE: send output to fragment shader
	frag_color = v_color ; // = v_color * vec3(sin(myTime), sin(myTime), sin(myTime));
	frag_normal = thisColor; // v_normal * vec3(sin(myTime), sin(myTime), sin(myTime));
	frag_texture_coord = v_texture_coord;
	if (isSea == 1) {
	// todo maybe sin(coord.x), that will make differetn height valuesS
		frag_position = v_position + vec3(0, sin(myTime + 1) / 3, 0);
	} else {
		frag_position = v_position;
	}
	
	// DONE: compute gl_Position
	gl_Position = Projection * View * Model * vec4(frag_position, 1.0);
}
