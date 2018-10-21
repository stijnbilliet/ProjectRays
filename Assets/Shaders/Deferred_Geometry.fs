#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gAlbedo;
layout (location = 2) out vec3 gNormal;
layout (location = 3) out vec4 gLightAcc;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_ambient1;

void main()
{   
	// store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal.rgb = normalize(Normal);
    // and the diffuse per-fragment color
    gAlbedo.rgb = texture(texture_diffuse1, TexCoords).rgb;
	//gLightAcc
	gLightAcc.r = texture(texture_ambient1, TexCoords).r;
}