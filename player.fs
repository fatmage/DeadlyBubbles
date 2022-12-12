#version 330 core

// Ouput data

out vec4 color;

in vec2 texPos;


uniform sampler2D tex;


void main() {
	vec4 bubbleColor = texture(tex, texPos);

	color = vec4(bubbleColor.xyz, 1.0);


}