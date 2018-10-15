#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gAlbedo;
uniform sampler2D gNormal;
uniform sampler2D gLightAcc;
uniform vec3 dLightPos;
uniform vec3 dLightCol;
uniform vec3 viewPos;

void main()
{             
    // retrieve data from gbuffer
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Diffuse = texture(gAlbedo, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Ambient = texture(gLightAcc, TexCoords).rgb;
	
	//lightingvar
	vec3 viewDir  = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(dLightPos - FragPos);
	
	// diffuse
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * dLightCol;
	
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
    vec3 specular = dLightCol * spec;
	
	Diffuse = (diffuse * Ambient.r) + specular;
    FragColor = vec4(Diffuse, 1.0f);
}