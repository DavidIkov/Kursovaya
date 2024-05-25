#include"Object3DData.h"
#include"Instance.h"

const std::type_index Object3DData::TypeIndex = std::type_index(typeid(Object3DData));


unsigned int Object3DData::AddVertex(const float(&& data)[]) {

	if (ObjectHasBeenInitialized) {
#if defined _DEBUG
		__debugbreak();//object has been initialized, you cant call this function
#endif
		return -1;
	}

	VertexesData.push_back({ {data,data + VertexesDataLayout[VertexesDataLayout.size() - 1].EndOffset} });
	return VertexesData.size() - 1;
}

void Object3DData::sSize(const Vector3&& nSize) {
	Size = nSize;
	if (Translator != nullptr and Translator->AutomaticallyRecalculateActualCords) {
		RecalculateActualCords(true);
		if (Translator->AutomaticallyUpdateVertexesCordsInFilteredOrder)
			UpdateVertexesCordsInFilteredOrder(true);
	}
}
Vector3 Object3DData::gSize() const {
	return Size;
}
const Vector3& Object3DData::grSize() const {
	return Size;
}
void Object3DData::sPosition(const Vector3&& nPosition) {
	Position = nPosition;
	if (Translator != nullptr and Translator->AutomaticallyRecalculateActualCords) {
		RecalculateActualCords(true);
		if (Translator->AutomaticallyUpdateVertexesCordsInFilteredOrder)
			UpdateVertexesCordsInFilteredOrder(true);
	}
}
Vector3 Object3DData::gPosition() const {
	return Position;
}
const Vector3& Object3DData::grPosition() const {
	return Position;
}
void Object3DData::sRotation(const Matrix&& nRotaiton) {
	Rotation = nRotaiton;
	if (Translator != nullptr and Translator->AutomaticallyRecalculateActualCords) {
		RecalculateActualCords(true);
		if (Translator->AutomaticallyUpdateVertexesCordsInFilteredOrder)
			UpdateVertexesCordsInFilteredOrder(true);
	}
}
Matrix Object3DData::gRotation() const {
	return Rotation;
}
const Matrix& Object3DData::grRotation() const {
	return Rotation;
}

