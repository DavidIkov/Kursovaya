
#include"Object2DData.h"
#include"UsefullStuff/InsertWithFilterInVector.h"

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

void Object2DData::RecalculateActualCords(bool recursive) {

	{//calculate actual cords
		Instance* inst = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex);
		if (inst->Parent != nullptr) {
			if (inst->Parent->IsContains(Object2DData::TypeIndex) and not inst->Parent->IsContains(TranslatorFrom2D::TypeIndex)) {
				Object2DData* parObj2d = inst->Parent->FindTypePtr<Object2DData>(Object2DData::TypeIndex);
				ActualSize = parObj2d->ActualSize * Size.grSV() + Size.grPV();
				RotationMatrix = Matrix(2, 2, { cosf(Rotation),sinf(Rotation),-sinf(Rotation),cosf(Rotation) }) * parObj2d->RotationMatrix;
				ActualPosition = parObj2d->ActualPosition + parObj2d->RotationMatrix * (Position.grSV() * parObj2d->ActualSize + Position.grPV());
			}
			else {
				ActualSize = Vector2((float)Translator->Width, (float)Translator->Height) * Size.grSV() + Size.grPV();
				RotationMatrix = Matrix(2, 2, { cosf(Rotation),sinf(Rotation),-sinf(Rotation),cosf(Rotation) });
				ActualPosition = Position.grSV() * Vector2((float)Translator->Width, (float)Translator->Height) + Position.grPV();
			}

			//apply camera transforms

			//apply position offset
			ActualPosition = ActualPosition - Translator->ActualCameraPosition;

			//apply rotation
			{
				RotationMatrix = Translator->ActualCameraRotationMatrix.grReversedMatrix() * RotationMatrix;
				ActualPosition = Vector2(RotationMatrix * ActualPosition);
			}

			//apply size
			{
				Vector2 frac(Translator->ActualCameraSize.gX() / (float)Translator->Width, Translator->ActualCameraSize.gY() / (float)Translator->Height);
				ActualPosition = ActualPosition / frac;
				ActualSize = ActualSize / frac;
			}

		}
	}


	if (recursive) {
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

}
void Object2DData::UpdateVertexesCordsInFilteredOrder(bool recursive) {

	if (Translator == nullptr) {
#if defined _DEBUG
		__debugbreak();//why would you call update when object dosent have translator???
#endif
		return;
	}

	{

		auto& translatorVertexesData = Translator->FilteredOrder[TranslatorFilteredOrderInd].VertexBufferData;
		unsigned int amountOfNumsInVertexData = VertexesDataLayout[VertexesDataLayout.size() - 1].EndOffset + 2;
		translatorVertexesData.resize(VertexesRenderingOrder.size() * amountOfNumsInVertexData);


		std::vector<float/*2 values per position so its like vector2*/> vertexesScreenPositions; vertexesScreenPositions.resize(VertexesData.size() * 2);
		for (unsigned int i = 0; i < VertexesData.size(); i++) {
			Vector2 endPos = Translator->GetPositionOnWindow(ActualPosition + RotationMatrix * (ActualSize / 2 * Vector2(VertexesData[i].Data[0], VertexesData[i].Data[1])));
			vertexesScreenPositions[i * 2] = endPos.gX();
			vertexesScreenPositions[i * 2 + 1] = endPos.gY();
		}

		for (unsigned int i = 0; i < VertexesRenderingOrder.size(); i++) {
			translatorVertexesData[i * amountOfNumsInVertexData] = vertexesScreenPositions[VertexesRenderingOrder[i] * 2];
			translatorVertexesData[i * amountOfNumsInVertexData + 1] = vertexesScreenPositions[VertexesRenderingOrder[i] * 2 + 1];
			for (unsigned int vdi = 0; vdi < amountOfNumsInVertexData - 2; vdi++) {
				translatorVertexesData[i * amountOfNumsInVertexData + 2 + vdi] = VertexesData[VertexesRenderingOrder[i]].Data[vdi];
			}
		}
	}

	if (recursive) {
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
}

void Object2DData::AddTo_DependentElementsAmount_ToParent(int add) {
	Object2DData* parentObj2d = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->Parent->FindTypePtr<Object2DData>(TypeIndex);
	if (parentObj2d != nullptr and parentObj2d->Translator == Translator) {
		parentObj2d->DependentElementsAmountInFilteredOrder += add;
		parentObj2d->AddTo_DependentElementsAmount_ToParent(add);
	}
}

void Object2DData::sShaderID(ShaderProgramID id) {

#if defined _DEBUG
	//////////////////////CHECKS IF SHADER IS COMPATIBLE
	auto program = ShaderProgramsManager::ShaderPrograms[id].ShaderProgramPtr;

	if (program->VertexBufferData.size() != VertexesDataLayout.size() + 1) __debugbreak();

	if (program->VertexBufferData[0].Type != ShaderProgramDataTypes::vector2) __debugbreak();//check for screen pos

	for (unsigned int i = 0; i < VertexesDataLayout.size(); i++)
		if (program->VertexBufferData[i + 1].Type != VertexesDataLayout[i].Type) __debugbreak();
#endif

	ShaderID = id;
}

void Object2DData::MakeTriangleOutOfVertexes(unsigned int i1, unsigned int i2, unsigned int i3) {
	VertexesRenderingOrder.reserve(VertexesRenderingOrder.size() + 3);
	VertexesRenderingOrder.push_back(i1);
	VertexesRenderingOrder.push_back(i2);
	VertexesRenderingOrder.push_back(i3);
}

void Object2DData::ApplyVertexTemplate(Vertex2DTemplates vertexTemplate) {
	VertexesData.clear();
	if (vertexTemplate == Vertex2DTemplates::Square) {
		unsigned int tr = AddVertex( {1.f,1.f,
			1.f, 1.f, 1.f, 1.f });
		unsigned int tl = AddVertex({ -1.f,1.f,
			1.f, 1.f, 1.f, 1.f });
		unsigned int lb = AddVertex({ -1.f,-1.f,
			1.f, 1.f, 1.f, 1.f });
		unsigned int rb = AddVertex( {1.f,-1.f,
			1.f, 1.f, 1.f, 1.f });

		MakeTriangleOutOfVertexes(tr, tl, lb);
		MakeTriangleOutOfVertexes(lb, rb, tr);
	}
	else if (vertexTemplate == Vertex2DTemplates::Triangle) {
		unsigned int tm = AddVertex( {0,1.f,
			1.f, 1.f, 1.f, 1.f });
		unsigned int lb = AddVertex( {-1.f,-1.f,
			1.f, 1.f, 1.f, 1.f });
		unsigned int rb = AddVertex( {1.f,-1.f,
			1.f, 1.f, 1.f, 1.f });

		MakeTriangleOutOfVertexes(tm, lb, rb);
	}
}

void Object2DData::AddLayoutDataToVertexesDataLayout(std::string name, ShaderProgramDataTypes type, unsigned int len) {

	unsigned int sOff =(VertexesDataLayout.size() == 0) ? 0 :
		(VertexesDataLayout[VertexesDataLayout.size() - 1].StartOffset + VertexesDataLayout[VertexesDataLayout.size() - 1].Length);
	VertexesDataLayout.push_back({ name,type,
		sOff,sOff + len,
		len });
	for (unsigned int i = 0; i < VertexesData.size(); i++) {
		VertexesData[i].Data.resize(VertexesData[i].Data.size() + len);
	}
}

unsigned int Object2DData::AddVertex(const float(&& data)[]) {
	VertexesData.push_back({ {data,data + VertexesDataLayout[VertexesDataLayout.size() - 1].EndOffset} });
	return VertexesData.size() - 1;
}

void Object2DData::SetVertexesParameterByIndex(unsigned int parameterInd, const float(&& data)[]) { 
	Renderer::DataLayoutElement* parameterData = &VertexesDataLayout[parameterInd];
	for (unsigned int vi = 0; vi < VertexesData.size(); vi++) {
		for (unsigned int oi = 0; oi < parameterData->Length; oi++) {
			VertexesData[vi].Data[parameterData->StartOffset + oi] = data[oi];
		}
	}
}
void Object2DData::SetVertexesParameterByName(const std::string&& parameterName, const float(&& data)[]) { SetVertexesParameterByIndex(FindVertexDataIndByName(std::move(parameterName)), std::move(data)); }

void Object2DData::SetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd, const float(&& data)[]) {
	Renderer::DataLayoutElement* parameterData = &VertexesDataLayout[parameterInd];
	for (unsigned int oi = 0; oi < parameterData->Length; oi++) {
		VertexesData[vertexInd].Data[parameterData->StartOffset + oi] = data[oi];
	}
}
void Object2DData::SetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName, const float(&& data)[]) { SetVertexParameterByIndex(vertexInd, FindVertexDataIndByName(std::move(parameterName)), std::move(data)); }


