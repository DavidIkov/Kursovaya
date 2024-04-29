#pragma once
#include"../../UsefullStuff/IDBasedVector.h"
#include"ShaderProgram.h"


typedef IDBasedVectorElementID ShaderProgramID;

namespace ShaderProgramManager {
	struct ShaderProgramData {
		std::string Name;
		ShaderProgram* ShaderProgramPtr;
	};
	inline IDBasedVector<ShaderProgramData> ShaderPrograms;
	inline std::vector<std::string> Names;
	inline std::vector<ShaderProgramID> IDs;

	ShaderProgramID GetShaderProgramIDByName(std::string name);
	ShaderProgramID AddShaderProgram(std::string name, ShaderProgram* prog);
	void RemoveShaderProgram(ShaderProgramID id);
}
