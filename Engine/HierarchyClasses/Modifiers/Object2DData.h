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
#include"TranslatorFrom2D.h"

class Object2DData {
	friend class TranslatorFrom2D;
	friend class TranslatorFrom3D;
	ClassesMap* instancePtr = nullptr;//"this" pointer from actual instance

	bool ObjectHasBeenInitialized = false;

	std::vector<unsigned int> InitializedRenderingDataLayout;

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

	
	std::vector<TranslatorFrom2D::VertexesDataElement> VertexesData;
	std::vector<Renderer::DataLayoutElement> VertexesDataLayout;
	std::vector<unsigned int> VertexesRenderingOrder;

	Event<const Uniform&> UpdateUniformDataEvent;

public:

	//can be called ONLY before setting object parent and adding vertexes/applying templates
	DLL_TREATMENT void AddLayoutDataToVertexesDataLayout(std::string name, ShaderProgramDataTypes type, unsigned int len);
	
	DLL_TREATMENT void RecalculateActualCords(bool recursive);

	DLL_TREATMENT void UpdateVertexesCordsInFilteredOrder(bool recursive);//updates vertexes positions in filtered order

private:

	float Priority = 0.f;

	ShaderProgramID ShaderID;


	

public:
	DLL_TREATMENT void sShaderID(ShaderProgramID id);

	static const std::type_index TypeIndex;
	Object2DData* gThis();
	DLL_TREATMENT Object2DData(ClassesMap* mapPtr);

	DLL_TREATMENT float gPriority() const;
	DLL_TREATMENT const float& grPriority() const;
	DLL_TREATMENT void sPriority(float newPriority);

	/*you should call "ApplyAddedVertexesChanges" when u finished adding vertexes*/
	DLL_TREATMENT unsigned int AddVertex(const float(&& data)[]);

	DLL_TREATMENT void MakeTriangleOutOfVertexes(unsigned int i1, unsigned int i2, unsigned int i3);

	DLL_TREATMENT void ApplyVertexTemplate(Vertex2DTemplates vertexTemplate);

	DLL_TREATMENT void SetVertexesParameterByIndex(unsigned int parameterInd, const float(&& data)[]);
	DLL_TREATMENT void SetVertexesParameterByName(const std::string&& parameterName, const float(&& data)[]);

	DLL_TREATMENT void SetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd, const float(&& data)[]);
	DLL_TREATMENT void SetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName, const float(&& data)[]);

	DLL_TREATMENT std::vector<TranslatorFrom2D::VertexesDataElement> gVertexes() const;
	DLL_TREATMENT const std::vector<TranslatorFrom2D::VertexesDataElement>& grVertexes() const;

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