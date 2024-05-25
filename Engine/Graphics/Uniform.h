#pragma once
#include"DLLStuff.h"
#include"Debug.h"
#include<vector>
/*why so many SetUniformWhatever functions you may ask? 
becouse of fucking opengl(as far as i know opengl was written in C and it dosent have different parmeters functions?)*/
class Uniform {
public:
	unsigned int ShaderProgID;
	
	//sets shader program id, so it can be used to change shader program uniforms
	DLL_TREATMENT void SetShaderProgID(unsigned int shaderProgID);

	
	//fucking a lot of uniforms!!!!

	DLL_TREATMENT void SetUniform1f(const char* name, float f1) const;
	DLL_TREATMENT void SetUniform2f(const char* name, float f1, float f2) const;
	DLL_TREATMENT void SetUniform3f(const char* name, float f1, float f2, float f3) const;
	DLL_TREATMENT void SetUniform4f(const char* name, float f1, float f2, float f3, float f4) const;

	DLL_TREATMENT void SetUniform1i(const char* name, int i1) const;

	DLL_TREATMENT void SetUniform1fv(const char* name, std::vector<float> vs) const;
	DLL_TREATMENT void SetUniform2fv(const char* name, std::vector<float> vs) const;
	DLL_TREATMENT void SetUniform3fv(const char* name, std::vector<float> vs) const;
	DLL_TREATMENT void SetUniform4fv(const char* name, std::vector<float> vs) const;

	DLL_TREATMENT void SetUniform1iv(const char* name, std::vector<int> vs) const;
	DLL_TREATMENT void SetUniform2iv(const char* name, std::vector<int> vs) const;
	DLL_TREATMENT void SetUniform3iv(const char* name, std::vector<int> vs) const;
	DLL_TREATMENT void SetUniform4iv(const char* name, std::vector<int> vs) const;
};