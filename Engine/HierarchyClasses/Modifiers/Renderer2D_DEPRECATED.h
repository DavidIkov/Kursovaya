#pragma once
#include"DLLStuff.h"
#include<vector>
#include"ClassesMap.h"
#include"Instance.h"



#include"Graphics/VertexBuffer.h"
#include"Graphics/IndexBuffer.h"
#include"Graphics/VertexArray.h"
#include"Graphics/BufferLayout.h"

class Object2DData;

class Renderer2D {
	friend Object2DData;

	struct TypesOfObjectsRendering_Element {
		VertexArray vVertexArray;
		VertexBuffer vVertexBuffer;
		IndexBuffer vIndexBuffer;
		BufferLayout vBufferLayout;
		TypesOfObjectsRendering_Element(const std::vector<unsigned int>& layout);
		bool CompareWithLayout(const std::vector<unsigned int>& vertexDataLens) const;
		void Bind(Object2DData* instPtr, std::vector<float>* vertexBuffer, std::vector<unsigned int>* indexBuffer);
		void Unbind();
	};
	std::vector<TypesOfObjectsRendering_Element> TypesOfObjectsRendering;
	unsigned int GetTypeOfObjectRenderingElementInd(const std::vector<unsigned int>& VertexDataEndOffsets) const;
	unsigned int GetTypeOfObjectRenderingElementInd(const std::vector<unsigned int>& VertexDataEndOffsets);

	ClassesMap* instancePtr = nullptr;//"this" pointer from actual instance
	std::vector<Object2DData*> FilteredOrder;
public:

	static const std::type_index TypeIndex;

	Renderer2D* gThis();

	DLL_TREATMENT Renderer2D(ClassesMap* nPtr);
	
	DLL_TREATMENT void Render(int width, int height);


};