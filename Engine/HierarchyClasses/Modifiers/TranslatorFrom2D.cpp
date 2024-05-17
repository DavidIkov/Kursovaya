#include"TranslatorFrom2D.h"
#include"UsefullStuff/FilterVector.h"
#include"Instance.h"

const std::type_index TranslatorFrom2D::TypeIndex = std::type_index(typeid(TranslatorFrom2D));

void TranslatorFrom2D::processInsertionOfObject(Object2DData* obj) {
	FilteredOrder[obj->TranslatorFilteredOrderInd].ShaderID = obj->ShaderID;
	FilteredOrder[obj->TranslatorFilteredOrderInd].UniformDataUpdateEvent = &obj->UpdateUniformDataEvent;
	FilteredOrder[obj->TranslatorFilteredOrderInd].IntContainer = &obj->TranslatorFilteredOrderInd;
	obj->UpdateVertexesCordsInFilteredOrder(false);
	obj->UpdateVertexesRenderingOrderInFilteredOrder(false);
}

unsigned int TranslatorFrom2D::recursive_IterateThroughChildrenToFilter(Instance* curInst, unsigned int startInd) {

	std::vector<ClassesMap*> filteredObjects;
	{
		std::vector<ClassesMap*> children; curInst->GetChildrenWithModifier(Object2DData::TypeIndex, children);
		filteredObjects = FilterVectorHL(children, 
			[](ClassesMap* data) {return data->FindTypePtr<Object2DData>(Object2DData::TypeIndex)->gPriority(); });
	}


	unsigned int retSum = 0;

	for (unsigned int i = 0; i < filteredObjects.size(); i++) {
		ClassesMap* child = filteredObjects[i];

		Object2DData* obj2dChild = child->FindTypePtr<Object2DData>(Object2DData::TypeIndex);
		obj2dChild->TranslatorFilteredOrderInd = startInd + i;

		if (child->IsContains(TranslatorFrom2D::TypeIndex)) {
			FilteredOrder[startInd + i] = { &child->FindTypePtr<TranslatorFrom2D>(TypeIndex)->FilteredOrder,{},{},obj2dChild->ShaderID,&obj2dChild->UpdateUniformDataEvent,obj2dChild->RenderingObjectTypeInd,&obj2dChild->TranslatorFilteredOrderInd };
		}
		else if (child->IsContains(TranslatorFrom3D::TypeIndex)) {
			__debugbreak();//havent finished! when im here it will be same as if child have 2d translator modifier, only change is filtered order pointer
		}
		else {//for sure contains Object2DData
			processInsertionOfObject(obj2dChild);
		}
		retSum += 1 + recursive_IterateThroughChildrenToFilter(child->FindTypePtr<Instance>(Instance::TypeIndex), startInd + i + 1);

	}

	return retSum;
}

