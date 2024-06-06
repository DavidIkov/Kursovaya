#pragma once
#include"DLLStuff.h"
#include"ClassesMap.h"
#include"Maths/Vector2.h"
#include"Maths/SPCS.h"
#include"Maths/Matrix.h"
#include"Renderer.h"

enum class Vertex2DTemplates {
	Square, Triangle
};

class Instance;
class Object2DData;
class TranslatorFrom2D {
	friend class Object2DData;
	friend class Renderer;
	friend class TranslatorFrom3D;
	ClassesMap* InstancePtr = nullptr;//"this" pointer from actual instance

	unsigned int DependentObjectsAmount = 0;


	std::vector<Renderer::RenderingDataElement> Objects;
	void InsertInObjects(ClassesMap* object, ClassesMap* parent);
	void RemoveFromObjects(ClassesMap* object);

	//void processInsertionOfObject(Object2DData* obj);//index in filtered order must be already set in object!
	//unsigned int recursive_IterateThroughChildrenToFilter(Instance* curInst, unsigned int startInd);//inserts ierarchy of object's children in filtered order, returns amount of inserted objects

	//returns position in off screen renderer
	Vector2 GetPositionInVirtualSpace(Vector2 positionInSpaceInPixels) const;

	SPCS CameraPosition = SPCS(0, 0, 0, 0);
	SPCS CameraSize = SPCS(1, 1, 0, 0);
	float CameraRotation = 0.f;

	Vector2 ActualCameraPosition = Vector2(0, 0);
	Vector2 ActualCameraSize = Vector2(0, 0);
	Matrix ActualCameraRotationMatrix = Matrix(2, 2, { 1,0,0,1 });

	Renderer* RenderingParent;

	void RecalculateCameraActualCords(bool tryToUpdateChildren);
public:

	DLL_TREATMENT const Vector2& gResolution() const;

	DLL_TREATMENT void sCameraSize(const SPCS&& nSize);
	DLL_TREATMENT void sCameraPosition(const SPCS&& nPos);
	DLL_TREATMENT void sCameraRotation(float nRot);

	DLL_TREATMENT const SPCS& gCameraSize() const;
	DLL_TREATMENT const SPCS& gCameraPosition() const;
	DLL_TREATMENT float gCameraRotation() const;

	DLL_TREATMENT void RenderInVirtualSpace() const;


	bool RecalculateCoordinatesOfChildrenInSpace_OnCameraCoordinatesUpdate = true;
	bool RecalculateCoordinatesOfChildInSpace_OnChildParentUpdate = true;//works for new parent, this value wont do anything from previous parent
	bool RecalculateCoordinatesOfChildInSpace_OnChildCoordinatesUpdate = true;
	bool UpdateRenderingDataOfChildren_OnCameraCoordinatesUpdate = true;
	bool UpdateRenderingDataOfChild_OnChildParentUpdate = true;//works for new parent, this value wont do anything from previous parent
	bool UpdateRenderingDataOfChild_OnChildCoordinatesUpdate = true;
	

	static const std::type_index TypeIndex;
	TranslatorFrom2D* gThis();
	DLL_TREATMENT TranslatorFrom2D(ClassesMap* mapPtr);
};