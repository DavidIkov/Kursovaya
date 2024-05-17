#pragma once
#include"DLLStuff.h"
#include"UsefullStuff/IDBasedVector.h"
#include"ShaderProgram.h"


typedef IDBasedVectorElementID ShaderProgramID;

namespace ShaderProgramsManager {
	struct ShaderProgramData {
		std::string Name;
		ShaderProgram* ShaderProgramPtr;
	};
	inline IDBasedVector<ShaderProgramData> ShaderPrograms;
	inline std::vector<std::string> Names;
	inline std::vector<ShaderProgramID> IDs;

	DLL_TREATMENT ShaderProgramID GetShaderProgramIDByName(std::string name);
	DLL_TREATMENT ShaderProgramID AddShaderProgram(std::string name, ShaderProgram* prog);
	DLL_TREATMENT void RemoveShaderProgram(ShaderProgramID id);
}
