#pragma once
#include"DLLStuff.h"
#include<vector>
//need to optimize, this is doing 1 copy + 1 asignment per value(pretty bad) when i THINK maybe it can do just 1 asignment, prob not
template<typename vecTyp>
DLL_TREATMENT void SwapIntervalsInVector(std::vector<vecTyp>& vec, unsigned int s1, unsigned int l1, unsigned int s2, unsigned int l2) {

	//fixing weird stuff
	if (s1 == s2) return;
	if (s1 > s2) {
		unsigned int prevVal = s2;
		s2 = s1;
		s1 = prevVal;
	}


	std::vector<vecTyp> region;
	for (unsigned int i = 0; i < l2; i++)
		region.push_back(vec[s2 + i]);
	for (unsigned int i = 0; i < s2 - s1 - l1; i++)
		region.push_back(vec[s1 + l1 + i]);
	for (unsigned int i = 0; i < l1; i++)
		region.push_back(vec[s1 + i]);

	for (unsigned int i = 0; i < region.size(); i++) vec[s1 + i] = region[i];
}