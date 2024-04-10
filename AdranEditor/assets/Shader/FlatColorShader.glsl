//Texture Shader
#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexSlot;
layout(location = 4) in float a_TexTiling;
layout(location = 5) in int a_EntityID;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
out vec4 v_Color;
out flat float v_TexSlot;
out float v_TexTiling;
out flat int v_EntityID;

void main()
{

	v_TexTiling = a_TexTiling;
	v_TexSlot = a_TexSlot;
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

in vec2 v_TexCoord;
in vec4 v_Color;
in flat float v_TexSlot;
in float v_TexTiling;
in flat int v_EntityID;

uniform float u_TilingFactor;
uniform sampler2D u_Textures[32];

void main()
{
	color = texture(u_Textures[int(v_TexSlot)], v_TexCoord * v_TexTiling) * v_Color;
	color2 = v_EntityID;
}