void Object3DData::RecalculateActualCords(bool recursive) {

	{//calculate actual cords
		Instance* inst = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex);
		if (inst->Parent != nullptr) {
			if (inst->Parent->IsContains(Object3DData::TypeIndex) and not inst->Parent->IsContains(TranslatorFrom3D::TypeIndex)) {
				Object3DData* parObj3d = inst->Parent->FindTypePtr<Object3DData>(Object3DData::TypeIndex);
				ActualPosition = parObj3d->ActualPosition + parObj3d->ActualRotation * ActualPosition;
				ActualRotation = parObj3d->ActualRotation * Rotation;
			}
			else {
				ActualPosition = Position;
				ActualRotation = Rotation;
			}

			//apply camera transforms
			////////////////////////////////////////////////////////TODO
			

		}
	}


	if (recursive) {
		//call update for children
		auto children = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->grChildren();
		for (unsigned int i = 0; i < children.size(); i++) {
			if (children[i]->IsContains(TypeIndex)) {

				Object3DData* childObj3d = children[i]->FindTypePtr<Object3DData>(TypeIndex);

				//in best should happen just once but im too lazy to separate it... checks if child is not a child of different translator
				if (Translator != childObj3d->Translator) break;

				childObj3d->RecalculateActualCords(true);

			}
		}
	}

}
void Object3DData::UpdateVertexesCordsInFilteredOrder(bool recursive) {

	if (Translator == nullptr) {
#if defined _DEBUG
		__debugbreak();//why would you call update when object dosent have translator???
#endif
		return;
	}

	{
		auto& vertexBufferData = Translator->Objects[TranslatorObjectsInd].VertexBufferData;
		unsigned int amountOfNumsPerVertexData = VertexesDataLayout[VertexesDataLayout.size() - 1].EndOffset + 3;
		unsigned int amountOfNumsPerTriangleData = TrianglesDataLayout[TrianglesDataLayout.size() - 1].EndOffset + 3;
		unsigned int amountOfNumsPerTriangleVertex = amountOfNumsPerVertexData + amountOfNumsPerTriangleData;
		vertexBufferData.resize(TrianglesData.size() * 3 * amountOfNumsPerTriangleVertex);

		std::vector<float/*3 values per position so its like vector3*/> vertexesScreenPositions; vertexesScreenPositions.resize(VertexesData.size() * 3);
		for (unsigned int i = 0; i < VertexesData.size(); i++) {
			Vector3 vertexPos = ActualPosition + ActualRotation * (Size / 2 * Vector3(VertexesData[i].Data[0], VertexesData[i].Data[1], VertexesData[i].Data[2]));
			Vector2 endPos = Translator->GetPositionOnWindow(vertexPos);
			vertexesScreenPositions[i * 3 ] = endPos.gX();
			vertexesScreenPositions[i * 3 + 1] = endPos.gY();
			vertexesScreenPositions[i * 3 + 2] = 1 - 1 / (1 + fabsf(vertexPos.gZ()));
		}

		{

			unsigned int offset = 0;
			for (unsigned int ti = 0; ti < TrianglesData.size(); ti++) {
				for (unsigned int vi = 0; vi < 3; vi++) {

					//apply vertex data
					unsigned int vertexInd = TrianglesData[ti].VertexesOrder[vi];
					vertexBufferData[offset] = vertexesScreenPositions[vertexInd * 3];
					vertexBufferData[offset + 1] = vertexesScreenPositions[vertexInd * 3 + 1];
					vertexBufferData[offset + 2] = vertexesScreenPositions[vertexInd * 3 + 2];
					for (unsigned int vdi = 0; vdi < amountOfNumsPerVertexData - 3; vdi++) {
						vertexBufferData[offset + 3 + vdi] = VertexesData[vertexInd].Data[vdi];
					}

					offset += amountOfNumsPerVertexData;

					//apply triangle data
					Vector3 rotatedNormal = ActualRotation * Vector3(TrianglesData[ti].Data[0], TrianglesData[ti].Data[1], TrianglesData[ti].Data[2]);
					vertexBufferData[offset] = rotatedNormal.gX();
					vertexBufferData[offset + 1] = rotatedNormal.gY();
					vertexBufferData[offset + 2] = rotatedNormal.gZ();
					for (unsigned int tdi = 0; tdi < amountOfNumsPerTriangleData - 3; tdi++) {
						vertexBufferData[offset + 3 + tdi] = TrianglesData[ti].Data[tdi];
					}

					offset += amountOfNumsPerTriangleData;
				}
			}
		}
	}

	if (recursive) {
		//call update for children
		auto children = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->grChildren();
		for (unsigned int i = 0; i < children.size(); i++) {
			if (children[i]->IsContains(TypeIndex)) {

				Object3DData* childObj3d = children[i]->FindTypePtr<Object3DData>(TypeIndex);

				//in best should happen just once but im too lazy to separate it... checks if child is not a child of different translator
				if (Translator != childObj3d->Translator) break;

				childObj3d->UpdateVertexesCordsInFilteredOrder(true);

			}
		}
	}
}

unsigned int Object3DData::MakeTriangleOutOfVertexes(unsigned int v1i, unsigned int v2i, unsigned int v3i, const float(&& data)[]) {

	if (ObjectHasBeenInitialized) {
#if defined _DEBUG
		__debugbreak();//object has been initialized, you cant call this function
#endif
		return -1;
	}

	TrianglesData.push_back({ {v1i,v2i,v3i},{data,data + TrianglesDataLayout[TrianglesDataLayout.size() - 1].EndOffset} });
	return TrianglesData.size() - 1;
}

