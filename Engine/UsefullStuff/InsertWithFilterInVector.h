#pragma once
#include"DLLStuff.h"
#include"FindFilterInsertIndexInVector.h"
#include<vector>
template<typename vecType, typename funcType>
DLL_TREATMENT void InsertWithFilterInVectorHL(std::vector<vecType>& vec, vecType& element,const funcType& getValFunc) {
	vec.insert(vec.begin() + FindFilterInsertIndexInVectorHL(vec, element, getValFunc), element);
}
template<typename vecType, typename elementTyp, typename getValFuncTyp, typename writeFuncTyp>
DLL_TREATMENT void InsertWithFilterInVectorHL(std::vector<vecType>& vec, elementTyp& element, const getValFuncTyp& getValFunc, const writeFuncTyp& writeFunc) {
	vec.insert(vec.begin() + FindFilterInsertIndexInVectorHL(vec, element, getValFunc), writeFunc(element));
}
template<typename vecType, typename funcType>
DLL_TREATMENT void InsertWithFilterInVectorLH(std::vector<vecType>& vec, vecType& element,const funcType& getValFunc) {
	vec.insert(vec.begin() + FindFilterInsertIndexInVectorLH(vec, element, getValFunc), element);
}
template<typename vecType, typename elementTyp, typename getValFuncTyp, typename writeFuncTyp>
DLL_TREATMENT void InsertWithFilterInVectorLH(std::vector<vecType>& vec, elementTyp& element, const getValFuncTyp& getValFunc, const writeFuncTyp& writeFunc) {
	vec.insert(vec.begin() + FindFilterInsertIndexInVectorLH(vec, element, getValFunc), writeFunc(element));
}