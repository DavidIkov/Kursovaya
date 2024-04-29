#include"../Maths/Vector2.h"
#include"Instance2DData.h"
#include"Renderer2D.h"
#include"../../GraphicsPrimitives/Debug.h"


#include"../../GraphicsPrimitives/ShaderProgramManager.h"

#include"../../Maths/Matrix.h"
#include"../../Maths/SPCS.h"
#include"../../Maths/Vector2.h"


const std::type_index Renderer2D::TypeIndex = std::type_index(typeid(Renderer2D));

Renderer2D::TypesOfObjectsRendering_Element::TypesOfObjectsRendering_Element(const std::vector<unsigned int>& layout) :vVertexArray(), vVertexBuffer(), vIndexBuffer(), vBufferLayout(GL_FLOAT, sizeof(float)) {
	for (unsigned int i = 0; i < layout.size(); i++) vBufferLayout.AddData(layout[i]);
	vBufferLayout.Bind();
	vVertexArray.Unbind();
}
bool Renderer2D::TypesOfObjectsRendering_Element::CompareWithLayout(const std::vector<unsigned int>& vertexDataLens) const {
	if (vertexDataLens.size() != vBufferLayout.Lengths.size()) return false;
	for (unsigned int i = 0; i < vertexDataLens.size(); i++) if (vertexDataLens[i] != vBufferLayout.Lengths[i]) return false;
	return true;
}
void Renderer2D::TypesOfObjectsRendering_Element::Bind(Instance2DData* instPtr, std::vector<float>* vertexBuffer, std::vector<unsigned int>* indexBuffer) {
	vVertexBuffer.ModifyData(vertexBuffer);
	vIndexBuffer.ModifyData(indexBuffer);
	vVertexArray.Bind();
	ShaderProgramManager::ShaderPrograms[instPtr->ShaderID].ShaderProgramPtr->Bind();
	instPtr->UpdateUniformData.FireEvent(ShaderProgramManager::ShaderPrograms[instPtr->ShaderID].ShaderProgramPtr->gUniform());
}
void Renderer2D::TypesOfObjectsRendering_Element::Unbind() {
	vVertexArray.Unbind();
}



unsigned int Renderer2D::GetTypeOfObjectRenderingElementInd(const std::vector<unsigned int>&VertexDataEndOffsets) const {
	std::vector<unsigned int> lens(VertexDataEndOffsets.size(), 0);
	lens[0] = VertexDataEndOffsets[0];
	for (unsigned int i = 1; i < lens.size(); i++) lens[i] = VertexDataEndOffsets[i] - VertexDataEndOffsets[i - 1];

	//search
	for (unsigned int i = 0; i < TypesOfObjectsRendering.size(); i++)
		if (TypesOfObjectsRendering[i].CompareWithLayout(lens))
			return i;

	//not found where to insert... in a function that wont insert anything(should pass shader paths so it can insert)... ERROR!!
	__debugbreak();

	return -1;
}
unsigned int Renderer2D::GetTypeOfObjectRenderingElementInd(const std::vector<unsigned int>& VertexDataEndOffsets) {
	std::vector<unsigned int> lens(VertexDataEndOffsets.size(), 0);
	lens[0] = VertexDataEndOffsets[0];
	for (unsigned int i = 1; i < lens.size(); i++) lens[i] = VertexDataEndOffsets[i] - VertexDataEndOffsets[i - 1];

	//search
	for (unsigned int i = 0; i < TypesOfObjectsRendering.size(); i++)
		if (TypesOfObjectsRendering[i].CompareWithLayout(lens))
			return i;

	//not found where to insert... create new
	TypesOfObjectsRendering.push_back({ lens });
	return TypesOfObjectsRendering.size() - 1;
}


Renderer2D* Renderer2D::gThis() {
	return this;
}

Renderer2D::Renderer2D(ClassesMap* nPtr) {
	instancePtr = nPtr;
}

