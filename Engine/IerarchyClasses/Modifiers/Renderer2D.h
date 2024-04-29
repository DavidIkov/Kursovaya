#pragma once
#include<vector>
#include"ClassesMap.h"
#include"Instance.h"



#include"../../GraphicsPrimitives/VertexBuffer.h"
#include"../../GraphicsPrimitives/IndexBuffer.h"
#include"../../GraphicsPrimitives/VertexArray.h"
#include"../../GraphicsPrimitives/BufferLayout.h"

class Instance2DData;

class Renderer2D {
	friend Instance2DData;

	struct TypesOfObjectsRendering_Element {
		VertexArray vVertexArray;
		VertexBuffer vVertexBuffer;
		IndexBuffer vIndexBuffer;
		BufferLayout vBufferLayout;
		TypesOfObjectsRendering_Element(const std::vector<unsigned int>& layout);
		bool CompareWithLayout(const std::vector<unsigned int>& vertexDataLens) const;
		void Bind(Instance2DData* instPtr, std::vector<float>* vertexBuffer, std::vector<unsigned int>* indexBuffer);
		void Unbind();
	};
	std::vector<TypesOfObjectsRendering_Element> TypesOfObjectsRendering;
	unsigned int GetTypeOfObjectRenderingElementInd(const std::vector<unsigned int>& VertexDataEndOffsets) const;
	unsigned int GetTypeOfObjectRenderingElementInd(const std::vector<unsigned int>& VertexDataEndOffsets);

	ClassesMap* instancePtr = nullptr;//"this" pointer from actual instance
	std::vector<Instance2DData*> FilteredOrder;
public:

	static const std::type_index TypeIndex;

	Renderer2D* gThis();

	Renderer2D(ClassesMap* nPtr);
	
	void Render(int width, int height);


};