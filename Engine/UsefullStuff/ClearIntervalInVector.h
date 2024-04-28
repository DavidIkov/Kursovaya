#pragma once
#include<vector>
template<typename vecTyp>
void ClearIntervalInVector(std::vector<vecTyp>& vec, unsigned int s, unsigned int l) {
	for (unsigned int i = 0; i < l; i++) {
		vec.erase(vec.begin() + s);
	}
}