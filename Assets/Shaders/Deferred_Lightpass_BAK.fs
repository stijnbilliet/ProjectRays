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
	vec4 Albedo = texture(gAlbedo, TexCoords);
	
	if(Albedo.a < 0.1) discard;
	
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec4 LightAcc = texture(gLightAcc, TexCoords);
	LightAcc = LightAcc + 0.4f;
	
	//lightingvar
	vec3 viewDir  = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(dLightPos - FragPos);
	
	// diffuse
    vec3 diffuse = max(dot(Normal, lightDir), 0.0f) * Albedo.rgb * dLightCol;
	
	// specular
	vec3 reflectDir = reflect(-lightDir, Normal);
    float specAngle = max(dot(reflectDir, viewDir), 0.0);
    // note that the exponent is different here
    vec3 specular = pow(specAngle, 50.0f) * dLightCol;
	specular *= LightAcc.a;
	
	diffuse += specular;
	clamp(diffuse, 0.0f, 1.0f);
	
	diffuse *= LightAcc.b;
	clamp(diffuse, 0.0f, 1.0f);
	
    FragColor = vec4(diffuse, 1.0f);
}