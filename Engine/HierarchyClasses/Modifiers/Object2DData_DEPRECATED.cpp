
#include"Instance.h"
#include"Renderer2D.h"
#include<typeinfo>
#include"Graphics/Uniform.h"
#include"UsefullStuff/SwapIntervalsInVector.h"
#include"UsefullStuff/ClearIntervalInVector.h"
#include"UsefullStuff/MoveIntervalInVector.h"
#include"UsefullStuff/InsertVectorInVector.h"
#include"UsefullStuff/FilterVector.h"
#include"UsefullStuff/FindFilterInsertIndexInVector.h"
#include"Object2DData.h"

const std::type_index Object2DData::TypeIndex = std::type_index(typeid(Object2DData));

void Object2DData::RecursivelyGetDependentFilteredByPriority(std::vector<Object2DData*>& vec) {
	{
		Object2DData* instPtr = instancePtr->FindTypePtr<Object2DData>(TypeIndex);
		vec.push_back(instPtr);
	}
	
	std::vector<ClassesMap*> children;
	instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->GetAllChildrenWithModifier(TypeIndex, 1, children);

	std::vector<ClassesMap*> filteredChildren = FilterVectorHL(children, [](ClassesMap* data) {return data->FindTypePtr<Object2DData>(TypeIndex)->Priority; });

	for (unsigned int i = 0; i < filteredChildren.size(); i++) {
		RecursivelyGetDependentFilteredByPriority(vec);
	}
}
void Object2DData::RecursivelyModifyParentDependentElementsAmount(int diff) {
	if (FirstGraphicsParent == nullptr) return;
	FirstGraphicsParent->DependentElementsAmount += diff;
	FirstGraphicsParent->RecursivelyModifyParentDependentElementsAmount(diff);
}

void Object2DData::AddVertexDataData(std::string name, unsigned int len, ShaderProgramTypeEnum type) {
	VertexDataTypes.push_back(type);
	VertexDataNames.push_back(name);
	VertexDataEndOffsets.push_back(len + ((VertexDataEndOffsets.size() == 0) ? 0 : VertexDataEndOffsets[VertexDataEndOffsets.size() - 1]));
}
unsigned int Object2DData::FindVertexDataNameInd(const std::string& name) const {
	for (unsigned int i = 0; i < VertexDataNames.size(); i++) if (VertexDataNames[i] == name) return i;
	__debugbreak();//name not found
	return -1;
}
Object2DData* Object2DData::gThis() {
	return this;
}

bool Object2DData::ShaderCanBeApplyed(ShaderProgramID id) {
	auto program = ShaderProgramManager::ShaderPrograms[id].ShaderProgramPtr;
	if (program->VertexBufferData.size() != VertexDataTypes.size()) return false;
	for (unsigned int i = 0; i < VertexDataTypes.size(); i++)
		if (program->VertexBufferData[i].Type != VertexDataTypes[i]) return false;
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

void Object2DData::sPriority(float nVal) {
	PriorityChange.FireBeforeEventFunctions(nVal);
	float prevZInd = Priority;
	Priority = nVal;
	PriorityChange.FireAfterEventFunctions(prevZInd);
}
float Object2DData::gPriority() const { return Priority; }


const Vector2& Object2DData::gLRPosition() const {
	return LRPosition;
}
const Vector2& Object2DData::gLRSize() const {
	return LRSize;
}

void Object2DData::SetVertexesParameterByIndex(unsigned int parameterInd, const std::vector<float>& data) {
	unsigned int parameterOffset = (parameterInd == 0) ? 0 : VertexDataEndOffsets[parameterInd - 1];
	for (unsigned int vi = 0; vi < Vertexes.size() / VertexDataEndOffsets[VertexDataEndOffsets.size() - 1]; vi++) {
		for (unsigned int oi = 0; oi < data.size(); oi++) {
			Vertexes[vi * VertexDataEndOffsets[VertexDataEndOffsets.size() - 1] + parameterOffset + oi] = data[oi];
		}
	}
}
void Object2DData::SetVertexesParameterByName(const std::string& parameterName, const std::vector<float>& data) { SetVertexesParameterByIndex(FindVertexDataNameInd(parameterName), data); }

void Object2DData::SetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd, const std::vector<float>& data) {
	unsigned int offset = vertexInd * VertexDataEndOffsets[VertexDataEndOffsets.size() - 1] + ((parameterInd == 0) ? 0 : VertexDataEndOffsets[parameterInd - 1]);
	for (unsigned int oi = 0; oi < data.size(); oi++) {
		Vertexes[offset + oi] = data[oi];
	}
}
void Object2DData::SetVertexParameterByName(unsigned int vertexInd, std::string parameterName, const std::vector<float>& data) { SetVertexParameterByIndex(vertexInd, FindVertexDataNameInd(parameterName), data); }

