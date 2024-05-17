#include"ShaderProgramsManager.h"

//struct ShaderProgramManager::ShaderProgramData {
//	std::string Name;
//	ShaderProgram* ShaderProgramPtr;
//};

ShaderProgramID ShaderProgramsManager::GetShaderProgramIDByName(std::string name) {
	for (unsigned int i = 0; i < Names.size(); i++)
		if (Names[i] == name)
			return IDs[i];
	__debugbreak();//not found name
	return -1;
}
ShaderProgramID ShaderProgramsManager::AddShaderProgram(std::string name, ShaderProgram* prog) {
	Names.push_back(name);
	IDBasedVectorElementID id = ShaderPrograms.Insert({ name,prog });;
	IDs.push_back(id);
	return id;
}
void ShaderProgramsManager::RemoveShaderProgram(ShaderProgramID id) {
	auto& foundProgram = ShaderPrograms[id];
	ShaderProgram* ptr = foundProgram.ShaderProgramPtr;
	for (unsigned int i = 0; i < Names.size(); i++)
		if (Names[i] == foundProgram.Name) {
			Names.erase(Names.begin() + i);
			IDs.erase(IDs.begin() + i);
			break;
		}
	ShaderPrograms.Remove(id);
	ptr->DeleteShader();
}