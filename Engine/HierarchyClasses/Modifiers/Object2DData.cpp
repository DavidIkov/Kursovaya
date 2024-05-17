
#include"Object2DData.h"
#include"UsefullStuff/InsertWithFilterInVector.h"
#include"TranslatorFrom2D.h"

const std::type_index Object2DData::TypeIndex = std::type_index(typeid(Object2DData));


float Object2DData::gPriority() const {
	return Priority;
}
const float& Object2DData::grPriority() const {
	return Priority;
}
void Object2DData::sPriority(float newPriority) {
	Priority = newPriority;
	if (Translator != nullptr) Translator->MoveInFilteredOrder(instancePtr);
}

void Object2DData::UpdateVertexesRenderingOrderInFilteredOrder(bool recursive) {

	if (Translator == nullptr) {
#if defined _DEBUG
		__debugbreak();//why would you call update when object dosent have translator???
#endif
		return;
	}


	Translator->FilteredOrder[TranslatorFilteredOrderInd].VertexesOrder.resize(VertexesRenderingOrder.size());
	for (unsigned int i = 0; i < VertexesRenderingOrder.size(); i++) {
		Translator->FilteredOrder[TranslatorFilteredOrderInd].VertexesOrder[i] = VertexesRenderingOrder[i];
	}


	if (recursive) return;

	//call update for children
	auto children = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->grChildren();
	for (unsigned int i = 0; i < children.size(); i++) {
		if (children[i]->IsContains(TypeIndex)) {

			Object2DData* childObj2d = children[i]->FindTypePtr<Object2DData>(TypeIndex);

			//in best should happen just once but im too lazy to separate it... checks if child is not a child of different translator
			if (Translator != childObj2d->Translator) break;

			childObj2d->UpdateVertexesRenderingOrderInFilteredOrder(true);

		}
	}
}
void Object2DData::RecalculateActualCords(bool recursive) {

	/////////////////////////////////NEED TO MAKE CAMERA WORK!!!!!!!!!!!!!!!!!!!!/////////////////////////////////////

	{//calculate actual cords
		Instance* inst = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex);
		if (inst->Parent != nullptr and inst->Parent->IsContains(Object2DData::TypeIndex) and not inst->Parent->IsContains(TranslatorFrom2D::TypeIndex)) {
			Object2DData* parObj2d = inst->Parent->FindTypePtr<Object2DData>(Object2DData::TypeIndex);
			ActualSize = parObj2d->ActualSize * Size.gSV() + Size.gPV();
			RotationMatrix = Matrix(2, 2, { cosf(Rotation),sinf(Rotation),-sinf(Rotation),cosf(Rotation) }) * parObj2d->RotationMatrix;
			ActualPosition = parObj2d->ActualPosition + parObj2d->RotationMatrix * (Position.gSV() * parObj2d->ActualSize + Position.gPV());
		}
		else {
			ActualSize = Vector2((float)Translator->Width, (float)Translator->Height) * Size.gSV() + Size.gPV();
			RotationMatrix = Matrix(2, 2, { cosf(Rotation),sinf(Rotation),-sinf(Rotation),cosf(Rotation) });
			ActualPosition = Position.gSV() * Vector2((float)Translator->Width, (float)Translator->Height) + Position.gPV();
		}
	}


	if (recursive) return;

	//call update for children
	auto children = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->grChildren();
	for (unsigned int i = 0; i < children.size(); i++) {
		if (children[i]->IsContains(TypeIndex)) {

			Object2DData* childObj2d = children[i]->FindTypePtr<Object2DData>(TypeIndex);

			//in best should happen just once but im too lazy to separate it... checks if child is not a child of different translator
			if (Translator != childObj2d->Translator) break;

			childObj2d->RecalculateActualCords(true);

		}
	}

}
void Object2DData::UpdateVertexesCordsInFilteredOrder(bool recursive) {

	if (Translator == nullptr) {
#if defined _DEBUG
		__debugbreak();//why would you call update when object dosent have translator???
#endif
		return;
	}

	{
		auto& translatorVertexesData = Translator->FilteredOrder[TranslatorFilteredOrderInd].VertexesData;
		unsigned int amountOfNumsInVertexData = 2 + VertexAditionalDataLayout[VertexAditionalDataLayout.size() - 1].EndOffset;
		translatorVertexesData.resize(VertexesData.size() * amountOfNumsInVertexData);
		for (unsigned int i = 0; i < VertexesData.size(); i++) {
			Vector2 endPos = Translator->GetPositionOnWindow(ActualPosition + RotationMatrix * (ActualSize / 2 * Vector2(VertexesData[i].AditionalData[0], VertexesData[i].AditionalData[1])));
			translatorVertexesData[amountOfNumsInVertexData * i] = endPos.gX();
			translatorVertexesData[amountOfNumsInVertexData * i + 1] = endPos.gY();
			for (unsigned int i2 = 0; i2 < amountOfNumsInVertexData - 2; i2++) {
				translatorVertexesData[amountOfNumsInVertexData * i + 2 + i2] = VertexesData[i].AditionalData[i2];
			}
		}
	}

	if (recursive) return;

	//call update for children
	auto children = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->grChildren();
	for (unsigned int i = 0; i < children.size(); i++) {
		if (children[i]->IsContains(TypeIndex)) {

			Object2DData* childObj2d = children[i]->FindTypePtr<Object2DData>(TypeIndex);

			//in best should happen just once but im too lazy to separate it... checks if child is not a child of different translator
			if (Translator != childObj2d->Translator) break;

			childObj2d->UpdateVertexesCordsInFilteredOrder(true);

		}
	}
}

