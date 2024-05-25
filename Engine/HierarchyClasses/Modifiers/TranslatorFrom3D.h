#pragma once
#include"DLLStuff.h"
#include"ClassesMap.h"
#include"Maths/Vector3.h"
#include"Maths/Matrix.h"
#include"Renderer.h"
enum class Vertex3DTemplates {
	Cube, Parallelepiped, Triangle
};
class TranslatorFrom3D {
	friend class TranslatorFrom2D;
	friend class Object3DData;
	ClassesMap* instancePtr = nullptr;//"this" pointer from actual instance

	std::vector<Renderer::ObjectsVectorElement> Objects;

	struct VertexesDataElement {
		std::vector<float> Data;
	};
	struct TrianglesDataElement {
		unsigned int VertexesOrder[3];
		std::vector<float> Data;
	};

	Vector3 CameraPosition = Vector3(0, 0, 0);
	Matrix CameraRotation = Matrix(3, 3, { 1,0,0,0,1,0,0,0,1 });
	Vector2 CameraAngle = Vector2(100.f/180.f*3.14f,40.f/180.f*3.14f);

	void RemoveFromObjects(ClassesMap* obj);
	void InsertInObjects(ClassesMap* obj);

	void recursive_IterateThroughObjectsToInsert(ClassesMap* obj);

	//returns position in scale of screen
	Vector2 GetPositionOnWindow(Vector3 positionInCameraCoordinates) const;

public:

	DLL_TREATMENT void sCameraAngle(const Vector2&& nRot);

	static const std::type_index TypeIndex;
	TranslatorFrom3D* gThis();
	DLL_TREATMENT TranslatorFrom3D(ClassesMap* mapPtr);

	bool AutomaticallyRecalculateActualCords = true;
	bool AutomaticallyUpdateVertexesRenderingOrderInFilteredOrder = true;
	bool AutomaticallyUpdateVertexesCordsInFilteredOrder = true;

};