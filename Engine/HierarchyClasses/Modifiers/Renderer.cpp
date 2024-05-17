#include"Renderer.h"
#include"TranslatorFrom2D.h"

const std::type_index Renderer::TypeIndex = std::type_index(typeid(Renderer));

Renderer* Renderer::gThis() {
	return this;
}
Renderer::Renderer(ClassesMap* mapPtr) {
	InstancePtr = mapPtr;
}

Renderer::TypesOfRenderingObjects_Element::TypesOfRenderingObjects_Element(const std::vector<VertexAditionalDataLayoutElement>& layout) :vVertexArray(), vVertexBuffer(), vIndexBuffer(), vBufferLayout(GL_FLOAT, sizeof(float)) {
	vBufferLayout.AddData(2);
	for (unsigned int i = 0; i < layout.size(); i++) vBufferLayout.AddData(layout[i].Length);
	vBufferLayout.Bind();
	vVertexArray.Unbind();
}
bool Renderer::TypesOfRenderingObjects_Element::CompareWithLayout(const std::vector<VertexAditionalDataLayoutElement>& layout) const {
	if (layout.size() != vBufferLayout.Lengths.size()) return false;
	for (unsigned int i = 0; i < layout.size(); i++) if (layout[i].Length != vBufferLayout.Lengths[i]) return false;
	return true;
}
void Renderer::TypesOfRenderingObjects_Element::Bind(unsigned int shaderID, Event<const Uniform&>* uniformEvent, std::vector<float>* vertexBuffer, std::vector<unsigned int>* indexBuffer) const {
	vVertexBuffer.ModifyData(vertexBuffer);
	vIndexBuffer.ModifyData(indexBuffer);
	vVertexArray.Bind();
	ShaderProgramsManager::ShaderPrograms[shaderID].ShaderProgramPtr->Bind();
	uniformEvent->FireEvent(ShaderProgramsManager::ShaderPrograms[shaderID].ShaderProgramPtr->gUniform());
}
void Renderer::TypesOfRenderingObjects_Element::Unbind() const {
	vVertexArray.Unbind();
}

unsigned int Renderer::GetTypeOfRenderingObject(const std::vector<VertexAditionalDataLayoutElement>& vertexDataAditionalDataLayout) {
	//search
	for (unsigned int i = 0; i < TypesOfRenderingObjects.size(); i++)
		if (TypesOfRenderingObjects[i].CompareWithLayout(vertexDataAditionalDataLayout))
			return i;

	//not found where to insert... create new
	TypesOfRenderingObjects.push_back({ vertexDataAditionalDataLayout });
	return TypesOfRenderingObjects.size() - 1;
}


void Renderer::Render() const {
	Render(&InstancePtr->FindTypePtr<TranslatorFrom2D>(TranslatorFrom2D::TypeIndex)->FilteredOrder);
}
void Renderer::Render(std::vector<FilteredOrderElement>* filteredOrder) const {

	for (unsigned int i = 0; i < filteredOrder->size(); i++) {
		auto& element = (*filteredOrder)[i];
		
		if (element.TranslatorFilteredOrder != nullptr)//this check makes so translators wont be rendered!!!
			Render(element.TranslatorFilteredOrder);
		else {//here is actual rendering...

			//fill data
			std::vector<float> bufferData(element.VertexesData.size(), 0.f);
			for (unsigned int vi = 0; vi < element.VertexesData.size(); vi++)
				bufferData[vi] = element.VertexesData[vi];

			TypesOfRenderingObjects[element.RenderingObjectTypeInd].Bind(element.ShaderID, element.UniformDataUpdateEvent, &(element.VertexesData), &(element.VertexesOrder));

			glSC(glDrawElements(GL_TRIANGLES, element.VertexesOrder.size(), GL_UNSIGNED_INT, 0));

			TypesOfRenderingObjects[element.RenderingObjectTypeInd].Unbind();
		}
	}
}