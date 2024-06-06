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

	VertexesData.push_back({});
	VertexesData[VertexesData.size() - 1].resize(VertexesNonTemporaryData_Layout[VertexesNonTemporaryData_Layout.size() - 1].EndOffset);

	for (unsigned int i = 0; i < VertexesNonTemporaryData_Layout[VertexesNonTemporaryData_Layout.size() - 1].EndOffset; i++)
		VertexesData[VertexesData.size() - 1][i] = data[i];

	return VertexesData.size() - 1;
}

void Object3DData::sSize(const Vector3&& nSize) {
	Size = nSize;
	if (Translator != nullptr and Translator->RecalculateCoordinatesOfChildInSpace_OnChildCoordinatesUpdate) {
		RecalculateCoordinatesInSpace(true);
		if (Translator->UpdateRenderingDataOfChild_OnChildCoordinatesUpdate)
			UpdateVertexesAndTrianglesDataInRenderingData(true);
	}
}
const Vector3& Object3DData::gSize() const {
	return Size;
}

void Object3DData::sPositionFromParent(const Vector3&& nPosition) {
	PositionFromParent = nPosition;
	if (Translator != nullptr and Translator->RecalculateCoordinatesOfChildInSpace_OnChildCoordinatesUpdate) {
		RecalculateCoordinatesInSpace(true);
		if (Translator->UpdateRenderingDataOfChild_OnChildCoordinatesUpdate)
			UpdateVertexesAndTrianglesDataInRenderingData(true);
	}
}
const Vector3& Object3DData::gPositionFromParent() const {
	return PositionFromParent;
}
void Object3DData::sRotationFromParent(const Matrix&& nRotaiton) {
	RotationFromParent = nRotaiton;
	if (Translator != nullptr and Translator->RecalculateCoordinatesOfChildInSpace_OnChildCoordinatesUpdate) {
		RecalculateCoordinatesInSpace(true);
		if (Translator->UpdateRenderingDataOfChild_OnChildCoordinatesUpdate)
			UpdateVertexesAndTrianglesDataInRenderingData(true);
	}
}
const Matrix& Object3DData::gRotationFromParent() const {
	return RotationFromParent;
}


const Vector3& Object3DData::gPositionInSpace() const {
	return PositionInSpace;
}
const Matrix& Object3DData::gRotationInSpace() const {
	return RotationInSpace;
}