void Object3DData::ApplyVertexTemplate(Vertex3DTemplates vertexTemplate, const float(&& data)[]) {

	if (ObjectHasBeenInitialized) {
#if defined _DEBUG
		__debugbreak();//object has been initialized, you cant call this function
#endif
		return;
	}

	VertexesData.clear();
	if (vertexTemplate == Vertex3DTemplates::Cube) {
		float scale = data[0];

		unsigned int rtf = AddVertex({scale,scale,scale,
			1.f, 1.f, 1.f, 1.f });
		unsigned int ltf= AddVertex({-scale, scale, scale,
			1.f, 1.f, 1.f, 1.f });
		unsigned int rbf = AddVertex({scale,-scale,scale,
			1.f, 1.f, 1.f, 1.f });
		unsigned int lbf = AddVertex({-scale,-scale,scale,
			1.f, 1.f, 1.f, 1.f });
		unsigned int rtb = AddVertex({scale,scale,-scale,
			1.f, 1.f, 1.f, 1.f });
		unsigned int ltb = AddVertex({-scale,scale,-scale,
			1.f, 1.f, 1.f, 1.f });
		unsigned int rbb = AddVertex({scale,-scale,-scale,
			1.f, 1.f, 1.f, 1.f });
		unsigned int lbb = AddVertex({-scale,-scale,-scale,
			1.f, 1.f, 1.f, 1.f });

		//front
		MakeTriangleOutOfVertexes(rtf, rbf, lbf, { 0, 0, 1 });
		MakeTriangleOutOfVertexes(lbf, ltf, rtf, { 0, 0, 1 });

		//back
		MakeTriangleOutOfVertexes(rtb, rbb, lbb, {0, 0, -1 });
		MakeTriangleOutOfVertexes(lbb, ltb, rtb, { 0, 0, -1 });

		//top
		MakeTriangleOutOfVertexes(rtf, ltf, rtb, {0, 1, 0 });
		MakeTriangleOutOfVertexes(ltf, ltb, rtb, {0, 1, 0,});

		//bottom
		MakeTriangleOutOfVertexes(rbf, lbf, rbb, {0, -1, 0 });
		MakeTriangleOutOfVertexes(lbf, lbb, rbb, {0, -1, 0 });

		//right
		MakeTriangleOutOfVertexes(rbf, rtf, rtb, {1, 0, 0 });
		MakeTriangleOutOfVertexes(rbf, rbb, rtb, {1, 0, 0 });

		//left
		MakeTriangleOutOfVertexes(lbf, ltf, ltb, {-1, 0, 0 });
		MakeTriangleOutOfVertexes(lbf, lbb, ltb, {-1, 0, 0 });

	}
	else if (vertexTemplate == Vertex3DTemplates::Parallelepiped) {
		__debugbreak();//should code this duh...
	}

	ApplyAddedVertexesChanges();
}

void Object3DData::SetVertexesParameterByIndex(unsigned int parameterInd, const float(&& data)[]) {
	Renderer::DataLayoutElement* parameterData = &VertexesDataLayout[parameterInd];
	for (unsigned int vi = 0; vi < VertexesData.size(); vi++) {
		for (unsigned int oi = 0; oi < parameterData->Length; oi++) {
			VertexesData[vi].Data[parameterData->StartOffset + oi] = data[oi];
		}
	}
}
void Object3DData::SetVertexesParameterByName(const std::string&& parameterName, const float(&& data)[]) { SetVertexesParameterByIndex(FindVertexDataIndByName(std::move(parameterName)), std::move(data)); }

void Object3DData::SetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd, const float(&& data)[]) {
	Renderer::DataLayoutElement* parameterData = &VertexesDataLayout[parameterInd];
	for (unsigned int oi = 0; oi < parameterData->Length; oi++) {
		VertexesData[vertexInd].Data[parameterData->StartOffset + oi] = data[oi];
	}
}
void Object3DData::SetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName, const float(&& data)[]) { SetVertexParameterByIndex(vertexInd, FindVertexDataIndByName(std::move(parameterName)), std::move(data)); }


