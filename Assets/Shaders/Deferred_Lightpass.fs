#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gAlbedo;
uniform sampler2D gNormal;
uniform sampler2D gLightAcc;

void main()
{             
    // retrieve data from gbuffer
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Diffuse = texture(gAlbedo, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Light = texture(gLightAcc, TexCoords).rgb;
	
    FragColor = vec4(Diffuse.r, Diffuse.g, Diffuse.b, 1.0);
}