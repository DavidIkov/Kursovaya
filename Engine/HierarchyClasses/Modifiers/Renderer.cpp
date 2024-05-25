#include"Renderer.h"
#include"TranslatorFrom2D.h"

const std::type_index Renderer::TypeIndex = std::type_index(typeid(Renderer));

static std::vector<float> VertexesCombinedData;
static void ClearCombinedData() {
	VertexesCombinedData.resize(0);
}


Renderer* Renderer::gThis() {
	return this;
}
Renderer::Renderer(ClassesMap* mapPtr) {
	InstancePtr = mapPtr;
}

Renderer::TypesOfRenderingObjects_Element::TypesOfRenderingObjects_Element(std::vector<unsigned int>& renderingDataLayout) :vVertexArray(), vVertexBuffer(), /*vIndexBuffer(),*/ vBufferLayout(GL_FLOAT, sizeof(float)) {
	for (unsigned int i = 0; i < renderingDataLayout.size(); i++) vBufferLayout.AddData(renderingDataLayout[i]);
	vBufferLayout.Bind();
	vVertexArray.Unbind();
}
bool Renderer::TypesOfRenderingObjects_Element::IsCompatible(std::vector<unsigned int>& renderingDataLayout) const {
	if (renderingDataLayout.size() != vBufferLayout.Lengths.size()) return false;
	for (unsigned int i = 0; i < renderingDataLayout.size(); i++) if (renderingDataLayout[i] != vBufferLayout.Lengths[i]) return false;
	return true;
}
void Renderer::TypesOfRenderingObjects_Element::Bind(unsigned int shaderID, Event<const Uniform&>* uniformEvent) const {
	vVertexBuffer.ModifyData(&VertexesCombinedData);
	//vIndexBuffer.ModifyData(&VertexesOrderCombinedData);
	vVertexArray.Bind();
	ShaderProgramsManager::ShaderPrograms[shaderID].ShaderProgramPtr->Bind();
	uniformEvent->FireEvent(ShaderProgramsManager::ShaderPrograms[shaderID].ShaderProgramPtr->gUniform());
}
void Renderer::TypesOfRenderingObjects_Element::Unbind() const {
	vVertexArray.Unbind();
}

unsigned int Renderer::GetTypeOfRenderingObject(std::vector<unsigned int>& renderingDataLayout) {
	//search
	for (unsigned int i = 0; i < TypesOfRenderingObjects.size(); i++)
		if (TypesOfRenderingObjects[i].IsCompatible(renderingDataLayout))
			return i;

	//not found where to insert... create new
	TypesOfRenderingObjects.push_back({ renderingDataLayout });
	return TypesOfRenderingObjects.size() - 1;
}


static bool GLDepthTestState = false;

void Renderer::Render() const {
	glSC(glDisable(GL_DEPTH_TEST));
	Render(&InstancePtr->FindTypePtr<TranslatorFrom2D>(TranslatorFrom2D::TypeIndex)->FilteredOrder);
}


void Renderer::Render(std::vector<ObjectsVectorElement>* filteredOrder) const {

	ClearCombinedData();

	for (unsigned int i = 0; i < filteredOrder->size(); i++) {
		auto& element = (*filteredOrder)[i];

		if (true) 1 + 1;
		else {};

		if (element.TranslatorObjectsVector != nullptr) {//this check makes so translators wont be rendered!!!
			bool prevState = GLDepthTestState;
			if (element.DepthTesting) { glSC(glEnable(GL_DEPTH_TEST)); }
			else { glSC(glDisable(GL_DEPTH_TEST)) };
			Render(element.TranslatorObjectsVector);
			ClearCombinedData();
			if (prevState) { glSC(glEnable(GL_DEPTH_TEST)); }
			else { glSC(glDisable(GL_DEPTH_TEST)) };
		}
		else {//here is actual rendering...


			if (i > 0 and (*filteredOrder)[i - 1].RenderingObjectTypeInd != element.RenderingObjectTypeInd) {
				ClearCombinedData();
			}
			
			VertexesCombinedData.reserve(VertexesCombinedData.size() + element.VertexBufferData.size());
			for (unsigned int vdi = 0; vdi < element.VertexBufferData.size(); vdi++)
				VertexesCombinedData.push_back(element.VertexBufferData[vdi]);

			if (i == filteredOrder->size() - 1 or element.RenderingObjectTypeInd != (*filteredOrder)[i + 1].RenderingObjectTypeInd) {
				TypesOfRenderingObjects[element.RenderingObjectTypeInd].Bind(element.ShaderID, element.UniformDataUpdateEvent);
				
				glSC(glDrawArrays(GL_TRIANGLES, 0, VertexesCombinedData.size() / TypesOfRenderingObjects[element.RenderingObjectTypeInd].vBufferLayout.SumOfLengths));

				TypesOfRenderingObjects[element.RenderingObjectTypeInd].Unbind();
			}
		}
	}
}