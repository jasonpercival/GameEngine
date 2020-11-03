#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 view;
uniform mat4 model;
uniform mat4 proj;

void main(){

	// add attenuation depending on particles distance from camera

	// get distance from camera:
	// vec4 = view * model * vec4(position, 1.0f);
	// then get length of xyz swizzle of the vec4
	// attenuation = inverse square root of 0.1 * the distance from the camera
	// set gl_Position 

	//vec4 pos = view * model * vec4(position, 1.0f);
	//float dist = pos.xyz.length;
	//float attenuation = inversesqrt(1.0f * dist);

	gl_Position = proj * view * model * vec4(position, 1.0f);
	TexCoords = texCoords;
}