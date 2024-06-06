#include"TranslatorFrom3D.h"
#include"Object3DData.h"
#include"TranslatorFrom2D.h"
#include"Object2DData.h"
#include"OffScreenRenderingData.h"

const std::type_index TranslatorFrom3D::TypeIndex = std::type_index(typeid(TranslatorFrom3D));

//void TranslatorFrom3D::RemoveFromObjects(ClassesMap* obj) {
//	Object3DData* obj3d = obj->FindTypePtr<Object3DData>(Object3DData::TypeIndex);
//	auto st = Objects.begin() + obj3d->TranslatorObjectsInd;
//	Objects.erase(st, st + obj3d->DependentObjectsAmount);
//	for (unsigned int i = obj3d->TranslatorObjectsInd; i < Objects.size(); i++)
//		(*Objects[i].IntContainer) -= obj3d->DependentObjectsAmount;
//}
//void TranslatorFrom3D::recursive_IterateThroughObjectsToInsert(ClassesMap* obj) {
//	Object3DData* obj3d = obj->FindTypePtr<Object3DData>(Object3DData::TypeIndex);
//	if (obj->IsContains(TypeIndex)) {
//		Objects.push_back({
//		true,&obj->FindTypePtr<TranslatorFrom3D>(TypeIndex)->Objects,&obj->FindTypePtr<OffScreenRenderingData>(OffScreenRenderingData::TypeIndex)->vFrameBuffer,
//		{},obj3d->ShaderID,&obj3d->UpdateUniformDataEvent,obj3d->RenderingObjectTypeInd,&obj3d->TranslatorObjectsInd
//			});
//		obj3d->TranslatorObjectsInd = Objects.size() - 1;
//	}
//	else if (obj->IsContains(TranslatorFrom2D::TypeIndex)) {
//		Objects.push_back({
//		false,&obj->FindTypePtr<TranslatorFrom2D>(TranslatorFrom2D::TypeIndex)->FilteredOrder,&obj->FindTypePtr<OffScreenRenderingData>(OffScreenRenderingData::TypeIndex)->vFrameBuffer,
//		{},obj3d->ShaderID,&obj3d->UpdateUniformDataEvent,obj3d->RenderingObjectTypeInd,&obj3d->TranslatorObjectsInd
//			});
//		obj3d->TranslatorObjectsInd = Objects.size() - 1;
//	}
//	else {
//		Objects.push_back({
//		false,nullptr,nullptr,
//		{},obj3d->ShaderID,&obj3d->UpdateUniformDataEvent,obj3d->RenderingObjectTypeInd,&obj3d->TranslatorObjectsInd
//			});
//		obj3d->TranslatorObjectsInd = Objects.size() - 1;
//
//		auto& children = obj->FindTypePtr<Instance>(Instance::TypeIndex)->Children;
//		for (unsigned int i = 0; i < children.size(); i++) {
//			if (children[i]->IsContains(Object3DData::TypeIndex)) recursive_IterateThroughObjectsToInsert(children[i]);
//		}
//
//	}
//}
//void TranslatorFrom3D::InsertInObjects(ClassesMap* obj) {
//	recursive_IterateThroughObjectsToInsert(obj);
//	
//}

void TranslatorFrom3D::RenderInVirtualSpace() const {
#if defined _DEBUG
	if (RenderingParent == nullptr) {
		__debugbreak();//you trying to render object without a parent or it dosent have any rendering parent
		return;
	}
#endif
	if (RenderingParent != nullptr) {
		RenderingParent->Render(Objects, instancePtr->FindTypePtr<OffScreenRenderingData>(OffScreenRenderingData::TypeIndex)->vFrameBuffer);
	}
}


void TranslatorFrom3D::InsertInObjects(ClassesMap* object, ClassesMap* parent) {

	Object3DData* obj2d = object->FindTypePtr<Object3DData>(Object3DData::TypeIndex);

	unsigned int insertInd = 0;
	if (parent->IsContains(Object3DData::TypeIndex)) {
		Object3DData* par2d = parent->FindTypePtr<Object3DData>(Object3DData::TypeIndex);

		//theoretically inserting it to end makes fixing indexes less expensive
		if (par2d->Translator == obj2d->Translator) insertInd = par2d->TranslatorObjectsIndex + par2d->DependentObjectsAmount_InObjectsVector;
	}

	obj2d->TranslatorObjectsIndex = insertInd;
	if (object->IsContains(TypeIndex) or object->IsContains(TranslatorFrom2D::TypeIndex)) {
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
			if (children[i]->IsContains(Object3DData::TypeIndex)) InsertInObjects(children[i], object);


	}
}
void TranslatorFrom3D::RemoveFromObjects(ClassesMap* object) {
	Object3DData* obj2d = object->FindTypePtr<Object3DData>(Object3DData::TypeIndex);
	auto st = Objects.begin() + obj2d->TranslatorObjectsIndex;
	Objects.erase(st, st + obj2d->DependentObjectsAmount_InObjectsVector);
	for (unsigned int i = obj2d->TranslatorObjectsIndex; i < Objects.size(); i++)
		(*Objects[i].IndexContainer) -= obj2d->DependentObjectsAmount_InObjectsVector;
}