const std::vector<float>& Object2DData::GetVertexes() const {
	return Vertexes;
}

std::vector<float> Object2DData::GetVertexParameterByIndex(unsigned int vertexInd, unsigned int parameterInd) const {
	unsigned int parameterOffset = (parameterInd == 0) ? 0 : VertexDataEndOffsets[parameterInd - 1];
	std::vector<float> retVec(VertexDataEndOffsets[parameterInd] - parameterOffset, 0.f);
	for (unsigned int i = 0; i < retVec.size(); i++) {
		retVec[i] = Vertexes[vertexInd * VertexDataEndOffsets[VertexDataEndOffsets.size() - 1] + parameterOffset + i];
	}
	return retVec;
}
std::vector<float> Object2DData::GetVertexParameterByName(unsigned int vertexInd, std::string parameterName) const { return GetVertexParameterByIndex(vertexInd, FindVertexDataNameInd(parameterName)); }

void Object2DData::ApplyVertexTemplate(VertexTemplateEnum vertexTemplate) {
	Vertexes.clear();
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
	UpdateIndexBuffer();
}


void Object2DData::AddVertex(std::vector<float> dataVec) {
	Vertexes.push_back(0.f);
	Vertexes.push_back(0.f);
	for (unsigned int i = 0; i < dataVec.size(); i++) {
		Vertexes.push_back(dataVec[i]);
	}
}

void Object2DData::UpdateIndexBuffer() {

	///////!!!!!!!!!!!!!!������ �� �������� ��� ����� � ������� ���� "����������", ���� �������(������)!!!
	unsigned int vertexesAmount = Vertexes.size() / VertexDataEndOffsets[VertexDataEndOffsets.size() - 1];
	if (vertexesAmount == 2) {
		IndexBufferData.assign(2, 0);
		IndexBufferData[0] = 0;
		IndexBufferData[1] = 1;
	}
	else if (vertexesAmount > 2) {
		IndexBufferData.assign(3, 0);
		IndexBufferData[0] = 0;
		IndexBufferData[1] = 1;
		IndexBufferData[2] = 2;
		for (unsigned int i = 3; i < vertexesAmount; i++) {
			unsigned int nextInd = (i == (vertexesAmount - 1)) ? 0 : (i + 1);
			IndexBufferData.push_back(i);
			IndexBufferData.push_back(i - 1);
			IndexBufferData.push_back(0);
		}
	}
}

