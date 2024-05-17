#pragma once
#include"DLLStuff.h"
#include"ClassesMap.h"
#include"Graphics/VertexBuffer.h"
#include"Graphics/IndexBuffer.h"
#include"Graphics/VertexArray.h"
#include"Graphics/BufferLayout.h"
#include"Graphics/ShaderProgramsManager.h"
#include"UsefullStuff/ClassEvent.h"
#include"Maths/Vector2.h"


class Renderer {
	friend class TranslatorFrom2D;
	friend class Object2DData;
	ClassesMap* InstancePtr = nullptr;//"this" pointer from actual instance

	struct FilteredOrderElement {
		std::vector<FilteredOrderElement>* TranslatorFilteredOrder;//if this exists then this member is translator
		std::vector<float> VertexesData;//first 2 floats are screen position... dummy if element is translator
		std::vector<unsigned int> VertexesOrder;//dummy if element is translator
		ShaderProgramID ShaderID;//dummy if element is translator
		Event<const Uniform&>* UniformDataUpdateEvent;//dummy if element is translator
		unsigned int RenderingObjectTypeInd;
		unsigned int* IntContainer;//pointer to data where index of element is located
	};
	struct VertexAditionalDataLayoutElement {
		std::string Name;
		ShaderProgramTypeEnum Type;

		//YES I KNOW I CAN JUST GET ALL THIS NUMS OUT OF SINGLE ONE BUT ITS MORE EASY TO DO THIS WAY FOR ACTUAL CODING!!!
		unsigned int StartOffset;
		unsigned int EndOffset;
		unsigned int Length;
	};
	struct VertexesDataElement {
		Vector2 LocalPosition;
		std::vector<float> AditionalData;
	};

	struct TypesOfRenderingObjects_Element {
		mutable VertexArray vVertexArray;
		mutable VertexBuffer vVertexBuffer;
		mutable IndexBuffer vIndexBuffer;
		BufferLayout vBufferLayout;
		TypesOfRenderingObjects_Element(const std::vector<VertexAditionalDataLayoutElement>& layout);
		bool CompareWithLayout(const std::vector<VertexAditionalDataLayoutElement>& layout) const;
		void Bind(unsigned int shaderID, Event<const Uniform&>* uniformEvent, std::vector<float>* vertexBuffer, std::vector<unsigned int>* indexBuffer) const;
		void Unbind() const;
	};
	std::vector<TypesOfRenderingObjects_Element> TypesOfRenderingObjects;

	unsigned int GetTypeOfRenderingObject(const std::vector<VertexAditionalDataLayoutElement>& vertexDataAditionalDataLayout);
	
public:
	static const std::type_index TypeIndex;
	Renderer* gThis();
	DLL_TREATMENT Renderer(ClassesMap* mapPtr);

	DLL_TREATMENT void Render() const;
private:
	void Render(std::vector<FilteredOrderElement>* fitleredOrder) const;
public:
};