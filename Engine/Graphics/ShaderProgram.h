#pragma once
#include"DLLStuff.h"
#include"Debug.h"
#include<vector>
#include<string>
#include"Uniform.h"


enum ShaderProgramDataTypes {
	vector2, vector3, vector4, floatType, martix4by4, image2D, integerType
};
std::string GetShaderProgramDataTypeName(ShaderProgramDataTypes type);

/*need to make so same if same shader program exists it wont create new same progam!!!!ALSO OPTIMISE COPYING, 
by making all data as pointers so i can just put pointer to original shader*/
class ShaderProgram {
public:
	static const unsigned int ShaderVersion = 330;

	struct DataElement {
		ShaderProgramDataTypes Type;
		std::string Name;
	};
	struct UniformDataElement {
		ShaderProgramDataTypes Type;
		std::string Name;
		unsigned int MaxArrayCapacity = 0;//if >0 then this is array
	};
private:
	Uniform vUniform;
	unsigned int ID;
	bool Compiled = false;
public:


	std::vector<DataElement> VertexBufferData;
	std::vector<DataElement> TransferData;

	std::vector<UniformDataElement> VertexShaderUniformsData;
	std::vector<UniformDataElement> FragmentShaderUniformsData;


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