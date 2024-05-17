#pragma once
#include"DLLStuff.h"
#include<iostream>
class Vector2;

class Matrix {
	unsigned int SizeX;
	unsigned int SizeY;
	float* Nums;
public:
	DLL_TREATMENT unsigned int gSizeX() const;
	DLL_TREATMENT unsigned int gSizeY() const;

	DLL_TREATMENT float operator[](unsigned int i) const;

	DLL_TREATMENT Matrix();
	DLL_TREATMENT Matrix(unsigned int sx, unsigned int sy, const float(&& nums)[]);
	DLL_TREATMENT Matrix(unsigned int sx, unsigned int sy);
	DLL_TREATMENT Matrix(const Matrix& mat);
	DLL_TREATMENT Matrix& operator=(const Matrix& mat);
	DLL_TREATMENT ~Matrix();

	DLL_TREATMENT bool IsIndInMatrix(unsigned int i) const;
	DLL_TREATMENT unsigned int GetIndByXY(unsigned int x, unsigned int y) const;
	DLL_TREATMENT bool IsXYInMatrix(unsigned int x, unsigned int y) const;
	DLL_TREATMENT Matrix operator+(const Matrix& mat);
	DLL_TREATMENT Matrix operator+(float num);
	DLL_TREATMENT Matrix operator-(const Matrix& mat);
	DLL_TREATMENT Matrix operator-(float num);

	DLL_TREATMENT Matrix operator*(const Matrix& mat);
	DLL_TREATMENT Matrix operator*(float num);

	DLL_TREATMENT Matrix operator*(const Vector2& vec);
	
};