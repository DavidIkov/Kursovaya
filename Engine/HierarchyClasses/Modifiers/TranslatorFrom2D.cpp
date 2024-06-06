#include"TranslatorFrom2D.h"
#include"UsefullStuff/FilterVector.h"
#include"Instance.h"
#include"Object2DData.h"
#include"TranslatorFrom3D.h"
#include"Object3DData.h"
#include"OffScreenRenderingData.h"

const std::type_index TranslatorFrom2D::TypeIndex = std::type_index(typeid(TranslatorFrom2D));


void TranslatorFrom2D::RecalculateCameraActualCords(bool tryToUpdateChildren) {

	const Vector2& resolution = gResolution();

	ActualCameraPosition = Vector2(resolution.gX() / 2, resolution.gY() / 2) * CameraPosition.gSV() + CameraPosition.gPV();
	ActualCameraSize = resolution * CameraSize.gSV() + CameraSize.gPV();
	ActualCameraRotationMatrix = Matrix(2, 2, { cosf(CameraRotation),sinf(CameraRotation),-sinf(CameraRotation),cosf(CameraRotation) });

	if (RecalculateCoordinatesOfChildrenInSpace_OnCameraCoordinatesUpdate) {
		auto& children = InstancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->gChildren();
		for (unsigned int i = 0; i < children.size(); i++) {
			Object2DData* child2d = children[i]->FindTypePtr<Object2DData>(Object2DData::TypeIndex);
			if (child2d != nullptr) {
				child2d->RecalculateCoordinatesInSpace(true);
				if (UpdateRenderingDataOfChildren_OnCameraCoordinatesUpdate)
					child2d->UpdateVertexesDataInRenderingData(true);
			}
		}
	}
}

const Vector2& TranslatorFrom2D::gResolution() const {
	return InstancePtr->FindTypePtr<OffScreenRenderingData>(OffScreenRenderingData::TypeIndex)->Resolution;
}

void TranslatorFrom2D::sCameraSize(const SPCS&& nSize) {
	CameraSize = nSize;
	RecalculateCameraActualCords(true);
}
void TranslatorFrom2D::sCameraPosition(const SPCS&& nPos) {
	CameraPosition = nPos;
	RecalculateCameraActualCords(true);
}
void TranslatorFrom2D::sCameraRotation(float nRot) {
	CameraRotation = nRot;
	RecalculateCameraActualCords(true);
}

const SPCS& TranslatorFrom2D::gCameraSize() const {
	return CameraSize;
}
const SPCS& TranslatorFrom2D::gCameraPosition() const {
	return CameraPosition;
}
float TranslatorFrom2D::gCameraRotation() const {
	return CameraRotation;
}


//void TranslatorFrom2D::processInsertionOfObject(Object2DData* obj) {
//	FilteredOrder[obj->TranslatorFilteredOrderInd].ShaderID = obj->ShaderID;
//	FilteredOrder[obj->TranslatorFilteredOrderInd].UniformDataUpdateEvent = &obj->UpdateUniformDataEvent;
//	FilteredOrder[obj->TranslatorFilteredOrderInd].RenderingObjectTypeInd = obj->RenderingObjectTypeInd;
//	FilteredOrder[obj->TranslatorFilteredOrderInd].IntContainer = &obj->TranslatorFilteredOrderInd;
//}

//unsigned int TranslatorFrom2D::recursive_IterateThroughChildrenToFilter(Instance* curInst, unsigned int startInd) {
//
//	std::vector<ClassesMap*> filteredObjects;
//	{
//		std::vector<ClassesMap*> children; curInst->GetChildrenWithModifier(Object2DData::TypeIndex, children);
//		filteredObjects = FilterVectorHL(children, 
//			[](ClassesMap* data) {return data->FindTypePtr<Object2DData>(Object2DData::TypeIndex)->gPriority(); });
//	}
//
//
//	unsigned int retSum = 0;
//
//	for (unsigned int i = 0; i < filteredObjects.size(); i++) {
//		ClassesMap* child = filteredObjects[i];
//
//		Object2DData* obj2dChild = child->FindTypePtr<Object2DData>(Object2DData::TypeIndex);
//		obj2dChild->TranslatorFilteredOrderInd = startInd + i;
//
//		if (child->IsContains(TranslatorFrom2D::TypeIndex)) {
//			FilteredOrder[startInd + i] = { false,&child->FindTypePtr<TranslatorFrom2D>(TypeIndex)->FilteredOrder,
//				&child->FindTypePtr<OffScreenRenderingData>(OffScreenRenderingData::TypeIndex)->vFrameBuffer,{},obj2dChild->ShaderID,&obj2dChild->UpdateUniformDataEvent,obj2dChild->RenderingObjectTypeInd,&obj2dChild->TranslatorFilteredOrderInd };
//		}
//		else if (child->IsContains(TranslatorFrom3D::TypeIndex)) {
//			FilteredOrder[startInd + i] = { true,&child->FindTypePtr<TranslatorFrom3D>(TranslatorFrom3D::TypeIndex)->Objects,
//			&child->FindTypePtr<OffScreenRenderingData>(OffScreenRenderingData::TypeIndex)->vFrameBuffer,{}, obj2dChild->ShaderID,
//				&obj2dChild->UpdateUniformDataEvent,obj2dChild->RenderingObjectTypeInd,&obj2dChild->TranslatorFilteredOrderInd };
//
//		}
//		else {//for sure contains Object2DData
//			processInsertionOfObject(obj2dChild);
//		}
//		retSum += 1 + recursive_IterateThroughChildrenToFilter(child->FindTypePtr<Instance>(Instance::TypeIndex), startInd + i + 1);
//
//	}
//
//	return retSum;
//}

