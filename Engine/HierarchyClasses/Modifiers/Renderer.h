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
	friend class TranslatorFrom3D;
	friend class Object2DData;
	friend class Object3DData;
	ClassesMap* InstancePtr = nullptr;//"this" pointer from actual instance

	struct ObjectsVectorElement {
		bool DepthTesting = false;//if this is translator then it works
		std::vector<ObjectsVectorElement>* TranslatorObjectsVector;//if this exists then this member is translator
		std::vector<float> VertexBufferData;//dummy if element is translator
		ShaderProgramID ShaderID = (unsigned int)(-1);//dummy if element is translator
		Event<const Uniform&>* UniformDataUpdateEvent;//dummy if element is translator
		unsigned int RenderingObjectTypeInd;
		unsigned int* IntContainer;//pointer to data where index of element is located
	};
	struct DataLayoutElement {
		std::string Name;
		ShaderProgramDataTypes Type;

		//YES I KNOW I CAN JUST GET ALL THIS NUMS OUT OF SINGLE ONE BUT ITS MORE EASY TO DO THIS WAY FOR ACTUAL CODING!!!
		unsigned int StartOffset;
		unsigned int EndOffset;
		unsigned int Length;
	};

	struct TypesOfRenderingObjects_Element {
		mutable VertexArray vVertexArray;
		mutable VertexBuffer vVertexBuffer;
		//mutable IndexBuffer vIndexBuffer;
		BufferLayout vBufferLayout;
		TypesOfRenderingObjects_Element(std::vector<unsigned int>& renderingDataLayout);

		bool IsCompatible(std::vector<unsigned int>& renderingDataLayout) const;
		
		void Bind(unsigned int shaderID, Event<const Uniform&>* uniformEvent) const;
		void Unbind() const;
	};
	std::vector<TypesOfRenderingObjects_Element> TypesOfRenderingObjects;

	unsigned int GetTypeOfRenderingObject(std::vector<unsigned int>& renderingDataLayout);
	
public:

	static const std::type_index TypeIndex;
	Renderer* gThis();
	DLL_TREATMENT Renderer(ClassesMap* mapPtr);

	DLL_TREATMENT void Render() const;
private:
	void Render(std::vector<ObjectsVectorElement>* fitleredOrder) const;
public:
};