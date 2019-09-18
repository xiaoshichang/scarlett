#version 410
layout(location = 0) in vec3 inputPosition;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec4 inputColor;

out vec4 Color;

void main () {
	gl_PointSize = 5.0f;
	gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(inputPosition, 1.0f);
	Color = inputColor;
}