void TranslatorFrom2D::RenderInVirtualSpace() const {
#if defined _DEBUG
	if (RenderingParent == nullptr and not InstancePtr->IsContains(Renderer::TypeIndex)) {
		__debugbreak();//you trying to render object without a parent or it dosent have any rendering parent and objects isnt even a root of hierarchy
		return;
	}
#endif

	Renderer* renderer = nullptr;
	if (RenderingParent != nullptr) renderer = RenderingParent;
	else if (InstancePtr->IsContains(Renderer::TypeIndex)) renderer = InstancePtr->FindTypePtr<Renderer>(Renderer::TypeIndex);

	if (renderer!=nullptr) renderer->Render(Objects, InstancePtr->FindTypePtr<OffScreenRenderingData>(OffScreenRenderingData::TypeIndex)->vFrameBuffer);

}

void TranslatorFrom2D::InsertInObjects(ClassesMap* object, ClassesMap* parent) {


	Object2DData* obj2d = object->FindTypePtr<Object2DData>(Object2DData::TypeIndex);

	unsigned int insertInd = 0;
	if (parent->IsContains(Object2DData::TypeIndex)) {
		Object2DData* par2d = parent->FindTypePtr<Object2DData>(Object2DData::TypeIndex);

		//theoretically inserting it to end makes fixing indexes less expensive
		if (par2d->Translator == obj2d->Translator) insertInd = par2d->TranslatorObjectsIndex + par2d->DependentObjectsAmount_InObjectsVector;
	}


	obj2d->TranslatorObjectsIndex = insertInd;
	if (object->IsContains(TypeIndex) or object->IsContains(TranslatorFrom3D::TypeIndex)) {
		Objects.insert(Objects.begin() + insertInd, {
		&object->FindTypePtr<OffScreenRenderingData>(OffScreenRenderingData::TypeIndex)->vFrameBuffer,
		{},obj2d->ShaderID,&obj2d->UpdateUniformDataEvent,obj2d->RenderingObjectTypeInd,&obj2d->TranslatorObjectsIndex,&obj2d->DependentObjectsAmount_InObjectsVector });

	}
	else {
		Objects.insert(Objects.begin() + insertInd, {
		nullptr,{},obj2d->ShaderID,&obj2d->UpdateUniformDataEvent,obj2d->RenderingObjectTypeInd,&obj2d->TranslatorObjectsIndex,&obj2d->DependentObjectsAmount_InObjectsVector });

		auto& children = object->FindTypePtr<Instance>(Instance::TypeIndex)->Children;

		//fix offsets
		for (unsigned int i = insertInd + 1; i < Objects.size(); i++) (*Objects[i].IndexContainer)++;

		for (unsigned int i = 0; i < children.size(); i++)
			if (children[i]->IsContains(Object2DData::TypeIndex)) InsertInObjects(children[i], object);

	}

	//Object2DData* obj2dData = object->FindTypePtr<Object2DData>(Object2DData::TypeIndex);

	////find closest child
	//Object2DData* closestChild = nullptr;
	//auto& children = newPar->FindTypePtr<Instance>(Instance::TypeIndex)->gChildren();
	//for (unsigned int i = 0; i < children.size(); i++) {
	//	Object2DData* childObj2d = children[i]->FindTypePtr<Object2DData>(Object2DData::TypeIndex);
	//	if (childObj2d != nullptr and (closestChild == nullptr or (childObj2d->Priority > obj2dData->Priority and childObj2d->Priority < closestChild->Priority))) {
	//		closestChild = childObj2d;
	//	}
	//}

	////get insert ind
	//unsigned int insertInd = 0;
	//if (closestChild != nullptr)
	//	insertInd = closestChild->TranslatorFilteredOrderInd + closestChild->DependentElementsAmountInFilteredOrder;
	//else {
	//	Object2DData* par2dData = newPar->FindTypePtr<Object2DData>(Object2DData::TypeIndex);
	//	if (par2dData != nullptr and par2dData->Translator == obj2dData->Translator)
	//		insertInd = par2dData->TranslatorFilteredOrderInd + 1;
	//}

	//obj2dData->TranslatorFilteredOrderInd = insertInd;

	//unsigned int insertLength = 0;

	////insert
	//std::vector<Renderer::ObjectsVectorElement>* filteredOrderPtr = nullptr;
	//if (object->IsContains(TypeIndex)) {
	//	FilteredOrder.insert(FilteredOrder.begin() + insertInd, 
	//		{ false,&object->FindTypePtr<TranslatorFrom2D>(TypeIndex)->FilteredOrder,&object->FindTypePtr<OffScreenRenderingData>(OffScreenRenderingData::TypeIndex)->vFrameBuffer,
	//		{},obj2dData->ShaderID,
	//		&obj2dData->UpdateUniformDataEvent,obj2dData->RenderingObjectTypeInd,&obj2dData->TranslatorFilteredOrderInd });

	//	insertLength = 1;
	//}
	//else if (object->IsContains(TranslatorFrom3D::TypeIndex)) {
	//	FilteredOrder.insert(FilteredOrder.begin() + insertInd,
	//		{ true,&object->FindTypePtr<TranslatorFrom3D>(TranslatorFrom3D::TypeIndex)->Objects,&object->FindTypePtr<OffScreenRenderingData>(OffScreenRenderingData::TypeIndex)->vFrameBuffer,
	//		{},obj2dData->ShaderID,
	//		&obj2dData->UpdateUniformDataEvent,obj2dData->RenderingObjectTypeInd,&obj2dData->TranslatorFilteredOrderInd });

	//	insertLength = 1;
	//}
	//else {
	//	FilteredOrder.insert(FilteredOrder.begin() + insertInd, obj2dData->DependentElementsAmountInFilteredOrder, {});
	//	processInsertionOfObject(obj2dData);
	//	insertLength = 1 + recursive_IterateThroughChildrenToFilter(object->FindTypePtr<Instance>(Instance::TypeIndex), insertInd + 1);
	//}

	//for (unsigned int i = insertInd + insertLength; i < FilteredOrder.size(); i++) {
	//	(*FilteredOrder[i].IntContainer) += insertLength;
	//}
}
void TranslatorFrom2D::RemoveFromObjects(ClassesMap* object) {
	Object2DData* obj2d = object->FindTypePtr<Object2DData>(Object2DData::TypeIndex);
	auto st = Objects.begin() + obj2d->TranslatorObjectsIndex;
	Objects.erase(st, st + obj2d->DependentObjectsAmount_InObjectsVector);
	for (unsigned int i = obj2d->TranslatorObjectsIndex; i < Objects.size(); i++)
		(*Objects[i].IndexContainer) -= obj2d->DependentObjectsAmount_InObjectsVector;
}


