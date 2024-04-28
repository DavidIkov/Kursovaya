#shader vertex

#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec2 textureCords;

out vec2 textureCordsFromVertex;

uniform mat4 u_MVP;

void main()
{
   gl_Position=u_MVP*position;
   textureCordsFromVertex=textureCords;
};

#shader fragment

#version 330 core

layout(location=0) out vec4 color;

in vec2 textureCordsFromVertex;

uniform float u_Num;
uniform sampler2D u_Texture;

void main()
{
	vec4 textureColor = texture(u_Texture, vec2(textureCordsFromVertex.x,1-textureCordsFromVertex.y));
	color = textureColor*u_Num;
};