#include"RandomNumber.h"

float RandNum(float s, float e) {
	return s + (float)std::rand() / RAND_MAX * (e - s);
}