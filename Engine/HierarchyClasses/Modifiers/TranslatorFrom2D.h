#pragma once
#include"DLLStuff.h"
#include"ClassesMap.h"
#include"TranslatorFrom3D.h"
#include"Object3DData.h"
#include"Object2DData.h"
#include"Maths/Vector2.h"
#include"Renderer.h"
class Instance;
class TranslatorFrom2D {
	friend class Object2DData;
	friend class Renderer;
	ClassesMap* InstancePtr = nullptr;//"this" pointer from actual instance

	unsigned int Width, Height;

	unsigned int DependentObjectsAmount = 0;

	std::vector<Renderer::FilteredOrderElement> FilteredOrder;
	void InsertInFilteredOrder(ClassesMap* object, ClassesMap* newPar);//object should not be in its new parent when calling function
	void RemoveFromFilteredOrder(ClassesMap* object);
	void MoveInFilteredOrder(ClassesMap* object);//finds new spot, and moves all data to it, object should not be in new parent when calling function

	void processInsertionOfObject(Object2DData* obj);//index in filtered order must be already set in object!
	unsigned int recursive_IterateThroughChildrenToFilter(Instance* curInst, unsigned int startInd);//inserts ierarchy of object's children in filtered order, returns amount of inserted objects

	//returns position in scale of screen
	Vector2 GetPositionOnWindow(Vector2 positionInPixels) const;

public:

	DLL_TREATMENT void sResolution(unsigned int w, unsigned int h);

	static const std::type_index TypeIndex;
	TranslatorFrom2D* gThis();
	DLL_TREATMENT TranslatorFrom2D(ClassesMap* mapPtr);

	//should be used to update all children at once(it wont update cildren in another translators)
	DLL_TREATMENT void UpdateFilteredOrder();
};