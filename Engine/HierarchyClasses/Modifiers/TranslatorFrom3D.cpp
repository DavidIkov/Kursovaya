#include"TranslatorFrom3D.h"
#include"Object3DData.h"
#include"TranslatorFrom2D.h"
#include"Object2DData.h"

const std::type_index TranslatorFrom3D::TypeIndex = std::type_index(typeid(TranslatorFrom3D));

void TranslatorFrom3D::RemoveFromObjects(ClassesMap* obj) {
	Object3DData* obj3d = obj->FindTypePtr<Object3DData>(Object3DData::TypeIndex);
	auto st = Objects.begin() + obj3d->TranslatorObjectsInd;
	Objects.erase(st, st + obj3d->DependentObjectsAmount);
	for (unsigned int i = obj3d->TranslatorObjectsInd; i < Objects.size(); i++)
		(*Objects[i].IntContainer) -= obj3d->DependentObjectsAmount;
}
void TranslatorFrom3D::recursive_IterateThroughObjectsToInsert(ClassesMap* obj) {
	Object3DData* obj3d = obj->FindTypePtr<Object3DData>(Object3DData::TypeIndex);
	if (obj->IsContains(TypeIndex)) {
		Objects.push_back({
		true,&obj->FindTypePtr<TranslatorFrom3D>(TypeIndex)->Objects,
		{},obj3d->ShaderID,&obj3d->UpdateUniformDataEvent,obj3d->RenderingObjectTypeInd,&obj3d->TranslatorObjectsInd
			});
		obj3d->TranslatorObjectsInd = Objects.size() - 1;
	}
	else if (obj->IsContains(TranslatorFrom2D::TypeIndex)) {
		Objects.push_back({
		false,&obj->FindTypePtr<TranslatorFrom2D>(TranslatorFrom2D::TypeIndex)->FilteredOrder,
		{},obj3d->ShaderID,&obj3d->UpdateUniformDataEvent,obj3d->RenderingObjectTypeInd,&obj3d->TranslatorObjectsInd
			});
		obj3d->TranslatorObjectsInd = Objects.size() - 1;
	}
	else {
		Objects.push_back({
		false,nullptr,
		{},obj3d->ShaderID,&obj3d->UpdateUniformDataEvent,obj3d->RenderingObjectTypeInd,&obj3d->TranslatorObjectsInd
			});
		obj3d->TranslatorObjectsInd = Objects.size() - 1;

		auto& children = obj->FindTypePtr<Instance>(Instance::TypeIndex)->Children;
		for (unsigned int i = 0; i < children.size(); i++) {
			if (children[i]->IsContains(Object3DData::TypeIndex)) recursive_IterateThroughObjectsToInsert(children[i]);
		}

	}
}
void TranslatorFrom3D::InsertInObjects(ClassesMap* obj) {
	recursive_IterateThroughObjectsToInsert(obj);
	
}

void TranslatorFrom3D::sCameraAngle(const Vector2&& nRot) {
	CameraAngle = nRot;
	if (AutomaticallyRecalculateActualCords) {
		auto& children = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->Children;
		for (unsigned int i = 0; i < children.size(); i++) {
			Object3DData* obj3d = children[i]->FindTypePtr<Object3DData>(Object3DData::TypeIndex);
			if (obj3d != nullptr) {
				obj3d->RecalculateActualCords(true);
				if (AutomaticallyUpdateVertexesCordsInFilteredOrder)
					obj3d->UpdateVertexesCordsInFilteredOrder(true);
			}
		}
	}
}

Vector2 TranslatorFrom3D::GetPositionOnWindow(Vector3 positionInCameraCoordinates) const {
	if (instancePtr->IsContains(Object3DData::TypeIndex)) {
		Object3DData* obj3dInst = instancePtr->FindTypePtr<Object3DData>(Object3DData::TypeIndex);
		return obj3dInst->Translator->GetPositionOnWindow(obj3dInst->ActualPosition + obj3dInst->ActualRotation * positionInCameraCoordinates);
	}
	else if (instancePtr->IsContains(Object2DData::TypeIndex)) {
		Object2DData* obj2dInst = instancePtr->FindTypePtr<Object2DData>(Object2DData::TypeIndex);
		Vector2 screenScaledPos = (positionInCameraCoordinates / positionInCameraCoordinates.gZ()).gvXY() / Vector2(tanf(CameraAngle.gX() / 2), tanf(CameraAngle.gY() / 2));
		return obj2dInst->Translator->GetPositionOnWindow(obj2dInst->ActualPosition + obj2dInst->RotationMatrix * (screenScaledPos * obj2dInst->ActualSize));
	}
	__debugbreak();//idk very strange how you can end up here
	return Vector2();
}

TranslatorFrom3D* TranslatorFrom3D::gThis() {
	return this;
}
TranslatorFrom3D::TranslatorFrom3D(ClassesMap* mapPtr) {
	instancePtr = mapPtr;
}