#pragma once
#include"Debug.h"
#include<vector>
#include<string>
#include"Uniform.h"


enum ShaderProgramTypeEnum {
	vector2, vector3, vector4, floatType, martix4by4, image2D
};
std::string GetShaderProgramTypeEnumName(ShaderProgramTypeEnum type);

//need to make so same if same shader program exists it wont create new same progam!!!!
class ShaderProgram {
public:
	static const unsigned int ShaderVersion = 330;

	struct DataElement {
		ShaderProgramTypeEnum Type;
		std::string Name;
	};
private:
	Uniform vUniform;
	unsigned int ID;
	bool Compiled = false;
public:

	

	std::vector<DataElement> VertexBufferData;
	std::vector<DataElement> TransferData;
	std::vector<DataElement> VertexShaderUniformsData;
	std::vector<DataElement> FragmentShaderUniformsData;

	std::vector<std::string> VertexShaderCodeParts;
	std::vector<std::string> FragmentShaderCodeParts;

	const Uniform& gUniform() const;

	bool gCompiled() const;

	void Bind();
	static void Unbind();

	//after shader is compiled, you CANT change how shader will threat data it gets fed or any of its code becouse its already compiled and cant be changed
	void CompileShader();
};