#pragma once
#include"DLLStuff.h"
#include"Maths/SPCS.h"
#include"UsefullStuff/ClassEvent.h"
#include"Graphics/ShaderProgramManager.h"
#include"Graphics/Uniform.h"
#include"ClassesMap.h"
#include"UsefullStuff/ClassEvent.h"
#include<map>
enum VertexTemplateEnum {
	Square, Triangle
};
class Renderer2D;
class Object2DData {
private:
	friend Renderer2D;
	
	ClassesMap* instancePtr = nullptr;//"this" pointer from actual instance

	Event<const Uniform&> UpdateUniformData;
	
	std::vector<unsigned int> IndexBufferData;

	unsigned int RendererObjectRenderTypeInd;

	void RecursivelyGetDependentFilteredByPriority(std::vector<Object2DData*>&);

	void RecursivelyModifyParentDependentElementsAmount(int);
	unsigned int IndOfElement = 0;
	unsigned int DependentElementsAmount = 1;

	float Priority = 1.f;

	std::vector<float> Vertexes;
	std::vector<std::string> VertexDataNames;
	std::vector<unsigned int> VertexDataEndOffsets;
	std::vector<ShaderProgramTypeEnum> VertexDataTypes;

	void AddVertexDataData(std::string name, unsigned int len, ShaderProgramTypeEnum type);
	unsigned int FindVertexDataNameInd(const std::string& name) const;

	Object2DData* FirstGraphicsParent = nullptr;
	Renderer2D* FirstRenderingParent = nullptr;

private:
	//last rendered position
	Vector2 LRPosition = Vector2(0, 0);
public:
	const Vector2& gLRPosition() const;
private:
	//last rendered size
	Vector2 LRSize = Vector2(0, 0);
public:
	const Vector2& gLRSize() const;
private:


private:
	ShaderProgramID ShaderID;
public:
	DLL_TREATMENT bool ShaderCanBeApplyed(ShaderProgramID id);
	DLL_TREATMENT void sShaderID(ShaderProgramID id);
public:
	static const std::type_index TypeIndex;

	Object2DData* gThis();

	DSEvent<float> PriorityChange;
	DLL_TREATMENT void sPriority(float nVal);
	DLL_TREATMENT float gPriority() const;


	DLL_TREATMENT Object2DData(ClassesMap* nPtr);

	
	DLL_TREATMENT void SetVertexesParameterByIndex(unsigned int parameterInd, const std::vector<float>& data);
	DLL_TREATMENT void SetVertexesParameterByName(const std::string& parameterName, const std::vector<float>& data);
	
	DLL_TREATMENT void SetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd, const std::vector<float>& data);
	DLL_TREATMENT void SetVertexParameterByName(unsigned int vertexInd, std::string parameterName, const std::vector<float>& data);
	
	DLL_TREATMENT const std::vector<float>& GetVertexes() const;

	DLL_TREATMENT std::vector<float> GetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd) const;
	DLL_TREATMENT std::vector<float> GetVertexParameterByName(unsigned int vertexInd, std::string parameterName) const;

	DLL_TREATMENT void ApplyVertexTemplate(VertexTemplateEnum vertexTemplate);

	

	/*you should call "UpdateIndexBuffer" when u finished adding vertexes, 
	also order of vertexes is important for drawing some weird shape objects*/
	DLL_TREATMENT void AddVertex(std::vector<float> dataVec);

	//recalculates indexes order for rendering, may be a heavy function if a lot of vetexes
	DLL_TREATMENT void UpdateIndexBuffer();

	SPCS Position = SPCS(0, 0, 0, 0);
	SPCS PositionOffset = SPCS(0, 0, 0, 0);
	SPCS Size = SPCS(0, 0, 30,30);
	float Rotation = 0.f;
	SPCS RotationOffset = SPCS(0, 0, 0, 0);



};


