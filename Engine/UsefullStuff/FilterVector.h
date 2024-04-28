#pragma once
#include"InsertWithFilterInVector.h"
#include<utility>

template<typename vecTyp, typename funcType>
std::vector<vecTyp> FilterVectorHL(std::vector<vecTyp>& vec,const funcType& getValFunc) {
	std::vector<vecTyp> retVec;
	for (unsigned int i = 0; i < vec.size(); i++) {
		InsertWithFilterInVectorHL(retVec, vec[i], std::move(getValFunc));
	}
	return retVec;
}
template<typename vecTyp, typename funcType>
std::vector<vecTyp> FilterVectorLH(std::vector<vecTyp>& vec,const funcType& getValFunc) {
	std::vector<vecTyp> retVec;
	for (unsigned int i = 0; i < vec.size(); i++) {
		InsertWithFilterInVectorLH(retVec, vec[i], getValFunc);
	}
	return retVec;
}