#version 330 core

in vec3 position;
in vec2 texCoord;
in vec3 normal;

uniform vec3 viewPos; 
uniform vec3 lightPos; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D Texture;

uniform float ambientCoefficient;
uniform float diffuseCoefficient;
uniform float specularCoefficient;
uniform int shinyAmount;

// Material Color

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

// outputs

out vec4 color;

void main()
{
	//texture
	color = texture(Texture, texCoord);

	//vertex positions
	vec3 vertPos = vec3(model * vec4(position, 1.0));
	gl_Position = projection * view * vec4(vertPos, 1.0);

	//diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - vertPos);
	float diffuse = max(dot(normal, lightDir), 0.0);

	//specular
	float specular = 0.0;
	if(diffuse > 0.0){  //if light on the correct side
		vec3 viewDir = normalize(viewPos - vertPos); //vector to viewer
		vec3 reflectDir = reflect(-lightDir, norm);  //reflected light
		float specAngle = max(dot(viewDir, reflectDir), 0.0);
		specular = pow(specAngle, shinyAmount);
	}

	color = vec4(ambientCoefficient * ambientColor + diffuseCoefficient * diffuse * diffuseColor + specularCoefficient * specular * specularColor, 1.0) * color;
};