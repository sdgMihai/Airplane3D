#version 330

// TODO: get values from fragment shader
in vec3 frag_normal;
in vec3 frag_color;
in vec3 frag_texture_coord;
in vec3 frag_position;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_frag_position;

void main()
{
	// TODO: write pixel out color
	// out_color = vec4(frag_color, 1);
	out_color = vec4(frag_normal, 1);
	out_frag_position = vec4(frag_position, 1);
}