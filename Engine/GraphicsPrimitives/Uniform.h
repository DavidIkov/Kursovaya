#pragma once
#include"Debug.h"
/*why so many SetUniformWhatever functions you may ask? 
becouse of fucking opengl(as far as i know opengl was written in C and it dosent have different parmeters functions?)*/
class Uniform {
	unsigned int ShaderProgID;
public:
	
	//sets shader program id, so it can be used to change shader program uniforms
	void SetShaderProgID(unsigned int shaderProgID);

	
	//fucking a lot of uniforms!!!!

	void SetUniform1f(const char* name, float f1);
	void SetUniform2f(const char* name, float f1, float f2);
	void SetUniform3f(const char* name, float f1, float f2, float f3);
	void SetUniform4f(const char* name, float f1, float f2, float f3, float f4);
};