Vector2 TranslatorFrom2D::GetPositionInVirtualSpace(Vector2 positionInSpaceInPixels) const {

	const Vector2& resolution = gResolution();

	Vector2 positionInSpaceFromCamera;

	{//apply camera

		Vector2 frac(ActualCameraSize.gX() / resolution.gX(), ActualCameraSize.gY() / resolution.gY());

		positionInSpaceFromCamera = positionInSpaceInPixels / frac;
		positionInSpaceFromCamera = positionInSpaceFromCamera - ActualCameraPosition / frac;
		positionInSpaceFromCamera = ActualCameraRotationMatrix.gReversedMatrix() * positionInSpaceFromCamera;

	}

	return positionInSpaceFromCamera / Vector2(resolution.gX() / 2, resolution.gY() / 2);
}

TranslatorFrom2D* TranslatorFrom2D::gThis() {
	return this;
}
TranslatorFrom2D::TranslatorFrom2D(ClassesMap* mapPtr) {
	InstancePtr = mapPtr;

	RecalculateCameraActualCords(true);

	InstancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->ParentChange.ConnectAfterEvent([this](ClassesMap* prevPar) {
		
		ClassesMap* foundRenderer = InstancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->FindFirstParentWithModifier(Renderer::TypeIndex);
		RenderingParent = foundRenderer->FindTypePtr<Renderer>(Renderer::TypeIndex);
		});
}

