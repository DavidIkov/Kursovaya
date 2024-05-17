#pragma once
#include"DLLStuff.h"
#include"Debug.h"
#include<vector>
#include<string>
#include"Uniform.h"


enum ShaderProgramTypeEnum {
	vector2, vector3, vector4, floatType, martix4by4, image2D
};
std::string GetShaderProgramTypeEnumName(ShaderProgramTypeEnum type);

/*need to make so same if same shader program exists it wont create new same progam!!!!ALSO OPTIMISE COPYING, 
by making all data as pointers so i can just put pointer to original shader*/
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

	DLL_TREATMENT const Uniform& gUniform() const;

	DLL_TREATMENT bool gCompiled() const;

	DLL_TREATMENT void Bind();
	DLL_TREATMENT static void Unbind();

	DLL_TREATMENT void DeleteShader();
	//after shader is compiled, you CANT change how shader will threat data it gets fed or any of its code becouse its already compiled and cant be changed
	DLL_TREATMENT void CompileShader();
};