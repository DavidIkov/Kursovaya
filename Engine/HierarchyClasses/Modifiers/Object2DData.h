#pragma once
#include"DLLStuff.h"
#include"ClassesMap.h"
#include"Maths/Matrix.h"
#include"Maths/SPCS.h"
#include"Maths/Vector2.h"
#include"Instance.h"
#include"Graphics/ShaderProgramsManager.h"
#include"UsefullStuff/ClassEvent.h"
#include"Renderer.h"
class TranslatorFrom2D;
enum VertexTemplateEnum {
	Square, Triangle
};
class Object2DData {
	friend class TranslatorFrom2D;
	ClassesMap* instancePtr = nullptr;//"this" pointer from actual instance

	unsigned int RenderingObjectTypeInd;

	SPCS Size = SPCS(0, 0, 10, 10);
	SPCS Position = SPCS(0, 0, 0, 0);
	float Rotation = 0.f;

	Vector2 ActualSize = Vector2(0, 0);//in pixels
	Vector2 ActualPosition = Vector2(0, 0);//in pixels
	Matrix RotationMatrix = Matrix(2, 2, { 1,0,0,1 });

	TranslatorFrom2D* Translator = nullptr;
	Renderer* RenderingParent = nullptr;

	unsigned int TranslatorFilteredOrderInd;

	void AddTo_DependentElementsAmount_ToParent(int add);
	unsigned int DependentElementsAmountInFilteredOrder = 1;//including object itself

	
	std::vector<Renderer::VertexesDataElement> VertexesData;
	
	std::vector<Renderer::VertexAditionalDataLayoutElement> VertexAditionalDataLayout;
	std::vector<unsigned int> VertexesRenderingOrder;

	Event<const Uniform&> UpdateUniformDataEvent;

	void AddVertexAditionalDataLayout(std::string name, ShaderProgramTypeEnum type, unsigned int len);
	
	//calculates positions, and updates vertexes positions in filtered order
	void UpdateVertexesCordsInFilteredOrder(bool recursive);

	void RecalculateActualCords(bool recursive);

	void UpdateVertexesRenderingOrderInFilteredOrder(bool recursive);

	float Priority = 0.f;

	ShaderProgramID ShaderID;
public:
	DLL_TREATMENT bool ShaderCanBeApplyed(ShaderProgramID id);
	DLL_TREATMENT void sShaderID(ShaderProgramID id);

public:
	static const std::type_index TypeIndex;
	Object2DData* gThis();
	DLL_TREATMENT Object2DData(ClassesMap* mapPtr);

	DLL_TREATMENT float gPriority() const;
	DLL_TREATMENT const float& grPriority() const;
	DLL_TREATMENT void sPriority(float newPriority);

	/*you should call "UpdateIndexBuffer" when u finished adding vertexes,
	also order of vertexes is important for drawing some weird shape objects*/
	DLL_TREATMENT void AddVertex(const float(&& uhh)[]);

	//recalculates indexes order for rendering, may be a heavy function if a lot of vetexes
	DLL_TREATMENT void UpdateVertexesRenderingOrder();

	DLL_TREATMENT void ApplyVertexTemplate(VertexTemplateEnum vertexTemplate);

	DLL_TREATMENT void SetVertexesParameterByIndex(unsigned int parameterInd, const float(&& data)[]);
	DLL_TREATMENT void SetVertexesParameterByName(const std::string&& parameterName, const float(&& data)[]);

	DLL_TREATMENT void SetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd, const float(&& data)[]);
	DLL_TREATMENT void SetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName, const float(&& data)[]);

	DLL_TREATMENT std::vector<Renderer::VertexesDataElement> gVertexes() const;
	DLL_TREATMENT const std::vector<Renderer::VertexesDataElement>& grVertexes() const;

	DLL_TREATMENT std::vector<float> GetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd) const;
	DLL_TREATMENT std::vector<float> GetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName) const;

	DLL_TREATMENT unsigned int FindVertexDataIndByName(const std::string&& name) const;



	DLL_TREATMENT void sSize(const SPCS&& nSize);
	DLL_TREATMENT void sPosition(const SPCS&& nPos);
	DLL_TREATMENT void sRotation(float nRot);
	
	DLL_TREATMENT const SPCS& grSize() const;
	DLL_TREATMENT SPCS gSize() const;
	DLL_TREATMENT const SPCS& grPosition() const;
	DLL_TREATMENT SPCS gPosition() const;
	DLL_TREATMENT const float& grRotation() const;
	DLL_TREATMENT float gRotation() const;

};