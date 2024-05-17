#pragma once
#include"DLLStuff.h"
#include"Debug.h"
#include<vector>
class BufferLayout {
	unsigned int SizeOfOneElement;
	unsigned int UseType;
public:


	unsigned int SumOfLengths = 0;
	std::vector<unsigned int> Lengths;

	DLL_TREATMENT BufferLayout(unsigned int useType, unsigned int size);

	//adds passed value to list of lengths for layout
	DLL_TREATMENT void AddData(unsigned int len);

	//binds layout.. should be used with vertex array enabled
	DLL_TREATMENT void Bind() const;

	/*unbinds layout..shouldnt be used.. if you use this then u got some problems in code, 
	try to use unique vertex buffer for each unique layout, not creating new one every time or reusing same layout,
	this class wont create somenthing in opengl when its created or deleted, its just about enabling certain layout for buffer*/
	DLL_TREATMENT void Unbind() const;
};