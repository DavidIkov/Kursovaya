#include"../Maths/SPCS.h"
#include"../UsefullStuff/ClassEvent.h"
#include"../../GraphicsPrimitives/ShaderProgram.h"
#include"../../GraphicsPrimitives/Uniform.h"
#include"ClassesMap.h"
#include"../../UsefullStuff/ClassEvent.h"
#include<map>
enum VertexTemplateEnum {
	Square, Triangle
};
class Renderer2D;
class Instance2DData {
private:
	friend Renderer2D;

	ShaderProgram Shader;
	
	ClassesMap* instancePtr = nullptr;//"this" pointer from actual instance

	Event<const Uniform&> UpdateUniformData;
	
	std::vector<unsigned int> IndexBufferData;

	unsigned int RendererObjectRenderTypeInd;

	void RecursivelyGetDependentFilteredByPriority(std::vector<Instance2DData*>&);

	void RecursivelyModifyParentDependentElementsAmount(int);
	unsigned int IndOfElement = 0;
	unsigned int DependentElementsAmount = 1;

	float Priority = 1.f;

	std::vector<float> Vertexes;
	std::vector<std::string> VertexDataNames;
	std::vector<unsigned int> VertexDataEndOffsets;

	void AddVertexDataData(std::string name, unsigned int len, ShaderProgramTypeEnum type);
	unsigned int FindVertexDataNameInd(const std::string& name) const;

	Instance2DData* FirstGraphicsParent = nullptr;
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


public:



	static const std::type_index TypeIndex;

	Instance2DData* gThis();

	DSEvent<float> PriorityChange;
	void sPriority(float nVal);
	float gPriority() const;


	Instance2DData(ClassesMap* nPtr);

	
	void SetVertexesParameterByIndex(unsigned int parameterInd, const std::vector<float>& data);
	void SetVertexesParameterByName(const std::string& parameterName, const std::vector<float>& data);
	
	void SetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd, const std::vector<float>& data);
	void SetVertexParameterByName(unsigned int vertexInd, std::string parameterName, const std::vector<float>& data);
	
	const std::vector<float>& GetVertexes() const;

	std::vector<float> GetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd) const;
	std::vector<float> GetVertexParameterByName(unsigned int vertexInd, std::string parameterName) const;

	void ApplyVertexTemplate(VertexTemplateEnum vertexTemplate);

	

	/*you should call "UpdateIndexBuffer" when u finished adding vertexes, 
	also order of vertexes is important for drawing some weird shape objects*/
	void AddVertex(std::vector<float> dataVec);

	//recalculates indexes order for rendering, may be a heavy function if a lot of vetexes
	void UpdateIndexBuffer();

	SPCS Position = SPCS(0, 0, 0, 0);
	SPCS PositionOffset = SPCS(0, 0, 0, 0);
	SPCS Size = SPCS(0, 0, 30,30);
	float Rotation = 0.f;
	SPCS RotationOffset = SPCS(0, 0, 0, 0);



};