void Object3DData::RecalculateCoordinatesInSpace(bool recursive) {

	{//calculate actual cords
		Instance* inst = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex);
		if (inst->Parent != nullptr) {
			if (inst->Parent->IsContains(Object3DData::TypeIndex) and not inst->Parent->IsContains(TranslatorFrom3D::TypeIndex)) {
				Object3DData* parObj3d = inst->Parent->FindTypePtr<Object3DData>(Object3DData::TypeIndex);
				PositionInSpace = parObj3d->PositionInSpace + parObj3d->RotationInSpace * PositionFromParent;
				RotationInSpace = parObj3d->RotationInSpace * RotationFromParent;
			}
			else {
				PositionInSpace = PositionFromParent;
				RotationInSpace = RotationFromParent;
			}
		}
	}


	if (recursive) {
		//call update for children
		auto& children = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->gChildren();
		for (unsigned int i = 0; i < children.size(); i++) {
			if (children[i]->IsContains(TypeIndex)) {

				Object3DData* childObj3d = children[i]->FindTypePtr<Object3DData>(TypeIndex);

				//in best should happen just once but im too lazy to separate it... checks if child is not a child of different translator
				if (Translator != childObj3d->Translator) break;

				childObj3d->RecalculateCoordinatesInSpace(true);

			}
		}
	}

}
void Object3DData::UpdateVertexesAndTrianglesDataInRenderingData(bool recursive) {

	if (Translator == nullptr) {
#if defined _DEBUG
		__debugbreak();//why would you call update when object dosent have translator???
#endif
		return;
	}

	{
		auto& vertexBufferData = Translator->Objects[TranslatorObjectsIndex].VertexBufferData;
		unsigned int amountOfNumsPerVertexTemporaryData = VertexesTemporaryData_Layout[VertexesTemporaryData_Layout.size() - 1].EndOffset;
		unsigned int amountOfNumsPerVertexNotTemporaryData = VertexesNonTemporaryData_Layout[VertexesNonTemporaryData_Layout.size() - 1].EndOffset;
		unsigned int amountOfNumsPerTriangleTemporaryData = TrianglesTemporaryData_Layout[TrianglesTemporaryData_Layout.size() - 1].EndOffset;
		unsigned int amountOfNumsPerTriangleNotTemporaryData = TrianglesNonTemporaryData_Layout[TrianglesNonTemporaryData_Layout.size() - 1].EndOffset;

		unsigned int amountOfNumsPerVertexData = amountOfNumsPerVertexTemporaryData + amountOfNumsPerVertexNotTemporaryData;
		unsigned int amountOfNumsPerTriangleData = amountOfNumsPerTriangleTemporaryData + amountOfNumsPerTriangleNotTemporaryData;
		unsigned int amountOfNumsPerTriangleVertex = amountOfNumsPerVertexData + amountOfNumsPerTriangleData;

		vertexBufferData.resize(TrianglesData.size() * 3 * amountOfNumsPerTriangleVertex);

		std::vector<float> vertexesTemporaryData; vertexesTemporaryData.resize(VertexesData.size() * amountOfNumsPerVertexTemporaryData);
		for (unsigned int i = 0; i < VertexesData.size(); i++) {
			Vector3 posInSpace = PositionInSpace + RotationInSpace * (Size / 2 * Vector3(VertexesData[i][0], VertexesData[i][1], VertexesData[i][2]));
			Matrix screenPosition = Translator->GetPositionInVirtualSpace(posInSpace);

			for (unsigned int di = 0; di < 4; di++)
				vertexesTemporaryData[amountOfNumsPerVertexTemporaryData * i + di] = screenPosition[di];

			vertexesTemporaryData[amountOfNumsPerVertexTemporaryData * i + 4] = posInSpace.gX();
			vertexesTemporaryData[amountOfNumsPerVertexTemporaryData * i + 4 + 1] = posInSpace.gY();
			vertexesTemporaryData[amountOfNumsPerVertexTemporaryData * i + 4 + 2] = posInSpace.gZ();

		}

		std::vector<float> trianglesTemporaryData; trianglesTemporaryData.resize(TrianglesData.size() * amountOfNumsPerTriangleTemporaryData);
		for (unsigned int i = 0; i < TrianglesData.size(); i++) {
			Vector3 rotatedNormal = RotationInSpace * Vector3(TrianglesData[i].Data[0], TrianglesData[i].Data[1], TrianglesData[i].Data[2]);

			trianglesTemporaryData[i * amountOfNumsPerTriangleTemporaryData] = rotatedNormal.gX();
			trianglesTemporaryData[i * amountOfNumsPerTriangleTemporaryData + 1] = rotatedNormal.gY();
			trianglesTemporaryData[i * amountOfNumsPerTriangleTemporaryData + 2] = rotatedNormal.gZ();

		}

		{

			unsigned int offset = 0;
			for (unsigned int ti = 0; ti < TrianglesData.size(); ti++) {
				for (unsigned int vi = 0; vi < 3; vi++) {


					//apply vertex data
					unsigned int vertexInd = TrianglesData[ti].VertexesOrder[vi];
					for (unsigned int di = 0; di < amountOfNumsPerVertexTemporaryData; di++)
						vertexBufferData[offset + di] = vertexesTemporaryData[vertexInd * amountOfNumsPerVertexTemporaryData + di];
					for (unsigned int di = 0; di < amountOfNumsPerVertexNotTemporaryData; di++)
						vertexBufferData[offset + amountOfNumsPerVertexTemporaryData + di] = VertexesData[vertexInd][di];

					offset += amountOfNumsPerVertexData;


					//apply triangle data
					for (unsigned int di = 0; di < amountOfNumsPerTriangleTemporaryData; di++)
						vertexBufferData[offset + di] = trianglesTemporaryData[ti * amountOfNumsPerTriangleTemporaryData + di];
					for (unsigned int di = 0; di < amountOfNumsPerTriangleNotTemporaryData; di++)
						vertexBufferData[offset + amountOfNumsPerTriangleTemporaryData + di] = TrianglesData[ti].Data[di];

					offset += amountOfNumsPerTriangleData;
				}
			}
		}
	}

	if (recursive) {
		//call update for children
		auto& children = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->gChildren();
		for (unsigned int i = 0; i < children.size(); i++) {
			if (children[i]->IsContains(TypeIndex)) {

				Object3DData* childObj3d = children[i]->FindTypePtr<Object3DData>(TypeIndex);

				//in best should happen just once but im too lazy to separate it... checks if child is not a child of different translator
				if (Translator != childObj3d->Translator) break;

				childObj3d->UpdateVertexesAndTrianglesDataInRenderingData(true);

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

	TrianglesData.push_back({ {v1i,v2i,v3i},{} });
	TrianglesData[TrianglesData.size() - 1].Data.resize(TrianglesNonTemporaryData_Layout[TrianglesNonTemporaryData_Layout.size() - 1].EndOffset);

	for (unsigned int i = 0; i < TrianglesNonTemporaryData_Layout[TrianglesNonTemporaryData_Layout.size() - 1].EndOffset; i++)
		TrianglesData[TrianglesData.size() - 1].Data[i] = data[i];

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
	else if (vertexTemplate == Vertex3DTemplates::Triangle) {
		float scale = data[0];

		unsigned int mt = AddVertex({ 0, scale, 0,
			1.f, 1.f, 1.f, 1.f });
		unsigned int rb = AddVertex({ scale,-scale,0,
			1.f, 1.f, 1.f, 1.f });
		unsigned int lb = AddVertex({ -scale,-scale,0,
			1.f, 1.f, 1.f, 1.f });
		MakeTriangleOutOfVertexes(rb,mt,lb, { 0,0,1 });
	}
	else if (vertexTemplate == Vertex3DTemplates::Sphere) {
		float scale = data[0];
		unsigned int segmentsByRotation = (unsigned int)data[1];
		unsigned int segmentsY = (unsigned int)data[2];

		if (segmentsY < 2 or segmentsByRotation == 0) {
#if defined _DEBUG
			__debugbreak();
#endif
			return;
		}

		unsigned int top = AddVertex({ 0,scale,0,
			1.f, 1.f, 1.f, 1.f });
		unsigned int bottom = AddVertex({ 0, -scale, 0,
			1.f, 1.f, 1.f, 1.f });


#define makeTriangle(i1,i2,i3) {\
		Vector3 p1(VertexesData[i1][0], VertexesData[i1][1], VertexesData[i1][2]);\
		Vector3 p2(VertexesData[i2][0], VertexesData[i2][1], VertexesData[i2][2]);\
		Vector3 p3(VertexesData[i3][0], VertexesData[i3][1], VertexesData[i3][2]);\
		Vector3 cross = (p2 - p1).Unit().Cross((p3 - p1).Unit());\
		MakeTriangleOutOfVertexes(i1, i2, i3, { cross.gX(),cross.gY(),cross.gZ() });};

		for (unsigned int segmentInd = 1; segmentInd < segmentsY - 1; segmentInd++) {
			float height = scale * sinf(3.14f / 2 * (1 - 2 * (float)segmentInd / (float)(segmentsY - 1)));

			float radius = sqrtf(scale * scale - height * height);
			unsigned int rv = AddVertex({ radius,height,0,
				1.f, 1.f, 1.f, 1.f });
			unsigned int prevVert = rv;
			for (unsigned int rotInd = 1; rotInd < segmentsByRotation; rotInd++) {
				float angle = 3.14f * 2 * rotInd / segmentsByRotation;
				unsigned int nv = AddVertex({ cosf(angle) * radius,height,sinf(angle) * radius,
				1.f, 1.f, 1.f, 1.f });

				unsigned int connector = top;
				if (segmentInd != 1) connector = VertexesData.size() - segmentsByRotation - 1;

				makeTriangle(connector, nv, prevVert);

				if (connector != top) {
					makeTriangle(connector - 1, connector, prevVert);
				}
				prevVert = nv;
			}

			if (segmentInd == 1) {
				makeTriangle(rv, top, VertexesData.size() - 1);
			}
			else {
				makeTriangle(rv, VertexesData.size() - 1, VertexesData.size() - 2 * segmentsByRotation);
				makeTriangle(VertexesData.size() - 2 * segmentsByRotation, VertexesData.size() - 1, VertexesData.size() - 1 - segmentsByRotation);
			}
		}

		for (unsigned int rotInd = 0; rotInd < segmentsByRotation; rotInd++) {
			unsigned int topline = VertexesData.size() - segmentsByRotation + rotInd;
			unsigned int toplinePrev = (rotInd == 0) ? (topline + segmentsByRotation - 1) : (topline - 1);
			makeTriangle(topline, toplinePrev, bottom);
		}
	}
}

void Object3DData::SetVertexesParameterByIndex(unsigned int parameterInd, const float(&& data)[]) {
	Renderer::DataLayoutElement* parameterData = &VertexesNonTemporaryData_Layout[parameterInd];
	for (unsigned int vi = 0; vi < VertexesData.size(); vi++) {
		for (unsigned int oi = 0; oi < parameterData->Length; oi++) {
			VertexesData[vi][parameterData->StartOffset + oi] = data[oi];
		}
	}
}
void Object3DData::SetVertexesParameterByName(const std::string&& parameterName, const float(&& data)[]) { SetVertexesParameterByIndex(FindVertexDataIndByName(std::move(parameterName)), std::move(data)); }

void Object3DData::SetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd, const float(&& data)[]) {
	Renderer::DataLayoutElement* parameterData = &VertexesNonTemporaryData_Layout[parameterInd];
	for (unsigned int oi = 0; oi < parameterData->Length; oi++) {
		VertexesData[vertexInd][parameterData->StartOffset + oi] = data[oi];
	}
}
void Object3DData::SetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName, const float(&& data)[]) { SetVertexParameterByIndex(vertexInd, FindVertexDataIndByName(std::move(parameterName)), std::move(data)); }


