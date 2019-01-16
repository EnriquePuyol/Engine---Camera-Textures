#version 400 core

out vec4 color;

uniform sampler2D occlusion_map;
uniform float k_ambient;

uniform sampler2D diffuse_map;
uniform vec4 diffuse_color;
uniform float k_diffuse;

uniform sampler2D specular_map;
uniform vec4 specular_color;
uniform float k_specular;

uniform float shininess;

in vec2 uv0;
in vec3 position;
in vec3 normal;

uniform mat4 proj; 
uniform mat4 view; 
uniform mat4 model;

uniform vec3 light_pos;
uniform float light_intensity;

void main()
{
	/* AMBIENT */
	vec3 ambientFinal = k_ambient * light_intensity * vec3(texture(occlusion_map, uv0));

	/* DIFFUSE */
	vec3 normal = normalize(normal);
	vec3 lightDir = normalize(light_pos - position);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 finalDiffuse = k_diffuse * diff * vec3(texture(diffuse_map, uv0) * diffuse_color);

	/* SPECULAR */
	vec3 viewPos = transpose(mat3(view))*(-view[3].xyz);
    vec3 viewDir = normalize(viewPos - position);
    vec3 half = normalize(lightDir + viewDir);
    float spec = pow(max(dot(half, normal), 0.0f), shininess);
    vec3 textureSpecularColor = vec3(texture(specular_map, uv0));
    vec3 finalSpecular = k_specular * spec * specular_color.rgb * textureSpecularColor;

	/* EMISSIVE */


	/* FINAL */
    color = vec4(finalDiffuse,1.0f) + vec4(finalSpecular, 1.0) + vec4(ambientFinal, 1.0f);
}
