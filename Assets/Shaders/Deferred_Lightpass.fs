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
    vec3 LightAcc = texture(gLightAcc, TexCoords).rgb;
	
	//lightingvar
	vec3 viewDir  = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(dLightPos - FragPos);
	
	// diffuse
    vec3 diffuse = max(dot(Normal, lightDir), 0.0f) * Diffuse * dLightCol;
		
	LightAcc = LightAcc + 0.4f;
	clamp(Diffuse, 0.0f, 1.0f);
	
	Diffuse = (diffuse * LightAcc);
	clamp(Diffuse, 0.0f, 1.0f);
	
    FragColor = vec4(Diffuse, 1.0f);
}