Object2DData::Object2DData(ClassesMap* nPtr){
	instancePtr = nPtr;

	//base
	AddVertexDataData("ScreenPosition", 2, ShaderProgramTypeEnum::vector2);


	AddVertexDataData("LocalPosition", 2, ShaderProgramTypeEnum::vector2);
	AddVertexDataData("Color", 4, ShaderProgramTypeEnum::vector4);

	/*Shader.TransferData.push_back({ ShaderProgramTypeEnum::vector4 ,"VertexColor" });
	Shader.VertexShaderCodeParts.push_back("t_VertexColor=i_Color;\ngl_Position = vec4(i_ScreenPosition.x, i_ScreenPosition.y, 0.f, 1.f);");
	Shader.FragmentShaderCodeParts.push_back("o_PixelColor=t_VertexColor;");*/


	{
		PriorityChange.ConnectAfterEvent([this](float prevPriority) {
			if (FirstRenderingParent == nullptr) return;

			auto& Elements = FirstRenderingParent->FilteredOrder;

			unsigned int insertInd = FirstGraphicsParent == nullptr ? 0 : (FirstGraphicsParent->IndOfElement + 1);

			{//find new spot
				Object2DData* closestInst = nullptr;
				unsigned int curInd = insertInd;
				while (curInd < Elements.size() and Elements[curInd]->FirstGraphicsParent == FirstGraphicsParent) {
					if (Elements[curInd]->IndOfElement != IndOfElement)
						if (Elements[curInd]->Priority < Priority and (closestInst == nullptr or closestInst->Priority < Elements[curInd]->Priority))
							closestInst = Elements[curInd];
					curInd = Elements[curInd]->IndOfElement + Elements[curInd]->DependentElementsAmount;
				}
				if (closestInst != nullptr) insertInd = closestInst->IndOfElement;
			}

			SwapIntervalsInVector(Elements, IndOfElement, DependentElementsAmount, insertInd,
				Elements[insertInd]->DependentElementsAmount);
			for (unsigned int i = IndOfElement; i < insertInd + Elements[insertInd]->DependentElementsAmount; i++) {
				Elements[i]->IndOfElement = i;
			}
			});
	}


	{
		instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->ParentChange.ConnectBeforeEvent([this](ClassesMap* newPar) {
			
			//compiles shader
			if (not ShaderProgramManager::ShaderPrograms[ShaderID].ShaderProgramPtr->gCompiled()) 
				ShaderProgramManager::ShaderPrograms[ShaderID].ShaderProgramPtr->CompileShader();

			RecursivelyModifyParentDependentElementsAmount(-(int)DependentElementsAmount);

			Object2DData* newGraphicsParent;
			Renderer2D* newRenderingParent;
			{
				Object2DData* checkOfPar = newPar->FindTypePtr<Object2DData>(TypeIndex);
				if (checkOfPar != nullptr) newGraphicsParent = checkOfPar;
				else {
					ClassesMap* firstPar = newPar->FindTypePtr<Instance>(Instance::TypeIndex)->FindFirstParentWithModifier(Object2DData::TypeIndex);
					newGraphicsParent = firstPar == nullptr ? nullptr : firstPar->FindTypePtr<Object2DData>(Object2DData::TypeIndex);
				}
			}
			{
				Renderer2D* checkOfPar = newPar->FindTypePtr<Renderer2D>(Renderer2D::TypeIndex);
				if (checkOfPar != nullptr) newRenderingParent = checkOfPar;
				else {
					ClassesMap* firstPar = newPar->FindTypePtr<Instance>(Instance::TypeIndex)->FindFirstParentWithModifier(Renderer2D::TypeIndex);
					newRenderingParent = firstPar == nullptr ? nullptr : firstPar->FindTypePtr<Renderer2D>(Renderer2D::TypeIndex);
				}
			}


			//thing about rendering... basically set index of layout that we need
			if (newRenderingParent != nullptr)
				RendererObjectRenderTypeInd = newRenderingParent->GetTypeOfObjectRenderingElementInd(VertexDataEndOffsets);


			
			if (newRenderingParent != nullptr) {
				auto& Elements = newRenderingParent->FilteredOrder;

				unsigned int insertInd = newGraphicsParent == nullptr ? 0 : newGraphicsParent->IndOfElement + 1;
				{//find new spot
					Object2DData* closestInst = nullptr;
					unsigned int curInd = insertInd;
					while (curInd < Elements.size() and Elements[curInd]->FirstGraphicsParent == newGraphicsParent) {
						if (Elements[curInd]->IndOfElement != IndOfElement)
							if (Elements[curInd]->Priority < Priority and (closestInst == nullptr or closestInst->Priority < Elements[curInd]->Priority))
								closestInst = Elements[curInd];
						curInd = Elements[curInd]->IndOfElement + Elements[curInd]->DependentElementsAmount;
					}
					if (closestInst != nullptr) insertInd = closestInst->IndOfElement;
				}

				if (FirstRenderingParent != nullptr) {
					MoveIntervalInVector(Elements, IndOfElement, DependentElementsAmount, insertInd);
					for (unsigned int i = IndOfElement; i < insertInd; i++) {//fix moved indexes
						Elements[i]->IndOfElement -= DependentElementsAmount;
					}
				}
				else {
					std::vector<Object2DData*> dependentFilteredList;
					RecursivelyGetDependentFilteredByPriority(dependentFilteredList);
					//construct new list for dependent

					InsertVectorInVector(Elements, dependentFilteredList, insertInd);
					for (unsigned int i = insertInd + DependentElementsAmount; i < Elements.size(); i++) {
						Elements[i]->IndOfElement += DependentElementsAmount;
					}
				}

				IndOfElement = insertInd;
			}
			else if (FirstRenderingParent != nullptr) ClearIntervalInVector(FirstRenderingParent->FilteredOrder, IndOfElement, DependentElementsAmount);



			FirstGraphicsParent = newGraphicsParent;
			FirstRenderingParent = newRenderingParent;
			});
		instancePtr->FindTypePtr<Instance>(Instance::TypeIndex)->ParentChange.ConnectAfterEvent([this](ClassesMap* prevPar) {


			RecursivelyModifyParentDependentElementsAmount(DependentElementsAmount);
			});
	}

}