#version 330 core

// Ouput data

out vec4 color;
in vec3 fragPos;
in vec3 fNormal;
uniform vec3 lightPos;
uniform vec3 viewPos;


void main()
{
	float ambientStrength = 0.2;
	float specularStrength = 0.6;
	vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 blue = vec3(0.2, 0.2, 0.8);


	vec3 norm = normalize(fNormal);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	vec3 result = (ambient + diffuse + specular) * blue;

	color = vec4(result, 1.0);


}