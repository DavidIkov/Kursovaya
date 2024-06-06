#pragma once
#include"DLLStuff.h"
#include"ClassesMap.h"
#include"Maths/Vector3.h"
#include"Maths/Matrix.h"
#include"Renderer.h"
enum class Vertex3DTemplates {
	Cube, Triangle, Sphere
};
class TranslatorFrom3D {
	friend class TranslatorFrom2D;
	friend class Object3DData;
	ClassesMap* instancePtr = nullptr;//"this" pointer from actual instance

	std::vector<Renderer::RenderingDataElement> Objects;

	/*struct VertexesDataElement {
		std::vector<float> Data;
	};*/
	struct TrianglesDataElement {
		unsigned int VertexesOrder[3];
		std::vector<float> Data;
	};

	Renderer* RenderingParent;

	Vector3 CameraPosition = Vector3(0, 0, 0);
	Matrix CameraRotation = Matrix(3, 3, { 1,0,0,0,1,0,0,0,1 });
	Vector2 CameraViewAngle = Vector2(100.f/180.f*3.14f,40.f/180.f*3.14f);

	float MinCameraDepth = 0.1f;
	float MaxCameraDepth = 100.f;

	void InsertInObjects(ClassesMap* obj, ClassesMap* par);
	void RemoveFromObjects(ClassesMap* obj);

	//void recursive_IterateThroughObjectsToInsert(ClassesMap* obj);

	//returns position in scale of screen
	Matrix GetPositionInVirtualSpace(Vector3 positionInSpace) const;

public:

	DLL_TREATMENT void RenderInVirtualSpace() const;
	
	DLL_TREATMENT void sCameraViewAngle(const Vector2& nAngle);
	DLL_TREATMENT const Vector2& gCameraViewAngle() const;
	DLL_TREATMENT void sCameraRotation(const Matrix& nMat);
	DLL_TREATMENT const Matrix& gCameraRotation() const;
	DLL_TREATMENT void sCameraPosition(const Vector3& nPos);
	DLL_TREATMENT const Vector3& gCameraPosition() const;

	static const std::type_index TypeIndex;
	TranslatorFrom3D* gThis();
	DLL_TREATMENT TranslatorFrom3D(ClassesMap* mapPtr);

	bool RecalculateCoordinatesOfChildrenInSpace_OnCameraCoordinatesUpdate = true;
	bool RecalculateCoordinatesOfChildInSpace_OnChildParentUpdate = true;//works for new parent, this value wont do anything from previous parent
	bool RecalculateCoordinatesOfChildInSpace_OnChildCoordinatesUpdate = true;
	bool UpdateRenderingDataOfChildren_OnCameraCoordinatesUpdate = true;
	bool UpdateRenderingDataOfChild_OnChildParentUpdate = true;//works for new parent, this value wont do anything from previous parent
	bool UpdateRenderingDataOfChild_OnChildCoordinatesUpdate = true;

};