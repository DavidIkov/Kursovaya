#include"ShaderProgram.h"


std::string GetShaderProgramTypeEnumName(ShaderProgramTypeEnum type) {
	switch (type) {
	case ShaderProgramTypeEnum::vector2: return "vec2";
	case ShaderProgramTypeEnum::vector3: return "vec3";
	case ShaderProgramTypeEnum::vector4: return "vec4";
	case ShaderProgramTypeEnum::floatType: return "float";
	case ShaderProgramTypeEnum::martix4by4: return "mat4";
	case ShaderProgramTypeEnum::image2D: return "sampler2D";

	}
	__debugbreak();//not found type
	return 0;
}


const Uniform& ShaderProgram::gUniform() const {
	return vUniform;
}

bool ShaderProgram::gCompiled() const {
	return Compiled;
}

void ShaderProgram::Bind() {
	glSC(glUseProgram(ID));
}
void ShaderProgram::Unbind() {
	glSC(glUseProgram(0));
}

void ShaderProgram::DeleteShader() {
	glSC(glDeleteProgram(ID));
}

void ShaderProgram::CompileShader() {

	glSC(ID = glCreateProgram());

	std::string vertexShader;
	vertexShader += "#version ";
	vertexShader += std::to_string(ShaderVersion);
	vertexShader += " core\n";
	{//apply buffer data
		for (unsigned int i = 0; i < VertexBufferData.size(); i++) {
			vertexShader += "layout(location=";
			vertexShader += std::to_string(i);
			vertexShader += ") in ";
			vertexShader += GetShaderProgramTypeEnumName(VertexBufferData[i].Type);
			vertexShader += " i_";
			vertexShader += VertexBufferData[i].Name;
			vertexShader += ";\n";
		}
	}
	{//apply transfer data
		for (unsigned int i = 0; i < TransferData.size(); i++) {
			vertexShader += "out ";
			vertexShader += GetShaderProgramTypeEnumName(TransferData[i].Type);
			vertexShader += " t_";
			vertexShader += TransferData[i].Name;
			vertexShader += ";\n";
		}
	}
	{//apply uniforms
		for (unsigned int i = 0; i < VertexShaderUniformsData.size(); i++) {
			vertexShader += "uniform ";
			vertexShader += GetShaderProgramTypeEnumName(VertexShaderUniformsData[i].Type);
			vertexShader += " u_";
			vertexShader += VertexShaderUniformsData[i].Name;
			vertexShader += ";\n";
		}
	}

	vertexShader += "void main()\n{\n";

	for (unsigned int i = 0; i < VertexShaderCodeParts.size(); i++) {
		vertexShader += "";
		vertexShader += VertexShaderCodeParts[i];
		vertexShader += "\n";
	}

	vertexShader += "};";



	glSC(unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER));
	{
		const char* ptr = vertexShader.c_str();
		glSC(glShaderSource(vertexShaderID, 1, &ptr, NULL));
		glSC(glCompileShader(vertexShaderID));

#if defined _DEBUG
		{//shader compiler errors handler
			int result;
			glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
			if (result != GL_TRUE)
			{
				int length = 0;
				glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &length);
				char* message = new char[length];
				glGetShaderInfoLog(vertexShaderID, length, &length, message);
				std::cout << "SHADER compilation error:" << std::string(message);
				delete[] message;
			}
		}
#endif
	}

	glSC(glAttachShader(ID, vertexShaderID));



	std::string fragmentShader;
	fragmentShader += "#version ";
	fragmentShader += std::to_string(ShaderVersion);
	fragmentShader += " core\nout vec4 o_PixelColor;\n";
	{//apply transfer data
		for (unsigned int i = 0; i < TransferData.size(); i++) {
			fragmentShader += "in ";
			fragmentShader += GetShaderProgramTypeEnumName(TransferData[i].Type);
			fragmentShader += " t_";
			fragmentShader += TransferData[i].Name;
			fragmentShader += ";\n";
		}
	}
	{//apply uniforms
		for (unsigned int i = 0; i < FragmentShaderUniformsData.size(); i++) {
			fragmentShader += "uniform ";
			fragmentShader += GetShaderProgramTypeEnumName(FragmentShaderUniformsData[i].Type);
			fragmentShader += " u_";
			fragmentShader += FragmentShaderUniformsData[i].Name;
			fragmentShader += ";\n";
		}
	}

	fragmentShader += "void main()\n{\n";

	for (unsigned int i = 0; i < FragmentShaderCodeParts.size(); i++) {
		fragmentShader += "";
		fragmentShader += FragmentShaderCodeParts[i];
		fragmentShader += "\n";
	}

	fragmentShader += "};";

	glSC(unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER));
	{
		const char* ptr = fragmentShader.c_str();
		glSC(glShaderSource(fragmentShaderID, 1, &ptr, NULL));
		glSC(glCompileShader(fragmentShaderID));

#if defined _DEBUG
		{//shader compiler errors handler
			int result;
			glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
			if (result != GL_TRUE)
			{
				int length = 0;
				glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &length);
				char* message = new char[length];
				glGetShaderInfoLog(fragmentShaderID, length, &length, message);
				std::cout << "SHADER compilation error:" << std::string(message);
				delete[] message;
			}
		}
#endif
	}

	std::cout << fragmentShader << '\n';
	std::cout << vertexShader << '\n';

	glSC(glAttachShader(ID, fragmentShaderID));

	glSC(glLinkProgram(ID));

#if defined _DEBUG
	{//linker error handler
		int result;
		glGetProgramiv(ID, GL_LINK_STATUS, &result);
		if (result != GL_TRUE)
		{
			int length = 0;
			glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &length);
			char* message = new char[length];
			glGetProgramInfoLog(ID, length, &length, message);
			std::cout << "SHADER LINKER error:" << std::string(message);
			delete[] message;
		}
	}
#endif

	glSC(glDeleteShader(vertexShaderID));
	glSC(glDeleteShader(fragmentShaderID));


	/*std::cout << vertexShader << '\n';
	std::cout << fragmentShader << '\n';*/
}