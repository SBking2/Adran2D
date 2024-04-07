//Texture Shader
#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec4 a_BoneIndex;
layout(location = 4) in vec4 a_BoneWeights;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat4 u_boneTransform[200];

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 FragPos;

void main()
{	
	mat4 boneTransform = mat4(0.0);
	boneTransform += u_boneTransform[int(a_BoneIndex.x)] * a_BoneWeights.x;
	boneTransform += u_boneTransform[int(a_BoneIndex.y)] * a_BoneWeights.y;
	boneTransform += u_boneTransform[int(a_BoneIndex.z)] * a_BoneWeights.z;
	boneTransform += u_boneTransform[int(a_BoneIndex.w)] * a_BoneWeights.w;
	
	vec4 pos;
	if(boneTransform == mat4(0.0))
	{
		pos = vec4(a_Position, 1.0);
	}
	else
	{
		pos = boneTransform * vec4(a_Position, 1.0);
	}

	v_TexCoord = a_TexCoord;
	v_Normal = a_Normal;
	FragPos = vec3(u_Transform * vec4(a_Position,1.0));
	gl_Position = u_ViewProjection * u_Transform * pos;
}

#type fragment
#version 450

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 FragPos;

uniform int u_EntityID;
uniform vec3 u_LightDire;
uniform float u_LightStrength;

uniform sampler2D u_diffuse;
uniform sampler2D u_specular;
uniform sampler2D u_emission;
uniform float u_shininess;
uniform vec4 u_diffuseColor;
uniform vec3 u_viewPosition;

void main()
{
	vec4 diffuseColor = texture(u_diffuse, v_TexCoord) * u_diffuseColor;
	vec3 normal = normalize(v_Normal);


	color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	//光线的方向
	vec3 lightDir = normalize(-u_LightDire);
	
	// ambient
	vec3 ambient = 0.1f * vec3(1.0f) * diffuseColor.rgb;
	
	// diffuse
	float diffuseIntensity = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = 1.0f * vec3(1.0f) * diffuseIntensity * diffuseColor.rgb;

	vec3 viewDir = normalize(u_viewPosition - FragPos);


	vec4 reflectColor = texture(u_specular, v_TexCoord);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specularIntensity = pow(max(dot(viewDir, reflectDir), 0.0), u_shininess);
	vec3 specular = 1.0f * vec3(1.0f) * 1.0f * reflectColor.rgb;


	color.rgb += ambient + diffuse + specular;
	color2 = u_EntityID;
}