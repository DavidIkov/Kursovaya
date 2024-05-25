#pragma once
#include"DLLStuff.h"
#include"ClassesMap.h"
#include"Renderer.h"
#include"Maths/Vector3.h"
#include"TranslatorFrom3D.h"

class Object3DData {
	friend class TranslatorFrom3D;

	ClassesMap* instancePtr = nullptr;//"this" pointer from actual instance

	bool ObjectHasBeenInitialized = false;

	std::vector<TranslatorFrom3D::VertexesDataElement> VertexesData;
	std::vector<Renderer::DataLayoutElement> VertexesDataLayout;

	std::vector<TranslatorFrom3D::TrianglesDataElement> TrianglesData;
	std::vector<Renderer::DataLayoutElement> TrianglesDataLayout;

	std::vector<unsigned int> InitializedRenderingDataLayout;

	Event<const Uniform&> UpdateUniformDataEvent;

	unsigned int DependentObjectsAmount = 1;//including object itself
	void AddTo_DependentObjectsAmount_ToParent(int add);

	Vector3 Size = Vector3(0,0,0);
	Vector3 Position = Vector3(0, 0, 0);
	Matrix Rotation = Matrix(3, 3, { 1,0,0,0,1,0,0,0,1 });

	Vector3 ActualPosition = Vector3(0, 0, 0);
	Matrix ActualRotation = Matrix(3, 3, { 1,0,0,0,1,0,0,0,1 });


	TranslatorFrom3D* Translator = nullptr;
	Renderer* RenderingParent = nullptr;

	unsigned int RenderingObjectTypeInd;

	unsigned int TranslatorObjectsInd;

	ShaderProgramID ShaderID;

public:

	DLL_TREATMENT void sSize(const Vector3&& nSize);
	DLL_TREATMENT Vector3 gSize() const;
	DLL_TREATMENT const Vector3& grSize() const;
	DLL_TREATMENT void sPosition(const Vector3&& nPosition);
	DLL_TREATMENT Vector3 gPosition() const;
	DLL_TREATMENT const Vector3& grPosition() const;
	DLL_TREATMENT void sRotation(const Matrix&& nRotation);
	DLL_TREATMENT Matrix gRotation() const;
	DLL_TREATMENT const Matrix& grRotation() const;

	DLL_TREATMENT void sShaderID(ShaderProgramID id);

	/*you should call "ApplyAddedVertexesChanges" when u finished adding vertexes*/
	DLL_TREATMENT unsigned int AddVertex(const float(&& data)[]);

	DLL_TREATMENT unsigned int MakeTriangleOutOfVertexes(unsigned int v1i, unsigned int v2i, unsigned int v3i, const float(&& data)[]);

	DLL_TREATMENT void ApplyAddedVertexesChanges();

	DLL_TREATMENT void ApplyVertexTemplate(Vertex3DTemplates vertexTemplate, const float(&& data)[]);

	DLL_TREATMENT void SetVertexesParameterByIndex(unsigned int parameterInd, const float(&& data)[]);
	DLL_TREATMENT void SetVertexesParameterByName(const std::string&& parameterName, const float(&& data)[]);

	DLL_TREATMENT void SetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd, const float(&& data)[]);
	DLL_TREATMENT void SetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName, const float(&& data)[]);

	DLL_TREATMENT std::vector<TranslatorFrom3D::VertexesDataElement> gVertexes() const;
	DLL_TREATMENT const std::vector<TranslatorFrom3D::VertexesDataElement>& grVertexes() const;

	DLL_TREATMENT std::vector<float> GetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd) const;
	DLL_TREATMENT std::vector<float> GetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName) const;

	DLL_TREATMENT unsigned int FindVertexDataIndByName(const std::string&& name) const;


	//can be called ONLY before setting object parent and adding vertexes/applying templates
	DLL_TREATMENT void AddLayoutDataToVertexDataLayout(std::string name, ShaderProgramDataTypes type, unsigned int len);

	DLL_TREATMENT void AddLayoutDataToTriangleDataLayout(std::string name, ShaderProgramDataTypes type, unsigned int len);
	

	DLL_TREATMENT void RecalculateActualCords(bool recursive);

	DLL_TREATMENT void UpdateVertexesCordsInFilteredOrder(bool recursive);//updates vertexes positions in filtered order

	static const std::type_index TypeIndex;
	Object3DData* gThis();
	DLL_TREATMENT Object3DData(ClassesMap* mapPtr);
};