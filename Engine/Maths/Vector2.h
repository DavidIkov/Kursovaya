#pragma once
#include"DLLStuff.h"
#include<math.h>
class Matrix;


class Vector2 {
	float X = 0.f;
	float Y = 0.f;
	mutable bool newLen = true;
	mutable float Len = 0.f;
	mutable float SqLen = 0.f;
	mutable bool newUnit = true;
	mutable float UnitX = 0.f;
	mutable float UnitY = 0.f;
public:

	DLL_TREATMENT static void RotateVectors(const Vector2& xv, const Vector2& yv, float angle, Vector2& xvTOWRITE, Vector2& yvTOWRITE);

	DLL_TREATMENT Vector2();
	DLL_TREATMENT Vector2(float nX, float nY);
	DLL_TREATMENT Vector2(float nAx);
	DLL_TREATMENT Vector2(const Matrix&& mat);
	
	DLL_TREATMENT Vector2& operator=(const Vector2& toCopy);

	DLL_TREATMENT float SqLength() const;
	DLL_TREATMENT float Length() const;
	DLL_TREATMENT Vector2 Unit() const;

	DLL_TREATMENT float DotProduct(const Vector2& vec) const;

	DLL_TREATMENT void sX(float nX);
	DLL_TREATMENT float gX() const;
	DLL_TREATMENT void sY(float nY);
	DLL_TREATMENT float gY() const;


	DLL_TREATMENT Vector2 operator+(float num) const;
	DLL_TREATMENT Vector2 operator-(float num) const;
	DLL_TREATMENT Vector2 operator*(float num) const;
	DLL_TREATMENT Vector2 operator/(float num) const;

	DLL_TREATMENT Vector2 operator+(const Vector2& vec) const;
	DLL_TREATMENT Vector2 operator-(const Vector2& vec) const;
	DLL_TREATMENT Vector2 operator*(const Vector2& vec) const;
	DLL_TREATMENT Vector2 operator/(const Vector2& vec) const;

	DLL_TREATMENT Vector2 operator+(const Matrix& mat) const;
	DLL_TREATMENT Vector2 operator-(const Matrix& mat) const;
	DLL_TREATMENT Vector2 operator*(const Matrix& mat) const;
	DLL_TREATMENT Vector2 operator/(const Matrix& mat) const;
};

