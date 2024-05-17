#include"Shader.h"
#include<fstream>

Shader::Shader(unsigned int type, const std::string& filepath) {

	std::string sourceCode;

	{//get source code
		std::ifstream stream(filepath);

		std::string line;
		while (getline(stream, line)) {
			sourceCode += line;
			sourceCode += '\n';
		}
	}
	glSC(ID = glCreateShader(type));
	const char* ptr = sourceCode.c_str();
	glSC(glShaderSource(ID, 1, &ptr, NULL));
	glSC(glCompileShader(ID));
}

unsigned int Shader::GetID() const {
	return ID;
}

void Shader::Delete() {
	glSC(glDeleteShader(ID));
}