std::vector<float> Object3DData::GetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd) const {
	unsigned int parameterOffset = VertexesNonTemporaryData_Layout[parameterInd].StartOffset;
	std::vector<float> retVec;
	retVec.resize(VertexesNonTemporaryData_Layout[parameterInd].Length);
	for (unsigned int i = 0; i < retVec.size(); i++)
		retVec[i] = VertexesData[vertexInd][parameterOffset + i];
	return retVec;
}
std::vector<float> Object3DData::GetVertexParameterByName(unsigned int vertexInd, const std::string&& parameterName) const { return GetVertexParameterByIndex(vertexInd, FindVertexDataIndByName(std::move(parameterName))); }

unsigned int Object3DData::FindVertexDataIndByName(const std::string&& name) const {
	for (unsigned int i = 0; i < VertexesNonTemporaryData_Layout.size(); i++) if (VertexesNonTemporaryData_Layout[i].Name == name) return i;
	__debugbreak();//name not found
	return -1;
}



void Object3DData::SetTrianglesParameterByIndex(unsigned int parameterInd, const float(&& data)[]) {
	Renderer::DataLayoutElement* parameterData = &TrianglesNonTemporaryData_Layout[parameterInd];
	for (unsigned int vi = 0; vi < TrianglesData.size(); vi++) {
		for (unsigned int oi = 0; oi < parameterData->Length; oi++) {
			TrianglesData[vi].Data[parameterData->StartOffset + oi] = data[oi];
		}
	}
}
void Object3DData::SetTrianglesParameterByName(const std::string&& parameterName, const float(&& data)[]) { SetTrianglesParameterByIndex(FindTriangleDataIndByName(std::move(parameterName)), std::move(data)); }