void Renderer2D::Render(int width, int height) {
	struct Cords {
		Instance2DData* InstancePtr;
		float PosX;
		float PosY;
		float SizeX;
		float SizeY;
		Matrix Axes;
	};
	Cords EmptyCords({ nullptr,0.f,0.f,(float)width,(float)height,{2,2,{1.f,0.f,0.f,1.f}} });
	std::vector<Cords> RecordedCords(1, EmptyCords);
	for (unsigned int i = 0; i < FilteredOrder.size(); i++) {
		

		Instance2DData* Element = FilteredOrder[i];

		Cords* FoundCords = &RecordedCords[0];
		for (unsigned int ci = RecordedCords.size() - 1; ci > 0; ci--) {
			if (RecordedCords[ci].InstancePtr == Element->FirstGraphicsParent) {
				FoundCords = &RecordedCords[ci];
				break;
			}
			if (RecordedCords[ci].InstancePtr->FirstGraphicsParent != Element->FirstGraphicsParent)
				RecordedCords.erase(RecordedCords.begin() + ci);
		}
		float ResPosX = FoundCords->PosX;
		float ResPosY = FoundCords->PosY;
		float ResSizeX = FoundCords->SizeX;
		float ResSizeY = FoundCords->SizeY;
		Matrix ResAxes = FoundCords->Axes;




		auto& Pos = Element->Position;
		auto& Size = Element->Size;
		float RotationAngle = Element->Rotation;

		{
			Matrix addVec(1, 2, { Pos.gPX() + ResSizeX * Pos.gSX() / 2 ,Pos.gPY() + ResSizeY * Pos.gSY() / 2 });
			Matrix rotatedAddVec = ResAxes * addVec;
			ResPosX += rotatedAddVec[0];
			ResPosY += rotatedAddVec[1];
		}
		ResSizeX = Size.gPX() + ResSizeX * Size.gSX() / 2;
		ResSizeY = Size.gPX() + ResSizeX * Size.gSX() / 2;





		{
			auto& rotationOffset = Element->RotationOffset;

			Matrix pos(1, 2, { ResPosX,ResPosY });


			Matrix rotationOffsetPos(1, 2, { rotationOffset.gPX() + rotationOffset.gSX() * ResSizeX / 2, rotationOffset.gPY() + rotationOffset.gSY() * ResSizeY / 2 });


			Matrix rotation = Matrix(2, 2, { cosf(RotationAngle),sinf(RotationAngle),-sinf(RotationAngle),cosf(RotationAngle) });

			Matrix rotatedPos = (rotation * (rotationOffsetPos*(-1.f))) + rotationOffsetPos + pos;

			ResAxes = ResAxes * rotation;

			ResPosX = rotatedPos[0];
			ResPosY = rotatedPos[1];

			RecordedCords.push_back({ Element,ResPosX,ResPosY,ResSizeX,ResSizeY,ResAxes});
		}

		auto& vertexes = Element->Vertexes;
		auto& dataEndOffsets = Element->VertexDataEndOffsets;
		auto& positionOffset = Element->PositionOffset;
		unsigned int vertexLen = dataEndOffsets[dataEndOffsets.size() - 1];


		Element->LRPosition.sX(ResSizeX / 2 * (positionOffset.gSX()) + positionOffset.gPX());
		Element->LRPosition.sY(ResSizeY / 2 * (positionOffset.gSY()) + positionOffset.gPY());
		
		Element->LRSize.sX(ResSizeX);
		Element->LRSize.sY(ResSizeY);


		std::vector<float> newBufferData(vertexes.size(), 0.f);
		for (unsigned int i = 0; i < vertexes.size() / vertexLen; i++) {
			unsigned int curDataOffset = i * vertexLen;
			Matrix vecAdd(1, 2, { ResSizeX / 2 * (vertexes[curDataOffset + 2] + positionOffset.gSX()) + positionOffset.gPX() ,
				ResSizeY / 2 * (vertexes[curDataOffset + 3] + positionOffset.gSY()) + positionOffset.gPY() });
			Matrix rotatedVecAdd = ResAxes * vecAdd;
			newBufferData[curDataOffset] = (ResPosX + rotatedVecAdd[0]) / width * 2;
			newBufferData[curDataOffset + 1] = (ResPosY + rotatedVecAdd[1]) / height * 2;
			for (unsigned int oi = 2; oi < vertexLen; oi++) {
				newBufferData[curDataOffset + oi] = vertexes[curDataOffset + oi];
			}
		}

		TypesOfObjectsRendering[Element->RendererObjectRenderTypeInd].Bind(Element, &newBufferData, &(Element->IndexBufferData));

		glSC(glDrawElements(GL_TRIANGLES, Element->IndexBufferData.size(), GL_UNSIGNED_INT, 0));

		TypesOfObjectsRendering[Element->RendererObjectRenderTypeInd].Unbind();
	}
}