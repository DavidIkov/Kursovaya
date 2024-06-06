
#include"Object2DData.h"
#include"UsefullStuff/InsertWithFilterInVector.h"
#include"OffScreenRenderingData.h"

const std::type_index Object2DData::TypeIndex = std::type_index(typeid(Object2DData));


float Object2DData::gPriority() const {
	return Priority;
}
void Object2DData::sPriority(float newPriority) {
	Priority = newPriority;
}

void Object2DData::RecalculateCoordinatesInSpace(bool recursive) {

	{//calculate actual cords
		Instance* inst = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex);
		if (inst->Parent != nullptr) {
			if (inst->Parent->IsContains(Object2DData::TypeIndex) and not inst->Parent->IsContains(TranslatorFrom2D::TypeIndex)) {
				Object2DData* parObj2d = inst->Parent->FindTypePtr<Object2DData>(Object2DData::TypeIndex);
				SizeInSpace = parObj2d->SizeInSpace * SizeFromParent.gSV() + SizeFromParent.gPV();
				RotationInSpace = Matrix(2, 2, { cosf(RotationFromParent),sinf(RotationFromParent),-sinf(RotationFromParent),cosf(RotationFromParent) }) * parObj2d->RotationInSpace;
				PositionInSpace = parObj2d->PositionInSpace + parObj2d->RotationInSpace * (PositionFromParent.gSV() * parObj2d->SizeInSpace + PositionFromParent.gPV());
			}
			else {
				const Vector2& resolution = Translator->gResolution();

				SizeInSpace = Vector2(resolution.gX(), resolution.gY()) * SizeFromParent.gSV() + SizeFromParent.gPV();
				RotationInSpace = Matrix(2, 2, { cosf(RotationFromParent),sinf(RotationFromParent),-sinf(RotationFromParent),cosf(RotationFromParent) });
				PositionInSpace = PositionFromParent.gSV() * Vector2(resolution.gX(), resolution.gY()) + PositionFromParent.gPV();
			}


		}
	}


	if (recursive) {
		//call update for children
		auto& children = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->gChildren();
		for (unsigned int i = 0; i < children.size(); i++) {
			if (children[i]->IsContains(TypeIndex)) {

				Object2DData* childObj2d = children[i]->FindTypePtr<Object2DData>(TypeIndex);

				//in best should happen just once but im too lazy to separate it... checks if child is not a child of different translator
				if (Translator != childObj2d->Translator) break;

				childObj2d->RecalculateCoordinatesInSpace(true);

			}
		}
	}

}
void Object2DData::UpdateVertexesDataInRenderingData(bool recursive) {

	if (Translator == nullptr) {
#if defined _DEBUG
		__debugbreak();//why would you call update when object dosent have translator???
#endif
		return;
	}
	
	{

		auto& translatorVertexesData = Translator->Objects[TranslatorObjectsIndex].VertexBufferData;
		unsigned int amountOfNumsInVertexData = VertexesTemporaryData_Layout[VertexesTemporaryData_Layout.size() - 1].EndOffset + VertexesNonTemporaryData_Layout[VertexesNonTemporaryData_Layout.size() - 1].EndOffset;
		translatorVertexesData.resize(VertexesRenderingOrder.size() * amountOfNumsInVertexData);


		std::vector<float/*2 values per position so its like vector2*/> vertexesScreenPositions; vertexesScreenPositions.resize(VertexesData.size() * 2);
		for (unsigned int i = 0; i < VertexesData.size(); i++) {
			Vector2 endPos = Translator->GetPositionInVirtualSpace(PositionInSpace + RotationInSpace * (SizeInSpace / 2 * Vector2(VertexesData[i][0], VertexesData[i][1])));
			vertexesScreenPositions[i * 2] = endPos.gX();
			vertexesScreenPositions[i * 2 + 1] = endPos.gY();
		}

		for (unsigned int i = 0; i < VertexesRenderingOrder.size(); i++) {
			translatorVertexesData[i * amountOfNumsInVertexData] = vertexesScreenPositions[VertexesRenderingOrder[i] * 2];
			translatorVertexesData[i * amountOfNumsInVertexData + 1] = vertexesScreenPositions[VertexesRenderingOrder[i] * 2 + 1];
			for (unsigned int vdi = 0; vdi < amountOfNumsInVertexData - 2; vdi++) {
				translatorVertexesData[i * amountOfNumsInVertexData + 2 + vdi] = VertexesData[VertexesRenderingOrder[i]][vdi];
			}
		}
	}

	if (recursive) {
		//call update for children
		auto& children = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->gChildren();
		for (unsigned int i = 0; i < children.size(); i++) {
			if (children[i]->IsContains(TypeIndex)) {

				Object2DData* childObj2d = children[i]->FindTypePtr<Object2DData>(TypeIndex);

				//in best should happen just once but im too lazy to separate it... checks if child is not a child of different translator
				if (Translator != childObj2d->Translator) break;

				childObj2d->UpdateVertexesDataInRenderingData(true);

			}
		}
	}
}