std::vector<TranslatorFrom3D::VertexesDataElement> Object3DData::gVertexes() const {
	return VertexesData;
}
const std::vector<TranslatorFrom3D::VertexesDataElement>& Object3DData::grVertexes() const {
	return VertexesData;
}

std::vector<float> Object3DData::GetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd) const {
	unsigned int parameterOffset = VertexesDataLayout[parameterInd].StartOffset;
	std::vector<float> retVec;
	retVec.reserve(VertexesDataLayout[parameterInd].Length);
	for (unsigned int i = 0; i < retVec.size(); i++)
		retVec[i] = VertexesData[vertexInd].Data[parameterOffset + i];
	return retVec;
}
std::vector<float> Object3DData::GetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName) const { return GetVertexParameterByIndex(vertexInd, FindVertexDataIndByName(std::move(parameterName))); }

unsigned int Object3DData::FindVertexDataIndByName(const std::string&& name) const {
	for (unsigned int i = 0; i < VertexesDataLayout.size(); i++) if (VertexesDataLayout[i].Name == name) return i;
	__debugbreak();//name not found
	return -1;
}

void Object3DData::ApplyAddedVertexesChanges() {

	if (ObjectHasBeenInitialized) {
#if defined _DEBUG
		__debugbreak();//object has been initialized, you cant call this function
#endif
		return;
	}

	if (Translator != nullptr and Translator->AutomaticallyUpdateVertexesCordsInFilteredOrder)
		UpdateVertexesCordsInFilteredOrder(false);
}

void Object3DData::AddLayoutDataToVertexDataLayout(std::string name, ShaderProgramDataTypes type, unsigned int len) {

	if (ObjectHasBeenInitialized) {
#if defined _DEBUG
		__debugbreak();//object has been initialized, you cant call this function
#endif
		return;
	}

	unsigned int sOff = (VertexesDataLayout.size() == 0) ? 0 :
		(VertexesDataLayout[VertexesDataLayout.size() - 1].StartOffset + VertexesDataLayout[VertexesDataLayout.size() - 1].Length);
	VertexesDataLayout.push_back({ name,type,
		sOff,sOff + len,
		len });
	for (unsigned int i = 0; i < VertexesData.size(); i++) {
		VertexesData[i].Data.resize(VertexesData[i].Data.size() + len);
	}
}

void Object3DData::AddLayoutDataToTriangleDataLayout(std::string name, ShaderProgramDataTypes type, unsigned int len) {

	if (ObjectHasBeenInitialized) {
#if defined _DEBUG
		__debugbreak();//object has been initialized, you cant call this function
#endif
		return;
	}

	unsigned int sOff = (TrianglesDataLayout.size() == 0) ? 0 :
		(TrianglesDataLayout[TrianglesDataLayout.size() - 1].StartOffset + TrianglesDataLayout[TrianglesDataLayout.size() - 1].Length);
	TrianglesDataLayout.push_back({ name,type,
		sOff,sOff + len,
		len });
	for (unsigned int i = 0; i < TrianglesData.size(); i++) {
		TrianglesData[i].Data.resize(TrianglesData[i].Data.size() + len);
	}
}

void Object3DData::AddTo_DependentObjectsAmount_ToParent(int add) {
	Object3DData* parentObj3d = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->Parent->FindTypePtr<Object3DData>(TypeIndex);
	if (parentObj3d != nullptr and parentObj3d->Translator == Translator) {
		parentObj3d->DependentObjectsAmount += add;
		parentObj3d->AddTo_DependentObjectsAmount_ToParent(add);
	}
}

