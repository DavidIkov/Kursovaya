#pragma once
#include"DLLStuff.h"
#include"InsertWithFilterInVector.h"
#include<utility>

template<typename vecTyp, typename funcType>
DLL_TREATMENT std::vector<vecTyp> FilterVectorHL(std::vector<vecTyp>& vec,const funcType&& getValFunc) {
	std::vector<vecTyp> retVec;
	for (unsigned int i = 0; i < vec.size(); i++) {
		InsertWithFilterInVectorHL(retVec, vec[i], getValFunc);
	}
	return retVec;
}
template<typename returnVecTyp, typename inputVecTyp, typename getValFuncTyp, typename writeToVecFuncTyp>
DLL_TREATMENT std::vector<returnVecTyp> FilterVectorHL(std::vector<inputVecTyp>& vec, const getValFuncTyp&& getValFunc,const writeToVecFuncTyp&& writeFunc) {
	std::vector<returnVecTyp> retVec;
	for (unsigned int i = 0; i < vec.size(); i++) {
		InsertWithFilterInVectorHL(retVec, vec[i], getValFunc, writeFunc);
	}
	return retVec;
}
template<typename vecTyp, typename funcType>
DLL_TREATMENT std::vector<vecTyp> FilterVectorLH(std::vector<vecTyp>& vec,const funcType&& getValFunc) {
	std::vector<vecTyp> retVec;
	for (unsigned int i = 0; i < vec.size(); i++) {
		InsertWithFilterInVectorLH(retVec, vec[i], getValFunc);
	}
	return retVec;
}
template<typename returnVecTyp, typename inputVecTyp, typename getValFuncTyp, typename writeToVecFuncTyp>
DLL_TREATMENT std::vector<returnVecTyp> FilterVectorLH(std::vector<inputVecTyp>& vec, const getValFuncTyp&& getValFunc, const writeToVecFuncTyp&& writeFunc) {
	std::vector<returnVecTyp> retVec;
	for (unsigned int i = 0; i < vec.size(); i++) {
		InsertWithFilterInVectorLH(retVec, vec[i], getValFunc, writeFunc);
	}
	return retVec;
}