void Object2DData::AddTo_DependentObjectsAmount_ToParent(int add) {
	Object2DData* parentObj2d = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->Parent->FindTypePtr<Object2DData>(TypeIndex);
	if (parentObj2d != nullptr and parentObj2d->Translator == Translator) {
		parentObj2d->DependentObjectsAmount_InObjectsVector += add;
		parentObj2d->AddTo_DependentObjectsAmount_ToParent(add);
	}
}

void Object2DData::sShaderID(ShaderProgramID id) {

#if defined _DEBUG
	//////////////////////CHECKS IF SHADER IS COMPATIBLE
	auto program = ShaderProgramsManager::ShaderPrograms[id].ShaderProgramPtr;

	if (program->VertexBufferData.size() != VertexesTemporaryData_Layout.size() + VertexesNonTemporaryData_Layout.size()) __debugbreak();

	for (unsigned int i = 0; i < VertexesTemporaryData_Layout.size(); i++)
		if (program->VertexBufferData[i].Type != VertexesTemporaryData_Layout[i].Type) __debugbreak();

	for (unsigned int i = 0; i < VertexesNonTemporaryData_Layout.size(); i++)
		if (program->VertexBufferData[VertexesTemporaryData_Layout.size() + i].Type != VertexesNonTemporaryData_Layout[i].Type) __debugbreak();
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

void Object2DData::AddLayoutDataToVertexesTemporaryData_Layout(ShaderProgramDataTypes type, unsigned int len) {

	if (ObjectHasBeenInitialized) {
#if defined _DEBUG
		__debugbreak();//object has been initialized, you cant call this function
#endif
		return;
	}

	unsigned int sOff = (VertexesTemporaryData_Layout.size() == 0) ? 0 :
		VertexesTemporaryData_Layout[VertexesTemporaryData_Layout.size() - 1].EndOffset;
	VertexesTemporaryData_Layout.push_back({ "",type,
		sOff,sOff + len,
		len });
}
void Object2DData::AddLayoutDataToVertexesNonTemporaryData_Layout(std::string name, ShaderProgramDataTypes type, unsigned int len) {

	if (ObjectHasBeenInitialized) {
#if defined _DEBUG
		__debugbreak();//object has been initialized, you cant call this function
#endif
		return;
	}

	unsigned int sOff =(VertexesNonTemporaryData_Layout.size() == 0) ? 0 :
		VertexesNonTemporaryData_Layout[VertexesNonTemporaryData_Layout.size() - 1].EndOffset;
	VertexesNonTemporaryData_Layout.push_back({ name,type,
		sOff,sOff + len,
		len });
}

unsigned int Object2DData::AddVertex(const float(&& data)[]) {

	if (ObjectHasBeenInitialized) {
#if defined _DEBUG
		__debugbreak();//object has been initialized, you cant call this function
#endif
		return -1;
	}

	VertexesData.push_back({});
	VertexesData[VertexesData.size() - 1].resize(VertexesNonTemporaryData_Layout[VertexesNonTemporaryData_Layout.size() - 1].EndOffset);

	for (unsigned int i = 0; i < VertexesNonTemporaryData_Layout[VertexesNonTemporaryData_Layout.size() - 1].EndOffset; i++)
		VertexesData[VertexesData.size() - 1][i] = data[i];
	return VertexesData.size() - 1;
}

void Object2DData::SetVertexesParameterByIndex(unsigned int parameterInd, const float(&& data)[]) {
	Renderer::DataLayoutElement* parameterData = &VertexesNonTemporaryData_Layout[parameterInd];
	for (unsigned int vi = 0; vi < VertexesData.size(); vi++) {
		for (unsigned int oi = 0; oi < parameterData->Length; oi++) {
			VertexesData[vi][parameterData->StartOffset + oi] = data[oi];
		}
	}
}
void Object2DData::SetVertexesParameterByName(const std::string&& parameterName, const float(&& data)[]) { SetVertexesParameterByIndex(FindVertexDataIndByName(std::move(parameterName)), std::move(data)); }

void Object2DData::SetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd, const float(&& data)[]) {
	Renderer::DataLayoutElement* parameterData = &VertexesNonTemporaryData_Layout[parameterInd];
	for (unsigned int oi = 0; oi < parameterData->Length; oi++) {
		VertexesData[vertexInd][parameterData->StartOffset + oi] = data[oi];
	}
}
void Object2DData::SetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName, const float(&& data)[]) { SetVertexParameterByIndex(vertexInd, FindVertexDataIndByName(std::move(parameterName)), std::move(data)); }