void Object2DData::AddTo_DependentElementsAmount_ToParent(int add) {
	Object2DData* parentObj2d = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->Parent->FindTypePtr<Object2DData>(TypeIndex);
	if (parentObj2d != nullptr and parentObj2d->Translator == Translator) {
		parentObj2d->DependentElementsAmountInFilteredOrder += add;
		parentObj2d->AddTo_DependentElementsAmount_ToParent(add);
	}
}

bool Object2DData::ShaderCanBeApplyed(ShaderProgramID id) {
	auto program = ShaderProgramsManager::ShaderPrograms[id].ShaderProgramPtr;
	if (program->VertexBufferData.size() - 1 != VertexAditionalDataLayout.size()) return false;
	for (unsigned int i = 0; i < VertexAditionalDataLayout.size(); i++)
		if (program->VertexBufferData[i + 1].Type != VertexAditionalDataLayout[i].Type) return false;
	return true;
}

void Object2DData::sShaderID(ShaderProgramID id) {
#if defined _DEBUG
	if (not ShaderCanBeApplyed(id)) {
		__debugbreak();//wrong shader settings
		return;
	}
#endif
	ShaderID = id;
}

void Object2DData::ApplyVertexTemplate(VertexTemplateEnum vertexTemplate) {
	VertexesData.clear();
	if (vertexTemplate == Square) {
		AddVertex({ 1.f,1.f,
			1.f, 1.f, 1.f, 1.f });
		AddVertex({ -1.f,1.f,
			1.f, 1.f, 1.f, 1.f });
		AddVertex({ -1.f,-1.f,
			1.f, 1.f, 1.f, 1.f });
		AddVertex({ 1.f,-1.f,
			1.f, 1.f, 1.f, 1.f });
	}
	else if (vertexTemplate == Triangle) {
		AddVertex({ 0,1.f,
			1.f, 1.f, 1.f, 1.f });
		AddVertex({ -1.f,-1.f,
			1.f, 1.f, 1.f, 1.f });
		AddVertex({ 1.f,-1.f,
			1.f, 1.f, 1.f, 1.f });
	}

	UpdateVertexesRenderingOrder();
	if (Translator != nullptr)
		UpdateVertexesCordsInFilteredOrder(false);
}

void Object2DData::AddVertexAditionalDataLayout(std::string name, ShaderProgramTypeEnum type, unsigned int len) {
	unsigned int sOff =(VertexAditionalDataLayout.size() == 0) ? 0 :
		(VertexAditionalDataLayout[VertexAditionalDataLayout.size() - 1].StartOffset + VertexAditionalDataLayout[VertexAditionalDataLayout.size() - 1].Length);
	VertexAditionalDataLayout.push_back({ name,type,
		sOff,sOff + len,
		len });
	for (unsigned int i = 0; i < VertexesData.size(); i++) {
		VertexesData[i].AditionalData.resize(VertexesData[i].AditionalData.size() + len);
	}
}

void Object2DData::AddVertex(const float(&&data)[]) {
	VertexesData.push_back({ {0,0},{data,data + VertexAditionalDataLayout[VertexAditionalDataLayout.size() - 1].EndOffset} });
}

void Object2DData::UpdateVertexesRenderingOrder() {

	///////!!!!!!!!!!!!!!сейчас не работает для фигур у которых есть "углубления", надо сделать(сложна)!!!
	unsigned int vertexesAmount = VertexesData.size();
	if (vertexesAmount == 2) {
		VertexesRenderingOrder.assign(2, 0);
		VertexesRenderingOrder[0] = 0;
		VertexesRenderingOrder[1] = 1;
	}
	else if (vertexesAmount > 2) {
		VertexesRenderingOrder.assign(3, 0);
		VertexesRenderingOrder[0] = 0;
		VertexesRenderingOrder[1] = 1;
		VertexesRenderingOrder[2] = 2;
		for (unsigned int i = 3; i < vertexesAmount; i++) {
			unsigned int nextInd = (i == (vertexesAmount - 1)) ? 0 : (i + 1);
			VertexesRenderingOrder.push_back(i);
			VertexesRenderingOrder.push_back(i - 1);
			VertexesRenderingOrder.push_back(0);
		}
	}

	if (Translator != nullptr)
		UpdateVertexesRenderingOrderInFilteredOrder(false);
}

