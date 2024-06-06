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

	std::vector<std::vector<float>> VertexesData;
	std::vector<Renderer::DataLayoutElement> VertexesNonTemporaryData_Layout;
	std::vector<Renderer::DataLayoutElement> VertexesTemporaryData_Layout;

	std::vector<TranslatorFrom3D::TrianglesDataElement> TrianglesData;
	std::vector<Renderer::DataLayoutElement> TrianglesNonTemporaryData_Layout;
	std::vector<Renderer::DataLayoutElement> TrianglesTemporaryData_Layout;

	std::vector<unsigned int> InitializedRenderingDataLayout;

	Event<const Uniform&> UpdateUniformDataEvent;

	unsigned int DependentObjectsAmount_InObjectsVector = 1;//including object itself
	void AddTo_DependentObjectsAmount_ToParent(int add);

	Vector3 Size = Vector3(0,0,0);

	Vector3 PositionFromParent = Vector3(0, 0, 0);
	Matrix RotationFromParent = Matrix(3, 3, { 1,0,0,0,1,0,0,0,1 });

	Vector3 PositionInSpace = Vector3(0, 0, 0);
	Matrix RotationInSpace = Matrix(3, 3, { 1,0,0,0,1,0,0,0,1 });


	TranslatorFrom3D* Translator = nullptr;
	Renderer* RenderingParent = nullptr;

	unsigned int RenderingObjectTypeInd;

	unsigned int TranslatorObjectsIndex;

	ShaderProgramID ShaderID;

public:

	//can be called ONLY before setting object parent and adding vertexes/applying templates
	DLL_TREATMENT void AddLayoutDataToVertexesNonTemporaryData_Layout(std::string name, ShaderProgramDataTypes type, unsigned int len);
private:
	void AddLayoutDataToVertexesTemporaryData_Layout(ShaderProgramDataTypes type, unsigned int len);
public:
	//can be called ONLY before setting object parent and adding vertexes/applying templates
	DLL_TREATMENT void AddLayoutDataToTrianglesNonTemporaryData_Layout(std::string name, ShaderProgramDataTypes type, unsigned int len);
private:
	void AddLayoutDataToTrianglesTemporaryData_Layout(ShaderProgramDataTypes type, unsigned int len);
public:

	DLL_TREATMENT void sSize(const Vector3&& nSize);
	DLL_TREATMENT const Vector3& gSize() const;

	DLL_TREATMENT void sPositionFromParent(const Vector3&& nPosition);
	DLL_TREATMENT const Vector3& gPositionFromParent() const;
	DLL_TREATMENT void sRotationFromParent(const Matrix&& nRotation);
	DLL_TREATMENT const Matrix& gRotationFromParent() const;
	
	DLL_TREATMENT const Vector3& gPositionInSpace() const;
	DLL_TREATMENT const Matrix& gRotationInSpace() const;

	DLL_TREATMENT void sShaderID(ShaderProgramID id);

	/*firsly you pass vertex data and then triangle data, all should be from user accessible layout*/
	DLL_TREATMENT unsigned int AddVertex(const float(&& data)[]);

	DLL_TREATMENT unsigned int MakeTriangleOutOfVertexes(unsigned int v1i, unsigned int v2i, unsigned int v3i, const float(&& data)[]);
	
	DLL_TREATMENT void ApplyVertexTemplate(Vertex3DTemplates vertexTemplate, const float(&& data)[]);

	DLL_TREATMENT void SetVertexesParameterByIndex(unsigned int parameterInd, const float(&& data)[]);
	DLL_TREATMENT void SetVertexesParameterByName(const std::string&& parameterName, const float(&& data)[]);

	DLL_TREATMENT void SetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd, const float(&& data)[]);
	DLL_TREATMENT void SetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName, const float(&& data)[]);

	DLL_TREATMENT std::vector<float> GetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd) const;
	DLL_TREATMENT std::vector<float> GetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName) const;

	DLL_TREATMENT unsigned int FindVertexDataIndByName(const std::string&& name) const;



	DLL_TREATMENT void SetTrianglesParameterByIndex(unsigned int parameterInd, const float(&& data)[]);
	DLL_TREATMENT void SetTrianglesParameterByName(const std::string&& parameterName, const float(&& data)[]);

	DLL_TREATMENT void SetTriangleParameterByIndex(unsigned int triangleInd, unsigned int parameterInd, const float(&& data)[]);
	DLL_TREATMENT void SetTriangleParameterByName(unsigned int triangleInd, const std::string&& parameterName, const float(&& data)[]);

	DLL_TREATMENT std::vector<float> GetTriangleParameterByIndex(unsigned int triangleInd, unsigned int parameterInd) const;
	DLL_TREATMENT std::vector<float> GetTriangleParameterByName(unsigned int triangleInd, const std::string&& parameterName) const;

	DLL_TREATMENT unsigned int FindTriangleDataIndByName(const std::string&& name) const;


	

	DLL_TREATMENT void RecalculateCoordinatesInSpace(bool recursive);

	DLL_TREATMENT void UpdateVertexesAndTrianglesDataInRenderingData(bool recursive);

	static const std::type_index TypeIndex;
	Object3DData* gThis();
	DLL_TREATMENT Object3DData(ClassesMap* mapPtr);
};