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

	unsigned int Width, Height;

	unsigned int DependentObjectsAmount = 0;


	struct VertexesDataElement {
		std::vector<float> Data;
	};


	std::vector<Renderer::ObjectsVectorElement> FilteredOrder;
	void InsertInFilteredOrder(ClassesMap* object, ClassesMap* newPar);//object should not be in its new parent when calling function
	void RemoveFromFilteredOrder(ClassesMap* object);
	void MoveInFilteredOrder(ClassesMap* object);//finds new spot, and moves all data to it, object should not be in new parent when calling function

	void processInsertionOfObject(Object2DData* obj);//index in filtered order must be already set in object!
	unsigned int recursive_IterateThroughChildrenToFilter(Instance* curInst, unsigned int startInd);//inserts ierarchy of object's children in filtered order, returns amount of inserted objects

	//returns position in scale of screen
	Vector2 GetPositionOnWindow(Vector2 positionInPixels) const;

	SPCS CameraPosition = SPCS(0, 0, 0, 0);
	SPCS CameraSize = SPCS(1, 1, 0, 0);
	float CameraRotation = 0.f;

	Vector2 ActualCameraPosition = Vector2(0, 0);
	Vector2 ActualCameraSize = Vector2(0, 0);
	Matrix ActualCameraRotationMatrix = Matrix(2, 2, { 1,0,0,1 });

	void RecalculateCameraActualCords(bool tryToUpdateChildren);
public:


	DLL_TREATMENT void sCameraSize(const SPCS&& nSize);
	DLL_TREATMENT void sCameraPosition(const SPCS&& nPos);
	DLL_TREATMENT void sCameraRotation(float nRot);

	DLL_TREATMENT const SPCS& grCameraSize() const;
	DLL_TREATMENT SPCS gCameraSize() const;
	DLL_TREATMENT const SPCS& grCameraPosition() const;
	DLL_TREATMENT SPCS gCameraPosition() const;
	DLL_TREATMENT const float& grCameraRotation() const;
	DLL_TREATMENT float gCameraRotation() const;

	bool AutomaticallyRecalculateActualCords = true;
	bool AutomaticallyUpdateVertexesRenderingOrderInFilteredOrder = true;
	bool AutomaticallyUpdateVertexesCordsInFilteredOrder = true;
	

	DLL_TREATMENT void sResolution(unsigned int w, unsigned int h);

	static const std::type_index TypeIndex;
	TranslatorFrom2D* gThis();
	DLL_TREATMENT TranslatorFrom2D(ClassesMap* mapPtr);

	//should be used to update all children at once(it wont update cildren in another translators)
	DLL_TREATMENT void UpdateFilteredOrder();
};