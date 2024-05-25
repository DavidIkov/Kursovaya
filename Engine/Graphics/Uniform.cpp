#include"Uniform.h"

void Uniform::SetShaderProgID(unsigned int shaderProgID) {
	ShaderProgID = shaderProgID;
}


void Uniform::SetUniform1f(const char* name, float f1) const {
	glSC(glUseProgram(ShaderProgID));
	glSC(glUniform1f(glGetUniformLocation(ShaderProgID, name), f1));
}
void Uniform::SetUniform2f(const char* name, float f1, float f2) const {
	glSC(glUseProgram(ShaderProgID));
	glSC(glUniform2f(glGetUniformLocation(ShaderProgID, name), f1, f2));
}
void Uniform::SetUniform3f(const char* name, float f1, float f2, float f3) const {
	glSC(glUseProgram(ShaderProgID));
	glSC(glUniform3f(glGetUniformLocation(ShaderProgID, name), f1, f2, f3));
}
void Uniform::SetUniform4f(const char* name, float f1, float f2, float f3, float f4) const {
	glSC(glUseProgram(ShaderProgID));
	glSC(glUniform4f(glGetUniformLocation(ShaderProgID, name), f1, f2, f3, f4));
}

void Uniform::SetUniform1i(const char* name, int i1) const {
	glSC(glUseProgram(ShaderProgID));
	glSC(glUniform1i(glGetUniformLocation(ShaderProgID, name), i1));
}

void Uniform::SetUniform1fv(const char* name, std::vector<float> vs) const {
	glSC(glUseProgram(ShaderProgID));
	glSC(glUniform1fv(glGetUniformLocation(ShaderProgID, name), vs.size(), &vs[0]));
}
void Uniform::SetUniform2fv(const char* name, std::vector<float> vs) const {
	glSC(glUseProgram(ShaderProgID));
	glSC(glUniform2fv(glGetUniformLocation(ShaderProgID, name), vs.size(), &vs[0]));
}
void Uniform::SetUniform3fv(const char* name, std::vector<float> vs) const {
	glSC(glUseProgram(ShaderProgID));
	glSC(glUniform3fv(glGetUniformLocation(ShaderProgID, name), vs.size(), &vs[0]));
}
void Uniform::SetUniform4fv(const char* name, std::vector<float> vs) const {
	glSC(glUseProgram(ShaderProgID));
	glSC(glUniform4fv(glGetUniformLocation(ShaderProgID, name), vs.size(), &vs[0]));
}

void Uniform::SetUniform1iv(const char* name, std::vector<int> vs) const {
	glSC(glUseProgram(ShaderProgID));
	glSC(glUniform1iv(glGetUniformLocation(ShaderProgID, name), vs.size(), &vs[0]));
}
void Uniform::SetUniform2iv(const char* name, std::vector<int> vs) const {
	glSC(glUseProgram(ShaderProgID));
	glSC(glUniform2iv(glGetUniformLocation(ShaderProgID, name), vs.size(), &vs[0]));
}
void Uniform::SetUniform3iv(const char* name, std::vector<int> vs) const {
	glSC(glUseProgram(ShaderProgID));
	glSC(glUniform3iv(glGetUniformLocation(ShaderProgID, name), vs.size(), &vs[0]));
}
void Uniform::SetUniform4iv(const char* name, std::vector<int> vs) const {
	glSC(glUseProgram(ShaderProgID));
	glSC(glUniform4iv(glGetUniformLocation(ShaderProgID, name), vs.size(), &vs[0]));
}