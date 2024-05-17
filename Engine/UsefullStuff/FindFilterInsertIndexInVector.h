#pragma once
#include"DLLStuff.h"
#include<vector>
template<typename vecType, typename funcType>
DLL_TREATMENT unsigned int FindFilterInsertIndexInVectorLH(std::vector<vecType>& vec, vecType& element, const funcType& getValFunc) {
	unsigned int insertInd = 0;
	for (unsigned int i = 0; i < vec.size(); i++) {
		if (getValFunc(vec[i]) > getValFunc(element)) {
			insertInd = i;
			break;
		}
	}
	return insertInd;
}
template<typename vecType, typename funcType>
DLL_TREATMENT unsigned int FindFilterInsertIndexInVectorHL(std::vector<vecType>& vec, vecType& element, const funcType& getValFunc) {
	unsigned int insertInd = vec.size();
	for (unsigned int i = 0; i < vec.size(); i++) {
		if (getValFunc(vec[i]) < getValFunc(element)) {
			insertInd = i;
			break;
		}
	}
	return insertInd;
}