void TranslatorFrom2D::InsertInFilteredOrder(ClassesMap* object, ClassesMap* newPar) {
	Object2DData* obj2dData = object->FindTypePtr<Object2DData>(Object2DData::TypeIndex);

	//find closest child
	Object2DData* closestChild = nullptr;
	auto& children = newPar->FindTypePtr<Instance>(Instance::TypeIndex)->grChildren();
	for (unsigned int i = 0; i < children.size(); i++) {
		Object2DData* childObj2d = children[i]->FindTypePtr<Object2DData>(Object2DData::TypeIndex);
		if (childObj2d != nullptr and (closestChild == nullptr or (childObj2d->Priority > obj2dData->Priority and childObj2d->Priority < closestChild->Priority))) {
			closestChild = childObj2d;
		}
	}

	//get insert ind
	unsigned int insertInd = 0;
	if (closestChild != nullptr)
		insertInd = closestChild->TranslatorFilteredOrderInd + closestChild->DependentElementsAmountInFilteredOrder;
	else {
		Object2DData* par2dData = newPar->FindTypePtr<Object2DData>(Object2DData::TypeIndex);
		if (par2dData != nullptr and par2dData->Translator == obj2dData->Translator)
			insertInd = par2dData->TranslatorFilteredOrderInd + 1;
	}

	obj2dData->TranslatorFilteredOrderInd = insertInd;

	unsigned int insertLength = 0;

	//insert
	std::vector<Renderer::FilteredOrderElement>* filteredOrderPtr = nullptr;
	if (object->IsContains(TypeIndex)) {
		FilteredOrder.insert(FilteredOrder.begin() + insertInd, 
			{ &object->FindTypePtr<TranslatorFrom2D>(TypeIndex)->FilteredOrder,{},{},obj2dData->ShaderID,
			&obj2dData->UpdateUniformDataEvent,obj2dData->RenderingObjectTypeInd,&obj2dData->TranslatorFilteredOrderInd });

		insertLength = 1;
	}
	else if (object->IsContains(TranslatorFrom3D::TypeIndex)) {
		__debugbreak();//not finished
	}
	else {
		FilteredOrder.insert(FilteredOrder.begin() + insertInd, obj2dData->DependentElementsAmountInFilteredOrder, {});
		processInsertionOfObject(obj2dData);
		insertLength = 1 + recursive_IterateThroughChildrenToFilter(object->FindTypePtr<Instance>(Instance::TypeIndex), insertInd + 1);
	}

	for (unsigned int i = insertInd + insertLength; i < FilteredOrder.size(); i++) {
		(*FilteredOrder[i].IntContainer) += insertLength;
	}
}
void TranslatorFrom2D::RemoveFromFilteredOrder(ClassesMap* object) {
	Object2DData* obj2d = object->FindTypePtr<Object2DData>(Object2DData::TypeIndex);
	auto st = obj2d->Translator->FilteredOrder.begin() + obj2d->TranslatorFilteredOrderInd;
	obj2d->Translator->FilteredOrder.erase(st, st + obj2d->DependentElementsAmountInFilteredOrder);
	for (unsigned int i = obj2d->TranslatorFilteredOrderInd; i < obj2d->Translator->FilteredOrder.size(); i++)
		(*obj2d->Translator->FilteredOrder[i].IntContainer) -= obj2d->DependentElementsAmountInFilteredOrder;
}
void TranslatorFrom2D::MoveInFilteredOrder(ClassesMap* object) {
	__debugbreak();//gah daym...(need to actually write code... BOOOO)
}

void TranslatorFrom2D::sResolution(unsigned int w, unsigned int h){
	Width = w;
	Height = h;
	
	auto& children = InstancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->grChildren();
	for (unsigned int i = 0; i < children.size(); i++) {
		Object2DData* child2d = children[i]->FindTypePtr<Object2DData>(Object2DData::TypeIndex);
		if (child2d != nullptr) {
			child2d->RecalculateActualCords(true);
			child2d->UpdateVertexesCordsInFilteredOrder(true);
		}
	}
}

Vector2 TranslatorFrom2D::GetPositionOnWindow(Vector2 positionInPixels) const {
	if (InstancePtr->IsContains(Object2DData::TypeIndex)) {
		Object2DData* obj2dInst = InstancePtr->FindTypePtr<Object2DData>(Object2DData::TypeIndex);
		return obj2dInst->Translator->GetPositionOnWindow(obj2dInst->ActualPosition + obj2dInst->RotationMatrix * (positionInPixels / Vector2((float)Width / 2, (float)Height / 2) * obj2dInst->ActualSize));
	}
	else if (InstancePtr->IsContains(Object3DData::TypeIndex)) {
		__debugbreak();//unfinished
	}
	else if (InstancePtr->IsContains(Renderer::TypeIndex)) {
		return positionInPixels / Vector2((float)Width / 2, (float)Height / 2);
	}
	else {
		__debugbreak();//idk very strange how you can end up here
		return Vector2();
	}
}

TranslatorFrom2D* TranslatorFrom2D::gThis() {
	return this;
}
TranslatorFrom2D::TranslatorFrom2D(ClassesMap* mapPtr) {
	InstancePtr = mapPtr;
}



void TranslatorFrom2D::UpdateFilteredOrder() {

	FilteredOrder.resize(DependentObjectsAmount);

	recursive_IterateThroughChildrenToFilter(InstancePtr->FindTypePtr<Instance>(Instance::TypeIndex), 0);

}