void TranslatorFrom3D::sCameraViewAngle(const Vector2& nAngle) {
	CameraViewAngle = nAngle;
	if (RecalculateCoordinatesOfChildrenInSpace_OnCameraCoordinatesUpdate) {
		auto& children = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->Children;
		for (unsigned int i = 0; i < children.size(); i++) {
			Object3DData* obj3d = children[i]->FindTypePtr<Object3DData>(Object3DData::TypeIndex);
			if (obj3d != nullptr) {
				obj3d->RecalculateCoordinatesInSpace(true);
				if (UpdateRenderingDataOfChildren_OnCameraCoordinatesUpdate)
					obj3d->UpdateVertexesAndTrianglesDataInRenderingData(true);
			}
		}
	}
}


const Vector2& TranslatorFrom3D::gCameraViewAngle() const {
	return CameraViewAngle;
}
void TranslatorFrom3D::sCameraRotation(const Matrix& nMat) {
	CameraRotation = nMat;
	if (RecalculateCoordinatesOfChildrenInSpace_OnCameraCoordinatesUpdate) {
		auto& children = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->Children;
		for (unsigned int i = 0; i < children.size(); i++) {
			Object3DData* obj3d = children[i]->FindTypePtr<Object3DData>(Object3DData::TypeIndex);
			if (obj3d != nullptr) {
				obj3d->RecalculateCoordinatesInSpace(true);
				if (UpdateRenderingDataOfChildren_OnCameraCoordinatesUpdate)
					obj3d->UpdateVertexesAndTrianglesDataInRenderingData(true);
			}
		}
	}
}
const Matrix& TranslatorFrom3D::gCameraRotation() const {
	return CameraRotation;
}
void TranslatorFrom3D::sCameraPosition(const Vector3& nPos) {
	CameraPosition = nPos;
	if (RecalculateCoordinatesOfChildrenInSpace_OnCameraCoordinatesUpdate) {
		auto& children = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->Children;
		for (unsigned int i = 0; i < children.size(); i++) {
			Object3DData* obj3d = children[i]->FindTypePtr<Object3DData>(Object3DData::TypeIndex);
			if (obj3d != nullptr) {
				obj3d->RecalculateCoordinatesInSpace(true);
				if (UpdateRenderingDataOfChildren_OnCameraCoordinatesUpdate)
					obj3d->UpdateVertexesAndTrianglesDataInRenderingData(true);
			}
		}
	}
}
const Vector3& TranslatorFrom3D::gCameraPosition() const {
	return CameraPosition;
}

Matrix TranslatorFrom3D::GetPositionInVirtualSpace(Vector3 positionInSpace) const {


	Vector2 RectHalfSize = Vector2(tanf(CameraViewAngle.gX() / 2), tanf(CameraViewAngle.gY() / 2));

	Vector3 positionInCameraCoordinates;
	{
		positionInCameraCoordinates = positionInSpace - CameraPosition;
		positionInCameraCoordinates = CameraRotation.gReversedMatrix() * positionInCameraCoordinates;
	}

	return Matrix(1, 4, {
		positionInCameraCoordinates.gX() / RectHalfSize.gX(),
		positionInCameraCoordinates.gY() / RectHalfSize.gY(),
		(2 * MinCameraDepth * MaxCameraDepth - positionInCameraCoordinates.gZ() * (MinCameraDepth + MaxCameraDepth)) / (MinCameraDepth - MaxCameraDepth),
		positionInCameraCoordinates.gZ() });
}

TranslatorFrom3D* TranslatorFrom3D::gThis() {
	return this;
}
TranslatorFrom3D::TranslatorFrom3D(ClassesMap* mapPtr) {
	instancePtr = mapPtr;

	instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->ParentChange.ConnectAfterEvent([this](ClassesMap* prevPar) {

		ClassesMap* foundRenderer = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->FindFirstParentWithModifier(Renderer::TypeIndex);
		RenderingParent = foundRenderer->FindTypePtr<Renderer>(Renderer::TypeIndex);
		});
}