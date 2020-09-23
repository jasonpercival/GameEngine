#version 450 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 Color;
in vec3 FragPosition;

out vec4 fColour;

struct Light {
	vec3 lightPos;
	float ambient;
	float diffuse;
	vec3 lightColor;
};

struct Material {
	float shininess;		// Ns (specular exponent)
	float transparency;		// d  (dissolve value)
	sampler2D diffuseMap;	// newmtl (texture)
	vec3 ambient;			// Ka (ambient color)
	vec3 diffuse;			// Kd (diffuse color)
	vec3 specular;			// Ks (specular color)
};

uniform vec3 viewPosition;
uniform Light light;
uniform Material material;

void main() {

	//ambient
	vec3 ambient = light.ambient * material.ambient * texture(material.diffuseMap, TexCoords).rgb * light.lightColor;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.lightPos-FragPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * material.diffuse) * texture(material.diffuseMap, TexCoords).rgb * light.lightColor;

	// specular	
	vec3 viewDir = normalize(viewPosition-FragPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (spec * material.specular) * light.lightColor;
	vec3 result = ambient + diffuse + specular;
	fColour = vec4(result, material.transparency);
}