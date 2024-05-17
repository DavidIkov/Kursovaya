#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec4 color;

out vec4 vertexColor;
void main(){ 
	vertexColor=color;
	gl_Position=vec4(aPos.x,aPos.y,0.f,1.f); 
};