#pragma once
#include"DLLStuff.h"
#include<vector>
template<typename vecType>
DLL_TREATMENT void InsertVectorInVector(std::vector<vecType>& v1, std::vector<vecType> v2, unsigned int s) {
	for (unsigned int i = v2.size() - 1;; i--) {
		v1.insert(v1.begin() + s, v2[i]);
		if (i == 0) break;
	}
}