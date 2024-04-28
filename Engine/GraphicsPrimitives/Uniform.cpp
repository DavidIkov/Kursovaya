#include"Uniform.h"

void Uniform::SetShaderProgID(unsigned int shaderProgID) {
	ShaderProgID = shaderProgID;
}


void Uniform::SetUniform1f(const char* name, float f1) {
	glSC(glUniform1f(glGetUniformLocation(ShaderProgID, name), f1));
}
void Uniform::SetUniform2f(const char* name, float f1, float f2) {
	glSC(glUniform2f(glGetUniformLocation(ShaderProgID, name), f1, f2));
}
void Uniform::SetUniform3f(const char* name, float f1, float f2, float f3) {
	glSC(glUniform3f(glGetUniformLocation(ShaderProgID, name), f1, f2, f3));
}
void Uniform::SetUniform4f(const char* name, float f1, float f2, float f3, float f4) {
	glSC(glUniform4f(glGetUniformLocation(ShaderProgID, name), f1, f2, f3, f4));
}