void Object3DData::sShaderID(ShaderProgramID id) {
#if defined _DEBUG
	//////////////////////CHECKS IF SHADER IS COMPATIBLE
	auto program = ShaderProgramsManager::ShaderPrograms[id].ShaderProgramPtr;

	if (program->VertexBufferData.size() != (VertexesDataLayout.size() + 1 + TrianglesDataLayout.size() + 1)) __debugbreak();
	
	if (program->VertexBufferData[0].Type != ShaderProgramDataTypes::vector3) __debugbreak();//check for screen pos
	if (program->VertexBufferData[VertexesDataLayout.size() + 1].Type != ShaderProgramDataTypes::vector3) __debugbreak();//check for normal

	for (unsigned int i = 0; i < VertexesDataLayout.size(); i++)
		if (program->VertexBufferData[i + 1].Type != VertexesDataLayout[i].Type) __debugbreak();
	for (unsigned int i = 0; i < TrianglesDataLayout.size(); i++)
		if (program->VertexBufferData[VertexesDataLayout.size() + 1 + 1 + i].Type != TrianglesDataLayout[i].Type) __debugbreak();
#endif

	ShaderID = id;
}

Object3DData* Object3DData::gThis() {
	return this;
}
Object3DData::Object3DData(ClassesMap* mapPtr) {
	instancePtr = mapPtr;

	AddLayoutDataToVertexDataLayout("LocalPosition", ShaderProgramDataTypes::vector3, 3);
	AddLayoutDataToVertexDataLayout("Color", ShaderProgramDataTypes::vector4, 4);

	AddLayoutDataToTriangleDataLayout("LocalNormalVector", ShaderProgramDataTypes::vector3, 3);
	

	instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->ParentChange.ConnectBeforeEvent([this](ClassesMap* newPar) {

		if (not ObjectHasBeenInitialized) {
			InitializedRenderingDataLayout.reserve(VertexesDataLayout.size() + 1 + TrianglesDataLayout.size() + 1);

			InitializedRenderingDataLayout.push_back(3);//position on screen
			for (unsigned int i = 0; i < VertexesDataLayout.size(); i++)
				InitializedRenderingDataLayout.push_back(VertexesDataLayout[i].Length);


			InitializedRenderingDataLayout.push_back(3);//space normal
			for (unsigned int i = 0; i < TrianglesDataLayout.size(); i++)
				InitializedRenderingDataLayout.push_back(TrianglesDataLayout[i].Length);
		}
		ObjectHasBeenInitialized = true;

		RenderingParent = newPar->IsContains(Renderer::TypeIndex) ? newPar->FindTypePtr<Renderer>(Renderer::TypeIndex) :
			newPar->FindTypePtr<Instance>(Instance::TypeIndex)->FindFirstParentWithModifier(Renderer::TypeIndex)->FindTypePtr<Renderer>(Renderer::TypeIndex);
		if (RenderingParent != nullptr)
			RenderingObjectTypeInd = RenderingParent->GetTypeOfRenderingObject(InitializedRenderingDataLayout);


		if (Translator != nullptr) {
			Translator->RemoveFromObjects(instancePtr);
			AddTo_DependentObjectsAmount_ToParent(-(int)DependentObjectsAmount);
		}


		if (newPar == nullptr) return;//no need to continue


		if (newPar->IsContains(TranslatorFrom3D::TypeIndex)) Translator = newPar->FindTypePtr<TranslatorFrom3D>(TranslatorFrom3D::TypeIndex);
		else Translator = newPar->FindTypePtr<Instance>(Instance::TypeIndex)->FindFirstParentWithModifier(TranslatorFrom3D::TypeIndex)->FindTypePtr<TranslatorFrom3D>(TranslatorFrom3D::TypeIndex);

		if (Translator != nullptr and instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->Parent == nullptr)
			Translator->InsertInObjects(instancePtr);
		

		});

	instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->ParentChange.ConnectAfterEvent([this](ClassesMap* prevPar) {

		if (Translator != nullptr) {
			if (Translator->AutomaticallyRecalculateActualCords) {
				RecalculateActualCords(true);
				if (Translator->AutomaticallyUpdateVertexesCordsInFilteredOrder) UpdateVertexesCordsInFilteredOrder(true);
			}
		}

		AddTo_DependentObjectsAmount_ToParent(DependentObjectsAmount);

		});
}