void Object2DData::SetVertexesParameterByIndex(unsigned int parameterInd, const float(&& data)[]) {
	Renderer::VertexAditionalDataLayoutElement* parameterData = &VertexAditionalDataLayout[parameterInd];
	for (unsigned int vi = 0; vi < VertexesData.size(); vi++) {
		for (unsigned int oi = 0; oi < parameterData->Length; oi++) {
			VertexesData[vi].AditionalData[parameterData->StartOffset + oi] = data[oi];
		}
	}
}
void Object2DData::SetVertexesParameterByName(const std::string&& parameterName, const float(&& data)[]) { SetVertexesParameterByIndex(FindVertexDataIndByName(std::move(parameterName)), std::move(data)); }

void Object2DData::SetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd, const float(&& data)[]) {
	Renderer::VertexAditionalDataLayoutElement* parameterData = &VertexAditionalDataLayout[parameterInd];
	for (unsigned int oi = 0; oi < parameterData->Length; oi++) {
		VertexesData[vertexInd].AditionalData[parameterData->StartOffset + oi] = data[oi];
	}
}
void Object2DData::SetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName, const float(&& data)[]) { SetVertexParameterByIndex(vertexInd, FindVertexDataIndByName(std::move(parameterName)), std::move(data)); }


std::vector<Renderer::VertexesDataElement> Object2DData::gVertexes() const {
	return VertexesData;
}
const std::vector<Renderer::VertexesDataElement>& Object2DData::grVertexes() const {
	return VertexesData;
}

std::vector<float> Object2DData::GetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd) const {
	unsigned int parameterOffset = VertexAditionalDataLayout[parameterInd].StartOffset;
	std::vector<float> retVec;
	retVec.reserve(VertexAditionalDataLayout[parameterInd].Length);
	for (unsigned int i = 0; i < retVec.size(); i++)
		retVec[i] = VertexesData[vertexInd].AditionalData[parameterOffset + i];
	return retVec;
}
std::vector<float> Object2DData::GetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName) const { return GetVertexParameterByIndex(vertexInd, FindVertexDataIndByName(std::move(parameterName))); }

unsigned int Object2DData::FindVertexDataIndByName(const std::string&& name) const {
	for (unsigned int i = 0; i < VertexAditionalDataLayout.size(); i++) if (VertexAditionalDataLayout[i].Name == name) return i;
	__debugbreak();//name not found
	return -1;
}

Object2DData* Object2DData::gThis() {
	return this;
}
Object2DData::Object2DData(ClassesMap* mapPtr) {
	instancePtr = mapPtr;

	AddVertexAditionalDataLayout("LocalPosition", ShaderProgramTypeEnum::vector2, 2);
	AddVertexAditionalDataLayout("Color", ShaderProgramTypeEnum::vector4, 4);

	instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->ParentChange.ConnectBeforeEvent([this](ClassesMap* newPar) {

		RenderingParent = newPar->IsContains(Renderer::TypeIndex) ? newPar->FindTypePtr<Renderer>(Renderer::TypeIndex) :
			newPar->FindTypePtr<Instance>(Instance::TypeIndex)->FindFirstParentWithModifier(Renderer::TypeIndex)->FindTypePtr<Renderer>(Renderer::TypeIndex);
		if (RenderingParent != nullptr)
			RenderingObjectTypeInd = RenderingParent->GetTypeOfRenderingObject(VertexAditionalDataLayout);

		if (Translator != nullptr) {
			Translator->RemoveFromFilteredOrder(instancePtr);
			Translator->DependentObjectsAmount -= DependentElementsAmountInFilteredOrder;
		}


		if (newPar == nullptr) return;//no need to continue


		if (newPar->IsContains(TranslatorFrom2D::TypeIndex)) Translator = newPar->FindTypePtr<TranslatorFrom2D>(TranslatorFrom2D::TypeIndex);
		else Translator = newPar->FindTypePtr<Instance>(Instance::TypeIndex)->FindFirstParentWithModifier(TranslatorFrom2D::TypeIndex)->FindTypePtr<TranslatorFrom2D>(TranslatorFrom2D::TypeIndex);
		
		if (Translator != nullptr) {

			Translator->DependentObjectsAmount += DependentElementsAmountInFilteredOrder;

			if (instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->Parent != nullptr)
				Translator->MoveInFilteredOrder(instancePtr);
			else
				Translator->InsertInFilteredOrder(instancePtr, newPar);

		}

		});

}

void Object2DData::sSize(const SPCS&& nSize) {
	Size = nSize;
	RecalculateActualCords(true);
	UpdateVertexesCordsInFilteredOrder(true);
}
void Object2DData::sPosition(const SPCS&& nPos) {
	Position = nPos;
	RecalculateActualCords(true);
	UpdateVertexesCordsInFilteredOrder(true);
}
void Object2DData::sRotation(float nRot) {
	Rotation = nRot;
	RecalculateActualCords(true);
	UpdateVertexesCordsInFilteredOrder(true);
}

const SPCS& Object2DData::grSize() const {
	return Size;
}
SPCS Object2DData::gSize() const {
	return Size;
}
const SPCS& Object2DData::grPosition() const {
	return Position;
}
SPCS Object2DData::gPosition() const {
	return Position;
}
const float& Object2DData::grRotation() const {
	return Rotation;
}
float Object2DData::gRotation() const {
	return Rotation;
}