void Object3DData::SetTriangleParameterByIndex(unsigned int triangleInd, unsigned int parameterInd, const float(&& data)[]) {
	Renderer::DataLayoutElement* parameterData = &TrianglesNonTemporaryData_Layout[parameterInd];
	for (unsigned int oi = 0; oi < parameterData->Length; oi++) {
		TrianglesData[triangleInd].Data[parameterData->StartOffset + oi] = data[oi];
	}
}
void Object3DData::SetTriangleParameterByName(unsigned int triangleInd, const std::string&& parameterName, const float(&& data)[]) { SetTriangleParameterByIndex(triangleInd, FindTriangleDataIndByName(std::move(parameterName)), std::move(data)); }


std::vector<float> Object3DData::GetTriangleParameterByIndex(unsigned int triangleInd, unsigned int parameterInd) const {
	unsigned int parameterOffset = TrianglesNonTemporaryData_Layout[parameterInd].StartOffset;
	std::vector<float> retVec;
	retVec.resize(TrianglesNonTemporaryData_Layout[parameterInd].Length);
	for (unsigned int i = 0; i < retVec.size(); i++)
		retVec[i] = TrianglesData[triangleInd].Data[parameterOffset + i];
	return retVec;
}
std::vector<float> Object3DData::GetTriangleParameterByName(unsigned int triangleInd, const std::string&& parameterName) const { return GetTriangleParameterByIndex(triangleInd, FindTriangleDataIndByName(std::move(parameterName))); }

