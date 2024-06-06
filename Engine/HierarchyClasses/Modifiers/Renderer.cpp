#include"Renderer.h"
#include"TranslatorFrom2D.h"
#include"Graphics/ShaderProgramsManager.h"
#include"OffScreenRenderingData.h"

const std::type_index Renderer::TypeIndex = std::type_index(typeid(Renderer));

static std::vector<float> VertexesCombinedData;


Renderer* Renderer::gThis() {
	return this;
}

Renderer::Renderer(ClassesMap* mapPtr) :BasicImageRendererType({ {2} }) {
	InstancePtr = mapPtr;

	BasicImageRenderer.VertexBufferData.push_back({ ShaderProgramDataTypes::vector2, "ScreenPosition" });
	BasicImageRenderer.TransferData.push_back({ ShaderProgramDataTypes::vector2,"ScreenPosition" });
	BasicImageRenderer.FragmentShaderUniformsData.push_back({ ShaderProgramDataTypes::image2D,"Image",0 });

	BasicImageRenderer.VertexShaderCodeParts.push_back("gl_Position=vec4(i_ScreenPosition.x,i_ScreenPosition.y,0.f,1.f);");
	BasicImageRenderer.VertexShaderCodeParts.push_back("t_ScreenPosition=i_ScreenPosition;");

	BasicImageRenderer.FragmentShaderCodeParts.push_back("o_PixelColor=texture(u_Image,(t_ScreenPosition+1)/2);");
	
	BasicImageRenderer.CompileShader();


}
 
Renderer::TypesOfRenderingObjects_Element::TypesOfRenderingObjects_Element(const std::vector<unsigned int>& renderingDataLayout) :vVertexArray(), vVertexBuffer(), vBufferLayout(GL_FLOAT, sizeof(float)) {
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
	vVertexArray.Bind();
	ShaderProgramsManager::ShaderPrograms[shaderID].ShaderProgramPtr->Bind();
	uniformEvent->FireEvent(ShaderProgramsManager::ShaderPrograms[shaderID].ShaderProgramPtr->gUniform());
}
void Renderer::TypesOfRenderingObjects_Element::Bind(const ShaderProgram& shader) const {
	vVertexArray.Bind();
	shader.Bind();
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

static const std::vector<float> BasicImageRendererVertexBufferData({
	1.f,1.f,
	-1.f,1.f,
	-1.f,-1.f,

	1.f,1.f,
	1.f,-1.f,
	-1.f,-1.f,
	});

void Renderer::Render() const {
	FrameBuffer::Unbind();
	BasicImageRendererType.vVertexBuffer.ModifyData(&BasicImageRendererVertexBufferData);
	BasicImageRendererType.Bind(BasicImageRenderer);
	BasicImageRenderer.Bind();
	InstancePtr->FindTypePtr<OffScreenRenderingData>(OffScreenRenderingData::TypeIndex)->vFrameBuffer.gTexture().Bind();

	glSC(glDrawArrays(GL_TRIANGLES, 0, BasicImageRendererVertexBufferData.size() / BasicImageRendererType.vBufferLayout.SumOfLengths));
}


void Renderer::Render(const std::vector<RenderingDataElement>& renderingData, const FrameBuffer& bufferToWrite) const {

	bufferToWrite.Bind();

	VertexesCombinedData.resize(0);

	for (unsigned int rdi = 0; rdi < renderingData.size(); rdi++) {
		auto& element = renderingData[rdi];

		if (element.TranslatorFrameBuffer != nullptr) {
			//element.TranslatorFrameBuffer->Bind();
			continue;
		}

		VertexesCombinedData.reserve(VertexesCombinedData.size() + element.VertexBufferData.size());
		for (unsigned int vdi = 0; vdi < element.VertexBufferData.size(); vdi++)
			VertexesCombinedData.push_back(element.VertexBufferData[vdi]);

		if (rdi == renderingData.size() - 1 or element.RenderingObjectTypeInd != renderingData[rdi + 1].RenderingObjectTypeInd or renderingData[rdi + 1].TranslatorFrameBuffer != nullptr) {

			TypesOfRenderingObjects[element.RenderingObjectTypeInd].Bind(element.ShaderID, element.UniformDataUpdateEvent);
			
#if defined _DEBUG
			if (VertexesCombinedData.size() % TypesOfRenderingObjects[element.RenderingObjectTypeInd].vBufferLayout.SumOfLengths != 0) __debugbreak();//wrong sum of lenghts
#endif

			glSC(glDrawArrays(GL_TRIANGLES, 0, VertexesCombinedData.size() / TypesOfRenderingObjects[element.RenderingObjectTypeInd].vBufferLayout.SumOfLengths));

			TypesOfRenderingObjects[element.RenderingObjectTypeInd].Unbind();

			VertexesCombinedData.resize(0);
		}

	}
	bufferToWrite.Unbind();


	/*ClearCombinedData();

	for (unsigned int i = 0; i < filteredOrder->size(); i++) {
		auto& element = (*filteredOrder)[i];


		if (element.TranslatorObjectsVector != nullptr) {//this check makes so translators wont be rendered!!!
			bool prevState = GLDepthTestState;
			if (element.DepthTesting) { glSC(glEnable(GL_DEPTH_TEST)); }
			else { glSC(glDisable(GL_DEPTH_TEST)); };
			Render(element.TranslatorObjectsVector);
			ClearCombinedData();
			if (prevState) { glSC(glEnable(GL_DEPTH_TEST)); }
			else { glSC(glDisable(GL_DEPTH_TEST)); };
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
				
#if defined _DEBUG
				if (VertexesCombinedData.size() % TypesOfRenderingObjects[element.RenderingObjectTypeInd].vBufferLayout.SumOfLengths != 0) __debugbreak();//wrong sum of lenghts
#endif
				glSC(glDrawArrays(GL_TRIANGLES, 0, VertexesCombinedData.size() / TypesOfRenderingObjects[element.RenderingObjectTypeInd].vBufferLayout.SumOfLengths));

				TypesOfRenderingObjects[element.RenderingObjectTypeInd].Unbind();
			}
		}
	}*/
}