std::vector<TranslatorFrom2D::VertexesDataElement> Object2DData::gVertexes() const {
	return VertexesData;
}
const std::vector<TranslatorFrom2D::VertexesDataElement>& Object2DData::grVertexes() const {
	return VertexesData;
}

std::vector<float> Object2DData::GetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd) const {
	unsigned int parameterOffset = VertexesDataLayout[parameterInd].StartOffset;
	std::vector<float> retVec;
	retVec.reserve(VertexesDataLayout[parameterInd].Length);
	for (unsigned int i = 0; i < retVec.size(); i++)
		retVec[i] = VertexesData[vertexInd].Data[parameterOffset + i];
	return retVec;
}
std::vector<float> Object2DData::GetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName) const { return GetVertexParameterByIndex(vertexInd, FindVertexDataIndByName(std::move(parameterName))); }

unsigned int Object2DData::FindVertexDataIndByName(const std::string&& name) const {
	for (unsigned int i = 0; i < VertexesDataLayout.size(); i++) if (VertexesDataLayout[i].Name == name) return i;
	__debugbreak();//name not found
	return -1;
}

Object2DData* Object2DData::gThis() {
	return this;
}
Object2DData::Object2DData(ClassesMap* mapPtr) {
	instancePtr = mapPtr;

	AddLayoutDataToVertexesDataLayout("LocalPosition", ShaderProgramDataTypes::vector2, 2);
	AddLayoutDataToVertexesDataLayout("Color", ShaderProgramDataTypes::vector4, 4);

	instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->ParentChange.ConnectBeforeEvent([this](ClassesMap* newPar) {

		if (not ObjectHasBeenInitialized) {
			InitializedRenderingDataLayout.reserve(VertexesDataLayout.size() + 1);

			InitializedRenderingDataLayout.push_back(2);//position on screen
			for (unsigned int i = 0; i < VertexesDataLayout.size(); i++) {
				InitializedRenderingDataLayout.push_back(VertexesDataLayout[i].Length);
			}

		}
		ObjectHasBeenInitialized = true;

		RenderingParent = newPar->IsContains(Renderer::TypeIndex) ? newPar->FindTypePtr<Renderer>(Renderer::TypeIndex) :
			newPar->FindTypePtr<Instance>(Instance::TypeIndex)->FindFirstParentWithModifier(Renderer::TypeIndex)->FindTypePtr<Renderer>(Renderer::TypeIndex);
		if (RenderingParent != nullptr)
			RenderingObjectTypeInd = RenderingParent->GetTypeOfRenderingObject(InitializedRenderingDataLayout);

		if (Translator != nullptr) {
			Translator->RemoveFromFilteredOrder(instancePtr);
			Translator->DependentObjectsAmount -= DependentElementsAmountInFilteredOrder;
			AddTo_DependentElementsAmount_ToParent(-(int)DependentElementsAmountInFilteredOrder);
		}


		if (newPar == nullptr) return;//no need to continue


		if (newPar->IsContains(TranslatorFrom2D::TypeIndex)) Translator = newPar->FindTypePtr<TranslatorFrom2D>(TranslatorFrom2D::TypeIndex);
		else Translator = newPar->FindTypePtr<Instance>(Instance::TypeIndex)->FindFirstParentWithModifier(TranslatorFrom2D::TypeIndex)->FindTypePtr<TranslatorFrom2D>(TranslatorFrom2D::TypeIndex);
		
		if (Translator != nullptr) {

			Translator->DependentObjectsAmount += DependentElementsAmountInFilteredOrder;

			if (instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->Parent != nullptr)
				Translator->MoveInFilteredOrder(instancePtr);
			else {
				Translator->InsertInFilteredOrder(instancePtr, newPar);

			}

		}

		});

	instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->ParentChange.ConnectAfterEvent([this](ClassesMap* prevPar) {

		if (Translator != nullptr) {
			if (Translator->AutomaticallyRecalculateActualCords) {
				RecalculateActualCords(true);
				if (Translator->AutomaticallyUpdateVertexesCordsInFilteredOrder) UpdateVertexesCordsInFilteredOrder(true);
			}
		}

		AddTo_DependentElementsAmount_ToParent(DependentElementsAmountInFilteredOrder);

		});

}

void Object2DData::sSize(const SPCS&& nSize) {
	Size = nSize;
	if (Translator!=nullptr and Translator->AutomaticallyRecalculateActualCords) {
		RecalculateActualCords(true);
		if (Translator->AutomaticallyUpdateVertexesCordsInFilteredOrder)
			UpdateVertexesCordsInFilteredOrder(true);
	}
}
void Object2DData::sPosition(const SPCS&& nPos) {
	Position = nPos;
	if (Translator != nullptr and Translator->AutomaticallyRecalculateActualCords) {
		RecalculateActualCords(true);
		if (Translator->AutomaticallyUpdateVertexesCordsInFilteredOrder)
			UpdateVertexesCordsInFilteredOrder(true);
	}
}
void Object2DData::sRotation(float nRot) {
	Rotation = nRot;
	if (Translator != nullptr and Translator->AutomaticallyRecalculateActualCords) {
		RecalculateActualCords(true);
		if (Translator->AutomaticallyUpdateVertexesCordsInFilteredOrder)
			UpdateVertexesCordsInFilteredOrder(true);
	}
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