unsigned int Object3DData::FindTriangleDataIndByName(const std::string&& name) const {
	for (unsigned int i = 0; i < TrianglesNonTemporaryData_Layout.size(); i++) if (TrianglesNonTemporaryData_Layout[i].Name == name) return i;
	__debugbreak();//name not found
	return -1;
}


void Object3DData::AddLayoutDataToVertexesTemporaryData_Layout(ShaderProgramDataTypes type, unsigned int len) {

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

void Object3DData::AddLayoutDataToVertexesNonTemporaryData_Layout(std::string name, ShaderProgramDataTypes type, unsigned int len) {

	if (ObjectHasBeenInitialized) {
#if defined _DEBUG
		__debugbreak();//object has been initialized, you cant call this function
#endif
		return;
	}

	unsigned int sOff = (VertexesNonTemporaryData_Layout.size() == 0) ? 0 :
		VertexesNonTemporaryData_Layout[VertexesNonTemporaryData_Layout.size() - 1].EndOffset;
	VertexesNonTemporaryData_Layout.push_back({ name,type,
		sOff,sOff + len,
		len });
}

void Object3DData::AddLayoutDataToTrianglesTemporaryData_Layout(ShaderProgramDataTypes type, unsigned int len) {

	if (ObjectHasBeenInitialized) {
#if defined _DEBUG
		__debugbreak();//object has been initialized, you cant call this function
#endif
		return;
	}

	unsigned int sOff = (TrianglesTemporaryData_Layout.size() == 0) ? 0 :
		TrianglesTemporaryData_Layout[TrianglesTemporaryData_Layout.size() - 1].EndOffset;
	TrianglesTemporaryData_Layout.push_back({ "",type,
		sOff,sOff + len,
		len });
}

void Object3DData::AddLayoutDataToTrianglesNonTemporaryData_Layout(std::string name, ShaderProgramDataTypes type, unsigned int len) {

	if (ObjectHasBeenInitialized) {
#if defined _DEBUG
		__debugbreak();//object has been initialized, you cant call this function
#endif
		return;
	}

	unsigned int sOff = (TrianglesNonTemporaryData_Layout.size() == 0) ? 0 :
		TrianglesNonTemporaryData_Layout[TrianglesNonTemporaryData_Layout.size() - 1].EndOffset;
	TrianglesNonTemporaryData_Layout.push_back({ name,type,
		sOff,sOff + len,
		len });
}

void Object3DData::AddTo_DependentObjectsAmount_ToParent(int add) {
	Object3DData* parentObj3d = instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->Parent->FindTypePtr<Object3DData>(TypeIndex);
	if (parentObj3d != nullptr and parentObj3d->Translator == Translator) {
		parentObj3d->DependentObjectsAmount_InObjectsVector += add;
		parentObj3d->AddTo_DependentObjectsAmount_ToParent(add);
	}
}

void Object3DData::sShaderID(ShaderProgramID id) {
#if defined _DEBUG
	//////////////////////CHECKS IF SHADER IS COMPATIBLE
	auto program = ShaderProgramsManager::ShaderPrograms[id].ShaderProgramPtr;

	if (program->VertexBufferData.size() !=
		VertexesTemporaryData_Layout.size() + VertexesNonTemporaryData_Layout.size() + TrianglesTemporaryData_Layout.size() + TrianglesNonTemporaryData_Layout.size()) __debugbreak();

	unsigned int offset = 0;

	for (unsigned int i = 0; i < VertexesTemporaryData_Layout.size(); i++)
		if (program->VertexBufferData[i].Type != VertexesTemporaryData_Layout[i].Type) __debugbreak();
	offset += VertexesTemporaryData_Layout.size();

	for (unsigned int i = 0; i < VertexesNonTemporaryData_Layout.size(); i++)
		if (program->VertexBufferData[offset + i].Type != VertexesNonTemporaryData_Layout[i].Type) __debugbreak();
	offset += VertexesNonTemporaryData_Layout.size();

	for (unsigned int i = 0; i < TrianglesTemporaryData_Layout.size(); i++)
		if (program->VertexBufferData[offset + i].Type != TrianglesTemporaryData_Layout[i].Type) __debugbreak();
	offset += TrianglesTemporaryData_Layout.size();

	for (unsigned int i = 0; i < TrianglesNonTemporaryData_Layout.size(); i++)
		if (program->VertexBufferData[offset + i].Type != TrianglesNonTemporaryData_Layout[i].Type) __debugbreak();
#endif

	ShaderID = id;
}

Object3DData* Object3DData::gThis() {
	return this;
}
Object3DData::Object3DData(ClassesMap* mapPtr) {
	instancePtr = mapPtr;

	AddLayoutDataToVertexesTemporaryData_Layout(ShaderProgramDataTypes::vector4, 4);//position in space
	AddLayoutDataToVertexesTemporaryData_Layout(ShaderProgramDataTypes::vector3, 3);//pos in space
	AddLayoutDataToVertexesNonTemporaryData_Layout("LocalPosition", ShaderProgramDataTypes::vector3, 3);
	AddLayoutDataToVertexesNonTemporaryData_Layout("Color", ShaderProgramDataTypes::vector4, 4);

	AddLayoutDataToTrianglesTemporaryData_Layout(ShaderProgramDataTypes::vector3, 3);//normal in space
	AddLayoutDataToTrianglesNonTemporaryData_Layout("LocalNormalVector", ShaderProgramDataTypes::vector3, 3);
	

	instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->ParentChange.ConnectBeforeEvent([this](ClassesMap* newPar) {

		if (not ObjectHasBeenInitialized) {
			InitializedRenderingDataLayout.reserve(
				VertexesTemporaryData_Layout.size() + VertexesNonTemporaryData_Layout.size() + TrianglesTemporaryData_Layout.size() + TrianglesNonTemporaryData_Layout.size());

			for (unsigned int i = 0; i < VertexesTemporaryData_Layout.size(); i++) {
				InitializedRenderingDataLayout.push_back(VertexesTemporaryData_Layout[i].Length);
			}
			for (unsigned int i = 0; i < VertexesNonTemporaryData_Layout.size(); i++) {
				InitializedRenderingDataLayout.push_back(VertexesNonTemporaryData_Layout[i].Length);
			}
			for (unsigned int i = 0; i < TrianglesTemporaryData_Layout.size(); i++) {
				InitializedRenderingDataLayout.push_back(TrianglesTemporaryData_Layout[i].Length);
			}
			for (unsigned int i = 0; i < TrianglesNonTemporaryData_Layout.size(); i++) {
				InitializedRenderingDataLayout.push_back(TrianglesNonTemporaryData_Layout[i].Length);
			}

		}
		ObjectHasBeenInitialized = true;

		RenderingParent = newPar->IsContains(Renderer::TypeIndex) ? newPar->FindTypePtr<Renderer>(Renderer::TypeIndex) :
			newPar->FindTypePtr<Instance>(Instance::TypeIndex)->FindFirstParentWithModifier(Renderer::TypeIndex)->FindTypePtr<Renderer>(Renderer::TypeIndex);
		if (RenderingParent != nullptr)
			RenderingObjectTypeInd = RenderingParent->GetTypeOfRenderingObject(InitializedRenderingDataLayout);


		if (Translator != nullptr) {
			Translator->RemoveFromObjects(instancePtr);
			AddTo_DependentObjectsAmount_ToParent(-(int)DependentObjectsAmount_InObjectsVector);
		}


		if (newPar == nullptr) return;//no need to continue


		if (newPar->IsContains(TranslatorFrom3D::TypeIndex)) Translator = newPar->FindTypePtr<TranslatorFrom3D>(TranslatorFrom3D::TypeIndex);
		else Translator = newPar->FindTypePtr<Instance>(Instance::TypeIndex)->FindFirstParentWithModifier(TranslatorFrom3D::TypeIndex)->FindTypePtr<TranslatorFrom3D>(TranslatorFrom3D::TypeIndex);

		if (Translator != nullptr and instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->Parent == nullptr)
			Translator->InsertInObjects(instancePtr, newPar);
		

		});

	instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->ParentChange.ConnectAfterEvent([this](ClassesMap* prevPar) {

		if (Translator != nullptr) {
			if (Translator->RecalculateCoordinatesOfChildInSpace_OnChildParentUpdate) {
				RecalculateCoordinatesInSpace(true);
				if (Translator->UpdateRenderingDataOfChild_OnChildParentUpdate) UpdateVertexesAndTrianglesDataInRenderingData(true);
			}
		}

		AddTo_DependentObjectsAmount_ToParent(DependentObjectsAmount_InObjectsVector);

		});
}