std::vector<float> Object2DData::GetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd) const {
	unsigned int parameterOffset = VertexesNonTemporaryData_Layout[parameterInd].StartOffset;
	std::vector<float> retVec;
	retVec.resize(VertexesNonTemporaryData_Layout[parameterInd].Length);
	for (unsigned int i = 0; i < retVec.size(); i++)
		retVec[i] = VertexesData[vertexInd][parameterOffset + i];
	return retVec;
}
std::vector<float> Object2DData::GetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName) const { return GetVertexParameterByIndex(vertexInd, FindVertexDataIndByName(std::move(parameterName))); }

unsigned int Object2DData::FindVertexDataIndByName(const std::string&& name) const {
	for (unsigned int i = 0; i < VertexesNonTemporaryData_Layout.size(); i++) if (VertexesNonTemporaryData_Layout[i].Name == name) return i;
	__debugbreak();//name not found
	return -1;
}

Object2DData* Object2DData::gThis() {
	return this;
}
Object2DData::Object2DData(ClassesMap* mapPtr) {
	instancePtr = mapPtr;

	AddLayoutDataToVertexesTemporaryData_Layout(ShaderProgramDataTypes::vector2, 2);

	AddLayoutDataToVertexesNonTemporaryData_Layout("LocalPosition", ShaderProgramDataTypes::vector2, 2);
	AddLayoutDataToVertexesNonTemporaryData_Layout("Color", ShaderProgramDataTypes::vector4, 4);

	instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->ParentChange.ConnectBeforeEvent([this](ClassesMap* newPar) {

		if (not ObjectHasBeenInitialized) {
			InitializedRenderingDataLayout.reserve(VertexesTemporaryData_Layout.size() + VertexesNonTemporaryData_Layout.size());

			for (unsigned int i = 0; i < VertexesTemporaryData_Layout.size(); i++) {
				InitializedRenderingDataLayout.push_back(VertexesTemporaryData_Layout[i].Length);
			}
			for (unsigned int i = 0; i < VertexesNonTemporaryData_Layout.size(); i++) {
				InitializedRenderingDataLayout.push_back(VertexesNonTemporaryData_Layout[i].Length);
			}

		}
		ObjectHasBeenInitialized = true;

		RenderingParent = newPar->IsContains(Renderer::TypeIndex) ? newPar->FindTypePtr<Renderer>(Renderer::TypeIndex) :
			newPar->FindTypePtr<Instance>(Instance::TypeIndex)->FindFirstParentWithModifier(Renderer::TypeIndex)->FindTypePtr<Renderer>(Renderer::TypeIndex);
		if (RenderingParent != nullptr)
			RenderingObjectTypeInd = RenderingParent->GetTypeOfRenderingObject(InitializedRenderingDataLayout);

		if (Translator != nullptr) {
			Translator->RemoveFromObjects(instancePtr);
			Translator->DependentObjectsAmount -= DependentObjectsAmount_InObjectsVector;
			AddTo_DependentObjectsAmount_ToParent(-(int)DependentObjectsAmount_InObjectsVector);
		}


		if (newPar == nullptr) return;//no need to continue


		if (newPar->IsContains(TranslatorFrom2D::TypeIndex)) Translator = newPar->FindTypePtr<TranslatorFrom2D>(TranslatorFrom2D::TypeIndex);
		else Translator = newPar->FindTypePtr<Instance>(Instance::TypeIndex)->FindFirstParentWithModifier(TranslatorFrom2D::TypeIndex)->FindTypePtr<TranslatorFrom2D>(TranslatorFrom2D::TypeIndex);
		
		if (Translator != nullptr) {

			Translator->DependentObjectsAmount += DependentObjectsAmount_InObjectsVector;

			if (instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->Parent == nullptr)
				Translator->InsertInObjects(instancePtr,newPar);

			

		}

		});

	instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->ParentChange.ConnectAfterEvent([this](ClassesMap* prevPar) {

		if (Translator != nullptr) {
			if (Translator->RecalculateCoordinatesOfChildInSpace_OnChildParentUpdate) {
				RecalculateCoordinatesInSpace(true);
				if (Translator->UpdateRenderingDataOfChild_OnChildParentUpdate) UpdateVertexesDataInRenderingData(true);
			}
		}

		AddTo_DependentObjectsAmount_ToParent(DependentObjectsAmount_InObjectsVector);

		});

}

