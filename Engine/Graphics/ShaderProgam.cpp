#include"ShaderProgram.h"


std::string GetShaderProgramDataTypeName(ShaderProgramDataTypes type) {
	switch (type) {
	case ShaderProgramDataTypes::vector2: return "vec2";
	case ShaderProgramDataTypes::vector3: return "vec3";
	case ShaderProgramDataTypes::vector4: return "vec4";
	case ShaderProgramDataTypes::floatType: return "float";
	case ShaderProgramDataTypes::martix4by4: return "mat4";
	case ShaderProgramDataTypes::image2D: return "sampler2D";
	case ShaderProgramDataTypes::integerType: return "int";

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

	vUniform.ShaderProgID = ID;

	std::string vertexShader;
	vertexShader += "#version ";
	vertexShader += std::to_string(ShaderVersion);
	vertexShader += " core\n";
	{//apply buffer data
		for (unsigned int i = 0; i < VertexBufferData.size(); i++) {
			vertexShader += "layout(location=";
			vertexShader += std::to_string(i);
			vertexShader += ") in ";
			vertexShader += GetShaderProgramDataTypeName(VertexBufferData[i].Type);
			vertexShader += " i_";
			vertexShader += VertexBufferData[i].Name;
			vertexShader += ";\n";
		}
	}
	{//apply transfer data
		for (unsigned int i = 0; i < TransferData.size(); i++) {
			vertexShader += "out ";
			vertexShader += GetShaderProgramDataTypeName(TransferData[i].Type);
			vertexShader += " t_";
			vertexShader += TransferData[i].Name;
			vertexShader += ";\n";
		}
	}
	{//apply uniforms
		for (unsigned int i = 0; i < VertexShaderUniformsData.size(); i++) {
			vertexShader += "uniform ";
			vertexShader += GetShaderProgramDataTypeName(VertexShaderUniformsData[i].Type);
			vertexShader += ((VertexShaderUniformsData[i].MaxArrayCapacity > 0) ? " ua_" : " u_");
			vertexShader += VertexShaderUniformsData[i].Name;
			if (VertexShaderUniformsData[i].MaxArrayCapacity > 0) {
				vertexShader += "[";
				vertexShader += std::to_string(VertexShaderUniformsData[i].MaxArrayCapacity);
				vertexShader += "]";
			}
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
			fragmentShader += GetShaderProgramDataTypeName(TransferData[i].Type);
			fragmentShader += " t_";
			fragmentShader += TransferData[i].Name;
			fragmentShader += ";\n";
		}
	}
	{//apply uniforms
		for (unsigned int i = 0; i < FragmentShaderUniformsData.size(); i++) {
			fragmentShader += "uniform ";
			fragmentShader += GetShaderProgramDataTypeName(FragmentShaderUniformsData[i].Type);
			fragmentShader += ((FragmentShaderUniformsData[i].MaxArrayCapacity > 0) ? " ua_" : " u_");
			fragmentShader += FragmentShaderUniformsData[i].Name;
			if (FragmentShaderUniformsData[i].MaxArrayCapacity > 0) {
				fragmentShader += "[";
				fragmentShader += std::to_string(FragmentShaderUniformsData[i].MaxArrayCapacity);
				fragmentShader += "]";
			}
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
				__debugbreak();
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
			__debugbreak();
		}
	}
#endif

	glSC(glDeleteShader(vertexShaderID));
	glSC(glDeleteShader(fragmentShaderID));
}