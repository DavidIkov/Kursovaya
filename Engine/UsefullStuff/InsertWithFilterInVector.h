#pragma once
#include"FindFilterInsertIndexInVector.h"
#include<vector>
template<typename vecType, typename funcType>
void InsertWithFilterInVectorLH(std::vector<vecType>& vec, vecType& element,const funcType& getValFunc) {
	vec.insert(vec.begin() + FindFilterInsertIndexInVectorLH(vec, element, getValFunc), element);
}
template<typename vecType, typename funcType>
void InsertWithFilterInVectorHL(std::vector<vecType>& vec, vecType& element,const funcType& getValFunc) {
	vec.insert(vec.begin() + FindFilterInsertIndexInVectorHL(vec, element, getValFunc), element);
}