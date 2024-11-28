#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 VertPos;


out vec4 outColor;

uniform vec3 viewPos;
uniform vec3 lightPos;  

uniform sampler2D Texture;

uniform float ambientCoefficient;
uniform float diffuseCoefficient;
uniform float specularCoefficient;
uniform float shinyAmount;

// Material Color

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;


void main()
{
	//texture
	outColor = texture(Texture, TexCoord);
		
	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - VertPos); 
	float diffuse = max(dot(norm, lightDir), 0.0); 

	//specular
	float specular = 0.0;
	if(diffuse > 0.0){  //if light on the correct side
		vec3 viewDir = normalize(viewPos - VertPos); //vector to viewer
		vec3 reflectDir = reflect(-lightDir, norm); //reflected light
		float specAngle = max(dot(viewDir, reflectDir), 0.0);
		specular = pow(specAngle, shinyAmount);
	}
	
	outColor = vec4(ambientCoefficient * ambientColor + diffuseCoefficient * diffuse * diffuseColor + specularCoefficient * specular * specularColor, 1.0) * outColor;
};