#pragma once
#include"DLLStuff.h"
#include<iostream>
class Vector2;
class Vector3;

//numeration is mirrored
class Matrix {
	unsigned int SizeX;
	unsigned int SizeY;
	float* Nums;

	mutable bool NeedToUpdateReversedMatrix = true;
	mutable Matrix* ReversedMatrix;
	void UpdateReversedMatrix() const;

	mutable bool NeedToUpdateDeterminant = true;
	mutable float Determinant;
	void UpdateDeterminant() const;

public:
	DLL_TREATMENT unsigned int gSizeX() const;
	DLL_TREATMENT unsigned int gSizeY() const;

	DLL_TREATMENT float operator[](unsigned int i) const;

	DLL_TREATMENT static Matrix FromAnglesXYZ(float xr, float yr, float zr);

	DLL_TREATMENT Matrix();
	DLL_TREATMENT Matrix(unsigned int sx, unsigned int sy, const float(&&nums)[]);
	DLL_TREATMENT Matrix(unsigned int sx, unsigned int sy, const float nums[]);
	DLL_TREATMENT Matrix(unsigned int sx, unsigned int sy);
	DLL_TREATMENT Matrix(unsigned int l, const Vector3*(&&nums)[]);
	DLL_TREATMENT Matrix(unsigned int l, const Vector2*(&&nums)[]);
	DLL_TREATMENT Matrix(const Matrix& mat);
	DLL_TREATMENT Matrix& operator=(const Matrix& mat);
	DLL_TREATMENT ~Matrix();

	DLL_TREATMENT float gDeterminant() const;
	DLL_TREATMENT const Matrix& gReversedMatrix() const;

	DLL_TREATMENT bool IsIndInMatrix(unsigned int i) const;
	DLL_TREATMENT unsigned int GetIndByXY(unsigned int x, unsigned int y) const;
	DLL_TREATMENT bool IsXYInMatrix(unsigned int x, unsigned int y) const;
	DLL_TREATMENT Matrix operator+(const Matrix& mat) const;
	DLL_TREATMENT Matrix operator+(float num) const;
	DLL_TREATMENT Matrix operator-(const Matrix& mat) const;
	DLL_TREATMENT Matrix operator-(float num) const;

	DLL_TREATMENT Matrix operator*(const Matrix& mat) const;
	DLL_TREATMENT Matrix operator*(float num) const;

	DLL_TREATMENT Matrix operator*(const Vector2& vec) const;
	DLL_TREATMENT Matrix operator*(const Vector3& vec) const;
	
};