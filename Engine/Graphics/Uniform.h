#pragma once
#include"DLLStuff.h"
#include"Debug.h"
/*why so many SetUniformWhatever functions you may ask? 
becouse of fucking opengl(as far as i know opengl was written in C and it dosent have different parmeters functions?)*/
class Uniform {
	unsigned int ShaderProgID;
public:
	
	//sets shader program id, so it can be used to change shader program uniforms
	DLL_TREATMENT void SetShaderProgID(unsigned int shaderProgID);

	
	//fucking a lot of uniforms!!!!

	DLL_TREATMENT void SetUniform1f(const char* name, float f1);
	DLL_TREATMENT void SetUniform2f(const char* name, float f1, float f2);
	DLL_TREATMENT void SetUniform3f(const char* name, float f1, float f2, float f3);
	DLL_TREATMENT void SetUniform4f(const char* name, float f1, float f2, float f3, float f4);
};