void Object2DData::sSize(const SPCS&& nSize) {
	SizeFromParent = nSize;
	if (Translator!=nullptr and Translator->RecalculateCoordinatesOfChildInSpace_OnChildCoordinatesUpdate) {
		RecalculateCoordinatesInSpace(true);
		if (Translator->UpdateRenderingDataOfChild_OnChildCoordinatesUpdate)
			UpdateVertexesDataInRenderingData(true);
	}
}
void Object2DData::sPosition(const SPCS&& nPos) {
	PositionFromParent = nPos;
	if (Translator != nullptr and Translator->RecalculateCoordinatesOfChildInSpace_OnChildCoordinatesUpdate) {
		RecalculateCoordinatesInSpace(true);
		if (Translator->UpdateRenderingDataOfChild_OnChildCoordinatesUpdate)
			UpdateVertexesDataInRenderingData(true);
	}
}
void Object2DData::sRotation(float nRot) {
	RotationFromParent = nRot;
	if (Translator != nullptr and Translator->RecalculateCoordinatesOfChildInSpace_OnChildCoordinatesUpdate) {
		RecalculateCoordinatesInSpace(true);
		if (Translator->UpdateRenderingDataOfChild_OnChildCoordinatesUpdate)
			UpdateVertexesDataInRenderingData(true);
	}
}

const SPCS& Object2DData::gSize() const {
	return SizeFromParent;
}
const SPCS& Object2DData::gPosition() const {
	return PositionFromParent;
}
float Object2DData::gRotation() const {
	return RotationFromParent;
}